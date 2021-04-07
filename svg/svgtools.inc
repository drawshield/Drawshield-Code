<?php 

function make_def($element, $content, $name = '', $title = '', $desc = '')
{
    $element_words = explode(' ', $element);
    $type = $element_words[0];
    if ( $name == '' )
        $name = $type;
    $myid = unique($name);
    $def = '<' . $element . ' id="' . $myid . '" >' . "\n";
    if ($title != '' )
        $def .= '<title>' . $title . "</title>\n";
    if ($desc != '')
        $def .= '<desc>' . $desc . "</desc>\n";
    $def .= $content;
    $def .= "\n</" . $type . ">\n";

    return [$myid, $def];
}

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

  list($myid, $def) = make_def($element, $content, $name, $title, $desc);
  $defs .= $def;
  return $myid; // and return an id unique to this SVG file
}

$targetColours = array (
  '#ff0000', '#00ff00', 
  '#0000ff', '#ffff00',
  '#00ffff', '#ff00ff',
  'rgb(255,0,0)', 'rgb(0,255,0)', 
  'rgb(0,0,255)', 'rgb(255,255,0)',
  'rgb(0,255,255)', 'rgb(255,0,255)',
  '#f00', '#0f0', 
  '#00f', '#ff0',
  '#0ff', '#f0f'
);

$strokeColours = array (
    '#cccccc',
);

function splitStyle ( $styles, $attributeKeys, $strokeCol, $strokeWidth ) {
  global $targetColours, $options, $strokeColours;
  $retval = '';

  $styleArray = explode(';', $styles . ';');
  foreach ($styleArray as $style) {
    if ( $style == null ) continue;
    list($setting,$value) = explode(':',$style);
    if (in_array($setting, $attributeKeys)) {
      continue; // actual attributes take priority
    }
    switch($setting) {
      case 'fill': 
       if ( array_key_exists("raw",$options) ) {
          $retval .= ' fill="' . $value . '" '; // leave alone
       } else {
            if (in_array(strtolower($value), $strokeColours)) {
                $retval .= 'fill="' . $strokeCol . '" ';
          // Any of the "primary" colours are just placeholders, to be replaced by the charge colour
            } elseif (!in_array( $value, $targetColours )) {
                $retval .= 'fill="' . $value . '" ';
            }
        }
        break; // any other colour is left as it is
      case 'fill-opacity':
      case 'opacity':
        if ( $value != '1' ) $retval .= $setting . '="' . $value . '" ';
        break;
      case 'stroke-width':
          $multiplier = ($strokeWidth != null) ? $strokeWidth : rgb('stroke-width');
          if ($multiplier != 1)
              $value = floatval($value) * $multiplier;
        $retval .= $setting . '="' . $value . '" ';
        break;
      case 'fill-rule':
        $retval .= $setting . '="' . $value . '" ';
        break;
      case 'stroke':
          if ($strokeCol) { // only set stroke if told to
            if ($value != 'none'){
                $value = $strokeCol;
            }
            $retval .= 'stroke="' . $value . '" ';
          }
        break;
    }
  }

  return $retval;
}


