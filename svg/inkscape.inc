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


function splitStyle ( $styles ) {
  $retval = '';
  $styles = explode(';', $styles . ';');
  foreach ($styles as $style) {
    if ( $style == null ) continue;
    list($setting,$value) = explode(':',$style);
    switch($setting) {
      case 'fill': // Any of the "primary" colours are just placeholders, to be replaced by the charge colour
        if ( !in_array( $value, array ( '#ff0000', '#00ff00', '#0000ff', '#ffff00')) ) $retval .= 'fill="' . $value . '" ';
        break; // any other colour is left as it is
      case 'fill-opacity':
      case 'opacity':
        if ( $value != '1' ) $retval .= $setting . '="' . $value . '" ';
        break;
      case 'stroke-width':
      case 'fill-rule':
        $retval .= $setting . '="' . $value . '" ';
        break;
      case 'stroke':
        if ( $value == '#000000' ) $value = 'inherit';
        $retval .= 'stroke="' . $value . '" ';
        break;
    }
  }
  return $retval;
}


function rewriteAttributes($attributes, $class = '') {
//  global $messages;
  $retval = '';
  // $myId = '';
  foreach ($attributes as $key => $data ) {
    switch ($key) {
      case 'id':
        // $myId = $data;
        // $retval .= " id=\"$data\" ";  // remove IDs as they will not be unique
        break;
      case 'class':
        $class .= " $data ";
        break;
      case 'style':
        $retval .= splitStyle( $data );
        break;
      case 'href':
        $retval .= 'xlink:href="' . $data . '" ';
        break;
      case 'fill': // Any of the "primary" colours are just placeholders, so we ignore them (will be replaced by the charge colour)
        if ( !in_array( strtolower($data), array ( '#ff0000', '#00ff00', '#0000ff', '#ffff00', '#ff00ff', '#00ffff')) )
          $retval .= ' fill="' . $data . '" ';
        break; // any other colour is left as it is
      case 'stroke':
        if ( $data == '#000000' ) $data = 'inherit';
        $retval .= ' stroke="' . $data . '" ';
        break;
      case 'transform':
        // $messages->addMessage('warning',"Transform in element $myId " );
        $retval = ' transform="' . $data . '" ';
        break;
      default:
        if (( strpos($key,'sodipodi:') === false ) and (strpos($key,'inkscape:') === false ))
          $retval .= $key . '="' . $data . '" ';
        break;
    }
  }
  if ( $class != '' ) $retval .= ' class="' . trim($class) . '" ';
  return $retval;
}

function addChild(SimpleXMLElement $element, $featureColours, $chargeGroup, $chargeName, $extras) {
  $name = $element->getName();
  if ( in_array($name, array( 'metadata', 'defs', 'title' ) ))
    return null;
  if (( strpos($name,'sodipodi:') !== false ) or (strpos($name,'inkscape:') !== false ))
    return null;
  $id = isset($element['id']) ? (string)$element['id'] : null;
  $class = '';
  //---------------------------------------------------
  // Handle invisible rectangles marking additional charge positions
  if ( $name == 'rect' and array_key_exists($id, $extras)) { // Special handling for extra items, e.g. crowns
    $item = '';
    if ( isset($element['transform']))
      $item .= '<g transform="' . $element['transform'] . '">'; // Apply the original transform (e.g. to tilt crown)
    if ( $id == 'crowned' ) { // crowns just use width, then drop to the bottom
      $scale = ($element['width'] / $extras[$id]['width']);
      $yAdjustment = $element['height'] - ($extras[$id]['height'] * $scale);
      $xAdjustment = 0;
    } else { // everything else scales to fit centrally in the rectangle
      $scale = min ( $element['height'] / $extras[$id]['height'], $element['width'] / $extras[$id]['width']);
      $yAdjustment = ($element['height'] - ($extras[$id]['height'] * $scale))/2;
      $xAdjustment = ($element['width'] - ($extras[$id]['width'] * $scale))/2;
    }
    $item .= '<g transform="translate(' . ($element['x'] + $xAdjustment) . ',' . ($element['y'] + $yAdjustment) . ')">'; // Move to the box position, adjust if we are shorter than the box
    // Scale item to the size of the original rectangle
    $item .= '<g transform="scale(' . $scale . ',' . $scale . ')">';
    if ( array_key_exists('crowned',$featureColours) )
      $item .= '<g fill="' . $featureColours['crowned'] . '">' . $extras[$id]['body'] . '</g>';
    else
      $item .= $extras[$id]['body'];
    $item .= '</g></g>';
    if ( isset($element['transform'])) $item .= '</g>'; // close the groups above
    return $item;
  }
  //-------------------------------------------------------------------------
  $item = "<$name ";
  if ($id != null and substr($id, 0, 4) != 'path' and substr($id, 0, 5) != 'group') {
    if (array_key_exists($id, $featureColours)) { // this is a feature with a given colour
      $class = "feature $id";
      $item .= ' fill="' . $featureColours[$id] . '" ';
    } elseif (array_key_exists('proper', $featureColours)) { // use proper for features not otherwise coloured
      if ($foundProper = getProper($chargeGroup, $chargeName, $id, false))
        $item .= ' fill="' . $foundProper . '" '; // only set colour if known, otherwise inherit parent colour
    } elseif ($id == 'outline') { // This is a path that is actually a stroke
      $item .= ' fill="' . rgb('charge-stroke') . '" ';
    }
  }
  $item .= rewriteAttributes($element->attributes(), $class);
  $subitem = '';
  if ( $name == 'text' or $name == 'tspan' or $name == 'textPath' ) // We need the element content as well
    $subitem = htmlentities((string)$element);
  foreach( $element->children() as $child )
      $subitem .= addChild($child,$featureColours, $chargeGroup, $chargeName, $extras);
  if ( $subitem != '') {
    $item .= ">$subitem</$name>\n";
  } else {
    $item .= "/>\n";
  }
  return $item;
}

function getSvgFragment ($svgCode, $featureTinctures, $chargeGroup, $chargeName, $extras) {
  global $messages, $options;
  $retval = array();

  if ( ($svg = simplexml_load_string($svgCode)) == null ) {
    $messages->addMessage('internal',"XML error reading charge data");
    error_log("charge data error - " . $options['blazon']);
    // Fallback item
    $svg = simplexml_load_string('<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                <svg
                xmlns="http://www.w3.org/2000/svg"
                width="100"
                height="100"
                version="1.1" >
                <g style="fill:#ffff00;fill-opacity:1;fill-rule:evenodd;stroke:#000000;stroke-width:3;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-opacity:1;stroke-dasharray:none" id="body"><path fill-rule="evenodd" d="M0,0h100v100h-100zM15,15h70v70h-70z" />
                 <text x="50" y="65" font-size="30" text-anchor="middle">????</text></g>
                </svg>');
  }
  $retval['height'] = floatval($svg['height']);
  $retval['width'] = floatval($svg['width']);
  $retval['licence'] = '(not known)';
  @$metadata = $svg->xpath('//cc:Agent/dc:title');
  if ( is_array($metadata) and count($metadata) > 0) {
    $retval['licence'] = $metadata[0];
  }
  $fragment = '';
  foreach( $svg->children() as $child )
    $fragment .= addChild($child, $featureTinctures, $chargeGroup, $chargeName, $extras);
  // Set the global stroke colour
  $fragment = '<g stroke="' . rgb('charge-stroke') . '">' . $fragment . '</g>';
  $retval['body'] = $fragment;
  return $retval;
}
