<?php

$testcases = "testscases/";
$expected = "expected/";
$responses = "responses/";

function getTest($filename) {
    global $testcase;

    $args = [];
    $arg = '';
    $content = file($testcase . $filename);
    foreach ($content as $line) {
        if ($line[0] == '#') continue;
        $arg .= $line;
        if (substr($line,-1,1) == '\\') {
            rtrim($arg,'\\');
            continue;
        }
        $eq = strpos($arg,'=');
        if ($eq === false) {
            echo "Warning, no separator in argument: $arg\n";
            continue;
        }
        list($name,$value) = explode('=', $arg);
        $args[$name] = $value;
    }

}