function rewriteAttributes($attributes, $strokeCol, $strokeWidth, $class = '', $fillSet = false) {
//  global $messages;
  global $targetColours, $options, $strokeColours;
  $retval = '';

  // Collect names of all the attributes
  $existingAttributes = array();
  foreach ($attributes as $key => $data ) {
    $existingAttributes[] = $key;
  }
  if ($fillSet) $existingAttributes[] = 'fill';
  $fill = '';
  $style = ''; // decide whether to use style or fill
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
        $style = splitStyle( $data, $existingAttributes, $strokeCol, $strokeWidth);
        break;
      case 'href':
        $retval .= 'xlink:href="' . $data . '" ';
        break;
      case 'fill': 
        if ( array_key_exists("raw",$options) ) {
          $fill = ' fill="' . $data . '" '; // leave alone
        } else {
            if (in_array(strtolower($data), $strokeColours)) {
                $fill = ' fill="' . $strokeCol . '" ';
            } elseif (!$fillSet && !in_array( strtolower($data), $targetColours) ) {
          // Any of the "primary" colours are just placeholders, so we ignore them (will be replaced by the charge colour)
                $fill = ' fill="' . $data . '" ';
            }
        }
        break; // any other colour is left as it is
      case 'stroke':
          if ($strokeCol) { // only set stroke if told to
            if ($data != 'none'){
                $data = $strokeCol;
            }
            $retval .= ' stroke="' . $data . '" ';
          }
        break;
      case 'transform':
        // $messages->addMessage('warning',"Transform in element $myId " );
        $retval .= ' transform="' . $data . '" ';
        break;
      case 'stroke-width':
          $multiplier = ($strokeWidth != null) ? $strokeWidth : rgb('stroke-width');
          if ($multiplier != 1)
              $data = floatval($data) * $multiplier;
        $retval .= ' stroke-width="' . $data . '" ';
       break; 
      default:
        if (( strpos($key,'sodipodi:') === false ) and (strpos($key,'inkscape:') === false ))
          $retval .= $key . '="' . $data . '" ';
        break;
    }
  }
  if (strpos($style,'fill=') !== false) {
      $fill = ''; // remove fill attribute if already set in style
  }
  if ( $class != '' ) $retval .= ' class="' . trim($class) . '" ';
  $retval .= $fill . ' ' . $style;
  return $retval;
}

function hold_charge($element, $held)
{
    $dest = new ShieldPos(floatval($element["cx"]), floatval($element["cy"]));
    $anchor_radius = floatval($element["r"] ?? $element["rx"]);
    $hold_point = $held["hold_point"];
    $anchor_transform = $element['transform'] ?? "";
    $xpath = new DomXPath($held["node"]->ownerDocument);

    $angle = 0;
    $orient = empty($held["horizontal"]) ? 1 : -1;
    if ( $orient == -1 )
        $angle += 90;

    $scale_x = $scale_y = $anchor_radius / $held["hold_radius"];
    $anchor_angle = 0;
    $matches = [];
    preg_match_all('/rotate\s*\(\s*([^,)]+)/', $anchor_transform, $matches);
    foreach ( $matches[1] as $match )
        $anchor_angle += (float)trim($match);

    $charge_mods = $xpath->query("modifier", $held["node"]);

    foreach ( $charge_mods as $mod )
    {
        switch ( $mod->getAttribute('keyterm') )
        {
            case "palewise":
                $angle = -$anchor_angle;
                break;
            case "fesswise":
                $angle = -$anchor_angle + $orient * 90;
                break;
            case "bendwise":
                $angle = -$anchor_angle - 45;
                break;
            case "bendsinwise":
                $angle = -$anchor_angle + 45;
                break;
            case 'inverted':
                $scale_y *= -1;
                break;
            case 'reversed':
                $scale_x *= -1;
                break;
            case 'rotated-dexter':
                $angle += -18;
                break;
            case 'rotated-sinister':
                $angle += 18;
                break;
            case 'slightly-rotated-dexter':
                $angle += -9;
                break;
            case 'slightly-rotated-sinister':
                $angle += 9;
                break;
            case 'much-rotated-dexter':
                $angle += -27;
                break;
            case 'much-rotated-sinister':
                $angle += 27;
                break;
            case 'very-much-rotated-dexter':
                $angle += -36;
                break;
            case 'very-much-rotated-sinister':
                $angle += 36;
                break;
            case 'narrower':
                $scale_x *= 0.8;
                break;
            case 'shorter':
                $scale_y *= 0.8;
                break;
            case 'taller':
                $scale_y *= 1.2;
                break;
            case 'wider':
                $scale_x *= 1.2;
                break;
            case 'much-narrower':
                $scale_x *= 0.8;
                break;
            case 'much-shorter':
                $scale_y *= 0.8;
                break;
            case 'much-taller':
                $scale_y *= 1.4;
                break;
            case 'much-wider':
                $scale_x *= 1.4;
                break;
            case 'slightly-narrower':
                $scale_x *= 0.9;
                break;
            case 'slightly-shorter':
                $scale_y *= 0.9;
                break;
            case 'slightly-taller':
                $scale_y *= 1.1;
                break;
            case 'slightly-wider':
                $scale_x *= 1.1;
                break;
            case 'very-much-narrower':
                $scale_x *= 0.6;
                break;
            case 'very-much-shorter':
                $scale_y *= 0.6;
                break;
            case 'very-much-taller':
                $scale_y *= 1.6;
                break;
            case 'very-much-wider':
                $scale_x *= 1.6;
                break;
            case 'smaller':
                $scale_x *= 0.8;
                $scale_y *= 0.8;
                break;
            case 'larger':
                $scale_x *= 1.2;
                $scale_y *= 1.2;
                break;
            case 'slightly-smaller':
                $scale_x *= 0.9;
                $scale_y *= 0.9;
                break;
            case 'slightly-larger':
                $scale_x *= 1.1;
                $scale_y *= 1.1;
                break;
            case 'much-smaller':
                $scale_x *= 0.6;
                $scale_y *= 0.6;
                break;
            case 'much-larger':
                $scale_x *= 1.4;
                $scale_y *= 1.4;
                break;
            case 'very-much-smaller':
                $scale_x *= 0.4;
                $scale_y *= 0.4;
                break;
            case 'very-much-larger':
                $scale_x *= 1.6;
                $scale_y *= 1.6;
                break;
            case 'raised':
            case 'lowered':
            case 'to-dexter':
            case 'to-sinister':
            case 'slightly-raised':
            case 'slightly-lowered':
            case 'slightly-to-dexter':
            case 'slightly-to-sinister':
            case 'much-raised':
            case 'much-lowered':
            case 'much-to-dexter':
            case 'much-to-sinister':
            case 'very-much-raised':
            case 'very-much-lowered':
            case 'very-much-to-dexter':
            case 'very-much-to-sinister':
                // unsupported
                break;
        }
    }

    $dx = $dest->x - $hold_point->x * $scale_x;
    $dy = $dest->y - $hold_point->y * $scale_y;

    $rx = $hold_point->x * $scale_x;
    $ry = $hold_point->y * $scale_y;
    $rotation = $angle == 0 ? "" : "rotate($angle, $rx, $ry)";

    $item = "<g transform='$anchor_transform translate($dx, $dy) $rotation scale($scale_x, $scale_y) '>";// scale($post_scale_x, $post_scale_y)'>";
    $body = $held["body"];
    $tincture = $xpath->query("tincture", $held["node"]);
    if ( count($tincture) )
        $body = apply_tincture($tincture[0], $body, $held["layout"], new ShieldSize($held["width"], $held["height"]), 0, false, false);
    $item .= $body;
    $item .= "</g>";
    return $item;
}

