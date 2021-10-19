<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see https://www.gnu.org/licenses/. */

include "tests.inc";
$curlHandle = null;

function getTest($filename) {
    global $testcases;

    $args = [];
    $arg = '';
    $content = file($testcases . $filename);
    foreach ($content as $line) {
        $line = trim($line);
        if ($line == '' || $line[0] == '#') continue;
        $arg .= $line;
        if (substr($line,-1,1) == '\\') {
            $arg = rtrim($arg,'\\');
            continue;
        }
        $eq = strpos($arg,'=');
        if ($eq === false) {
            echo "Warning, no separator in argument: $arg\n";
            continue;
        }
        list($name,$value) = explode('=', $arg);
        $args[$name] = $value;
        $arg = '';
    }
    return $args;

}

function initCurl() {
    global $url, $target, $curlHandle;

    $curlHandle = curl_init();
    curl_setopt($curlHandle, CURLOPT_URL, $url . $target);
    curl_setopt($curlHandle, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($curlHandle, CURLOPT_HEADER, true);
    curl_setopt($curlHandle, CURLOPT_POST, true);
    curl_setopt($curlHandle, CURLOPT_CONNECTTIMEOUT, 10);
    curl_exec($curlHandle);
    $responseCode = curl_getinfo($curlHandle, CURLINFO_HTTP_CODE);
    if ($responseCode != "200") {
        echo "Error response from server - is it running? $responseCode\n";
        exit(1);
    }
    curl_setopt($curlHandle, CURLOPT_HEADER, false);
}

function getResponse($args) {
    global $curlHandle;

    curl_setopt($curlHandle, CURLOPT_POSTFIELDS, $args);
    $return = curl_exec($curlHandle);

    return $return;
}

function doCompare($basename) {
    global $expected, $responses, $resultFile;

    if (!file_exists($expected . $basename . ".svg"))
        return "missing expected file";
    $command = "diff -I \"timestamp\" -I \"release-id\" -I \"licence\" \"$expected$basename.svg\" \"$responses$basename.svg\"";
    exec ($command, $output, $return);
    if ($return == 0) {
        return '';
    } else {
        file_put_contents($resultFile, implode("\n", $output), FILE_APPEND);
    }
    return $output[0] . " - " . $output[1];
}

function startswith($string, $start) {
    $len = strlen($start);
    return (substr($string, 0, $len) === $start);
}

function refresh($match = null) {
    global $expected, $testcases;

    $cases = scandir($testcases);
    foreach ($cases as $key => $case) {
        if ($case[0] == '.') continue;
        if ($match != '' && !startswith($case, $match)) continue;
        if (substr($case, -4) != ".txt") continue;
        $basename = substr($case,0,-4);
        echo $basename . ": ";
        $response = getResponse(getTest($case));
        file_put_contents($expected . $basename . ".svg", $response);
        echo "CREATED";
        echo "\n";
        sleep(1);
    }
}

function runtests($match = null) {
    global $expected, $responses, $testcases;

    $cases = scandir($testcases);
    foreach ($cases as $key => $case) {
        if ($case[0] == '.') continue;
        if ($match != '' && !startswith($case, $match)) continue;
        if (substr($case, -4) != ".txt") continue;
        $basename = substr($case,0,-4);
        echo $basename . ": ";
        $response = getResponse(getTest($case));
        file_put_contents($responses . $basename . ".svg", $response);
        $result = doCompare($basename);
        if ($result == '')
            echo "PASSED";
        else
            echo "FAILED - $result";
        echo "\n";
        sleep(1);
    }
}

initCurl();
$refresh = false;
$matches = [ null ];

if ($argc > 1) {
    if ($argv[1] == "-refresh") {
        $refresh = true;
        if ($argc > 2) {
            $matches = array_slice($argv, 2);
        }
    } else {
        $matches = array_slice($argv,1);
    }
}

foreach ($matches as $match) {
    if ($refresh)
        refresh($match);
    else
        runtests($match);
}

