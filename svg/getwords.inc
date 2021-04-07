<?php

Class WordBuilder
{
    function __construct($fontName) {
        global $messages;

        $this->invert = false;
        if (!file_exists("svg/fonts/$fontName.svg")) {
            $messages->addMessage('internal',"Unknown font - $fontName");
            return;
        }
        $this->defaultXSize = 500;
        $this->ySize = 500;
        $font = simplexml_load_file("svg/fonts/$fontName.svg");
        libxml_clear_errors();
        $this->glyphs = [];
        foreach($font->children() as $child) {
            if ($child->getName() == 'defs') {
                foreach($child->children() as $defs) {
                    if ($defs->getName() == 'font') {
                        $this->defaultXSize = $defs['horiz-adv-x'];
                        foreach ($defs->children() as $child) {
                            if ($child->getName() == 'font-face') {
                                $this->ySize = abs($child['ascent']) + abs($child['descent']);
                                if (isset($child['invert'])) $this->invert = true;
                                if (isset($child['credit']))
                                    $messages->addMessage('licence',$child['credit']);
                            } elseif ($child->getName() == 'glyph') {
                                $this->glyphs[] = $child;
                            }
                        }
                    }
                }
            }
        }
    }

    private function stringToGlyphs($words) {
        $svg = '';
        $xLength = 0;
        foreach (str_split($words) as $char) {
            if ($char == ' ') {
                $xLength += $this->defaultXSize * 0.6;
                continue;
            }
            $glyph = null;
            foreach ($this->glyphs as $possible) {
                if ($possible['unicode'] == $char) {
                    $glyph = $possible;
                    break;
                }
            }
            if ($glyph == null) continue;
            $d = $glyph['d'];
            $x = $glyph['horiz-adv-x'];
            $svg .= "<path transform=\"translate($xLength)\" d=\"$d\" />\n";
            if ($x != null) {
                $xLength += $glyph['horiz-adv-x'];
            } else {
                $xLength += $this->defaultXSize;
            }
        }
        if ($this->invert) {
            $svg = '<g transform="scale(1,-1) translate(0, -' . $this->ySize . ')">' . $svg . "</g>";
        }
        return Array('body' => $svg, 'width' => $xLength);
    }


    public function fitWordsToRect($text, $height, $width, $fill = "#3F3F3F") {
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
            $glyphLine = $this->stringToGlyphs($line);
            $scale = min($width / $glyphLine['width'], $height / $lineCount / $this->ySize);
            $centering = ($width - ($glyphLine['width'] * $scale)) / 2;
            $temp = "<g fill=\"$fill\" transform=\"translate($centering,$yShift) scale($scale, -$scale)\">\n" .
                $glyphLine['body'] . "</g>\n";
            $yShift += $height / $lineCount;
            $retval .= $temp;
        }
        return "<g>$retval</g>\n";
    }

    public function inlineText($text) {
        $line = $this->stringToGlyphs($text);
        $retval = '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" height="' .
        $this->ySize . '" width="' . $line['width'] . '"><g id="body">' . $line['body'] . '</g></svg>';
        error_log($retval);
        return $retval;
    }


}

