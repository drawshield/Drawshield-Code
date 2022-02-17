<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

     DrawShield is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */

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
    const ENARCHED = 'enarched';
    const EMBOWED = 'embowed';
    const ENCIRCLED = 'encircled';
    const WAVY = 'wavy';

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
            $messages->addMessage('licence',$this->fontInfo['credit']);
        elseif (isset($this->fontInfo['licence']))
            $messages->addMessage('licence',$this->fontInfo['licence']);
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

    private function getGlyph($char) {
        $svg = '';
        $notdef = null;       
        if ($char == ' ') {
            $x = $this->defaultXSize * 0.6;
            return compact('x', 'svg');
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
               return Array($x => 0, $svg => '');
        }
        $d = $glyph['d'];
        $x = $glyph['horiz-adv-x'] ?? $this->defaultXSize;;
        $svg .= "<path d=\"$d\" />\n";
        // Manage underlining - should we underline this character?
        if (isset($this->textOptions[self::UNDERLINE]) && (mb_strpos($this->fontInfo['descenders'],$char) === false) ) {
            $offset = 0; $skew = 0; // these are displacements if the font slopes
            if (isset($this->fontInfo['slope'])) {
                $factor = tan(deg2rad(abs($this->fontInfo['slope'])));
                $offset = $factor * abs($this->fontInfo['underline-position']);
                $skew = $factor * $this->fontInfo['underline-thickness'];
            }
            $svg .= "<path d=\""
                . 'M-' . $offset . ' ' .  $this->fontInfo['underline-position'] // start bottom left
                . 'h' . $x                                                    // horizontally to bottom right
                . ' l' . $skew . ',' . $this->fontInfo['underline-thickness'] // move top right
                . ' h-' . $x . 'z"/>';                          // horizontally to top left & close
        }
        if (isset($this->fontInfo['invert'])) {
            $svg = '<g transform="scale(1,-1) translate(0, -' . $this->yHeight . ')">' . $svg . "</g>";
        }
        return compact('x', 'svg');

    }
    
    private function stringToGlyphs($words) {
        $svgList = '';
        $xLength = 0;
        foreach ($this->str_split_unicode($words) as $char) {
            extract($this->getGlyph($char)); // creates $x and $svg
            if ($svg != '')
                $svgList .= "<g transform=\"translate($xLength,0)\">$svg</g>\n";
            $xLength += $x;
        }
        return Array('body' => $svgList, 'width' => $xLength);
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
        $svgList = '';
        $xLength = 0;
        foreach ($this->str_split_unicode($text) as $char) {
            extract($this->getGlyph($char)); // creates $x and $svg
            $svgList .= "<g transform=\"translate($xLength,0)\">$svg</g>\n";
            $xLength += $x;
        }
        $retval = '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" height="' .
        $this->ySize . '" width="' . $xLength . '"><g id="body">' . $svgList . '</g></svg>';
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

    private function bezierCircle($r, $shape = null)
    {
        $c = 0.551915 * $r;
        $cBar = $r - $c;
        switch ($shape) {
            case 'top':
                $path = "M -$r,0 C -$r,-$c -$c,-$r 0,-$r  $c,-$r $r,-$c $r,0";
                break;
            case 'bottom':
                $path = "M $r,0  $r,$c $c,$r 0,$r  -$c,$r -$r,$cBar -$r,";
                break;
            default:
                $path = "M -$r,0 C -$r,-$c -$c,-$r 0,-$r  $c,-$r $r,-$c $r,0  $r,$c $c,$r 0,$r  -$c,$r -$r,$cBar -$r,0";
                break;
        }
        return $path;
    }

    private function stringToArch($text) {
        $retval = '';
        $pathD = ''; // this and following to suppress warnings
        $pathHeight = $pathWidth = $yShift = 0;
        $dParser = new SvgDParser();
        switch ($this->textOptions[self::LAYOUT]) {
            case self::EMBOWED:
                if (strlen($text) < 2) $text = " $text "; // make sure a single character ends up central
                // put a space in between character so they don't overlap at the top
                $newText = '';
                $len = strlen($text);
                for ($i = 0; $i < $len - 1; $i++) {
                    $newText .= $text[$i];
                    $newText .= ' ';
                }
                $newText .= $text[$len - 1];
                $text = $newText;
                $pathD = "m 3.04,1.64 c 0,0 195.18,149.86 501.61,151.46 C 811.1,154.7 1003.4,1.64 1003.4,1.64";
                $pathWidth = 1000; // need to hard code these, sorry
                $pathHeight = 151;
                $yShift = $this->yHeight * -1;
                break;
            case self::ENCIRCLED:
                if (strlen($text) < 4) $text = " $text "; // Not sensible to have < 4 characters
                $text .= ' '; //  put in a space so we don't eat our own tail
                // We build our path once we have the total length of glyphs
                $yShift = $this->yHeight * -1;
                break;
            case self::WAVY:
                if (strlen($text) < 2) $text = " $text "; // make sure a single character ends up central
                $pathD = "m 2.72,149.64 c 0,0 195.18,-149.86 501.62,-151.46 306.4,-1.6 498.66,151.46 498.66,151.46 m 0,0 c 0,0 195.2,149.86 501.7,151.46 306.4,1.6 498.7,-151.46 498.7,-151.46";
                $pathWidth = 2000;
                $pathHeight = 301;
                $yShift = $this->yHeight * -1;
                break;
            default:
            case self::ENARCHED:
                if (strlen($text) < 2) $text = " $text "; // make sure a single character ends up central
                $pathD = "M 0,285 C250,0 750,0 1000,285";
                $pathWidth = 1000; // need to hard code these, sorry
                $pathHeight = 155;
                $yShift = $this->yHeight * -1;
                break;
        }
        // Get all the glyphs so we know the length we need to cater for
        $glyphs = [];
        $glyphCount = 0;
        $totalGlyphWidth = 0;
        $y = $this->yHeight;
        foreach ($this->str_split_unicode($text) as $char) {
            $glyph = $this->getGlyph($char);
            $x = $glyph['x'];
            // debug, put box around glyph extents
            // $glyph['svg'] = "<path d=\"m 0 0 h$x v$y h-$x v-$y \" stroke=\"black\" fill=\"none\" stroke-width=\"10px\"/>"
            //      . $glyph['svg'];
            $glyphs[] = $glyph;
            $totalGlyphWidth += $x;
            $glyphCount += 1;
        }
        $requiredRadius = 0;
        if ($this->textOptions['layout'] == self::ENCIRCLED) {
            $requiredRadius = $totalGlyphWidth / ( 2 * pi());
            $pathD = $this->bezierCircle($requiredRadius);
            $pathWidth = $pathHeight = $requiredRadius * 2;
        }
        // Now get the length of the arch shape
        $arch = $dParser->parse_d($pathD);
        $compiledArch = $arch->compile();
        $archLength = 0;
        foreach($compiledArch->sub_paths() as $sub_path) {
            $archLength += $sub_path->length();
        }

        // We want to spread the glyphs along the shape so scale them to fit
        $scale = $archLength / $totalGlyphWidth;
        $f = 0;
        $overhang = null;
        $currentSubPath = 0;
        for ($i = 0; $i < $glyphCount; $i++) {
            $glyph = $glyphs[$i];
            $halfGlyph = $glyph['x'] / 2;
            $f += $halfGlyph * $scale;
            if ($f > $compiledArch->sub_paths()[$currentSubPath]->length()) {
                $currentSubPath += 1;
                $f = $glyph['x'] * $scale;
            }
            $segment = $compiledArch->sub_paths()[$currentSubPath]->segments()[0]->segment_at_length($f);
            $angle = rad2deg($segment->tangent());
            // Calculate how much we "stick out" at the ends (for final width calculation)
            // strictly speaking the angles might be different at each end so should really
            // calculate both sides, not assume they are the same...
            if ($overhang == null) {
                $overhang = ($y * $scale) * cos($segment->tangent());
            }
            $point = $compiledArch->sub_paths()[$currentSubPath]->point_at_length($f);
            $retval .= "<g transform=\"translate({$point->x},{$point->y}) rotate($angle) scale($scale) translate(-$halfGlyph,$yShift)\">{$glyph['svg']}</g>";
            $f += $halfGlyph * $scale;
        }
        // debug - visible path
        // $retval .= "<path d=\"$pathD\" stroke=\"black\" fill=\"none\" width=\"10px\"/>";
        // calculate the actual dimensions (based on font height)
        switch ($this->textOptions[self::LAYOUT]) {
            case self::ENCIRCLED:
                $height = $width = $pathHeight + (2 * $y * $scale);
                $overhang = $requiredRadius + ($y *  $scale);
                $retval ="<g transform=\"translate($overhang,$overhang)\">$retval</g>";
                break;
            case self::EMBOWED:
                $height = ($y * $scale) - $pathHeight;
                $width = $pathWidth;
                break;
            default:
            case self::WAVY:
                $height = $pathHeight + ($y * $scale);
                $width = $pathWidth + $overhang;
                $retval ="<g transform=\"translate($overhang,0)\">$retval</g>";
                break;
            case self::ENARCHED:
                $height = $pathHeight + ($y * $scale);
                $width = $pathWidth + $overhang + $overhang;
                $retval ="<g transform=\"translate($overhang,0)\">$retval</g>";
                break;
        }
        $retval = '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" height="' . $height . '" ' .
            'width="' . $width . '"><g id="body">' . $retval . '</g></svg>';
        return $retval;
    }
        

    public function getSVG($text) {
        $retval = '';
        switch ($this->textOptions[self::LAYOUT]) {
        case self::ENCIRCLED:
        case self::ENARCHED:
        case self::WAVY:
        case self::EMBOWED:
            $retval = $this->stringToArch($text);
            break;
        case self::INLINE:
            $line = $this->stringToGlyphs($text);
            $retval = '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" height="' .
                $this->ySize . '" width="' . $line['width'] . '"><g id="body">' . $line['body'] . '</g></svg>';
            break;
        case self::STACKED:
            $this->splitWords($text);
            // first render each word
            $renders = [];
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

