<?php

include "utilities.inc";
include "terminals.inc";
include "productions.inc";

$blazon = implode(' ', array_slice($argv,1));
if ($blazon == '') {
    $blazon = file_get_contents("blazon.txt");
}

$xml = new blazonML();

$terminalMaker = new terminals('english', $blazon, $xml);

$parseTree = $terminalMaker->getTerminals();

$terminalMaker = null; // discard terminal maker as no longer needed

/* echo "Terminal Symbols:\n";
foreach($parseTree as $item) {
    $xml->prettyPrint($item);
}  */

$ruleset = new productions("english", $xml);
$parseTree = $ruleset->applyRules($parseTree);

echo "Final tree:\n";
foreach($parseTree as $item) {
    $xml->prettyPrint($item);
    // var_dump($item);
}

foreach($ruleset->getMessages() as $message) {
    echo "$message\n";
}

$ruleset = null; // discard production rules, no longer needed