function addChild(SimpleXMLElement $element, $featureColours, $chargeGroup, $chargeName, 
    $extras, $strokeCol, $strokeWidth, $fillSet = false ) {
    $name = $element->getName();
    if ( in_array($name, array( 'metadata', 'defs', 'title' ) ))
        return null;
    if (( strpos($name,'sodipodi:') !== false ) or (strpos($name,'inkscape:') !== false ))
        return null;
    $id = isset($element['id']) ? (string)$element['id'] : null;
    if ( $id == "hold-point" )
        return '';
    $class = '';
    //---------------------------------------------------
    // Handle invisible rectangles marking additional charge positions
    if ( $name == 'rect' ) {
        if ( array_key_exists($id, $extras)) { // Special handling for extra items, e.g. crowns

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

            if ( isset($element['transform']) )
                $item .= '</g>'; // close the groups above

            return $item;
        }
    }
    else if ( ($name == "ellipse" || $name == "circle") && array_key_exists($id, $extras) )
    {
        return hold_charge($element, $extras[$id]);
    }
  //-------------------------------------------------------------------------
  $visible = true;
  if ( isset($element["style"]) && strpos($element["style"], "display:none") !== false )
    $visible = false;


  $item = "<$name ";
    if ($id == 'textPath') $item .= ' id="textPath" '; // need this for the motto
    if ($id != null && substr($id, 0, 4) != 'path' && substr($id, 0, 5) != 'group') {
    // We allow multiple features with numeric flags at the end
    $len = strlen($id);
    if ($len >= 2 && $id[$len-2] == '-' && ctype_digit($id[$len-1])) {
      $id = substr($id, 0, $len - 2);
    }
    if (array_key_exists($id, $featureColours)) { // this is a feature with a given colour
      $class = "feature $id";
      $item .= ' fill="' . $featureColours[$id] . '" ';
      $fillSet = true;
      $visible = true;
    } elseif (substr($id, 0, 7) == 'outline') { // This is a path that is actually a stroke
        $item .= ' fill="' . $strokeCol . '" ';
        $fillSet = true;
    } elseif (array_key_exists('proper', $featureColours)) { // use proper for features not otherwise coloured
      if ($foundProper = getProper($chargeGroup, $chargeName, $id, false))
        $item .= ' fill="' . $foundProper . '" '; // only set colour if known, otherwise use parent colour
        $fillSet = true;
    }
  }

  if ( !$visible )
    return '';

  $item .= rewriteAttributes($element->attributes(), $strokeCol, $strokeWidth, $class, $fillSet);
  $subitem = '';
  if ( $name == 'text' or $name == 'tspan' or $name == 'textPath' ) {
      // We need to preserve some of the important text related attributes
      if (isset($element['style'])) $item .= ' style="' . $element['style'] . '" ';
      $item .= ' xml:space="preserve" ';
      // We need the element content as well
      $subitem = htmlentities((string)$element, ENT_XML1);
  }
  foreach( $element->children() as $child )
      $subitem .= addChild($child,$featureColours, $chargeGroup, $chargeName, $extras, $strokeCol, $strokeWidth, $fillSet );
  if ( $subitem != '') {
    $item .= ">$subitem</$name>\n";
  } else {
    $item .= "/>\n";
  }
  return $item;
}

