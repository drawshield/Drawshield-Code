<?php

include "grammar.inc";

class productions {
    private $parseTree;
    private $ruleset;
    private $messages;
    private $xml;
    private $fixups;

    public function __construct($language, $xml) {
        $this->ruleset = new grammar ("$language/blazonry.txt"); // should be called ruleset.txt?
        include "$language/fixups.inc";
        $this->fixups = new fixups($xml);
        $this->messages = array();
        $this->xml = $xml;
    }

    protected function getSubItems($pPtr, $numMatched) {
    
        $subItems = array();
        for ($i = 0; $i < $numMatched; $i++) {
            if (!$this->parseTree[$pPtr]->hasAttribute("discard"))
                $subItems[] = $this->parseTree[$pPtr];
            $pPtr++;
        }
        return $subItems;
    }

    public function getMessages() {
        return $this->messages;
    }

    protected function firstAndLast($str) {
        $nums = explode('-',$str);
        return $nums[0] . '-' . $nums[count($nums)-1];
    }
    
    protected function testProduction($symbolList, $startPoint) {
    
        // recursion for alternatives
        if (is_array($symbolList)) {
            foreach ($symbolList as $alternate) {
                if ($matched = $this->testProduction($alternate, $startPoint))  {
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
            if ($treePtr >= count($this->parseTree)) {
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
                return $matchedPTIs; // only get here if all symbols match
            }
            if (!$repeatable) { // process flags
                $symbolToMatch = $symbols[$symbolPtr];
                $optional = (strpos($symbolToMatch[0],'?') !== false );
                $repeatable = (strpos($symbolToMatch,'*') !== false ) ? 1 : 0;
                $symbolToMatch = str_replace(array('<','>','/','?','^','*','='),'',$symbolToMatch);
            }
            // echo "Comparing (" . $this->parseTree[$treePtr]->symbol . ') & ('. $symbolToMatch . ")\n";
            // Compare the current two symbols
            $found = false;
            if (is_array($this->parseTree[$treePtr])) {
                foreach($this->parseTree[$treePtr] as $altPTI) { 
                    if ($altPTI->nodeName == $symbolToMatch) {
                        $found = true;
                    }
                }
            } else {
                if ($this->parseTree[$treePtr]->nodeName == $symbolToMatch) {
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


    protected function applyProduction($symbols, $startPoint, $matchedPTIs) {
   
        $symbolPtr = 0;
        $treePtr = $startPoint;
        $copyToParent = false;
        $repeatable = 0;
    
        while ($symbolPtr < count($symbols)) {
            if ($treePtr >= count($this->parseTree)) {
                // if any of the matched PTIs were arrays, replace it by the matched array item
                for ($i = $startPoint; $i < $startPoint + $matchedPTIs; $i++) {
                    if (is_array($this->parseTree[$i])) {
                        // echo "Picking matched item from array\n";
                        $types = '';
                        foreach ( $this->parseTree[$i] as $array_item) {
                            $types .= substr($array_item->nodeName,2) . ' ';
                            if ($array_item->hasAttribute("matched")) {
                                $newPTI = $array_item;
                            }
                        }
                        $newPTI->setAttribute("types",$types);
                        array_splice($this->parseTree, $i, 1, [$newPTI]);
                    } 
                }
                return;
            }


             if (!$repeatable) { // process flags
                $symbolToMatch = $symbols[$symbolPtr];
                $optional = (strpos($symbolToMatch[0],'?') !== false );
                $repeatable = (strpos($symbolToMatch,'*') !== false ) ? 1 : 0;
                $discard =  (strpos($symbolToMatch,'^') !== false);
                $copyToParent = (strpos($symbolToMatch,'=') !== false);
                $symbolToMatch = str_replace(array('<','>','/','?','^','*','='),'',$symbolToMatch);
            }
           // Compare the current two symbols
            $found = false;
            if (is_array($this->parseTree[$treePtr])) {
                foreach($this->parseTree[$treePtr] as $altPTI) { 
                    if ($altPTI->nodeName == $symbolToMatch) {
                        $altPTI->setAttribute("matched","true");
                        if ($copyToParent) {
                            $altPTI->setAttribute("copy","true");
                            $altPTI->setAttribute("discard","true");
                        }
                        $found = true;
                    } else {
                        if ($discard) $altPTI->setAttribute("discard","true");
                    }
                }
            } else {
                if ($this->parseTree[$treePtr]->nodeName == $symbolToMatch) {
                    if ($discard) $this->parseTree[$treePtr]->setAttribute("discard","true");
                    if ($copyToParent) {
                        $this->parseTree[$treePtr]->setAttribute("copy","true");
                        $this->parseTree[$treePtr]->setAttribute("discard","true");
                    }
                    $found = true;
                }
            }
            $treeInc = 1;
            if (!$found) {
                //echo "No match at this location\n";
                if ($optional || $repeatable > 1) {    // This is OK, just
                    $treeInc = 0; // don't advance tree pointer
                    $repeatable = 0;
                } 
            }    // either found, or optional
            if ($repeatable) {
                $repeatable++; // don't advance pointer, increase count
            } else {
                $symbolPtr++;
            }
            $treePtr += $treeInc; 
        }
        return;
    }

  
    public function applyRules( $parseTree ) {
        $this->parseTree = $parseTree;

        do {
            // For each entry in the list of grammar productions
            $matchedSymbols = null;
            $newSymbol = '';
            $longestMatch = 0;
            $startPoint = 0;
            $message = null;
            for ($gPtr = 0; $gPtr < $this->ruleset->numProductions; $gPtr++) {
                $symbolList = $this->ruleset->productions[$gPtr][1];
                if (!is_array($symbolList)) {
                    $symbolList = [$symbolList];
                }
                foreach ($symbolList as $alternate) {
                    for ($pPtr = 0; $pPtr < count($this->parseTree); $pPtr++) {
                        if (($numMatched = $this->testProduction($symbolList, $pPtr)) > $longestMatch) {
                            $longestMatch = $numMatched;
                            $matchedSymbols = $symbolList;
                            $newSymbol = $this->ruleset->productions[$gPtr][0];
                            $startPoint = $pPtr;
                            if (count( $this->ruleset->productions[$gPtr] ) > 2) {
                                $message = $this->ruleset->productions[$gPtr][2];
                            } else {
                                $message = '';
                            }

                        }
                    }
                }
            }

            if ($matchedSymbols) {  // we have an expansion
                $copyAttributes = strpos($newSymbol,'@') === false;
                $newSymbol = str_replace('@','',$newSymbol);
                $symbolList = $matchedSymbols;
                // Mark up the sub-nodes, as per the flags in the symbol list
                $this->applyProduction($symbolList, $startPoint, $longestMatch);      

                $params = [];
                $linkedTokens = '';
                $linkedLines = '';
                if ($copyAttributes) {
                    $linkedLines = $this->parseTree[$startPoint]->getAttribute(blazonML::A_LINENUMBER);
                    $endLine = $this->parseTree[$startPoint + $longestMatch -1]->getAttribute(blazonML::A_LINENUMBER);
                    if ($endLine != $linkedLines) {
                        $linkedLines = $this->firstAndLast("$linkedLines-$endLine");
                    }
                }
                for ($i = $startPoint; $i < $startPoint + $longestMatch; $i++) {
                    if ($copyAttributes) {
                        if ($this->parseTree[$i]->getAttribute(blazonML::A_TOKENS)) {
                            $linkedTokens .= $this->parseTree[$i]->getAttribute(blazonML::A_TOKENS) . ' ';
                        }
                        if ($this->parseTree[$i]->hasAttribute("copy")) {
                            $params[blazonML::A_KEYTERM] = $this->parseTree[$i]->getAttribute(blazonML::A_KEYTERM);
                            if ($this->parseTree[$i]->hasAttribute(blazonML::A_PARAM)) {
                                $params[blazonML::A_PARAM] = $this->parseTree[$i]->getAttribute(blazonML::A_PARAM);
                            }
                            if ($this->parseTree[$i]->hasAttribute("types")) {
                                $params["types"] = $this->parseTree[$i]->getAttribute("types");
                            }
                        }
                    }
                }
                $linkedTokens = trim($linkedTokens);
                if (strpos($newSymbol,'^') === false ) {
                    $newPTI = $this->xml->makeNode($newSymbol,  $params, $linkedTokens, $linkedLines);
                    foreach ($this->getSubItems($startPoint, $longestMatch ) as $subItem) 
                        $newPTI->appendChild($subItem);
                    if (method_exists($this->fixups, $newSymbol)) {
                        $newPTI = $this->fixups->$newSymbol($newPTI);
                    }
                    array_splice($this->parseTree, $startPoint, $longestMatch, [$newPTI]);
                } else {
                    array_splice($this->parseTree, $startPoint, $longestMatch);
                }
                if ($message) {
                    $this->messages[] =
                        $this->ruleset->productions[$gPtr][2] . ": $linkedTokens near line $linkedLines";
                }
            }
        } while ($matchedSymbols);
        return $this->parseTree;
    }
    
    
}