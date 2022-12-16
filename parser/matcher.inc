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

class Lexeme
{
    public $category;
    public $keyterm;
    public $tokens;
    public $startPos;
    public $endPos;
    public $value;

    public function __construct($category, $keyterm, $tokens, $start, $end, $value = null)
    {
        $this->category = $category;
        $this->keyterm = $keyterm;
        $this->tokens = $tokens;
        $this->startPos = $start;
        $this->endPos = $end;
        $this->value = $value;
    }

}

class matcher
{
    public $furthestWord = 0;
    protected $tokenList;
    protected $stateStore;
    /**
     * @var lexicon $patternDB
     */
    protected $patternDB;
    protected $errors = array();
    protected $value = null;
    protected $matchedValue = null;
    protected $allMatches = array();
    protected $currentMatch = 0;
    protected $finalMatches = array();
    protected $currentFinal = -1;
    protected $matchedTokens = '';
    protected $numTokens;
    protected $curToken;

    // Constructor
    function __construct($tokenList, $patternDB)
    {
        $this->tokenList = $tokenList;
        $this->patternDB = $patternDB;
        $this->stateStore = new persistentStore();
        $this->numTokens = count($tokenList);
        $this->getAllMatches();
        // var_dump($this->allMatches);
        // exit();
    }

    public function getState()
    {
        return (array($this->currentMatch, $this->currentFinal, $this->matchedTokens));
    }

    public function restoreState($state)
    {
        if (!is_null($state) && is_array($state) && count($state) == 3) {
            $this->currentMatch = $state[0];
            $this->currentFinal = $state[1];
            $this->matchedTokens = $state[2];
        }
    }


    public function save()
    {
        return $this->stateStore->store(array($this->currentMatch, $this->currentFinal, $this->matchedTokens));
    }

    public function discard($state)
    {
        $this->stateStore->discard($state);
    }

    public function restore($state)
    {
        $data = $this->stateStore->restore($state);
        if (!is_null($data) && is_array($data) && count($data) == 3) {
            $this->currentMatch = $data[0];
            $this->currentFinal = $data[1];
            $this->matchedTokens = $data[2];
        }
    }

    public function flush()
    {
        $this->stateStore->flush();
    }

    protected function lexemeFromTokens($category, $keyterm, $first, $last = null, $value = null) {
        $tokens = $this->tokenList[$first]->text;
        $startPos = $this->tokenList[$first]->startPos;
        if (!is_null($last)) {
            if ($last >= $this->numTokens) $last = $this->numTokens - 1;
            $endPos = $this->tokenList[$last]->endPos;
            for ($i = $first + 1; $i <= $last; $i++) {
                $tokens .= ' ' . $this->tokenList[$i]->text;
            }
        } else {
            $endPos = $this->tokenList[$first]->endPos;
        }
        return new Lexeme($category, $keyterm, $tokens, $startPos, $endPos, $value);

    }

