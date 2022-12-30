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

require_once("transform.inc");

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
    'rgb(255, 0, 0)',  'rgb(0, 255, 0)',
    'rgb(0, 0, 255)',  'rgb(255, 255, 0)',
    'rgb(0, 255, 255)',  'rgb(255, 0, 255)',
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
            if ($strokeColours && in_array(strtolower($value), $strokeColours)) {
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
      case 'xlink:href':
      case 'href':
        $retval .= 'xlink:href="' . $data . '" ';
        break;
      case 'fill': 
        if ( array_key_exists("raw",$options) ) {
          $fill = ' fill="' . $data . '" '; // leave alone
        } else {
            if (in_array(strtolower($data), $strokeColours)) {
                $fill = ' fill="' . $strokeCol . '" ';
            } elseif ($data == 'none' || (!$fillSet && !in_array( strtolower($data), $targetColours) )) {
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

function hold_charge($element, $held, ShieldRect &$bounding_box)
{
    $dest = new ShieldPos(floatval($element["cx"]), floatval($element["cy"]));
    $anchor_radius = floatval($element["r"] ?? $element["rx"]);
    $hold_point = $held["hold_point"];
    $anchor_transform = $element['transform'] ?? "";
    $xpath = new DomXPath($held["node"]->ownerDocument);


    $hints = new PlacementHints;
    $horizontal = $held["horizontal"] ?? false;;
    if ( $horizontal )
        $hints->angle += 90;

    $hints->scale_x = $hints->scale_y = $anchor_radius / $held["hold_radius"];
    $anchor_angle = 0;
    $matches = [];
    preg_match_all('/rotate\s*\(\s*([^,)]+)/', $anchor_transform, $matches);
    foreach ( $matches[1] as $match )
        $anchor_angle += (float)trim($match);

    $charge_mods = $xpath->query("modifier", $held["node"]);

    $held_move_scale = $held["height"] / 1000;

    foreach ( $charge_mods as $mod )
    {
        $keyterm = $mod->getAttribute('keyterm');
        switch ( $keyterm )
        {
            case "palewise":
                $hints->angle = -$anchor_angle;
                if ( $horizontal )
                    $hints->angle += 90;
                break;
            case "fesswise":
                $hints->angle = -$anchor_angle + 90;
                if ( $horizontal )
                    $hints->angle -= 90;
                break;
            case "bendwise":
                $hints->angle = -$anchor_angle - 45;
                if ( $horizontal )
                    $hints->angle += 90;
                break;
            case "bendsinwise":
                $hints->angle = -$anchor_angle + 45;
                if ( $horizontal )
                    $hints->angle += 90;
                break;
            case 'inverted':
                $hints->scale_y *= -1;
                break;
            case 'reversed':
                $hints->scale_x *= -1;
                break;
            default:
                $hints->adjust($keyterm);
                break;
        }
    }

    $hold_point->y -= $hints->offset_y * $held_move_scale;

    $dx = $dest->x - $hold_point->x * $hints->scale_x;
    $dy = $dest->y - $hold_point->y * $hints->scale_y;

    $rx = $hold_point->x * $hints->scale_x;
    $ry = $hold_point->y * $hints->scale_y;

    $transform = TransformMatrix::from_svg_transform($anchor_transform);
    $transform->translate($dx, $dy);
    if ( $hints->angle != 0 )
        $transform->svg_rotate($hints->angle, $rx, $ry);
    $transform->scale($hints->scale_x, $hints->scale_y);

    $transform_attr = $transform->to_svg_transform();
    $bb = $transform->apply_box($held["bounding_box"]);
    $bounding_box->combine($bb);

    $item = "<g transform='$transform_attr'>";
    $body = $held["body"];
    $tincture = $xpath->query("tincture", $held["node"]);
    if ( count($tincture) )
        $body = apply_tincture($tincture[0], $body, $held["layout"], new ShieldSize($held["width"], $held["height"]), 0, false, false);
    $item .= $body;
    $item .= "</g>";
    return $item;
}

function handle_extra($element, $extra, $featureColours, &$bounding_box)
{
    $item = '';

    if ( $element['id'] == 'crowned' || $element['id'] == 'gorged' ) { // crowns just use width, then drop to the bottom
        $scale = ($element['width'] / $extra['width']);
        $yAdjustment = $element['height'] - ($extra['height'] * $scale);
        $xAdjustment = 0;
    } else { // everything else scales to fit centrally in the rectangle
        $scale = min ( $element['height'] / $extra['height'], $element['width'] / $extra['width']);
        $yAdjustment = ($element['height'] - ($extra['height'] * $scale))/2;
        $xAdjustment = ($element['width'] - ($extra['width'] * $scale))/2;
    }

    // Move to the box position, adjust if we are shorter than the box
    $transform = TransformMatrix::from_svg_transform($element['transform'] ?? "");
    $transform->translate($element['x'] + $xAdjustment, $element['y'] + $yAdjustment);
    $transform->scale($scale);

    if ( !($element['id'] == 'crowned' || $element['id'] == 'gorged') && isset($extra["bounding_box"]) )
    {
        $bb = $transform->apply_box($extra["bounding_box"]);
        $bounding_box->combine($bb);
    }

    $item .= '<g transform="' . $transform->to_svg_transform() . '">';

    if ( $element['id'] == 'crowned' && array_key_exists('crowned', $featureColours) )
        $item .= '<g fill="' . $featureColours['crowned'] . '">' . $extra['body'] . '</g>';
    elseif ($element['id'] == 'gorged' && array_key_exists('gorged', $featureColours) )
        $item .= '<g fill="' . $featureColours['gorged'] . '">' . $extra['body'] . '</g>';
    else
        $item .= $extra['body'];

    $item .= '</g>';

    return $item;
}

function addChild(SimpleXMLElement $element, $featureColours, $chargeGroup, $chargeName, 
    $extras, $strokeCol, $strokeWidth, ShieldRect &$bounding_box, $fillSet = false ) {
    $name = $element->getName();
    if ( $name == "image" )
        return null;
    if ( in_array($name, array( 'metadata', 'defs', 'title' ) ))
        return null;
    if (( strpos($name,'sodipodi:') !== false ) or (strpos($name,'inkscape:') !== false ))
        return null;
    $id = isset($element['id']) ? (string)$element['id'] : "";
    if ( $id == "hold-point" )
        return '';
    $class = '';
    //---------------------------------------------------
    // Handle invisible rectangles marking additional charge positions
    // Special handling for extra items, e.g. crowns
    if ( array_key_exists($id, $extras) )
    {
        if ( $name == 'rect' )
            return handle_extra($element, $extras[$id], $featureColours, $bounding_box);
        else if ( $name == "ellipse" || $name == "circle" )
            return hold_charge($element, $extras[$id], $bounding_box);
    }
  //-------------------------------------------------------------------------
  $visible = true;
  if ( isset($element["style"]) && strpos($element["style"], "display:none") !== false )
    $visible = false;


  $item = "<$name ";
    if (substr($id, 0, 8) == 'textPath') $item .= ' id="' . $id . '" '; // need this for the motto
    if ($id != null && substr($id, 0, 4) != 'path' && substr($id, 0, 5) != 'group') {
    // We allow multiple features with numeric flags at the end
        $id = rtrim($id, "0123456789");
        $id = rtrim($id, "-");
/*    $len = strlen($id);
    if ($len >= 2 && $id[$len-2] == '-' && ctype_digit($id[$len-1])) {
      $id = substr($id, 0, $len - 2);
    } */
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
      {
        $item .= ' fill="' . $foundProper . '" '; // only set colour if known, otherwise use parent colour
        $fillSet = true;
      }
    }
  }

  if ( !$visible )
    return '';

   $item .= rewriteAttributes($element->attributes(), $strokeCol, $strokeWidth, $class, $fillSet);
    $item .= rewriteAttributes($element->attributes("xlink",true), $strokeCol, $strokeWidth );
  $subitem = '';
  if ( $name == 'text' or $name == 'tspan' or $name == 'textPath' ) {
      // We need to preserve some of the important text related attributes
      if (isset($element['style'])) $item .= ' style="' . $element['style'] . '" ';
      $item .= ' xml:space="preserve" ';
      // We need the element content as well
      $subitem = htmlentities((string)$element, ENT_XML1);
  }
  foreach( $element->children() as $child )
      $subitem .= addChild($child,$featureColours, $chargeGroup, $chargeName, $extras, $strokeCol, $strokeWidth, $bounding_box, $fillSet);
  if ( $subitem != '') {
    $item .= ">$subitem</$name>\n";
  } else {
    $item .= "/>\n";
  }
  return $item;
}

function getSvgFragment ($svg, $featureTinctures, $chargeGroup, $chargeName, $extras, $strokeCol = null, $strokeWidth = null, $verbatim = false)
{
    global $messages, $options;
    $retval = array();

    $viewBoxTransform = "";

    $viewbox = $svg['viewBox'] ?? '0 0 100 100';
    list($x0,$y0,$x1,$y1) = preg_split('/[ ,]/',$viewbox);
    // safety checks in case of dodgy SVG
    if (!is_numeric($x0)) $x0 = 0;
    if (!is_numeric($y0)) $y0 = 0;
    if (!is_numeric($x1)) $x1 = 100;
    if (!is_numeric($y1)) $y1 = 100;

    // do we need to adapt for offset view box (e.g. traceable heraldic art)
    if ( $x0 != 0 || $y0 != 0 )
    {
        $y1 -= $y0;
        $x1 -= $x0;
        $x0 *= -1;
        $y0 *= -1;
        $viewBoxTransform = "translate($x0,$y0)";
    }

    $heightVal = $svg['height'] ?? $x1;
    if (substr($heightVal,-1) == '%')
        $height = ($y1 - $y0) * intval($heightVal) / 100;
    else
        $height = floatval($heightVal);

    $widthVal = $svg['width'] ?? $y1;
    if (substr($widthVal,-1) == '%')
        $width = ($x1 - $x0) * intval($widthVal) / 100;
    else
        $width = floatval($widthVal);

    $bounding_box = ShieldRect::from_corners(0, 0, $width, $height);

    // set a default stroke colour
    $fragment = '';
    if (!$verbatim) { // we don't need this as we using the unmodified SVG instead
        foreach ($svg->children() as $child)
            $fragment .= addChild($child, $featureTinctures, $chargeGroup, $chargeName, $extras, $strokeCol, $strokeWidth, $bounding_box);
    }

    $x0 += $bounding_box->left();
    $y0 += $bounding_box->top();

    if ($viewBoxTransform != "") {
        $fragment = "<g transform=\"$viewBoxTransform\">$fragment</g>\n";
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
        $retval["hold_point"] = new ShieldPos($width/2, $height/2);
        $retval["hold_radius"] = $width/12;
    }

    $retval['height'] = $bounding_box->size->height;
    $retval['width'] = $bounding_box->size->width;
    $retval['bounding_box'] = $bounding_box;

    return $retval;
}
