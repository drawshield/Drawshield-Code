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
$tokens = $terminalMaker->getTokens();

$terminalMaker = null; // discard terminal maker as no longer needed

// echo "Terminal Symbols:\n";
//  foreach($parseTree as $item) {
//     //  $xml->showXML($item);
//      $xml->prettyPrint($item);
//  }

$ruleset = new productions("english", $xml);
$parseTree = $ruleset->applyRules($parseTree);

$root = $xml->createElement('blazon');
$root->setAttribute('blazonText', urlencode($blazon));
$root->setAttribute('tokens', implode(' ',$tokens));
$root->appendChild($parseTree[0]);
$xml->appendChild($root);

// echo "Final tree:\n";
$xml->formatOutput = true;
//$xml->showXML($root);
foreach($parseTree as $item) {
    // $xml->prettyPrint($item);
    $xml->showXML($item);
    // var_dump($item);
}

foreach($ruleset->getMessages() as $message) {
    echo "$message\n";
}

$ruleset = null; // discard production rules, no longer needed

