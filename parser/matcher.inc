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

class matcher {
  public $furthestWord = 0;
  protected $tokenList;
  /**
   * @var lexicon $patternDB
   */
  protected $patternDB;
  protected $errors = array();
  protected $value = null;
  protected $matchedValue = null;
  
  // Constructor
  function __construct($tokenList, $patternDB) {
    $this->tokenList = $tokenList;
    $this->patternDB = $patternDB;
  }
    
  // Try to match each of the regular expressions in the array
  // with words in the input. Return 0 on fail, number of input
  // words matched on success
  // An initial '?' indicates match is optional
  protected function check_match ( $match_string ) {
    if ( !$match_string ) {
      $this->errors[] = 'null match string';
      return false;
    }
    //$this->value = null;

    $regexs = explode(' ', $match_string);
  
    $t_word = $this->tokenList->cur_word;
    foreach ( $regexs as $regex ) {
      if ( $regex == '' ) continue;
      // Check for optionality
      $optional = false;
      if ( $regex{0} == '?' ) {
        $optional = true;
        $regex = substr($regex, 1);
      }
      // Do we want the whole of the next token?
      // Optional & negative do not make sense here, so ignore
      if ( $regex == '*' ) {
        $this->value = $this->tokenList->words[$t_word];
        $t_word++;
        continue;
      }
      // Check for negative
      $negative = false;
      if ( $regex{0} == '!' ) {
        $negative = true;
        $regex = substr($regex,1);
      }
      while ( ($t_word < $this->tokenList->num_tokens) and ($this->tokenList->words[$t_word]{0} == '[') ) 
        $t_word++;
      if ( $t_word >= $this->tokenList->num_tokens ) { // no more input
        if ( $optional or $negative ) {
          continue;
        } else {
           return 0;
        }
      }
      if ( $regex == '#' ) {
        $state = $this->tokenList->cur_word;
        $this->tokenList->cur_word = $t_word;
        if ( $num = self::searchMatch(languageDB::NUMBER) ) {
          $this->value = $num;
          $t_word = $this->tokenList->cur_word - 1;
          $res = 1;
        } else {
          $res = 0;
        }
        $this->tokenList->cur_word = $state;
      } else {
        $res = @preg_match('/^' . $regex . '$/i', $this->tokenList->words[$t_word]);
      }
      if ( $res === 0 ) { // no match
        if ( ! ($optional || $negative)) {
          return 0;
        }
        else
          continue;
      }
      if ( $res === false ) {
        $this->errors[] = 'Bad regex - ' . $regex;
        continue;
      }
      // else $res === 1
      if ( $negative ) { // Got a match, but didn't want it
        $this->value = null; // TODO rewrite as a try/catch block?
        return 0;
      }
      $t_word += 1; // Got a match, all OK, so move on
      if ( !$optional and $t_word > $this->furthestWord )
        $this->furthestWord = $t_word;
    }
    // should only get here on a full match
    return $t_word;
  }

  public function searchMatch ( $patterns ) {
    global $trace;
    $this->matchedvalue = null;
    
    if ( $trace ) echo "<p>$patterns</p>\n"; // DEBUG
  
    $first_token = $this->tokenList->cur_word;
    if ( $first_token >= $this->tokenList->num_tokens ) return null;
    // First, check if the input has been marked for us (indicated by leading '{'
    $first_word = $this->tokenList->words[$first_token];
    if ( $first_word{0} == '{' ) { // See what type it is
      if ( ($colPos = strpos( $first_word, ':' )) === false ) { // assume it is a charge
        $given = 'charge';
      } else { // or get what has been given
        $given = trim(substr( $first_word, 1, $colPos - 1 ));
      }
      if ( $patterns == $given ) { // If this is what we are looking for
        $this->tokenList->cur_word++; // Return the rest of it
        return ( trim(substr( $first_word, $colPos + 1, -1)) );
      } // Never match anything else
      return false;
    } // Otherwise, carry out normal search routine
    $max_match = 0;
    $match = null;
    $max_word = null;
    $terms = $this->patternDB->getPatterns($patterns);
    if ($terms == null ) { 
      $this->errors[] = 'Bad search pattern - ' . $patterns;
      return null;
    } 
    foreach ($terms as $term) {
       if ( is_array($term) ) {
         $searchTerm = $keyTerm = $term[0];
         if ( isset($term[1])) $keyTerm = $term[1];
       } else {
         $searchTerm = $keyTerm = $term;
       }
       if ( ( $count = self::check_match($searchTerm)) ) {
         if ( $count > $max_match ) {
           $max_match = $count;
           $match = $keyTerm;
           $this->matchedValue = $this->value;
         }
      }
    }
    if ( $match != null ) {
      if ( $trace ) echo "<p>FOUND $match</p>\n"; // DEBUG
      $this->tokenList->cur_word = $max_match;
    }
    return $match;
  }
  
  public function getMatchedTokens($reset = false) {
    static $prevIndex = 0;

    if ( $reset ) {
      $prevIndex = 0;
      return null;
    }
    $length = $this->tokenList->cur_word - $prevIndex;
    $retval = implode(' ', array_slice($this->tokenList->words, $prevIndex, $length ));
    $prevIndex = $this->tokenList->cur_word;
    $retval = preg_replace('/ ?\[.*?\] ?/', ' ', $retval );
    return $retval;
  }
  
  public function getValue() {
    $retVal = $this->matchedValue;
    $this->matchedValue = null;
    return $retVal;
  }
  
  public function getTokenOffset() {
    return $this->tokenList->cur_word;
  }
  
  public function lookahead($inc = 1) {
    $step = $inc > 0 ? 1 : -1;
    $inc = abs($inc);
    $here = $this->tokenList->cur_word;
    $min = 0;
    $max = $this->tokenList->num_tokens;
    
    while ( $inc > 0 ) {
      if ( $here <= $min or  $here >= $max ) break;
      $here += $step;
      if ( $this->tokenList->words[$here]{0} != '[' )
        $inc--;
    }
    $this->tokenList->cur_word = $here;
  }
  
  public function getTokens() {
    return $this->tokenList->words;
  }
  
  public function getMatcherErrors() {
    return $this->errors;
  }
}

