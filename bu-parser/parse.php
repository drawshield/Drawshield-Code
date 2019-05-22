<?php

include "utilities.inc";
include "grammar.inc";
include "terminals.inc";

$blazon = implode(' ', array_slice($argv,1));
if ($blazon == '') {
    $blazon = file_get_contents("blazon.txt");
}

$xml = new blazonML();

$terminalMaker = new terminals('english', $blazon, $xml);

$parseTree = $terminalMaker->getTerminals();

$terminalMaker = null;



echo "Terminal Symbols:\n";
foreach($parseTree as $item) {
    $xml->prettyPrint($item);
} 

    
function firstAndLast($str) {
    $nums = explode('-',$str);
    return $nums[0] . '-' . $nums[count($nums)-1];
}



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
    $repeatable = 0;

    while (true) {
        $allOptional = false;
        if ($treePtr >= count($parseTree)) {
            // not an error if all remaining symbols are optional...
            $allOptional = true;
            for (; $symbolPtr < count($symbols); $symbolPtr++) {
                if ($symbols[$symbolPtr][0] != '?' && $repeatable < 1) {
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
                        if ($array_item->hasAttribute("matched")) {
                            $newPTI = $array_item;
                            array_splice($parseTree, $i, 1, [$newPTI]);
                            break;
                        }
                    }
                } 
            }
            return $matchedPTIs; // only get here if all symbols match
        }
        if (!$repeatable) { // process flags
            $symbolToMatch = $symbols[$symbolPtr];
            $optional = (strpos($symbolToMatch[0],'?') !== false );
            $repeatable = (strpos($symbolToMatch,'*') !== false ) ? 1 : 0;
            $discard =  (strpos($symbolToMatch,'^') !== false);
            $symbolToMatch = str_replace(array('<','>','/','?','^','*'),'',$symbolToMatch);
        }
        // echo "Comparing (" . $parseTree[$treePtr]->symbol . ') & ('. $symbolToMatch . ")\n";
        // Compare the current two symbols
        $found = false;
        if (is_array($parseTree[$treePtr])) {
            foreach($parseTree[$treePtr] as $altPTI) { 
                if ($altPTI->nodeName == $symbolToMatch) {
                    if ($optional) $altPTI->setAttribute("optional","true");
                    $altPTI->setAttribute("matched","true");
                    $found = true;
                } else {
                    if ($discard) $altPTI->setAttribute("discard","true");
                }
            }
        } else {
            if ($parseTree[$treePtr]->nodeName == $symbolToMatch) {
                if ($optional) $parseTree[$treePtr]->setAttribute("optional","true");
                if ($discard) $parseTree[$treePtr]->setAttribute("discard","true");
                $found = true;
            }
        }
        $treeInc = 1;
        if (!$found) {
            //echo "No match at this location\n";
            if ($optional || $repeatable > 1) {    // This is OK, just
                $treeInc = 0; // don't advance tree pointer
                $repeatable = 0;
            } else {
                return false; 
            }
        }    // either found, or optional
        if ($repeatable) {
            $repeatable++; // don't advance pointer, increase count
        } else {
            $symbolPtr++;
        }
        $treePtr += $treeInc; 
        $matchedPTIs += $treeInc;   
    }
    // shouldn't get here
    return false;
}

function getSubItems($pPtr, $numMatched) {
    global $parseTree;

    $subItems = array();
    for ($i = 0; $i < $numMatched; $i++) {
        if (!$parseTree[$pPtr]->hasAttribute("discard"))
            $subItems[] = $parseTree[$pPtr];
        $pPtr++;
    }
    return $subItems;
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
                $linkedLines = $parseTree[$pPtr]->getAttribute(blazonML::A_LINENUMBER);
                $endLine = $parseTree[$pPtr + $numMatched -1]->getAttribute(blazonML::A_LINENUMBER);
                if ($endLine != $linkedLines) {
                    $linkedLines = firstAndLast("$linkedLines-$endLine");
                }
                for ($i = $pPtr; $i < $pPtr + $numMatched; $i++) {
                    if ($parseTree[$i]->getAttribute(blazonML::A_TOKENS)) {
                        $linkedTokens .= $parseTree[$i]->getAttribute(blazonML::A_TOKENS) . ' ';
                    }
                }
                $linkedTokens = trim($linkedTokens);
                $newPTI = $xml->makeNode($newSymbol, '', $linkedTokens, $linkedLines);
                foreach (getSubItems($pPtr, $numMatched ) as $subItem) 
                    $newPTI->appendChild($subItem);
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

echo "Final tree:\n";
foreach($parseTree as $item) {
    $xml->prettyPrint($item);
   // var_dump($item);
}

foreach($messageList as $message) {
    echo "$message\n";
}