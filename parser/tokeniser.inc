<?php /* Copyright 2014 Karl R. Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

class tokeniser {

  public $words = array(); // Used internally to manage tokens
  public $badWords = false; // Set if unknown token encountered
  public $cur_word;
  public $num_tokens;
  // Constructor
  // Fill the words array with words from the blazon
  // Punctuation and spaces ignored
  function __construct($blazonText) {
    
    $blazon = iconv("UTF-8", "ASCII//TRANSLIT", $blazonText);
    $this->words = array();
    $this->cur_word = 0;
  
    $i = 0;
    $comment = false;
    $in_string = false;
    $incAll = false;
    $given = false;
    $part_word = '';
    $end = strlen($blazon);
    while ( $i < $end ) {
      switch ( $blazon[$i] ) {
        case ' ':
        case "\t":
        case "\n":
        case "\r":
          if ( $incAll )
            $part_word .= ' ';
          elseif ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; }
            $part_word = '';
          } // else ignore it
          break;
        case '{':
          if ( $incAll ) {
            $part_word .= $blazon[$i];
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; }
          }
          $part_word = '{';
          $incAll = true;
          break;
        case '}':
          if ( $incAll ) {
            $part_word .= $blazon[$i];
            break;
          }
          if ( $part_word != '' ) {
            $part_word .= '}';
            $this->words[] = $part_word;
          }
          $part_word = '';
          $incAll = false;
          break;
        case '(':
        case '[':
        if ( $incAll ) {
          $part_word .= $blazon[$i];
          break;
        }
          if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; }
            $part_word = '';
          }
          $comment = true;
          break;
        case ')':
        case ']':
        if ( $incAll ) {
          $part_word .= $blazon[$i];
          break;
        }
        if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; }
            $part_word = '';
          }
          $comment = false;
          break;
        case '#':
        case '/':
        case ':':
          if ( $incAll ) {
            $part_word .= $blazon[$i];
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; }
            $part_word = '';
          }
          $i = $end;
          break;
        case '-':
          if ( $incAll ) {
            $part_word .= ' ';
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; }
            $part_word = '';
          }
          if ( $i > 0 and strstr(  " \t\n\r'\",.", $blazon[$i-1])) {
            $i = $end;
          }
          if ( $i < $end and $blazon[$i+1] == '-' ) {
            $i = $end;
          }
          break;
        case ';':
        case ',':
        case '.':
          if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; }
            $part_word = '';
          }
          if ( !$comment ) $this->words[] = $blazon[$i];
          break;
        case '"':
          if ( $in_string ) {
            if ( $part_word != '' ) $this->words[] = $part_word;
            $part_word = '';
            $in_string = false;
            $incAll = false;
          } else {
            // $part_word = '"';
            $incAll = true;
            $in_string = true;
          }
          break;
        case '\\':
          if ( $incAll ) {
            if ( ($i+1) < $end and $blazon[$i+1] == '"' ) {
              $part_word .= '"';
              $i++;
            } else
              $part_word .= '\\';
          }
          break;
        case "'":
        case '`':
        case '~':
        case '^':
          if ( $incAll ) {
            $part_word .= $blazon[$i];
          }
          break;
        default:
          $part_word .= $blazon[$i];
          break;
      }
      $i++;
    }
    if ( $part_word != '' ) {
      if ( !$comment ) { $this->words[] = $part_word; }
    }
    $this->num_tokens = count($this->words);
    $this->origWords = $this->words;
  }
  
  public function ignoreWord($offset) {
    global $trace;
    
    $this->badWords = true;
    if ( $offset >= $this->num_tokens )
      $offset = $this->num_tokens - 1;
    elseif ( $offset < 0 )
      $offset = 0;
    // in case already ignored, try ignoring next word
    while ( $this->words[$offset]{0} == '[' and $offset < $this->num_tokens  )
      $offset++;
    $this->words[$offset] = '[' . $this->words[$offset] . ']';
    if ($trace) echo "<p>Ignoring - " . $this->words[$offset] . "</p>\n";
  }

  public function restoreWord($offset) {
    global $trace;


    if ( $offset >= $this->num_tokens or $offset < 0 ) return;
    $this->words[$offset] = trim($this->words[$offset],'[]');

    if ($trace) echo "<p>Restoring - " . $this->words[$offset] . "</p>\n";
  }
  
  public function resetToken() {
    global $trace;
    
    if ($trace) echo "<p>RESET TOKENS</p>\n"; // DEBUG
    $this->cur_word = 0;
  }
  
  public function moreInput() {
    return ( $this->cur_word < $this->num_tokens );
  }
  
  public function getTokens() {
    return $this->words;
  }
  
}
     
?>
