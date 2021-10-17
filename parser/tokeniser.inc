<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see https://www.gnu.org/licenses/.
 */

class tokeniser {

  public $words = array(); // Used internally to manage tokens
  public $lineNos = array();
  public $badWords = false; // Set if unknown token encountered
  public $cur_word;
  public $num_tokens;
  // Constructor
  // Fill the words array with words from the blazon
  // Punctuation and spaces ignored
  function __construct($blazon) {
    
    #$blazon = iconv("UTF-8", "ASCII//TRANSLIT", $blazonText);
    #$blazon = preg_replace(array('/è/','/é/','/ê/','/ç/','/à/','/á/','/â/','/È/','/É/','/Ê/','/Ç/','/À/','/Á/','/Â/'),
    #				       array('e','e','e','c','a','a','a','e','e','e','c','a','a','a'),
    #               $blazonText);
                   
    $this->words = array();
    $this->lineNos = array();
    $this->cur_word = 0;
    $lineNo = 1;
  
    $i = 0;
    $lineComment = false;
    $comment = false;
    $blockComment = false;
    $in_string = false;
    $incAll = false;
    $given = false;
    $part_word = '';
    $end = strlen($blazon);
    $lookBehind = '';
    while ( $i < $end ) {
      $lookAhead = '';
      if ($i + 1 < $end) {
        $lookAhead = $blazon[$i+1];
      }
      if ($blockComment) { // ignore everything to matching close
        if ($blazon[$i] == '*' && $lookAhead == '/') {
          $blockComment = false;
          $i++;
        }
        $i++;
        continue;
      }
      if ($lineComment) { // shortcut, ignore all up to new line
        if ($blazon[$i] == "\n") {
          $lineNo += 1;
          $lineComment = false;    
        } 
        $i++;
        continue;
      }
      switch ( $blazon[$i] ) {
        case "'": // like a space (unless in a string)
        case "`":
          if ( $incAll ) {
            $part_word .= "'";
          }
          // elseif ( $part_word == '' ) {
          //   if ( !$comment ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            // $part_word = '';
          // } // else ignore it
          break;
        case "\n":
        case ' ':
        case "\t":
        case "\r":
          if ( $incAll )
            $part_word .= ' ';
          elseif ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            $part_word = '';
          } // else ignore it
          if ($blazon[$i] == "\n") {
            $lineNo += 1;
          }
          break;
        case '{':
          if ( $incAll ) {
            $part_word .= $blazon[$i];
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word;  $this->lineNos[] = $lineNo; }
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
            $this->lineNos[] = $lineNo;
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
            $this->words[] = $part_word; 
            $this->lineNos[] = $lineNo;
            $part_word = '';
          }
          $comment = $blazon[$i];
          break;
        case '*':
          if ( $incAll ) {
            $part_word .= $blazon[$i];
            break;
          }
          if ('*' == $comment && $lookAhead == '/') { // match bracket type
            $comment = false;
            $part_word = '';
            $i += 1;
          } else {
            if ( $part_word != '' ) {
              if ( $part_word != '' ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            }
            $this->words[] = '*';
            $this->lineNos[] = $lineNo;
            $part_word = '';
          }
          break;
        case ')':
          if ( $incAll ) {
            $part_word .= $blazon[$i];
            break;
          }
          if ($comment == '(') { // match bracket type
            $comment = false;
            $part_word = '';
          } else {
            if ( $part_word != '' ) {
              if ( $part_word != '' ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            }
            $this->words[] = $blazon[$i];
            $this->lineNos[] = $lineNo;
            $part_word = '';
          }
          break;
        case ']':
          if ( $incAll ) {
            $part_word .= $blazon[$i];
            break;
          }
          if ($comment == '[') { // match bracket type
            $comment = false;
            $part_word = '';
          } else {
            if ( $part_word != '' ) {
              if ( $part_word != '' ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            }
          }
          break;
        case '/':
          if ( $incAll ) {
            $part_word .= '/';
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            $part_word = '';
          }
          if ($lookAhead == '/') {
            $lineComment = true;
            $i += 1;
          } elseif ($lookAhead == '*') {
            $blockComment = true;
            $i += 1;
          }
          break;
         case '#':
          if ( $incAll ) {
            $part_word .= '#';
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            $part_word = '';
          }
          $lineComment = true;
          break;
        case '-':
          if ( $incAll ) {
            $part_word .= ' ';
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            $part_word = '';
          }
          if ($comment) break;
          if ( $i > 0 && strstr(  " \t\n\r'\",.", $blazon[$i-1])) {
            $i = $end;
          }
          if ( $i < $end - 1 && $blazon[$i+1] == '-' ) {
            $i = $end;
          }
          break;
        case '.':
        case ':':
          if ( $incAll ) {
            $part_word .= $blazon[$i];
            break;
          }
          if (ctype_digit($lookBehind) && ctype_digit($lookAhead)) {
            $part_word .= $blazon[$i];
            break;
          } elseif ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            $part_word = '';
          }
          if ( !$comment ) { $this->words[] = $blazon[$i]; $this->lineNos[] = $lineNo; }
          break;
        case ';':
          if ( $incAll ) {
            $part_word .= $blazon[$i];
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            $part_word = '';
          }
          if ( !$comment ) {
            $temp_word = ';';
            if ($lookAhead == ';') { // treat double semi-colon as its own symbol
              $temp_word = ";;";
              $i += 1;
            }
            $this->words[] = $temp_word; $this->lineNos[] = $lineNo;
          }
          break;
        case ',':
          if ( $incAll ) {
            $part_word .= $blazon[$i];
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            $part_word = '';
          }
          if ( !$comment ) { $this->words[] = $blazon[$i]; $this->lineNos[] = $lineNo; }
          break;
        case '"':
          if ( $in_string ) {
            if ( $part_word != '' ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
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
            if ( $lookAhead == '"' ) {
              $part_word .= '"';
              $i++;

            } else
              $part_word .= '\\';
          }
          break;
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
      if ($i < $end) $lookBehind = $blazon[$i];
      $i++;
    }
    if ( $part_word != '' ) {
      if ( !$comment ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
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
    while ( $this->words[$offset]{0} == '[' && $offset < $this->num_tokens  )
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
  
  public function getLineNos() {
    return $this->lineNos;
  }
  
}
     
?>
