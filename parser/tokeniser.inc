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

class Token {
  public $text;     // this is original set of characters from the blazon
  public $match;    // this is a length limited, lowercase version of the above for use in comparisons
  public $startPos;     // this is a string in the form line:character giving the position of the START of the token
    public $endPos;  // as above for the end, if null, just add the length of the tex
  public $category; // if not null this is the category to assign to the token, as determined by the tokeniser

  public function __construct($text, $start = null, $end = null, $category = null) {
    $this->text = $text;
    $this->match = transliterate(mb_strtolower(mb_substr($text, 0, 192, "UTF-8")));
    $this->startPos = $start;
    if (is_null($end)) {
        list($line,$char) = preg_split('/:/', $start);
        $end = $line . ':' . $char + mb_strlen($text) - 1;
    }
    $this->endPos = $end;
    $this->category = $category;
  }

  public function __toString() {
      $retval = "{$this->text} ({$this->startPos}-{$this->endPos})";
      if (!is_null($this->category)) {
          $retval .= " [{$this->category}]";
      }
      return $retval;
  }

}

class Phrase {
    public $keyterm;
    protected $text;
    protected $start;
    protected $end;
    protected $state;

    public function __construct($state = null) {
        $this->text = '';
        $this->state = $state;
        $this->start = null;
    }

    public function getState() {
        return $this->state;
    }

    public function isEmpty() {
        return is_null($this->start);
    }

    public function add($lexeme): ?Lexeme {
        if (!is_null($lexeme)) {
            $this->text .= $lexeme->tokens . ' ';
            if (is_null($this->start)) {
                $this->start = $lexeme->startPos;
            }
            $this->end = $lexeme->endPos;
            // We always use the most recently found keyterm (but can override it)
            $this->keyterm = $lexeme->keyterm;
        }
        return $lexeme; // so we can chain assignment
    }

    public function __toString() {
        return rtrim($this->text);
    }

    public function create($keyterm = null, $value = null): Lexeme {
        if (is_null($keyterm)) $keyterm = $this->keyterm;
        return new Lexeme("phrase", $keyterm, rtrim($this->text), $this->start, $this->end, $value);
    }
}

class Tokeniser {

  // Constructor
  // Fill the words array with words from the blazon
  // Punctuation and spaces ignored
  function __construct() {
      // set any options here
  }

