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

function getSvgTreatment ($filename) {
  global $messages;
  $retval = array();

  if ( ($svg = simplexml_load_file($filename)) == null ) {
    $messages->addMessage('internal',"XML error reading treatment data");
    return null;
  }
  $retval['height'] = floatval($svg['height']);
  $retval['width'] = floatval($svg['width']);
  $metadata = @$svg->xpath('//cc:Agent/dc:title');
  if ( is_array($metadata) and count($metadata) > 0) {
    $messages->addMessage('licence', $metadata[0]);
  }
  $fragment = '';
  foreach( $svg->children() as $child ) {
    if ( $child->getName() != 'metadata' and $child->getName() != 'defs' )
      $fragment .= $child->asXML();
  }
  $retval['body'] = $fragment;
  return $retval;
}


function makeTreatment ( $node, $layout, $counter = false ) {
  global $messages;
  $folder = __dir__ . '/treatments/';
  $treatment_data = [];

  $name = $node->getAttribute('keyterm');
  $tinctures = array();
  $children = $node->childNodes;
  $parent = $node->parentNode;
  $reverse = true;
  if ($name == 'tartan-no-repeat') {
      $name = 'tartan';
      $reverse = false;
  }
  // This file can set $filename and/or overwrite $tinctures  (and has access to the other variables above)
  if ( file_exists("svg/treatments/$name.inc"))
    include "svg/treatments/$name.inc";
  if (array_key_exists('pattern', $treatment_data)) {
        $size_x = $treatment_data['width'];
        $size_y = $treatment_data['height'];
        $pattern = $treatment_data['pattern'];
        $base = '';
  } else {
        foreach ($children as $child) {
            if ($child->nodeName == 'tincture')
                $tinctures[$child->getAttribute('index')] = $child;
        }
        if (!isset($filename))
            $filename = "$folder$name.svg";
        if (file_exists($filename))
            $treatment_data = getSvgTreatment($filename);
        else {
            $treatment_data = getSvgTreatment("${folder}checky.svg");
            $messages->addMessage('warning', "Checky substituted for treatment $name");
        }
        $treatment = $treatment_data['body'];
        $size_x = $treatment_data['width'];
        $size_y = $treatment_data['height'];
        $size = new ShieldSize($size_x, $size_y);
        if ($counter) {
            $base = apply_tincture($tinctures[2], '<rect x="0" y="0" width="' .
                $size_x . '" height="' . $size_y . '" />', $layout, $size);
            $pattern = apply_tincture($tinctures[1], $treatment, $layout, $size);
        } else {
            $base = apply_tincture($tinctures[1], '<rect x="0" y="0" width="' .
                $size_x . '" height="' . $size_y . '" />', $layout, $size);
            $pattern = apply_tincture($tinctures[2], $treatment, $layout, $size);
        }
  }
  // Do we need to offset to make treatment symmetrical on the field?
  $offset = (1000 % intval($size_x) / 2);
  if (in_array($name, array('gingham', 'buffalo-check', 'glen-check', 'houndstooth', 'shepherd-check'))) $offset += intval($size_x / 4);
  $retval = array ( 'width' =>  $size_x, 'height' => $size_y, 'offset' => $offset,
      'body' => $base . '<g stroke-width="2" stroke="' . rgb('pattern-stroke') . '" >' .$pattern . '</g>' );
  return $retval;
}

