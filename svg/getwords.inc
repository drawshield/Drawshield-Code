<?php

function getWords($fontName,$text,$height, $width) {
    //global $messages;

    if (!file_exists("svg/fonts/$fontName.svg")) {
        //$messages->addMessage('internal',"Unknown font - $fontName");
        echo "No File\n";
        return null;
    }
    $defaultXSize = 500;
    $ySize = 500;
    $glyphs = [];

    libxml_use_internal_errors(true);
    $svg = simplexml_load_file("svg/fonts/$fontName.svg");
    libxml_clear_errors();
    foreach($svg->children() as $child) {
        if ($child->getName() == 'defs') {
            foreach($child->children() as $defs) {
                if ($defs->getName() == 'font') {
                    $defaultXSize = $defs['horiz-adv-x'];
                    foreach ($defs->children() as $font) {
                        if ($font->getName() == 'font-face') {
                            $ySize = abs($font['ascent']) + abs($font['descent']);
                        } elseif ($font->getName() == 'glyph') {
                            $glyphs[] = $font;
                        }
                    }
                }
            }
        }
    }
    $retval = '';
    $lines = [];
    $length = 0;
    $line = '';
    $lineBreak = 10 + (strlen($text) / 6);
    foreach (str_split($text) as $char) {
        if (++$length > $lineBreak && $char == ' ') {
            $lines[] = $line;
            $line = '';
            $length = 0;
        } else {
            $line .= $char;
        }
    }
    if ($line != '') $lines[] = $line;
    $lineCount = count($lines);
    $yShift = 0;
    if ($lineCount > 1) {
        $yShift = ($height / $lineCount) * ($lineCount - 1) * -1;
    }
    foreach ($lines as $line) {
        $xOffset = 0;
        $temp = '';
        foreach (str_split($line) as $char) {
            if ($char == ' ') {
                $xOffset += $defaultXSize * 0.6;
                continue;
            }
            $glyph = null;
            foreach ($glyphs as $possible) {
                if ($possible['unicode'] == $char) {
                    $glyph = $possible;
                    break;
                }
            }
            if ($glyph == null) continue;
            $d = $glyph['d'];
            $x = $glyph['horiz-adv-x'];
            $temp .= "<path transform=\"translate($xOffset)\" d=\"$d\" />\n";
            if ($x != null) {
                $xOffset += $glyph['horiz-adv-x'];
            } else {
                $xOffset += $defaultXSize;
            }
        }
        $scale = min($width / $xOffset, $height / $lineCount / $ySize);
        $centering = ($width - ($xOffset * $scale)) / 2;
        $temp = "<g fill=\"#3f3f3f\" transform=\"translate($centering,$yShift) scale($scale, -$scale)\">\n" .
            $temp . "</g>\n";
        $yShift += $height / $lineCount;
        $retval .= $temp;
    }
    return "<g>$retval</g>\n";
}