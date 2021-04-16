<?php

Class WordBuilder
{
    private $textOptions;

    const UNDERLINE = "underlined";
    const LAYOUT = "layout";
    const STACKED = 'stacked';
    const INLINE = 'inline';
    const ALIGN = 'alignment';
    const LEFT = 'alignLeft';
    const RIGHT = 'alignRight';
    const CENTRE = 'alignCentre';

    function __construct($fontName = 'serif') {
        global $messages;

        if (!$fontName) $fontName = 'serif'; // in case null is passed
        $this->invert = false;
        if (!file_exists("svg/fonts/$fontName.svg")) {
            $fontName = $this->fontMap($fontName);
        }
        $this->textOptions = [ // set some defaults
                self::LAYOUT => self::INLINE,
                self::ALIGN => self::CENTRE,
        ];
        $this->defaultXSize = 500;
        $this->ySize = 500;
        $this->words = [];
        $this->longest = null;
        $font = simplexml_load_file("svg/fonts/$fontName.svg");
        if (!$font) {
              $messages->addMessage('internal', "No font file for $fontName");
              return;
        }
        libxml_clear_errors();
        $this->glyphs = [];
        foreach($font->children() as $child) {
            if ($child->getName() == 'defs') {
                foreach($child->children() as $defs) {
                    if ($defs->getName() == 'font') {
                        foreach ($defs->children() as $child) {
                            if ($child->getName() == 'font-face') {
                                $this->fontInfo = $child;
                            } elseif ($child->getName() == 'glyph') {
                                $this->glyphs[] = $child;
                            }
                        }
                    }
                }
            }
        }
        $this->yHeight = abs($this->fontInfo['ascent']);
        $this->ySize = abs($this->fontInfo['ascent']) + abs($this->fontInfo['descent']);
        if (isset($child['invert'])) $this->invert = true;
        if (isset($this->fontInfo['credit']))
            $messages->addMessage('licence',$child['credit']);
        // set a guesstimate when underline data not set in the font file
        if (!isset($this->fontInfo['underline-thickness'])) $this->fontInfo['underline-thickness'] = $this->ySize / 20;
        if (!isset($this->fontInfo['underline-position'])) $this->fontInfo['underline-position'] = $this->ySize * 0.9;
        if (!isset($this->fontInfo['descenders'])) $this->fontInfo['descenders'] = '';

    }

    /* map basic names to our specific chosen font files */
    private function fontMap($fontName) {
        switch ($fontName) {
            case 'cursive':
            case 'script':
                $fontName = 'lisbon-cursive';
                break;
            case 'serif':
                $fontName = 'noto-serif';
                break;
            case 'fantasy':
            case 'monospace':
            case 'times':
            case 'baskerville':
            case 'verdana':
            case 'symbol':
            case 'plain':
            case 'san serif':
            case 'sans serif':
              $fontName = 'lucida-sans';
              break;
            default:
              $messages->addMessage('Warning', "Unrecognised font");
                $fontName = 'noto-serif';
              break;
        }
        return($fontName);
  }

    public function option($name, $value = true) {
        $this->textOptions[$name] = $value;
    }

    private function splitWords($text) {
        $this->words = mb_split('\s', $text);
    }

    private function longestWord() {
        $longestLength = 0;
        for ($i = 0; $i < count($this->words); $i++) {
            $wordLen = mb_strlen($words[$i]);
            if ($wordLen > $longestLength) {
                $longestLength = $wordLen;
                $this->longest = $i;
            }
        }
        return $longestLength;
    }



    private function  str_split_unicode($str, $length = 1) {
        $tmp = preg_split('~~u', $str, -1, PREG_SPLIT_NO_EMPTY);
        if ($length > 1) {
            $chunks = array_chunk($tmp, $length);
            foreach ($chunks as $i => $chunk) {
                $chunks[$i] = join('', (array) $chunk);
            }
            $tmp = $chunks;
        }
        return $tmp;
    }

    private function stringToGlyphs($words) {
        $svg = '';
        $xLength = 0;
        $notdef = null;
        foreach ($this->str_split_unicode($words) as $char) {
            if ($char == ' ') {
                $xLength += $this->defaultXSize * 0.6;
                continue;
            }
            $glyph = null;
            foreach ($this->glyphs as $possible) {
                if (!$notdef && $possible['glyph-name'] == '.notdef') {
                    $notdef = $possible;
                }
                if ($possible['unicode'] == $char) {
                    $glyph = $possible;
                    break;
                }
            }
            if ($glyph == null) {
                if ($notdef)
                    $glyph = $notdef;
                else
                   continue;
            }
            $d = $glyph['d'];
            $x = $glyph['horiz-adv-x'] ?? $this->defaultXSize;;
            $svg .= "<path transform=\"translate($xLength)\" d=\"$d\" />\n";
            // Manage underlining - should we underline this character?
            if (isset($this->textOptions[self::UNDERLINE]) && (mb_strpos($this->fontInfo['descenders'],$char) === false) ) {
                $offset = 0; $skew = 0; // these are displacements if the font slopes
                if (isset($this->fontInfo['slope'])) {
                    $factor = tan(deg2rad(abs($this->fontInfo['slope'])));
                    $offset = $factor * abs($this->fontInfo['underline-position']);
                    $skew = $factor * $this->fontInfo['underline-thickness'];
                }
                $svg .= "<path transform=\"translate($xLength)\" d=\""
                    . 'M-' . $offset . ' ' .  $this->fontInfo['underline-position'] // start bottom left
                    . 'h' . $x                                                    // horizontally to bottom right
                    . ' l' . $skew . ',' . $this->fontInfo['underline-thickness'] // move top right
                    . ' h-' . $x . 'z"/>';                          // horizontally to top left & close
            }
            $xLength += $x;
        }
        if (isset($this->fontInfo['invert'])) {
            $svg = '<g transform="scale(1,-1) translate(0, -' . $this->yHeight . ')">' . $svg . "</g>";
        }
        // temporary box to show boundaries
        $svg .= '<path fill="none" stroke="black" stroke-width="10" d="M0,0 h' . $xLength . ' v' . $this->ySize 
            . ' h-' . $xLength . 'z" />';
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
        return $retval;
    }

    /* public function textInBox($text,$aspectRatio) {
        // first, rough character count
        $chars = mb_strlen($text);
        $this->splitWords($text)
        // then guess an ideal width (in characters)
        $width = max($this->longest, intval(ceil(sqrt($chars/$aspectRatio))));
        $height = intval(ceil($aspectRatio/$width));
    } */
        

    public function getSVG($text) {
        $retval = '';
        switch ($this->textOptions[self::LAYOUT]) {
        case self::INLINE:
            $line = $this->stringToGlyphs($text);
            $retval = '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" height="' .
                $this->ySize . '" width="' . $line['width'] . '"><g id="body">' . $line['body'] . '</g></svg>';
            break;
        case self::STACKED:
            $this->splitWords($text);
            // first render each word
            $renders = [];
            $lengths = [];
            $longest = 0;
            for ($i = 0; $i < count($this->words); $i++) {
                $render = $this->stringToGlyphs($this->words[$i]);
                if ($render['width'] > $longest) $longest = $render['width'];
                $renders[] = $render;
            }
            // now adjust based on alignment
            for ($i = 0; $i < count($renders); $i++) {
                switch ($this->textOptions[self::ALIGN]) {
                case self::LEFT:
                    $retval .= '<g transform="translate(0,' . ($this->ySize * $i)  . ')">' . $renders[$i]['body'] . '</g>';
                    break;
                case self::RIGHT:
                    $retval .= '<g transform="translate(' . ($longest - $renders[$i]['width'])
                        . ',' . ($this->ySize * $i)  . ')">' . $renders[$i]['body'] . '</g>';
                    break;
                case self::CENTRE:
                    $retval .= '<g transform="translate(' . (($longest - $renders[$i]['width']) / 2)
                        . ',' . ($this->ySize * $i)  . ')">' . $renders[$i]['body'] . '</g>';
                    break;
                default:
                    break;
                }
            }
            $retval = '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" height="' .
                ($this->ySize * $i) . '" width="' . $longest . '"><g id="body">' . $retval . '</g></svg>';
        default:
            break;
        }
        return $retval;
    }

        

}

