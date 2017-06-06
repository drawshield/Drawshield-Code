<?php /* Copyright 2010 Karl R. Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

// word

$charge['default_colour'] = 'sable';

// defaults
$fontWeight = 'normal';
$fontStyle = 'normal';
$textDecoration = 'none';
$fontFamily = 'italic';
$textAlign = 'middle';
$textLayout = 'block';

if ( existModifierWithKeyterm($node,'bold') ) $fontWeight = 'bold';
if ( existModifierWithKeyterm($node,'italic') ) $fontStyle = 'italic';
if ( existModifierWithKeyterm($node,'underlined') ) $textDecoration = 'underline';
if ( $font = getModifierValueByKeyterm($node,'font') ) {
  switch ($font) {
    case 'serif':
    case 'cursive':
    case 'fantasy':
    case 'monospace':
    case 'times':
    case 'baskerville':
      case 'verdana':
      case 'symbol':
      // No action needed
      break;
    case 'san serif':
    case 'sans serif':
      $font = 'sans-serif';
      break;
    default:
      $messages->addMessage('Warning', "Unrecognised font");
      $font = 'serif';
      break;
  }
  $fontFamily = $font;
}

if ( existModifierWithKeyterm($node,'laying')) {
  if (existModifierWithKeyterm($node, 'to-dexter')) $textAlign = 'start';
  elseif (existModifierWithKeyterm($node, 'to-sinister')) $textAlign = 'end';
  elseif (existModifierWithKeyterm($node, 'to-centre')) $textAlign = 'middle';
}
  if (existModifierWithKeyterm($node,'encircled')) { // This is first, so any modifier added for "encircled-by" overrides others
    $textLayout = 'encircled';
  } elseif ( existModifierWithKeyterm($node,'text-inline')) {
    $textLayout = 'inline';
  } elseif (existModifierWithKeyterm($node,'text-stacked')) {
    $textLayout = 'stacked';
  } elseif (existModifierWithKeyterm($node,'text-block')) {
    $textLayout = 'block';
  } elseif (existModifierWithKeyterm($node,'enarched')) {
    $textLayout = 'enarched';
  } elseif (existModifierWithKeyterm($node,'embowed')) {
    $textLayout = 'embowed';
  } elseif (existModifierWithKeyterm($node,'wavy')) {
    $textLayout = 'wavy';
}

$fontAdjustments = array (
  'monospace' => 0.75,
  'cursive' => 0.86,
  'verdana' => 0.8,
  'symbol' => 0.9,
  'sans-serif' => 0.95,
);

$item = getModifierValueByKeyterm( $node, 'value' );
if ( $item == null ) $item = 'huh';
$lines = array();
$words = explode(' ', $item);

$longestWord = 0;
foreach ($words as $word)
  if ( ($length = strlen($word)) > $longestWord ) $longestWord = $length;
if (count($words) == 1 and $textLayout == 'block')
  $textLayout = 'inline'; // No point complicating things if only one word

$height = 100;
$width = 100;
$svgText = '';
switch ($textLayout) {
  case 'stacked': //-----------------------------------------------------
    $width = 25 * $longestWord;
    $height = 44 * count($words);
    $y = 44;
    switch ($textAlign) {
      case 'start':
        $x = 0;
        break;
      case 'end':
        $x = $width;
        break;
      case 'middle':
        $x = $width / 2;
        break;
    }
    $svgText .= '<text x="0" y="0" font-size="44" font-style="' .
      $fontStyle . '" font-family="' . $fontFamily . '" text-decoration="' . $textDecoration .
      '" font-weight="' . $fontWeight . '">';
    foreach ($words as $word) {
      $svgText .= '<tspan x="' . $x . '" y="' .  $y . '" text-anchor="' . $textAlign . '" >' . htmlentities($word) . '</tspan>';
      $y += 44;
    }
    $svgText .= '</text>';
    break;
  case 'inline': //----------------------------------------------------------
    $width = 25 * strlen($item);
    $height = 44;
    $svgText .= '<text x="0" y="0" font-size="44" font-style="' .
      $fontStyle . '" font-family="' . $fontFamily . '" text-decoration="' . $textDecoration .
      '" font-weight="' . $fontWeight . '">';
    $svgText .= '<tspan x="' . ($width / 2) . '" y="44" text-anchor="middle" >' . htmlentities($item) . '</tspan>';
    $svgText .= '</text>';
    break;
  case 'block': //--------------------------------------------------------------
    $totalLength = strlen($item);
    $idealLength = intval($totalLength / sqrt($totalLength/2)); // roughly n lines of 2n characters
    if ($longestWord > $idealLength) $idealLength = $longestWord; // unless a word is too long!
    $lines = array();
    $line = '';
    $lineLength = 0;
    $longestLine = 0;
    foreach ( $words as $word ) {
      if ( $lineLength + strlen($word) > $idealLength ) {
        $line = trim($line);
        if (strlen($line) > $longestLine ) $longestLine = strlen($line);
        $lines[] = $line;
        $line = $word . ' ';
        $lineLength = strlen($word);
      } else {
        $line .= $word . ' ';
        $lineLength += strlen($word) + 1;
      }
    }
    $line = trim($line);
    if ($line) {
      $lines[] = $line;
      if (strlen($line) > $longestLine ) $longestLine = strlen($line);
    }
    $width = 25 * $longestLine;
    $height = 44 * count($lines);
    $y = 44;
    switch ($textAlign) {
      case 'start':
        $x = 0;
        break;
      case 'end':
        $x = $width;
        break;
      case 'middle':
        $x = $width / 2;
        break;
    }
    $svgText .= '<text x="0" y="0" font-size="44" font-style="' .
      $fontStyle . '" font-family="' . $fontFamily . '" text-decoration="' . $textDecoration .
      '" font-weight="' . $fontWeight . '">';
    foreach ($lines as $line) {
      $svgText .= '<tspan x="' . $x . '" y="' .  $y . '" text-anchor="' . $textAlign . '" >' . htmlentities($line) . '</tspan>';
      $y += 44;
    }
    $svgText .= '</text>';
    break;
  case 'enarched': //-----------------------------------------------------------
  case 'embowed':
  $width = 450;
  $height = 200;
  $fontSize = 1000 / strlen($item); // rough font scaling for the arc below
  if ( array_key_exists($fontFamily,$fontAdjustments)) $fontSize *= $fontAdjustments[$fontFamily];
  if ( $fontSize > 150 ) $fontSize = 150; // but not too large!
  if ( $textAlign == 'middle')
    $startOffset = '50%';
  elseif ( $textAlign == 'start')
    $startOffset = '0%';
  else
    $startOffset = '100%';
  if ( $fontSize > 150 ) $fontSize = 150; // but not too large!
  if ( $textLayout == 'enarched')
    $arch = add_def('path d="m 25,200 a1,0.7 0 0,1 400,0"',null);
  else
    $arch = add_def('path d="m 25,0 a1,0.7 0 0,0 400,0"',null);
  $svgText .= '<defs><path id="#' . $arch .  '" d="m 0,0 "/></defs>'; // dummy defs to make svgCode parse in simplexml_load
  $svgText .= '<text text-anchor="' . $textAlign . '" font-weight="' . $fontWeight . '" font-style="' . $fontStyle . '" font-family="' . $fontFamily . '" font-size="' . $fontSize . '" x="0" y="400"><textPath startOffset="' . $startOffset . '" href="#' . $arch . '">' . htmlentities($item) . '</textPath></text>';
    break;
  case 'wavy':
    $width = 800;
    $height = 320;
    if ( $textAlign == 'middle')
      $startOffset = '50%';
    elseif ( $textAlign == 'start')
      $startOffset = '0%';
    else // 'end'
      $startOffset = '100%';
    $fontSize = 2400 / strlen($item); // rough font scaling for the arc below
    if ( array_key_exists($fontFamily,$fontAdjustments)) $fontSize *= $fontAdjustments[$fontFamily];
    if ( $fontSize > 150 ) $fontSize = 150; // but not too large!
    $arch = add_def('path d="m 25,160 a1,0.7 0 0,1 400,0 a1,0.7 0 0,0 400,0"',null);
    $svgText .= '<defs><path id="#' . $arch .  '" d="m 0,0 "/></defs>'; // dummy defs to make svgCode parse in simplexml_load
    $svgText .= '<text text-anchor="' . $textAlign . '" font-weight="' . $fontWeight . '" font-style="' . $fontStyle . '" font-family="' . $fontFamily . '" font-size="' . $fontSize . '" x="0" y="400"><textPath startOffset="' . $startOffset . '" href="#' . $arch . '">' . htmlentities($item) . '</textPath></text>';
    break;
  case 'encircled':
    $width = 500;
    $height = 400;
    $fontSize = 3000 / strlen($item); // rough font scaling for the arc below
    if ( array_key_exists($fontFamily,$fontAdjustments)) $fontSize *= $fontAdjustments[$fontFamily];
    if ( $fontSize > 150 ) $fontSize = 150; // but not too large!
    $arch = add_def('path d="m 50,200 a200,200 0 1,1 400,0 200,200 0 1,1 -400,0"',null);
    $svgText .= '<defs><path id="#' . $arch .  '" d="m 0,0 "/></defs>'; // dummy defs to make svgCode parse in simplexml_load
    $svgText .= '<text text-anchor="middle" font-weight="' . $fontWeight . '" font-style="' . $fontStyle . '" font-family="' . $fontFamily . '" font-size="' . $fontSize . '" x="0" y="400"><textPath startOffset="50%" href="#' . $arch . '">' . htmlentities($item) . '</textPath></text>';
    break;
}

$charge['svgCode'] = '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" height="' .
        $height . '" width="' . $width . '"><g id="body">' . $svgText . '</g></svg>';