  function tokenise($blazon) {

    #$blazon = iconv("UTF-8", "ASCII//TRANSLIT", $blazonText);
    #$blazon = preg_replace(array('/è/','/é/','/ê/','/ç/','/à/','/á/','/â/','/È/','/É/','/Ê/','/Ç/','/À/','/Á/','/Â/'),
    #				       array('e','e','e','c','a','a','a','e','e','e','c','a','a','a'),
    #               $blazonText);
                   
    $words = array();
    $lineNo = 1;
    $charCount = 1;
  
    $i = 0;
    $lineComment = false;
    $comment = false;
    $blockComment = false;
      $tildeComment = false;
    $in_string = false;
    $incAll = false;
    $part_word = '';
    $end = strlen($blazon);
    $lookBehind = '';
    $anyEndQuote = false;
    $wordStart = 1;
    while ( $i < $end ) {
      $lookAhead = '';
      if ($i + 1 < $end) {
        $lookAhead = mb_substr($blazon, $i+1, 1);
      }
      $curChar = mb_substr($blazon, $i, 1);
      if ($blockComment) { // ignore everything to matching close
        if ($curChar == '*' && $lookAhead == '/') {
          $blockComment = false;
          $charCount++;
          $i++;
        }
        $i++;
          $charCount++;
        continue;
      }
      if ($lineComment) { // shortcut, ignore all up to new line
        if ($curChar == "\n") {
          $lineNo += 1;
          $charCount = 0;
          $lineComment = false;    
        } 
        $i++;
        continue;
      }
      switch ( $curChar ) {
        case "\n":
        case ' ':
        case "\t":
        case "\r":
          if ( $incAll )
            $part_word .= ' ';
          elseif ( $part_word != '' ) {
            if ( !$comment ) { $words[] = new Token($part_word, "$lineNo:$wordStart"); }
            $part_word = ''; $wordStart = $charCount + 1;
          } else {
              $wordStart = $charCount + 1;
          } // else ignore it
          if ($curChar == "\n") {
            $lineNo += 1;
            $charCount = 0;
            $wordStart = 1;
          }
          break;
        case '{':
          if ( $incAll ) {
            $part_word .= $curChar;
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $words[] = new Token($part_word, "$lineNo:$wordStart"); }
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
            $words[] = new Token($part_word, "$lineNo:$wordStart");
          }
          $part_word = ''; $wordStart = $charCount + 1;
          $incAll = false;
          break;
        case '(':
        case '[':
          if ( $incAll ) {
            $part_word .= $curChar;
            break;
          }
          if ( $part_word != '' ) {
            $words[] = new Token($part_word, "$lineNo:$wordStart");
            $part_word = ''; $wordStart = $charCount + 1;
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
            $part_word = ''; $wordStart = $charCount + 1;
            $i += 1;
              $charCount++;
          } else {
            if ( $part_word != '' ) {
              $words[] = new Token($part_word, "$lineNo:$wordStart");
            }
            $words[] = new Token('*', "$lineNo:$wordStart");
            $part_word = ''; $wordStart = $charCount + 1;
          }
          break;
        case ')':
          if ( $incAll ) {
            $part_word .= $curChar;
            break;
          }
          if ($comment == '(') { // match bracket type
            $comment = false;
            $part_word = ''; $wordStart = $charCount + 1;
          } else {
              if ( $part_word != '' ) {
                $words[] = new Token($part_word, "$lineNo:$wordStart");
            }
            $words[] = new Token($curChar, "$lineNo:$wordStart");
            $part_word = ''; $wordStart = $charCount + 1;
          }
          break;
        case ']':
          if ( $incAll ) {
            $part_word .= $curChar;
            break;
          }
          if ($comment == '[') { // match bracket type
            $comment = false;
            $part_word = ''; $wordStart = $charCount + 1;
          } elseif ( $part_word != '' ) {
            $words[] = new Token($part_word, "$lineNo:$wordStart");
          }
          break;
        case '/':
          if ( $incAll ) {
            $part_word .= '/';
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $words[] = new Token($part_word, "$lineNo:$wordStart"); }
            $part_word = ''; $wordStart = $charCount + 1;
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
            if ( !$comment ) { $words[] = new Token($part_word, "$lineNo:$wordStart"); }
            $part_word = ''; $wordStart = $charCount + 1;
          }
          $lineComment = true;
          break;
        case '-':
          if ( $incAll ) {
            $part_word .= ' ';
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $words[] = new Token($part_word, "$lineNo:$wordStart"); }
            $part_word = ''; $wordStart = $charCount + 1;
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
            if ( !$comment ) { $words[] = new Token($part_word, "$lineNo:$wordStart"); }
            $part_word = ''; $wordStart = $charCount + 1;
          }
        if ( !$comment ) { $words[] = new Token($curChar, "$lineNo:$wordStart", null, languageDB::PERIOD); }
          break;
        case ';':
          if ( $incAll ) {
            $part_word .= $curChar;
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $words[] = new Token($part_word, "$lineNo:$wordStart"); }
            $part_word = ''; $wordStart = $charCount + 1;
          }
          if ( !$comment ) {
              $type = languageDB::SEMICOLON;
            $temp_word = ';';
            if ($lookAhead == ';') { // treat double semi-colon as its own symbol
                $type = languageDB::DOUBLESEMI;
              $temp_word = ";;";
              $i += 1;
                $charCount++;
            }
            $words[] = new Token($temp_word, "$lineNo:$wordStart", null, $type);
          }
          break;
        case ',':
          if ( $incAll ) {
            $part_word .= $curChar;
            break;
          }
          if ( $part_word != '' ) {
            if ( !$comment ) { $words[] = new Token($part_word, "$lineNo:$wordStart"); }
            $part_word = ''; $wordStart = $charCount + 1;
          }
          if ( !$comment ) { $words[] = new Token($curChar, "$lineNo:$wordStart", null, languageDB::COMMA); }
          break;
          case "'": // single quotes are always apostrophes, just ignore unless part of a string
          case "`":
          case "’":
          case "‛":
              if ( $incAll ) {
                  $part_word .= "'";
              }
              break;
        case "«": // list of quote chars from https://stackoverflow.com/questions/20025030/convert-all-types-of-smart-quotes-with-php
        case "»":
        case "‘":
        case "‚":
        case "“":
        case "”":
        case "„":
        case "‟":
        case "‹":
        case "›":
          if ( $in_string ) { // If we are inside normal quotes just add to the word
            if ($anyEndQuote) {
              if ($part_word != '') {
                $words = new Token($part_word, "$lineNo:$wordStart", null, "string");
              }
              $part_word = ''; $wordStart = $charCount + 1;
              $in_string = false;
              $incAll = false;
              $anyEndQuote = false;
            } else {
              $part_word .= $curChar;
            }
          } else { // otherwise treat it the same as a normal quote
            if ( $in_string ) {
              if ( $part_word != '' ) { $words[] = new Token($part_word, "$lineNo:$wordStart", null, "string"); }
              $part_word = ''; $wordStart = $charCount + 1;
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
            if ( $part_word != '' ) { $words[] = new Token($part_word, "$lineNo:$wordStart", null, "string"); }
            $part_word = ''; $wordStart = $charCount + 1;
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
                $charCount++;
              $i++;

            } else
              $part_word .= '\\';
          }
          break;
        case '~':
            if ( $incAll ) {
                $part_word .= '~';
                break;
            }
            if ( $part_word != '' ) {
                if ( !$comment ) { $words[] = new Token($part_word, "$lineNo:$wordStart"); }
                $part_word = ''; $wordStart = $charCount + 1;
            }
            if ($lookAhead == '~') {
                $i += 1;
                if ($tildeComment) {
                    $tildeComment = false;
                    $comment = false;
                } else {
                    $tildeComment = true;
                    $comment = true;
                }
            }
            break;
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
        $charCount++;
    }
    if ( $part_word != '' ) {
      if ( !$comment ) { $words[] = new Token($part_word, "$lineNo:$wordStart"); }
    }
    return $words;
  }
  
}
     
