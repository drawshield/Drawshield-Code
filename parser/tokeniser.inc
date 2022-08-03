<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

     DrawShield is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
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
    $part_word = '';
    $end = strlen($blazon);
    $lookBehind = '';
    $anyEndQuote = false;
    while ( $i < $end ) {
      $lookAhead = '';
      if ($i + 1 < $end) {
        $lookAhead = mb_substr($blazon, $i+1, 1);
      }
      $curChar = mb_substr($blazon, $i, 1);
      if ($blockComment) { // ignore everything to matching close
        if ($curChar == '*' && $lookAhead == '/') {
          $blockComment = false;
          $i++;
        }
        $i++;
        continue;
      }
      if ($lineComment) { // shortcut, ignore all up to new line
        if ($curChar == "\n") {
          $lineNo += 1;
          $lineComment = false;    
        } 
        $i++;
        continue;
      }
      switch ( $curChar ) {
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
          if ($curChar == "\n") {
            $lineNo += 1;
          }
          break;
        case '{':
          if ( $incAll ) {
            $part_word .= $curChar;
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
            $part_word .= $curChar;
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
            $part_word .= $curChar;
            break;
          }
          if ( $part_word != '' ) {
            $this->words[] = $part_word; 
            $this->lineNos[] = $lineNo;
            $part_word = '';
          }
          $comment = $curChar;
          break;
        case '*':
          if ( $incAll ) {
            $part_word .= $curChar;
            break;
          }
          if ('*' == $comment && $lookAhead == '/') { // match bracket type
            $comment = false;
            $part_word = '';
            $i += 1;
          } else {
            if ( $part_word != '' ) {
              $this->words[] = $part_word; $this->lineNos[] = $lineNo;
            }
            $this->words[] = '*';
            $this->lineNos[] = $lineNo;
            $part_word = '';
          }
          break;
        case ')':
          if ( $incAll ) {
            $part_word .= $curChar;
            break;
          }
          if ($comment == '(') { // match bracket type
            $comment = false;
            $part_word = '';
          } else {
              if ( $part_word != '' ) {
                $this->words[] = $part_word; $this->lineNos[] = $lineNo;
            }
            $this->words[] = $curChar;
            $this->lineNos[] = $lineNo;
            $part_word = '';
          }
          break;
        case ']':
          if ( $incAll ) {
            $part_word .= $curChar;
            break;
          }
          if ($comment == '[') { // match bracket type
            $comment = false;
            $part_word = '';
          } elseif ( $part_word != '' ) {
                $this->words[] = $part_word; $this->lineNos[] = $lineNo;
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
          if ( $i > 0 && mb_strstr(  " \t\n\r'\",.", mb_substr($blazon, $i-1, 1))) {
            $i = $end;
          }
          if ( $i < $end - 1 && mb_substr($blazon,$i+1, 1) == '-' ) {
            $i = $end;
          }
          break;
        case '.':
        case ':':
          if ( $incAll ) {
            $part_word .= $curChar;
            break;
          }
          if (ctype_digit($lookBehind) && ctype_digit($lookAhead)) {
            $part_word .= $curChar;
            break;
          } elseif ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            $part_word = '';
          }
          if ( !$comment ) { $this->words[] = $curChar; $this->lineNos[] = $lineNo; }
          break;
        case ';':
          if ( $incAll ) {
            $part_word .= $curChar;
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
            $part_word .= $curChar;
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
            $part_word = '';
          }
          if ( !$comment ) { $this->words[] = $curChar; $this->lineNos[] = $lineNo; }
          break;
        case "«": // list of quote chars from https://stackoverflow.com/questions/20025030/convert-all-types-of-smart-quotes-with-php
        case "»":
        case "‘":
        case "’":
        case "‚":
        case "‛":
        case "“":
        case "”":
        case "„":
        case "‟":
        case "‹":
        case "›":
          if ( $in_string ) { // If we are inside normal quotes just add to the word
            if ($anyEndQuote) {
              if ($part_word != '') {
                $this->words[] = $part_word;
                $this->lineNos[] = $lineNo;
              }
              $part_word = '';
              $in_string = false;
              $incAll = false;
              $anyEndQuote = false;
            } else {
              $part_word .= $curChar;
            }
          } else { // otherwise treat it the same as a normal quote
            if ( $in_string ) {
              if ( $part_word != '' ) { $this->words[] = $part_word; $this->lineNos[] = $lineNo; }
              $part_word = '';
              $in_string = false;
              $incAll = false;
            } else {
              // $part_word = '"';
              $incAll = true;
              $in_string = true;
              $anyEndQuote = true;
            }
          }
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
            $anyEndQuote = false;
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
        case '~':
        case '^':
          if ( $incAll ) {
            $part_word .= $curChar;
          }
          break;
        default:
          $part_word .= $curChar;
          break;
      }
      if ($i < $end) $lookBehind = $curChar;
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
    while ( mb_substr($this->words[$offset],0,1) == '[' && $offset < $this->num_tokens  )
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