function getSvgFragment ($svgCode, $featureTinctures, $chargeGroup, $chargeName, $extras, $strokeCol = null, $strokeWidth = null) {
  global $messages, $options;
  $retval = array();

  libxml_use_internal_errors(true);
  if ( ($svg = @simplexml_load_string($svgCode, null, LIBXML_NOENT)) == null ) {
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
  libxml_clear_errors();
  $viewbox = $svg['viewBox'] ?? '0 0 100 100';
  list($x0,$y0,$x1,$y1) = explode(' ',$viewbox);
  $heightVal = $svg['height'] ?? $x1;
  if (substr($heightVal,-1) == '%') {
    $retval['height'] = ($y1 - $y0) * intval($heightVal) / 100;
  } else {
    $retval['height'] = floatval($heightVal);
  }
  $widthVal = $svg['width'] ?? $y1;
  if (substr($widthVal,-1) == '%') {
    $retval['width'] = ($x1 - $x0) * intval($widthVal) / 100;
  } else {
    $retval['width'] = floatval($widthVal);
  }
  // do we need to adapt for offset view box (e.g. traceable heraldic art)
  $offset = false;
  if ($x0 != 0 || $y0 != 0) {
    $offset = "translate(-$x0,-$y0)";
  }
   @$metadata = $svg->xpath('//dc:title');
  if ( is_array($metadata) and count($metadata) > 0) {
    $licence = 'unknown';
    foreach($metadata as $meta) {
      if (strlen($meta)) {
        $licence = $meta;
        break;
      }
    }
    // var_dump($metadata);
    $messages->addMessage('licence', "$chargeGroup/$chargeName - $licence");
//    $retval['licence'] = $metadata[0];
  }
  // set a default stroke colour
  $fragment = '';
  foreach( $svg->children() as $child )
    $fragment .= addChild($child, $featureTinctures, $chargeGroup, $chargeName, $extras, $strokeCol, $strokeWidth );
  // Set the global stroke colour
  // if ($addStroke){
  //   $fragment = '<g stroke="' . $addStroke . '">' . $fragment . '</g>';
  // }
  if ($offset) {
    $fragment = "<g transform=\"$offset\">$fragment</g>\n";
  }
  $retval['body'] = $fragment;

    $hold_point = $svg->xpath("//*[@id='hold-point']");
    if ( count($hold_point) > 0 )
    {
        $retval["hold_point"] = new ShieldPos(floatval($hold_point[0]["cx"]), floatval($hold_point[0]["cy"]));
        $retval["hold_radius"] = floatval($hold_point[0]["r"] ?? $hold_point[0]["rx"]);
    }
    else
    {
        $retval["hold_point"] = new ShieldPos($retval['width']/2, $retval['height']/2);
        $retval["hold_radius"] = $retval['width']/12;
    }

  return $retval;
}