    protected function getAllMatches()
    {
        global $options, $startTime;
        $group = '';
        $partString = '';
        $partStart = 0;
        $partEnd = 0;
        $this->curToken = 0;

        $terms = $this->patternDB->patterns;
        while ($this->curToken < $this->numTokens) {
            // If the tokeniser has already identified this category, use that
            $givenCategory = $this->tokenList[$this->curToken]->category;
            if ($givenCategory != null) {
                $this->allMatches[] = array( $this->lexemeFromTokens($givenCategory, $givenCategory, $this->curToken, null, $this->tokenList[$this->curToken]->text));
                $this->curToken += 1; // move on for now
                continue;
            }
            $category = '';
            $start = $end = $this->curToken;
            $matches = array();
            $this->matchedvalue = null;

            $first_token = $this->curToken;
            if ($first_token >= $this->numTokens) return null;
            $max_match = 0;
            foreach ($terms as $term) {
                $term = ltrim($term);
                if ($term == '' || $term[0] == '#') continue;
                if ($term[0] == '=') { // this is a new category
                    if (strpos($term, '/') !== false) { // there is a group here
                        list($category, $group) = preg_split(':/:', $term);
                        $group .= '/';
                    } else {
                        $group = '';
                        $category = $term;
                    }
                    $category = substr($category, 1);
                    continue;
                }
                list($searchTerm, $keyTerm) = preg_split('/=/', $term);
                if ($keyTerm == '') $keyTerm = $searchTerm;
                if (($count = self::check_match($searchTerm))) {
                    // check_match updates cur_word on a successful match
                    $end = $this->furthestWord - 1;
                    $lexeme = $this->lexemeFromTokens($category, $group . $keyTerm, $start, $end, $this->value);
                    if ($count > 0 && $count == $max_match) {
                        $matches[] = $lexeme;
                    } elseif ($count > $max_match) {
                        $matches = array(); // discard previous
                        $matches[] = $lexeme;
                        $max_match = $count;
                    }
                }
            }
            if (count($matches) == 0) { // no match found
                if ($partString != '') {
                    $partString .= ' ';
                } else {
                    $partStart = $this->curToken;
                }
                $partEnd = $this->curToken;
                $partString .= $this->tokenList[$this->curToken]->text;
                $this->curToken += 1; // move on for now
            } else { // Match found
                if ($partString != "") { // Do we need to add previously found string?
                    $this->allMatches[] = Array($this->lexemeFromTokens("string", "unknown", $partStart, $partEnd, $partString));
                    $partString = '';
                }
                $this->curToken = $max_match;
                $this->allMatches[] = $matches;
            }
        }
        // last word may be a string, if so add it.
        if ($partString != "") {
            $partEnd = $this->curToken;
            $this->allMatches[] = Array ($this->lexemeFromTokens("string", "unknown", $partStart, $partEnd, $partString));
        }
        $this->currentMatch = 0; // reset counter for future search_match
        $this->patternDB->patterns = array(); // don't need this anymore, save the space
        if ($options['stage'] == 'lexemes') {
            foreach ($this->allMatches as $myMatch) {
                echo $myMatch[0]->tokens . "=";
                foreach ($myMatch as $possible) {
                    if (!is_null($possible->value)) {
                        $value = ' (' . $possible->value . ')';
                    } else {
                        $value = '';
                    }
                    echo $possible->category . ':' . $possible->keyterm . $value . '   ';
                }
                echo "\n";
            }
            echo "Execution time: " . microtime(true) - $startTime;
            exit(0);
        }
    }

    public function listLexemes() {
        $lexemeList = [];
        foreach ($this->allMatches as $myMatch) {
            $listItem =  $myMatch[0]->startPos . ' ' . $myMatch[0]->tokens . "=";
            foreach ($myMatch as $possible) {
                if (!is_null($possible->value)) {
                    $value = ' (' . $possible->value . ')';
                } else {
                    $value = '';
                }
                $listItem .=  $possible->category . ':' . $possible->keyterm . $value . '   ';
            }
            $lexemeList[] = $listItem;
        }
        return $lexemeList;
    }

