<?php /* Copyright 2010 Karl R Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License */

// letter

$charge['default_colour'] = 'sable';
$value = getModifierValueByKeyterm ( $node, 'value' );
$font = getModifierValueByKeyterm ( $node, 'font' );
if (is_null($font) || !file_exists($folder . "fonts/" . $font . '.svg')) $font = "gothic";
$glyphs = simplexml_load_file($folder . "fonts/" . $font . '.svg' );
$numChars = strlen($value);
// set vertical flex quite high?


$totalWidth = 0;
$offset = 0;
$svg = '';
$defaultWidth = $glyphs->defs->font['horiz-adv-x'];
if ($defaultWidth == 0) { $defaultWidth = 700; } // Guess, issue warning?
$fontHeight = $glyphs->defs->font->{'font-face'}['ascent'];
if ($fontHeight == 0) { $fontHeight = 750; } // ditto
foreach (str_split($value) as $val) {
  $svg .= '<g transform="scale(1,-1) translate(' . $totalWidth . ',-' . $fontHeight . ')">';
  foreach ($glyphs->defs->font->glyph as $glyph) {
    if ($glyph['unicode'] == $val) {
      $svg .= '<path style="stroke:#808080;stroke-width:16" d="' . $glyph['d'] . '"/>';
      $charWidth = $glyph['horiz-adv-x'];
      if ($charWidth == 0) { $charWidth = $defaultWidth; }
      $totalWidth += $charWidth;
      break;
    }
  }
  $totalWidth += 50; // leave a gap
  $svg .= '</g>';
}
$totalWidth -= 50; // remove last gap
$svg .= '</g></svg>';

$svg = '<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
 <svg version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0" y="0" height="' . $fontHeight  . '" width="' . $totalWidth . '" ><g id="body">' . $svg;

$charge['svgCode'] = $svg;
