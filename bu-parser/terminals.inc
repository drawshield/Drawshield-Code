<?php
include "tokeniser.inc";
include "matcher.inc";
include "lexicon.inc";
include "english/lexicon.inc";


class terminals {
    private $phraseMatcher;
    private $tokenList;
    private $xml;
    private $patternDB;

    public function __construct($language, $blazon, $xml) {
        $this->tokenList = new tokeniser($blazon);
        $this->patternDB = new languageDB($language);
        $this->phraseMatcher =new matcher($this->tokenList, $this->patternDB);
        $this->xml = $xml;
    }

    protected function minimiseNos($numbers) {
        $prevNum = $minimised = $numbers[0];
        for ($i = 1; $i < count($numbers); $i++) {
            if ($numbers[$i] != $prevNum) {
                $prevNum = $numbers[$i];
                $minimised .= ",$prevNum";
            }
        }
        return $minimised;
    }

    public function getTerminals() {
        $parseTree = [];

        while ($this->tokenList->moreInput()) {
            $longestMatch = 0;
            $possibles = [];
            foreach ($this->patternDB->listKeys() as $key) {
                $prev_word = $this->tokenList->cur_word;
                $match = $this->phraseMatcher->searchMatch($key);
                if ($match) {
                    $matchLength = $this->tokenList->cur_word - $prev_word;
                    $tokens = '';
                    for ($i = 0; $i < $matchLength; $i++ ) {
                        if ($this->tokenList->words[$prev_word + $i]) {
                            $tokens .= $this->tokenList->words[$prev_word + $i] . ' ';
                        }
                    }
                    $tokens = trim($tokens);
                    $lineNos = $this->minimiseNos(array_slice($this->tokenList->lineNos,$prev_word,$matchLength));
                    if ($matchLength > $longestMatch) {
                        $longestMatch = $matchLength;
                        $possibles = [];
                        $possibles[] = $this->xml->makeNode('t-' . $key, [blazonML::A_KEYTERM => $match], $tokens, $lineNos );
                    } elseif ($matchLength == $longestMatch) {
                        $possibles[] = $this->xml->makeNode('t-' . $key, [blazonML::A_KEYTERM => $match], $tokens, $lineNos );
                    }
                }
                $this->tokenList->cur_word = $prev_word;
            }
            switch (count($possibles)) {
                case 0:
                    $this->tokenList->cur_word += 1;
                    $parseTree[] = $this->xml->makeNode('t-unknown',[blazonML::A_KEYTERM => $this->tokenList->words[$this->tokenList->cur_word]], '',    $this->tokenList->lineNos[$this->tokenList->cur_word] );
                    break;
                case 1: 
                    $parseTree[] = $possibles[0];
                    $this->tokenList->cur_word += $longestMatch;
                    break;
                    default: // > 1 possible
                    $parseTree[] = $possibles;
                    $this->tokenList->cur_word += $longestMatch;
                    break;
            }
        }
        return $parseTree;
    }
    
    
}