function makeSemy( $node, $layout ) {
  global $messages;
  $children = $node->childNodes;
  $background = $charge1 = $charge2 = null;
  foreach ( $children as $child ) {
    if ( $child->nodeName == 'tincture' ) {
        $background = $child;
    } elseif ( $child->nodeName == 'charge' ) {
        if ($charge1 == null)
            $charge1 = $child;
        else
            $charge2 = $child;
    }
  }
  if ( $background == null or $charge1 == null ) {
      $messages->addMessage('internal', 'Bad semyde element');
      return null;
  }

  if ($charge2 == null) {
//    $semy = PlaceCharge($charge1, 100, 100, 100, 100);
//    $semy .= PlaceCharge($charge1, 100, 100, 0, 0);
//    $semy .= PlaceCharge($charge1, 100, 100, 200, 0);
//    $semy .= PlaceCharge($charge1, 100, 100, 0, 200);
//    $semy .= PlaceCharge($charge1, 100, 100, 200, 200);
    $semy = PlaceCharge($charge1, $layout, 100, 100, 100, 0);
    $semy .= PlaceCharge($charge1, $layout, 100, 100, 100, 200);
    $semy .= PlaceCharge($charge1, $layout, 100, 100, 0, 100);
    $semy .= PlaceCharge($charge1, $layout, 100, 100, 200, 100);
    $base = apply_tincture ( $background, '<rect x="0" y="0" width="200" height="200" />', $layout );
    return array (  'width' => 200, 'height' => 200, 'body' => $base . $semy );
 } // else we have 2 charges, different pattern
    $semy = PlaceCharge($charge1, $layout, 100, 100, 200, 150);
    $semy .= PlaceCharge($charge1, $layout, 100, 100, 50, 0);
    $semy .= PlaceCharge($charge1, $layout, 100, 100, 350, 0);
    $semy .= PlaceCharge($charge1, $layout, 100, 100, 50, 300);
    $semy .= PlaceCharge($charge1, $layout, 100, 100, 350, 300);
    $semy .= PlaceCharge($charge2, $layout, 100, 100, 200, 0);
    $semy .= PlaceCharge($charge2, $layout, 100, 100, 200, 300);
    $semy .= PlaceCharge($charge2, $layout, 100, 100, 50, 150);
    $semy .= PlaceCharge($charge2, $layout, 100, 100, 350, 150);
    $base = apply_tincture ( $background, '<rect x="0" y="0" width="300" height="300" />', $layout );
    return array (  'width' => 300, 'height' => 300, 'body' => $base . $semy );
}

function makeGrady( $node ) {
  global $messages;
  $colours = [];
  $type = $node->getAttribute("keyterm");
  $inverted = existModifierWithKeyterm($node, languageDB::INVERSE);
  if ($type == 'grady') $type = 'grady-in-pale'; // Default if not set by herald knowledge
  $children = $node->childNodes;
  $background = $charge = null;
  foreach ( $children as $child ) {
    if ( $child->nodeName == 'tincture' ) {
      if ($child->hasChildNodes() && $child->firstChild->nodeName == 'colour') {
        $colours[] = $child->firstChild->getAttribute("keyterm");
      } else {
        $messages->addMessage('blazon',"Grady tinctures must be colours");
        $colours[] = 'gray';
      }
    }
  }
  $stops = [];
  if (count($colours) < 2) { // $colours[] = "grey";
    $colours[1] = shiftRGB(rgb($colours[0]),$inverted ? "darker" : "lighter");
    $colours[0] = shiftRGB(rgb($colours[0]),$inverted ? "lighter" : "darker");
  }
  $numColours= count($colours);
  for ($i = 0; $i < $numColours; $i++) {
    if ($i == 0) $offset = 0;
    elseif ($i == $numColours - 1) $offset = 100;
    else ( $offset = 100/($i - 1));
    if ($type == 'grady-in-annulo') $offset = 100 - $offset;
    $offset = sprintf("%.3f", $offset);
    $rgbCode = $colours[$i][0] == '#' ? $colours[$i] : rgb($colours[$i]);
    $stops[] = "<stop offset=\"$offset%\" style=\"stop-color: $rgbCode\" />";
  }
  if ($type == 'grady-in-annulo') $stops = array_reverse($stops);
  $stop = implode("\n",$stops);

  $gradientType = $type == "grady-in-annulo" ? "radial" : "linear";
  $direction = '';
  switch($type) {
    case "grady-in-pale":
      $direction = ' x1="0%" y1="0%" x2="0%" y2="100%"';
      break;
    case "grady-in-bend":
      $direction = ' x1="0%" y1="0%" x2="100%" y2="100%"';
      break;
    case "grady-in-bendsin":
      $direction = ' x1="100%" y1="0%" x2="0%" y2="100%"';
      break;
  }
  $gradient = add_def("${gradientType}Gradient$direction", $stop, "gradient");
  return $gradient;
}