    // Try to match each of the regular expressions in the array
    // with words in the input. Return 0 on fail, number of input
    // words matched on success
    // An initial '?' indicates match is optional
    protected function check_match($match_string)
    {
        if (!$match_string) {
            $this->errors[] = 'null match string';
            return false;
        }
        $this->value = null;

        $regexs = explode(' ', $match_string);

        $t_word = $this->curToken;
        foreach ($regexs as $regex) {
            if ($regex == '') continue;
            // Check for optionality
            $optional = false;
            if (substr($regex, 0, 1) == '?') {
                $optional = true;
                $regex = substr($regex, 1);
            }
            // Do we want the whole of the next token?
            // Optional & negative do not make sense here, so ignore
            if ($regex == '*' && isset($this->tokenList[$t_word])) {
                $this->value = $this->tokenList[$t_word]->text;
                $t_word++;
                continue;
            }
            // Check for negative
            $negative = false;
            if (substr($regex, 0, 1) == '^') {
                $negative = true;
                $regex = substr($regex, 1);
            }
            // Check for missing regex (not fatal)
            if (strlen($regex) < 1) {
                continue;
            }
            while (($t_word < $this->numTokens) && mb_substr($this->tokenList[$t_word]->match, 0, 1) == '[')
                $t_word++;
            if ($t_word >= $this->numTokens) { // no more input
                if ($optional or $negative) {
                    continue;
                } else {
                    return 0;
                }
            }
            if ($regex == '%') {
                $state = $this->curToken;
                $this->curToken = $t_word;
                $res = 0;
                if (ctype_digit($this->tokenList[$t_word]->match)) {
                    $this->value = intval($this->tokenList[$t_word]->match);
                    $res = 1;
                } else {
                    $numbers = array ( 1 => 'one', 2 => 'two', 3 => 'three', 4 => 'four',
                        5 => 'five', 6 => 'six', 7 => 'seven', 8 => 'eight', 9 => 'nine',
                        10 => 'ten', 11 => 'eleven', 12 => 'twelve', 13 => 'thirteen',
                        14 => 'fourteen', 15 => 'fifteen', 16 => 'sixteen', 17 => 'seventeen',
                        18 => 'eighteen', 19 => 'nineteen', 20 => 'twenty' );
                    if ($num = array_search($this->tokenList[$t_word]->match, $numbers) !== false) {
                        $this->value = $num;
                        $res = 1;
                    }
                }
                $this->curToken = $state;
            } else {
                if (ctype_alpha($regex)) { // small optimisation, just check for equality if it is just a plain word
                    $res = $regex == $this->tokenList[$t_word]->match ? 1 : 0;
                } else {
                    $res = @preg_match('/^' . $regex . '$/', $this->tokenList[$t_word]->match);
                }
            }
            if ($res === 0) { // no match
                if (!($optional || $negative)) {
                    return 0;
                } else
                    continue;
            }
            if ($res === false) {
                $this->errors[] = 'Bad regex - ' . $regex;
                return 0;
            }
            // else $res === 1
            if ($negative) { // Got a match, but didn't want it
                $this->value = null; // TODO rewrite as a try/catch block?
                return 0;
            }
            $t_word += 1; // Got a match, all OK, so move on
            if (!$optional and $t_word > $this->furthestWord)
                $this->furthestWord = $t_word;
        }
        // should only get here on a full match
        return $t_word;
    }


    public function matchLexeme($catList)
    {
        if ($this->currentMatch >= count($this->allMatches)) {
            return null;
        }
        if (!is_array($catList)) $catList = array($catList);
        $lexArray = $this->allMatches[$this->currentMatch];
        foreach ($catList as $category) {
            foreach ($lexArray as $lexeme) {
                if ($lexeme->category == $category) {
                    $this->finalMatches[] = $lexeme;
                    $this->currentFinal += 1;
                    $this->currentMatch += 1;
                    if ($this->matchedTokens != '') $this->matchedTokens .= ' ';
                    $this->matchedTokens .= $lexeme->tokens;
                    $this->matchedValue = $lexeme->value;
                    return $lexeme;
                }
            }
        }
        return null;
    }

