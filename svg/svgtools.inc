<?php 

function add_def($element = '', $content = '', $name = '', $title = '', $desc = ''){
  static $defs;

  if ( $element == '' ) { // If no arguments then return a complete <defs> section
    if ( $defs == '' ) { // Unless nothing has been defined yet
      $retval = '';
    } else {
      $retval = '<defs>' . $defs . '</defs>';
      $defs = ''; // Clear defs, ready for some more
    }
    return $retval;
  } // else if we given some arguments, then create an approprate definition
  $element_words = explode(' ', $element);
  $type = $element_words[0];
  if ( $name == '' ) { $name = $type; }
  $myid = unique($name);
  $def = '<' . $element . ' id="' . $myid . '" >' . "\n";
  if ($title != '' ) {
    $def .= '<title>' . $title . "</title>\n";
  }
  if ($desc != '') {
    $def .= '<desc>' . $desc . "</desc>\n";
  }
  $def .= $content;
  $def .= "\n</" . $type . ">\n";

  $defs .= $def;
  return $myid; // and return an id unique to this SVG file
}


function splitStyle ( $styles, $attributeKeys ) {
  $retval = '';
  $styles = explode(';', $styles . ';');
  foreach ($styles as $style) {
    if ( $style == null ) continue;
    list($setting,$value) = explode(':',$style);
    if (in_array($setting, $attributeKeys)) continue; // actual attributes take priority
    switch($setting) {
      case 'fill': // Any of the "primary" colours are just placeholders, to be replaced by the charge colour
        if ( !in_array( $value, array ( '#ff0000', '#00ff00', '#0000ff', '#ffff00', '#00ffff', '#ff00ff',
                                        'rgb(255,0,0)', 'rgb(255,255,0)', 'rgb(0,255,0)', 'rgb(0,0,255)')) ) $retval .= 'fill="' . $value . '" ';
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


function rewriteAttributes($attributes, $class = '', $fillSet = false) {
//  global $messages;
  $retval = '';
  // $myId = '';
  // Collect names of all the attributes
  $existingAttributes = array();
  foreach ($attributes as $key => $data ) {
    $existingAttributes[] = $key;
  }
  if ($fillSet) $existingAttributes[] = 'fill';
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
        $retval .= splitStyle( $data, $existingAttributes);
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
      case 'stroke-width':
        $retval .= ' stroke-width="' . $data . '" ';
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
  if ( $name == 'rect' && array_key_exists($id, $extras)) { // Special handling for extra items, e.g. crowns
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
  $fillSet = false;
  $item = "<$name ";
  if ($id != null && substr($id, 0, 4) != 'path' && substr($id, 0, 5) != 'group') {
    // We allow multiple features with numeric flags at the end
    $len = strlen($id);
    if ($id[$len-2] == '-' && ctype_digit($id[$len-1])) $id = substr($id, 0, $len - 2);
    if (array_key_exists($id, $featureColours)) { // this is a feature with a given colour
      $class = "feature $id";
      $item .= ' fill="' . $featureColours[$id] . '" ';
      $fillSet = true;
    } elseif (array_key_exists('proper', $featureColours)) { // use proper for features not otherwise coloured
      if ($foundProper = getProper($chargeGroup, $chargeName, $id, false))
        $item .= ' fill="' . $foundProper . '" '; // only set colour if known, otherwise inherit parent colour
      $fillSet = true;
    } elseif ($id == 'outline') { // This is a path that is actually a stroke
      $item .= ' fill="' . rgb('charge-stroke') . '" ';
      $fillSet = true;
    }
  }
  $item .= rewriteAttributes($element->attributes(), $class, $fillSet);
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

function getSvgFragment ($svgCode, $featureTinctures, $chargeGroup, $chargeName, $extras, $addStroke = true) {
  global $messages, $options;
  $retval = array();

  if ( ($svg = simplexml_load_string($svgCode)) == null ) {
    $messages->addMessage('internal',"XML error reading charge data");
    error_log("XML error reading charge data - " . $chargeGroup . '/' . $chargeName);
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
  if ($addStroke){
    $fragment = '<g stroke="' . rgb('charge-stroke') . '">' . $fragment . '</g>';
  }
  $retval['body'] = $fragment;
  return $retval;
}
