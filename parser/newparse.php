<?php

// include "utilities.inc";
include "tokeniser.inc";
include "matcher.inc";
include "lexicon.inc";
include "english/lexicon.inc";

function transliterate($string) {
    return preg_replace(array('/è/','/é/','/ê/','/ç/','/à/','/á/','/â/','/È/','/É/','/Ê/','/Ç/','/À/','/Á/','/Â/'),
                                 array('e','e','e','c','a','a','a','e','e','e','c','a','a','a'), $string);
}

$blazon = implode(' ', array_slice($argv,1));;

$tokenList = new tokeniser($blazon);
$patternDB = new languageDB();

$phraseMatcher = new matcher($tokenList, $patternDB);

class parseTreeItem {
    public $symbol;
    public $value;
    public $tokens;
    public $subItems;
    public $lineNo;

    function __construct($symbol, $value, $tokens, $lineNo) {
        $this->symbol = $symbol;
        $this->value = $value;
        $this->tokens = $tokens;
        $this->lineNo = $lineNo;
        $this->subItems = array();
    }
}

$grammar = [
// New symbol, array of symbols to match, ...?
   [ 'treatment', ['t-tincture', 't-treatment', 't-tincture'] ],
   [ 'treatment', ['t-treatment', 't-tincture', '?t-andd', 't-tincture'] ],
   [ 'field', ['treatment'] ],
   [ 'field', ['t-tincture'] ],
];

$parseTree = array();

while ($tokenList->moreInput()) {
    $longestMatch = 0;
    $possibles = [];
    foreach ($patternDB->listKeys() as $key) {
        $prev_word = $tokenList->cur_word;
        $match = $phraseMatcher->searchMatch($key);
        if ($match) {
            $matchLength = $tokenList->cur_word - $prev_word;
            if ($matchLength > $longestMatch) {
                $longestMatch = $matchLength;
                $possibles = [];
                $possibles[] = new parseTreeItem('t-' . $key, $match, $phraseMatcher->getMatchedTokens(), $phraseMatcher->getLineNo() );
            } elseif ($matchLength == $longestMatch) {
                $possibles[] = new parseTreeItem('t-' . $key, $match, $phraseMatcher->getMatchedTokens(), $phraseMatcher->getLineNo() );
            }
        }
        $tokenList->cur_word = $prev_word;
    }
    switch (count($possibles)) {
        case 0:
            $tokenList->cur_word += 1;
            $parseTree[] = new parseTreeItem('???','', $phraseMatcher->getMatchedTokens(), $phraseMatcher->getLineNo() );
            break;
        case 1: 
            $parseTree[] = $possibles[0];
            $tokenList->cur_word += $longestMatch;
            break;
            default: // > 1 possible
            $parseTree[] = $possibles;
            $tokenList->cur_word += $longestMatch;
            break;
    }
}
// var_dump($parseTree);

// For each entry in the list of grammar reductions
for ($gPtr = 0; $gPtr < count($grammar); $gPtr++) {
    $newSymbol = $grammar[$gPtr][0];
    $symbolsToMatch = $grammar[$gPtr][1];
    // Starting from each item still in the parse tree
    for ($pPtr = 0; $pPtr < count($parseTree); $pPtr++) {
        // cross compare symbols
        $startP = $endP = $pPtr;
        for ($sPtr = 0; $sPtr < count($symbolsToMatch) && $endP < count($parseTree); $sPtr++ ) {
            $optional = false;
            $symbolToMatch = $symbolsToMatch[$sPtr];
            if ($symbolToMatch[0] == '?') {
                $optional = true;
                $symbolToMatch = substr($symbolToMatch,1);
            }
            // Compare the current two symbols
            if ($parseTree[$endP]->symbol != $symbolToMatch) {
                break 2; // NO match AT THIS POINT
            }
            $endP++;
        }
        // Got here, so must have been a full match
        // replace the matched symbols with a new PTI
        $linkedTokens = '';
        $linkedLines = '';
        for ($i = $startP; $i < $endP; $i++) {
            $linkedTokens .= $parseTree[$i]->tokens . ' ';
            $linkedLines .= $parseTree[$i]->lineNo . ',';
        }
        $newPTI = new parseTreeItem($newSymbol, '', $linkedTokens, $linkedLines);
        $newPTI->subItems = array_splice($parseTree, $startP, $endP - $startP, $newPTI );
        // Just for now...
        break 2;
    }
    // if here, didn't match current reduction
}
// If here, didn't match anything at all

var_dump($parseTree);
