<?php

class grammar {
    public $productions;
    public $numProductions;
    private $fileHandle;

    protected function getNextLine() {
        if (($line = fgets($this->fileHandle)) !== false) {
            if (($pos = strpos($line, '#')) !== false) {
                $line = substr($line,0,$pos); // strip comments
            }
            $line = trim($line);
        }
        return $line;
    }

    protected function getFullLine() {
        $fullLine = false;
        while (($tempLine = $this->getNextLine()) !== false) {
            if ($tempLine == '') continue; // skip blanks
            $fullLine .= $tempLine;
            if (substr($tempLine,-1,1) == '\\') { // add continuation
                $fullLine = rtrim($fullLine,'\\');
            } else {
                break;
            }
        }
        return $fullLine;
    }

    function __construct($filename) {
        $this->fileHandle = fopen($filename, "r");

        $this->productions = array();

        while (($line = $this->getFullLine()) !== false) {
            $parts = preg_split('/\s*::=\s*/',$line);
            $message = false;
            if (count($parts) == 2) {
                if (($pos = strpos($parts[1], '!')) !== false) {
                    $message = substr($parts[1],$pos);
                    $parts[1] = substr($parts[1],0,$pos - 1);
                }
                $symbol = $parts[0];
                if (strpos($parts[1],'|') !== false) {
                    $terminals = preg_split('/\s*\|\s*/', $parts[1]);
                } else {
                    $terminals = $parts[1];
                }
                if ($message) {
                    $this->productions[] = [$symbol, $terminals, $message];
                } else {
                    $this->productions[] = [$symbol, $terminals];
                }
            } else {
                echo "Warning - bad grammar $line";
            }
        }

        $this->numProductions = count($this->productions);
    }
}