<?php

// include "utilities.inc";
include "tokeniser.inc";
include "matcher.inc";
include "lexicon.inc";
include "english/new-lexicon.inc";

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
    public $matched;

    function __construct($symbol, $value, $tokens, $lineNo) {
        $this->symbol = $symbol;
        $this->value = $value;
        $this->tokens = $tokens;
        $this->lineNo = $lineNo;
        $this->subItems = array();
        $this->matched = false;
    }
}

$grammar = [
// New symbol, array of symbols to match, ...?
[ 'treatment', 't-treatment2 t-colour ?t-andd t-colour'],
[ 'treatment', 't-colour t-treatment2 t-colour' ],
[ 'ordinary', 't-number t-ordinary | t-amount t-ordinary' ],
   //[ 'field', 'treatment | t-tincture'],
   // [ 'drawn-mod', 't-drawn ?t_andd'],
];

$parseTree = array();

while ($tokenList->moreInput()) {
    $longestMatch = 0;
    $possibles = [];
    foreach ($patternDB->listKeys() as $key) {
        $prev_word = $tokenList->cur_word;
        $match = $phraseMatcher->searchMatch($key);
        if ($match) {
          //  $tokens = $phraseMatcher->getMatchedTokens();
          //  $lineNo = $phraseMatcher->getLineNo();
            $matchLength = $tokenList->cur_word - $prev_word;
            if ($matchLength > $longestMatch) {
                $longestMatch = $matchLength;
                $possibles = [];
                $possibles[] = new parseTreeItem('t-' . $key, $match, $phraseMatcher->getMatchedTokens(), $phraseMatcher->getLineNo() );
            } elseif ($matchLength == $longestMatch) {
                $possibles[] = new parseTreeItem('t-' . $key, $match, $phraseMatcher->getMatchedTokens(), $phraseMatcher->getLineNo() );
            }
            $phraseMatcher->getMatchedTokens(true);
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

$tokenList = null;
$patternDB = null;
$phraseMatcher = null;


echo "Terminal Symbols:\n";
foreach($parseTree as $item) {
    printPTI($item);
}

function testReduction($symbolList, $startPoint) {
    global $parseTree;

    // recursion for alternatives
    $alternatives = preg_split('/\|/', $symbolList, -1, PREG_SPLIT_NO_EMPTY);
    if (count($alternatives) > 1) {
        foreach ($alternatives as $alternate) {
            if ($matched = testReduction($alternate, $startPoint))  {
                return $matched;
            }
        }
        return false;
    }

    $symbolPtr = 0;
    $treePtr = $startPoint;
    $matchedPTIs = 0;
    $symbols = preg_split('/\s+/', $symbolList, -1, PREG_SPLIT_NO_EMPTY);

    while (true) {
        if ($symbolPtr >= count($symbols)) { // all symbols match
            // if any of the matched PTIs were arrays, replace it by the matched array item
            for ($i = $startPoint; $i < $startPoint + $matchedPTIs; $i++) {
                if (is_array($parseTree[$i])) {
                    // echo "Picking matched item from array\n";
                    foreach ( $parseTree[$i] as $array_item) {
                        if ($array_item->matched == true) {
                            $newPTI = $array_item;
                            array_splice($parseTree, $i, 1, [$newPTI]);
                            break;
                        }
                    }
                } 
            }
            return $matchedPTIs; // only get here if all symbols match
        }
        if ($treePtr >= count($parseTree)) {
            return false; // not matched all symbols, but no input left
        }
        $optional = false;
        $symbolToMatch = $symbols[$symbolPtr];
        if ($symbolToMatch[0] == '?') {
            $optional = true;
            $symbolToMatch = substr($symbolToMatch,1);
        }
        // echo "Comparing (" . $parseTree[$treePtr]->symbol . ') & ('. $symbolToMatch . ")\n";
        // Compare the current two symbols
        $found = false;
        if (is_array($parseTree[$treePtr])) {
            foreach($parseTree[$treePtr] as $altPTI) { 
                if ($altPTI->symbol == $symbolToMatch) {
                    $altPTI->matched = true;
                    $found = true;
                } else {
                    $altPTI->matched = false;
                }
            }
        } else {
            if ($parseTree[$treePtr]->symbol == $symbolToMatch) {
                $found = true;
            }
        }
        $treeInc = 1;
        if (!$found) {
            //echo "No match at this location\n";
            if ($optional) {    // This is OK, just
                $treeInc = 0; // don't advance tree pointer
            } else {
                return false; 
            }
        }    
        $symbolPtr++;
        $treePtr += $treeInc; 
        $matchedPTIs += $treeInc;   
    }
    // shouldn't get here
    return false;
}

do {
    $numMatched = false;
    // For each entry in the list of grammar reductions
    $grammarLength = count($grammar);
    for ($gPtr = 0; $gPtr < $grammarLength; $gPtr++) {
        $newSymbol = $grammar[$gPtr][0];
        $symbolList = $grammar[$gPtr][1];
        // echo "Looking for $newSymbol\n";
        $parseTreeLength = count($parseTree);
        // Starting from each item still in the parse tree
        for ($pPtr = 0; $pPtr < $parseTreeLength; $pPtr++) {
            if ($numMatched = testReduction($symbolList, $pPtr)) {
                $linkedTokens = '';
                $linkedLines = '';
                for ($i = $pPtr; $i < $pPtr + $numMatched; $i++) {
                    $linkedTokens .= $parseTree[$i]->tokens . '+';
                    $linkedLines .= $parseTree[$i]->lineNo . ',';
                }
                $newPTI = new parseTreeItem($newSymbol, '', $linkedTokens, $linkedLines);
                $subItems = array_slice($parseTree, $pPtr, $numMatched );
                $newPTI->subItems = $subItems;
                array_splice($parseTree, $pPtr, $numMatched, [$newPTI]);
                break 2;
            }
           // echo $numMatched ? "true" : "false" . " $gPtr of $grammarLength $pPtr of $parseTreeLength \n";
        }
    }
    //break;
} while ($numMatched);

function printPTI($PTI, $indent = 0) {
    $prefix = str_repeat(' ', $indent);
    if (is_array($PTI)) {
        echo $prefix."[\n";
        foreach($PTI as $item) {
            printPTI($item, $indent + 2);
        }
        echo "$prefix]\n";
    } else {
        $match = $PTI->matched ? '[Y] ' : '[N] ';
        echo $prefix . $PTI->symbol . ' (' . $PTI->value . ') ' . $match . $PTI->tokens . ' ' . $PTI->lineNo . "\n";
        if (is_array($PTI->subItems)) {
            foreach ($PTI->subItems as $item) {
                printPTI($item, $indent + 4);
            }
        }
    }
}

echo "Final tree:\n";
foreach($parseTree as $item) {
     printPTI($item);
   // var_dump($item);
}
