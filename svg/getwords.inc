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

    // option names
    const FONT = 'font';
    const LAYOUT = "layout";
    const ALIGN = 'alignment';
    const TEXT1 = 'text1';
    const TEXT2 = 'text2';
    const GIVENPATH = 'path';
    const LEFTOVERHANG = 'overl';
    const RIGHTOVERHANG = 'overr';
    const STROKE =  'stroke';
    const STROKECOLOUR =  'strokecolour';
    const FILL = 'fill';
    const MAXHEIGHT = 'maxheight';
    const STANDALONE = 'standalone';

    // const BOLD = 'bold'; // these 2 are ignored for now
    // const ITALIC = 'italic';
    // option values
    const UNDERLINE = "underlined";
    const STACKED = 'stacked';
    const INLINE = 'inline';
    const LEFT = 'alignLeft';
    const RIGHT = 'alignRight';
    const CENTRE = 'alignCentre';
    const BLOCK = 'block';
    const ENARCHED = 'enarched';
    const EMBOWED = 'embowed';
    const ENCIRCLED = 'encircled';
    const GIVEN = 'given';
    const WAVY = 'wavy';

    function __construct($fontname = null)
    {
        $this->textOptions = [ // set some defaults
            self::LAYOUT => self::INLINE,
            self::ALIGN => self::CENTRE,
            self::FONT => 'serif',
            self::LEFTOVERHANG => 0,
            self::RIGHTOVERHANG => 0,
            self::STROKECOLOUR => '#CCCCCC',
            self::STANDALONE => true,
        ];
        $this->defaultXSize = 500;
        $this->ySize = 500;
        $this->words = [];
        $this->longest = null;
        $this->yHeight = 500;
        $this->strokeWidth = 25;
        $this->finalHeight = 0;
        $this->finalWidth = 0;

        if (!is_null($fontname)) {
            $this->textOptions[self::FONT] = $fontname;
            $this->compile();
        }
    }

    public function setOptionsFromModifiers($node)
    {
        global $messages;

        foreach ($node->childNodes as $child) {
            if ($child->nodeName == blazonML::E_MODIFIER) {
                switch ($child->getAttribute(blazonML::A_KEYTERM)) {
                    case 'opposite':
                        $item = $child->getAttribute(blazonML::A_PARAM);
                        if ($item == null) $item = 'huh?';
                        $this->setOption(self::TEXT2, $item);
                        break;
                    case 'bold':
                    case 'italic':
                        $messages->addMessage('blazon', 'Bold and italic are now longer supported');
                        break;
                    case 'font':
                        $this->setOption(self::FONT, getModifierValueByKeyterm($node, 'font'));
                        break;
                    case 'engraved':
                    case 'cursive':
                    case 'gothic':
                    case 'celtic':
                    case 'serif':
                    case 'plain':
                    case 'carved':
                        $this->setOption(self::FONT, $child->getAttribute(blazonML::A_KEYTERM));
                        break;
                    case 'underlined':
                        $this->setOption(self::UNDERLINE);
                        break;
                    case  'to-dexter':
                        $this->setOption(self::ALIGN, self::LEFT);
                        break;
                    case  'to-sinister':
                        $this->setOption(self::ALIGN, self::RIGHT);
                        break;
                    case  'to-centre':
                        $this->setOption(self::ALIGN, self::CENTRE);
                        break;
                    case 'encircled':
                        $this->setOption(self::LAYOUT, self::ENCIRCLED);
                        break;
                    case 'text-inline':
                        $this->setOption(self::LAYOUT, self::INLINE);
                        break;
                    case 'text-stacked':
                        $this->setOption(self::LAYOUT, self::STACKED);
                        break;
                    case 'text-block':
                        $messages->addMessage('blazon', 'Text in blocks is no longer supported');
                        // $this->setOption(self::LAYOUT, self::BLOCK);
                        break;
                    case 'enarched':
                        $this->setOption(self::LAYOUT, self::ENARCHED);
                        break;
                    case 'embowed':
                        $this->setOption(self::LAYOUT, self::EMBOWED);
                        break;
                    case 'wavy':
                        $this->setOption(self::LAYOUT, self::WAVY);
                        break;
                    case 'stroked':
                        $this->setOption(self::STROKE);
                        $this->setOption(self::STROKECOLOUR, rgb($child->getAttribute(blazonML::A_PARAM)));
                        break;

                }
            }
        }
    }



    public function compile() {
        global $messages;

        $this->invert = false;
        $fontName = $this->fontMap();
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
        $this->yHeight = abs(intval($this->fontInfo['ascent']));
        $this->ySize = $this->yHeight + abs(intval($this->fontInfo['descent']));
        $this->strokeWidth = $this->ySize / 50;
        if (isset($child['invert'])) $this->invert = true;
        if (isset($child['horiz-adv-x'])) {
            $t = intval($child['horiz-adv-x']);
            if ($t != 0) {
                $this->defaultXSize = $t;
            }
        }
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
    private function fontMap(): string
    {
        global $messages;

        switch ($this->textOptions[self::FONT]) {
            case 'gothic':
            case 'gothik':
            case 'gothick':
                $fontName = 'gothicE';
                break;
            case'celtic':
                $fontName = 'IrishUncialfabeta-Bold';
                break;
            case 'hooge':
                $fontName = 'hooge';
                break;
            case 'cursive':
            case 'script':
            case 'fantasy':
                $fontName = 'lisbon-cursive';
                break;
            case 'carved':
                $fontName = 'merrycle-carved';
                break;
            case 'serif':
            case 'roman':
                $fontName = 'noto-serif';
                break;
            case 'engraved':
            case 'graven':
                $fontName = 'de-soto-engraved';
                break;
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

    public function setOption($name, $value = true) {
        $this->textOptions[$name] = $value;
    }

    public function getOption($name) : string {
        if (array_key_exists($name, $this->textOptions)) {
            return $this->textOptions[$name];
        } // else
        return '';
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
        $x = $glyph['horiz-adv-x'] ?? $this->defaultXSize;
        $stroke = '';
        if ($this->getOption(self::STROKE)) {
            $stroke = " stroke-width=\"" . $this->strokeWidth . "\" stroke=\"" . $this->getOption(self::STROKECOLOUR) . "\" ";
        }
        $fill = '';
        if ($this->getOption(self::FILL)) {
            $fill = " fill=\"" . $this->getOption(self::FILL) . "\" ";
        }
        $svg .= "<path $stroke $fill d=\"$d\" />\n";
        // Manage underlining - should we underline this character?
        if (isset($this->textOptions[self::UNDERLINE]) && (mb_strpos($this->fontInfo['descenders'],$char) === false) ) {
            $offset = 0; $skew = 0; // these are displacements if the font slopes
            if (isset($this->fontInfo['slope'])) {
                $factor = tan(deg2rad(abs(intval($this->fontInfo['slope']))));
                $offset = $factor * abs(intval($this->fontInfo['underline-position']));
                $skew = $factor * floatval($this->fontInfo['underline-thickness']);
            }
            $svg .= "<path d=\""
                . 'M-' . $offset . ' ' .  $this->fontInfo['underline-position'] // start bottom left
                . 'h' . $x                                                    // horizontally to bottom right
                . ' l' . $skew . ',' . $this->fontInfo['underline-thickness'] // move top right
                . ' h-' . $x . 'z"/>';                          // horizontally to top left & close
        }
        // debug, put box around glyph extents
        /* $y = $this->ySize;
        $svg= "<path d=\"m 0 0 h$x v$y h-$x v-$y \" stroke=\"black\" fill=\"none\" stroke-width=\"10px\"/>"
            . $svg; */
        if (isset($this->fontInfo['invert'])) {
            $svg = '<g transform="scale(1,-1) translate(0, -' . $this->yHeight . ')">' . $svg . "</g>";
        }
        return compact('x', 'svg');

    }
    
    private function stringToSVG($words): array
    {
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
            $glyphLine = $this->stringToSVG($line);
            $scale = min($width / $glyphLine['width'], $height / $lineCount / $this->ySize);
            $centering = ($width - ($glyphLine['width'] * $scale)) / 2;
            $temp = "<g fill=\"$fill\" transform=\"translate($centering,$yShift) scale($scale, -$scale)\">\n" .
                $glyphLine['body'] . "</g>\n";
            $yShift += $height / $lineCount;
            $retval .= $temp;
        }
        return "<g>$retval</g>\n";
    }

    // overlap is a percentage to overlap the previous character, 0 = none, 100 = completely
    public function inlineText($text, $overlap = 0) {
        $text = htmlentities($text, ENT_XML1);
        $svgList = '';
        $xLength = 0;
        foreach ($this->str_split_unicode($text) as $char) {
            if (isset($x) && $overlap != 0) { // i.e. 2nd character or more
                $xLength -= $x * ($overlap / 100); // shift back a bit
            }
            extract($this->getGlyph($char)); // creates $x and $svg
            $svgList = "<g transform=\"translate($xLength,0)\">$svg</g>\n" . $svgList;
            $xLength += $x;
        }
        $retval = '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" height="' .
        $this->ySize . '" width="' . $xLength . '"><g id="body">' . $svgList . '</g></svg>';
        return $retval;
    }

    private function bezierCircle($r, $shape = null): string
    {
        $c = 0.551915 * $r;
        $cBar = $r - $c;
        switch ($shape) {
            case 'top':
                $path = "M -$r,0 C -$r,-$c -$c,-$r 0,-$r  $c,-$r $r,-$c $r,0";
                break;
            case 'bottom':
                $path = "M -$r,0 C -$r,$c -$cBar,$r 0,$r $c,$r $r,$c $r,0";
                break;
            default:
                $path = "M -$r,0 C -$r,-$c -$c,-$r 0,-$r  $c,-$r $r,-$c $r,0  $r,$c $c,$r 0,$r  -$c,$r -$r,$cBar -$r,0";
                break;
        }
        return $path;
    }

    public function totalPathLength($compiledPath) : float {
        $pathLength = 0.0;
        foreach($compiledPath->sub_paths() as $sub_path) {
            $pathLength += $sub_path->length();
        }
        return $pathLength;
    }

    public function glyphsAsArray($text) : array {
        $glyphs = [];
        $glyphCount = 0;
        $totalGlyphWidth = 0;
        foreach ($this->str_split_unicode($text) as $char) {
            $glyph = $this->getGlyph($char);
            $x = $glyph['x'];
            $glyphs[] = $glyph;
            $totalGlyphWidth += $x;
            $glyphCount += 1;
        }
        return array('glyphs' => $glyphs, 'width' => $totalGlyphWidth, 'count' => $glyphCount);
    }

    public function glyphsAlongPathAsSVG($glyphs, $compiledPath, $scale = 1, $side = 'above', $offset = 0) : string
    {
        // We want to spread the glyphs along the shape so scale them to fit
        $f = $offset;
        switch($side) {
            default:
            case 'above':
                $yShift = $this->yHeight * -1;
                break;
            case 'below':
                $yShift = 0;
                break;
            case 'raised':
                $yShift = $this->ySize * -0.95;
                break;
            case 'hanging':
                $yShift = $this->ySize * -0.05;
                break;
        }
        // $yShift = $side == 'above' ? $this->yHeight * -1 : 0;
        $retval = '';
        $glyphCount = count($glyphs);
        $currentSubPath = 0;
        for ($i = 0; $i < $glyphCount; $i++) {
            $glyph = $glyphs[$i];
            $halfGlyph = $glyph['x'] / 2;
            $f += $halfGlyph * $scale;
            if ($f > $compiledPath->sub_paths()[$currentSubPath]->length() && !is_null($compiledPath->sub_paths()[$currentSubPath + 1])) {
                $currentSubPath += 1;
                $f = $glyph['x'] * $scale;
            }
            $segment = $compiledPath->sub_paths()[$currentSubPath]->segments()[0]->segment_at_length($f);
            if ($i == 0) {
                $this->setOption(self::LEFTOVERHANG, ($this->yHeight * $scale) * cos($segment->tangent()));
            }
            if ($i == $glyphCount - 1) {
                $this->setOption(self::RIGHTOVERHANG, ($this->yHeight * $scale) * cos($segment->tangent()));
            }
            $angle = rad2deg($segment->tangent());
            $point = $compiledPath->sub_paths()[$currentSubPath]->point_at_length($f);
            $retval .= "<g transform=\"translate({$point->x},{$point->y}) rotate($angle) scale($scale) translate(-$halfGlyph,$yShift)\">{$glyph['svg']}</g>";
            $f += $halfGlyph * $scale;
        }
        return $retval;
    }
    private function oppositeTextAsSVG($text1, $text2) : string {
        $dParser = new SvgDParser();
        if (mb_strlen($text1) < 4) $text1 = " $text1 "; // Not sensible to have < 4 characters
        if (mb_strlen($text2) < 4) $text2 = " $text2 "; // Not sensible to have < 4 characters
        // Get all the glyphs so we know the length we need to cater for
        $glyphArrays[] = $this->glyphsAsArray($text1);
        $glyphArrays[] = $this->glyphsAsArray($text2);
        $longestText = ($glyphArrays[0]['width'] > $glyphArrays[1]['width']) ? 0 : 1;
        $difference = abs(intval($glyphArrays[0]['width']) - intval($glyphArrays[1]['width']));
        $requiredRadius = ($glyphArrays[$longestText]['width'] / ( 2 * pi())) * 2;
        // We build our path once we have the total length of glyphs
        $upperPath = $this->bezierCircle($requiredRadius, 'top');
        // adjust the lower radius to account for descenders
        // TODO This is different if the lower text is the longest...
        // $lowerPath = $this->bezierCircle($requiredRadius - ($this->ySize - $this->yHeight), 'bottom');
        $lowerPath = $this->bezierCircle($requiredRadius, 'bottom');
        $pathHeight = $requiredRadius * 2;
        $path = $dParser->parse_d($upperPath);
        $compiledUpper = $path->compile();
        $path = $dParser->parse_d($lowerPath);
        $compiledLower = $path->compile();
        if ($longestText == 0) {
            $retval = $this->glyphsAlongPathAsSVG($glyphArrays[0]['glyphs'], $compiledUpper, 1, 'raised');
            // adjust the difference to account for descenders
            // $difference -= ((pi() / 2) * ($this->ySize - $this->yHeight)) * 2; // is already negative
            $retval .= $this->glyphsAlongPathAsSVG($glyphArrays[1]['glyphs'], $compiledLower, 1, 'hanging', $difference / 2) ;
        } else {
            $retval = $this->glyphsAlongPathAsSVG($glyphArrays[0]['glyphs'], $compiledUpper, 1, 'raised', $difference / 2);
            $retval .= $this->glyphsAlongPathAsSVG($glyphArrays[1]['glyphs'], $compiledLower, 1, 'hanging');
        }
        //$height = $width = $pathHeight + (2 * $this->yHeight);
        // $overhang = $requiredRadius + $this->yHeight;

        $height = $width = $pathHeight + (2 * $this->ySize);
        $overhang = $requiredRadius + $this->ySize;
        $retval ="<g transform=\"translate($overhang,$overhang)\">$retval</g>";
        $retval = $this->wrapSVG($retval, $height, $width);
        return $retval;
    }

    private function encircleTextAsSVG($text) : string {
        $dParser = new SvgDParser();
        // encircled text is slightly different. We first pad the string so it is at least 3 characters long
        // then always add a final space so that the end does not abut the beginning.
        // We then find out the total length of all the glyphs and construct an approximate circle of
        // that circumference from 4 bezier curves, starting at the 9 o'clock position. The glyphs are then
        // spread around this path as usual. We can't determine our bounding box until we have the
        // bezier path so this is done later.
        if (mb_strlen($text) < 4) $text = " $text "; // Not sensible to have < 4 characters
        $text .= ' '; //  put in a space so we don't eat our own tail
        // Get all the glyphs so we know the length we need to cater for
        $glyphArray = $this->glyphsAsArray($text);
        $glyphs = $glyphArray['glyphs'];
        $totalGlyphWidth = $glyphArray['width'];
        $requiredRadius = $totalGlyphWidth / ( 2 * pi());
        // We build our path once we have the total length of glyphs
        $pathD = $this->bezierCircle($requiredRadius);
        $pathHeight = $requiredRadius * 2;
        $path = $dParser->parse_d($pathD);
        $compiledPath = $path->compile();
        $retval = $this->glyphsAlongPathAsSVG($glyphs, $compiledPath);
        $height = $width = $pathHeight + (2 * $this->yHeight);
        $overhang = $requiredRadius + $this->yHeight;
        $retval ="<g transform=\"translate($overhang,$overhang)\">$retval</g>";
        $retval = $this->wrapSVG($retval, $height, $width);
        return $retval;
    }

    private function textToShapeAsSVG($text, $shape): string
    {
        // following to suppress warnings
        $side = 'above';
        $dParser = new SvgDParser();
        switch ($shape) {
            case self::GIVEN:
                $pathD = $this->getOption(self::GIVENPATH);
                $width = $pathWidth = 1000; // need to hard code these, sorry
                $height = $pathHeight = 151; // in this case they are ignored anyway
                break;
            case self::EMBOWED:
                // Embowed text is scaled so that it fits exactly along a pre-defined arc
                // We also need to know the bounding box for that arc but it is easier to
                // hard code these as $pathHeight and $pathWidth below
                if (mb_strlen($text) < 2) $text = " $text "; // make sure a single character ends up central
                $pathD = "m 3.04,1.64 c 0,0 195.18,149.86 501.61,151.46 C 811.1,154.7 1003.4,1.64 1003.4,1.64";
                $pathWidth = 1000; // need to hard code these, sorry
                $pathHeight = 151;
                $side = 'below';
                break;
            case self::WAVY:
                // wavy text uses the same technique as above. As the "wave" is quite shallow we don't
                // worry about characters overlapping at the top corners
                if (mb_strlen($text) < 2) $text = " $text "; // make sure a single character ends up central
                $pathD = "m 2.72,149.64 c 0,0 195.18,-149.86 501.62,-151.46 306.4,-1.6 498.66,151.46 498.66,151.46 c 0,0 195.2,149.86 501.7,151.46 306.4,1.6 498.7,-151.46 498.7,-151.46";
                $pathWidth = 2000;
                $pathHeight = 301;
                break;
            default:
            case self::ENARCHED:
                // enarched text again uses a pre-define arc and hard coded bounding box
                if (mb_strlen($text) < 2) $text = " $text "; // make sure a single character ends up central
                $pathD = "M 0,285 C250,0 750,0 1000,285";
                $pathWidth = 1000; // need to hard code these, sorry
                $pathHeight = 155;
                break;
            }
        // Get all the glyphs so we know the length we need to cater for
        $glyphArray = $this->glyphsAsArray($text);
        $glyphs = $glyphArray['glyphs'];
        $totalGlyphWidth = $glyphArray['width'];
        $y = $this->yHeight;
        // Now get the length of the arch shape
        $arch = $dParser->parse_d($pathD);
        $compiledArch = $arch->compile();
        $archLength = $this->totalPathLength($compiledArch);

        // We want to spread the glyphs along the shape so scale them to fit
        $scale = $archLength / $totalGlyphWidth;
        $offset = 0;
        if (($maxHeight = $this->getOption(self::MAXHEIGHT)) != false) {
            if ($this->ySize * $scale > $maxHeight) {
                $scale = $maxHeight / $this->ySize;
                $offset = ($archLength - ( $totalGlyphWidth * $scale)) / 2;
            }
        }
        $retval = $this->glyphsAlongPathAsSVG($glyphs, $compiledArch, $scale, $side, $offset);
        // debug - visible pah
        // $retval .= "<path d=\"$pathD\" stroke=\"black\" fill=\"none\" width=\"10px\"/>";
        // calculate the actual dimensions (based on font height)
        switch ($this->textOptions[self::LAYOUT]) {
            case self::WAVY:
                $height = $pathHeight + ($y * $scale);
                $overhang = floatval($this->getOption(self::LEFTOVERHANG));
                $width = $pathWidth + $overhang;
                $retval ="<g transform=\"translate($overhang,0)\">$retval</g>";
                break;
            case self::ENARCHED:
            case self::EMBOWED:
                $height = $pathHeight + ($y * $scale);
                $leftOverhang = floatval($this->getOption(self::LEFTOVERHANG));
                $rightOverhang = floatval($this->getOption(self::RIGHTOVERHANG));
                $width = $pathWidth + $leftOverhang + $rightOverhang;
                $retval ="<g transform=\"translate($leftOverhang,0)\">$retval</g>";
                break;
            default:
                break;
        }
        $retval = $this->wrapSVG($retval, $height, $width);
        return $retval;
    }

    private function wrapSVG($svg, $height = 0, $width = 0) {
        if ($this->getOption(self::STANDALONE) == true) {

            $svg = '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" height="' .
                $height . '" width="' . $width . '"><g id="body">' . $svg . '</g></svg>';
        }
        $this->finalHeight = $height;
        $this->finalWidth = $width;
        return $svg;
}
        

    public function getSVG($asArray = false) {
        $retval = '';
        $text = $this->textOptions[self::TEXT1];
        $text = htmlentities($text, ENT_XML1);
        switch ($this->textOptions[self::LAYOUT]) {
        case self::ENCIRCLED:
            $text2 = $this->getOption(self::TEXT2);
            if ($text2 == '') { // just a normal circle
                $retval = $this->encircleTextAsSVG($text);
            } else { // flip lower text
                $retval = $this->oppositeTextAsSVG($text, $text2);
            }
            break;
        case self::ENARCHED:
        case self::WAVY:
        case self::EMBOWED:
        case self::GIVEN:
            $retval = $this->textToShapeAsSVG($text, $this->textOptions[self::LAYOUT]);
            break;
        case self::INLINE:
            $line = $this->stringToSVG($text);
            $retval = $this->wrapSVG($line['body'], $this->ySize, $line['width']);
            break;
        case self::STACKED:
            $this->splitWords($text);
            // first render each word
            $renders = [];
            $longest = 0;
            for ($i = 0; $i < count($this->words); $i++) {
                $render = $this->stringToSVG($this->words[$i]);
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
            $retval = $this->wrapSVG($retval, ($this->ySize * $i), $longest);
        default:
            break;
        }
        if ($asArray) {
            return array ('svg' => $retval, 'width' => $this->finalWidth, 'height' => $this->finalHeight);
        } // else
        return $retval;
    }

        

}

