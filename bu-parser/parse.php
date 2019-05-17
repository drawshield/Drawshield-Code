<?php

include "grammar.inc";
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
//var_dump($tokenList);
//exit;
$patternDB = new languageDB();

$phraseMatcher = new matcher($tokenList, $patternDB);

class parseTreeItem {
    public $category;
    public $keyword;
    public $tokens;
    public $subItems;
    public $lineNo;
    public $matched;
    public $optional;

    function __construct($category, $keyword, $tokens, $lineNo) {
        $this->category = $category;
        $this->keyword = $keyword;
        $this->tokens = $tokens;
        $this->lineNo = $lineNo;
        $this->subItems = array();
        $this->matched = false;
        $this->optional = false;
    }
}

$parseTree = array();

function minimiseNos($numbers) {
    $prevNum = $minimised = $numbers[0];
    for ($i = 1; $i < count($numbers); $i++) {
        if ($numbers[$i] != $prevNum) {
            $prevNum = $numbers[$i];
            $minimised .= ",$prevNum";
        }
    }
    return $minimised;
}

while ($tokenList->moreInput()) {
    $longestMatch = 0;
    $possibles = [];
    foreach ($patternDB->listKeys() as $key) {
        $prev_word = $tokenList->cur_word;
        $match = $phraseMatcher->searchMatch($key);
        if ($match) {
            $matchLength = $tokenList->cur_word - $prev_word;
            $tokens = implode(' ',array_slice($tokenList->words,$prev_word,$matchLength));
            $lineNos = minimiseNos(array_slice($tokenList->lineNos,$prev_word,$matchLength));
            if ($matchLength > $longestMatch) {
                $longestMatch = $matchLength;
                $possibles = [];
                $possibles[] = new parseTreeItem('t-' . $key, $match, $tokens, $lineNos );
            } elseif ($matchLength == $longestMatch) {
                $possibles[] = new parseTreeItem('t-' . $key, $match, $tokens, $lineNos );
            }
        }
        $tokenList->cur_word = $prev_word;
    }
    switch (count($possibles)) {
        case 0:
            $tokenList->cur_word += 1;
            $parseTree[] = new parseTreeItem('???', $tokenList->words[$tokenList->cur_word], '', $tokenList->lineNos[$tokenList->cur_word] );
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
$english = null;


/* echo "Terminal Symbols:\n";
foreach($parseTree as $item) {
    printPTI($item);
} */

function testReduction($symbolList, $startPoint) {
    global $parseTree;

    // recursion for alternatives
    if (is_array($symbolList)) {
        foreach ($symbolList as $alternate) {
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
    $repeatable = false;

    while (true) {
        $allOptional = false;
        if ($treePtr >= count($parseTree)) {
            // not an error if all remaining symbols are optional...
            $allOptional = true;
            for (; $symbolPtr < count($symbols); $symbolPtr++) {
                if ($symbols[$symbolPtr][0] != '?') {
                    $allOptional = false;
                    break;
                }
            }
            if (!$allOptional)
                return false; // not matched all symbols, but no input left
        }
        if ($allOptional || $symbolPtr >= count($symbols)) { // all symbols match
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
        $optional = false;
        $symbolToMatch = $symbols[$symbolPtr];
        if ($symbolToMatch[0] == '?') {
            $optional = true;
            $symbolToMatch = substr($symbolToMatch,1);
        }
        if (substr($symbolToMatch,-1) == '*') {
            if ($repeatable) {
                $optional = true;
            } else {
                $repeatable = true;
            }
            $symbolToMatch = substr($symbolToMatch,0,-1);
            echo $symbolToMatch . "\n";
        }
        // echo "Comparing (" . $parseTree[$treePtr]->symbol . ') & ('. $symbolToMatch . ")\n";
        // Compare the current two symbols
        $found = false;
        if (is_array($parseTree[$treePtr])) {
            foreach($parseTree[$treePtr] as $altPTI) { 
                if ($altPTI->category == $symbolToMatch) {
                    $altPTI->optional = $optional;
                    $altPTI->matched = true;
                    $found = true;
                } else {
                    $altPTI->matched = false;
                }
            }
        } else {
            if ($parseTree[$treePtr]->category == $symbolToMatch) {
                $parseTree[$treePtr]->optional = $optional;
                $found = true;
            }
        }
        $treeInc = 1;
        if (!$found) {
            //echo "No match at this location\n";
            if ($optional) {    // This is OK, just
                $repeatable = false;
                $treeInc = 0; // don't advance tree pointer
            } else {
                return false; 
            }
        }    
        if (!$repeatable) $symbolPtr++;
        $treePtr += $treeInc; 
        $matchedPTIs += $treeInc;   
    }
    // shouldn't get here
    return false;
}

$messageList = array();
$english = new grammar("english/blazonry.txt");
$grammarLength = count($english->reductions);
do {
    $numMatched = false;
    // For each entry in the list of grammar reductions
    for ($gPtr = 0; $gPtr < $grammarLength; $gPtr++) {
        $newSymbol = $english->reductions[$gPtr][0];
        $symbolList = $english->reductions[$gPtr][1];
        // echo "Looking for $newSymbol\n";
        $parseTreeLength = count($parseTree);
        // Starting from each item still in the parse tree
        for ($pPtr = 0; $pPtr < $parseTreeLength; $pPtr++) {
            if ($numMatched = testReduction($symbolList, $pPtr)) {
                $linkedTokens = '';
                $linkedLines = $parseTree[$pPtr]->lineNo;
                $endLine = $parseTree[$pPtr + $numMatched -1]->lineNo;
                if ($endLine != $linkedLines) {
                    $linkedLines .= "-$endLine";
                }
                for ($i = $pPtr; $i < $pPtr + $numMatched; $i++) {
                    $linkedTokens .= $parseTree[$i]->tokens . ' ';
                }
                $newPTI = new parseTreeItem($newSymbol, '', $linkedTokens, $linkedLines);
                $subItems = array_slice($parseTree, $pPtr, $numMatched );
                $newPTI->subItems = $subItems;
                array_splice($parseTree, $pPtr, $numMatched, [$newPTI]);
                if (count( $english->reductions[$gPtr] ) > 2) {
                    $messageList[] = $english->reductions[$gPtr][2] . ": $linkedTokens near $linkedLines";
                }
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
        if ( $PTI->optional ) {
            echo $prefix . '?(' . $PTI->category . '/' . $PTI->keyword . ' ' . $PTI->tokens . ' ' . $PTI->lineNo . ")?\n";
        } else {
             echo $prefix . $PTI->category . '/' . $PTI->keyword . ' ' . $PTI->tokens . ' ' . $PTI->lineNo . "\n";
        }
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

foreach($messageList as $message) {
    echo "$message\n";
}