    // Either sync TO a lexeme matching the category list
    // OR discard unknowns, strings and punctuation
    // return true if a valid lexeme is available, false otherwise
    public function syncOver($catList = null, $only = false) {
        if ($only) {
            $checkList = [];
        } else { // automatically add in some usual stuff
            $checkList = array(languageDB::COMMA, languageDB::COLON, languageDB::PERIOD);
        }
        if (is_array($catList)) {
            $checkList = array_merge($checkList, $catList);
        } elseif (is_string($catList)) {
            $checkList[] = $catList;
        }
        while ($this->currentMatch < count($this->allMatches)) {
            $lexArray = $this->allMatches[$this->currentMatch];
            foreach ($lexArray as $lexeme) {
                $category = $lexeme->category;
                // silently ignore punctuation
                if (in_array($category, $checkList)) {
                    $this->currentMatch += 1;
                    continue 2;
                }
                if ($category == "string") {
                    switch ($lexeme->keyterm) {
                        case "string":
                            $this->errors[] = "String not valid ({$lexeme->value}) at {$lexeme->startPos}-{$lexeme->endPos}";
                            break;
                        case "unknown":
                            $this->errors[] = "Unknown word ({$lexeme->tokens})  at {$lexeme->startPos}-{$lexeme->endPos}";
                            break;
                    }
                    $this->currentMatch += 1;
                    continue 2;
                }
            }
            break;
        }
    }

    public function syncTo($catList) {
        $skipList = '';
        $skipStart = '';
        $skipEnd = '';
        while ($this->currentMatch < count($this->allMatches)) {
            foreach ($catList as $category) {
                $lexArray = $this->allMatches[$this->currentMatch];
                foreach ($lexArray as $lexeme) {
                    if ($category == $lexeme->category) {
                        $this->errors[] = "Skipping: $skipList ($skipStart-$skipEnd)";
                        return true;
                    }
                }
            }
            if ($skipList == '') $skipStart = $lexArray[0]->startPos;
            $skipEnd = $lexArray[0]->endPos;
            $skipList .= $lexArray[0]->tokens . ' ';
            $this->currentMatch += 1;
        }
        if ($skipList != '') {
            $this->errors[] = "Not able to process: $skipList ($skipStart-$skipEnd)";
        }
        return ($this->currentMatch < count($this->allMatches));
    }

    public function prevMatch($category)
    { // match the previously found lexeme
        if ($this->currentFinal < 0) return null;
        $lexeme = $this->finalMatches[$this->currentFinal];
        if ($lexeme->category == $category) {
            return $lexeme->keyterm;
        }
        $lexeme = $this->finalMatches[count($this->finalMatches) - 1];
        if ($lexeme->category == $category) {
            return $lexeme->keyterm;
        }
        return null;
    }

    public function putBack($category)
    {
        if ($this->currentFinal < 0) return null;
        $lexeme = $this->finalMatches[$this->currentFinal];
        if ($lexeme->category == $category) {
            array_pop($this->finalMatches);
            $this->matchedTokens = '';
            $this->currentFinal -= 1;
            return true;
        }
        return null;
    }

    // public function searchDigits() {
    //   $index = $this->tokenList->cur_word;
    //   $word = $this->tokenList->words[$index];
    //   if (is_numeric($word)) {
    //     $val = intval($word);
    //     $this->value = $val;
    //     $this->tokenList->cur_word = $index + 1;
    //     return "$val";
    //   }
    //   return false;
    // }


    public function getMatchedTokens($reset = false)
    {
        if ($reset) return null;
        if ($this->currentFinal > count($this->finalMatches)) return null;
        $retval = $this->matchedTokens;
        $this->matchedTokens = '';
        return $retval;
    }

    public function getLineNo()
    {
        if ($this->currentFinal < 0 || $this->currentFinal > count($this->finalMatches)) return null;
        $lexeme = $this->finalMatches[$this->currentFinal];
        $start = $lexeme->startPos;
        $end = $lexeme->endPos;
        if ($start == $end)
            return "$start";
        else
            return "$start-$end";
    }

    public function getValue()
    {
        return $this->matchedValue;
    }

    public function getTokenOffset()
    {
        return $this->tokenList->cur_word;
    }

    public function getMatcherErrors()
    {
        return $this->errors;
    }

    public function moreInput()
    {
        return ($this->currentMatch < count($this->allMatches));
    }
}

