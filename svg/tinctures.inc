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


function readTinctureFile($filename, $prefix) {
    $retval = array();
    foreach (file($filename) as $line) {
        // remove comments
        if (($comment = strpos($line, '//')) !== false) {
            $line = substr($line, 0, $comment);
        }
        $line = trim($line);
        // Now remove any internal spaces
        $line = str_replace(' ', '', $line);
        $line = str_replace("\t", '', $line);
        if ($line != '') {
            list($key,$value) = explode('=', $line);
            if (!is_null($key) && !is_null($value)) {
//                if ($value[0] != '#') {
//                    $value = '#' . $value;
//                }
                $retval["$prefix/$key"] = $value;
            }
        }
    }
    return $retval;
}

function load_tincures($palette, $web, $warhammer, $tartan, $custom)
{
    global $messages;

    // always read in the drawshield colours as these are the defaults for any not given
    $tinctures = readTinctureFile(__dir__ . '/schemes/drawshield.txt','heraldic');

    if (!file_exists(__dir__ . '/schemes/' . $palette . '.txt'))
        $messages->addMessage('internal', "unknown colour scheme - $palette" );
    else
        $tinctures = array_merge($tinctures, readTinctureFile(__dir__ . '/schemes/' . $palette . '.txt', 'heraldic'));

    if ( $web )
        $tinctures = array_merge($tinctures, readTinctureFile(__dir__ . '/schemes/web.txt','web'));

    if ( $warhammer )
        $tinctures = array_merge($tinctures, readTinctureFile(__dir__ . '/schemes/warhammer.txt', 'wh'));

    if ( $tartan )
        $tinctures = array_merge($tinctures, readTinctureFile(__dir__ . '/schemes/tartan.txt','tartan'));

    $tinctures = array_merge($tinctures, $custom);

    return $tinctures;
}

function rgb($keyterm) {
    global $options, $messages;

    static $loaded = false;
    static $errorColours = array ('#eeeeee','#dddddd','#cccccc', '#bbbbbb', '#aaaaaa',
                                  '#999999', '#888888', '#777777', '#666666', '#555555');
    static $tinctures = array ();

    if (!$loaded) {
        // always read in the drawshield colours as these are the defaults for any not given
        $tinctures = load_tincures(
            $options['palette'],
            $options['useWebColours'] == true || $options['shape'] == 'flag',
            $options['useWarhammerColours'] == true || $options['shape'] == 'pauldron',
            $options['useTartanColours'] == true,
            isset($options['customPalette']) && is_array($options['customPalette']) ? $options['customPalette'] : []
        );

        if ($options['effect'] == 'inked') { // override strokes
            $tinctures['charge-stroke']='#000000';
            $tinctures['ordinary-stroke']='#000000';
            $tinctures['pattern-stroke']='#000000';
            $tinctures['division-stroke']='#000000';
            $tinctures['stroke-width'] = 2;
        }
		$loaded = true;
    }

    if (!strpos($keyterm, '/')) {
        $keyterm = 'heraldic/' . $keyterm;
    }
    list($type,$name) = explode('/',$keyterm);

    if (array_key_exists($keyterm, $tinctures)) {
        // in case of a mismatch between the parser and the tincture file
        $retval = TinctureData::fix_url($tinctures[$keyterm]);
        if (empty($retval)) $retval = '#eeeeee';
        return $retval;
    } else {
        switch($type) {
          case 'heraldic': // should not happen
            $messages->addMessage('internal', "unknown colour - $name" );
          break;
          case 'web':
            $messages->addMessage('warning', "$name is not a heraldic colour, to use, enable web colours in preferences" );
          break;
          case 'tartan':
            $messages->addMessage('warning', "$name is not a heraldic colour, to use, enable tartan colours in preferences" );
          break;
		  case 'wh':
            $messages->addMessage('warning', "$name is not a heraldic colour, to use, enable warhammer colours in preferences" );
          break;
        }
    }
    if (count($errorColours)) 
      return array_pop($errorColours);
    else
      return '#ffffff';
}

function lightnessValue($hex) {
  $prefix = '';
  if ($hex[0] == '#') {
    $prefix = '#';
    $hex = substr($hex,1);
  }
  $red = intval('0x' . substr($hex,0,2),16);
  $green = intval('0x' . substr($hex,2,2),16);
  $blue = intval('0x' . substr($hex,4,2),16);
  $hsp = sqrt(0.299 * ($red * $red) + 0.587 * ($green * $green) + 0.114 * ($blue * $blue));
  return ($hsp);
}

// Algorithm for stroke colour
// ===========================
// The colour scheme file defines 4 values:
// charge-stroke : the default value if the item is multi-coloured
// stroke-light : the stroke colour to use on light coloured objects
// stroke-dark : the stroke colour to use on dark coloured objects
// light-dark-threshold : the point at which to switch from light to dark
//
// Operation
// ---------
// 1) if $fixedStroke is true, always return the default value
// 2) $tincture can be two things:
// a) A string, which should be a colour name
//    in which case we calculate whether it is light or dark and return appropriately
// b) A tincture node
//    in which case we do things depending on the type of tincture this is
//    For now, we only deal with "proper", "semyDe" and "colour", we extract the hex value
//    and use that to return light or dark appropriately
//
//    In future we should probably do something with furs as well since these usually have a dominant colour
//
function getStroke($tincture, $fixedStroke = false) {
    $retval = rgb('charge-stroke'); // sensible default
    if ($fixedStroke) { // always use mid colour
        return($retval);
    }
    if (is_string($tincture)) { // should be a colour name
        $hex = rgb($tincture);
        if (lightnessValue($hex) < rgb('light-dark-threshold')) {
            // light
            $retval = rgb('stroke-dark');
        } else {
            // dark
            $retval = rgb('stroke-light'); 
        }
        return($retval);
    }
    // bad call protection
    if (!is_object($tincture) || !$tincture->hasChildNodes()) {
        return ($retval);
    }
    // otherwise, depends on the type of tincture
    switch ($tincture->firstChild->nodeName) {
    case 'proper': // should already have been resolved by now
        if ($tincture->firstChild->hasAttribute('spec')) {
            $bodyCol = $tincture->firstChild->getAttribute('spec');
            if (lightnessValue($bodyCol) < rgb('light-dark-threshold')) {
                // light
                $retval = rgb('stroke-dark');
            } else {
                // dark
                $retval = rgb('stroke-light'); 
            }
        }
        break;
    case 'treatment': // has two colours so probably mid-gray is best?
    case 'counterChanged': // ditto
    case 'division': // ditto
        break;
    case 'fur': // probably should improve this one and choose a stroke 
        // colour depending on the main colour of the fur
        break; // just go with default mid-gray for now
    case 'colour':
        $bodyCol = get_rgb($tincture);
        if (lightnessValue($bodyCol) < rgb('light-dark-threshold')) {
            // light
            $retval = rgb('stroke-dark');
        } else {
            // dark
            $retval = rgb('stroke-light'); // use default colour
        }
        break;
      case 'semyDe': // use the main background colour
          $retval = getStroke($tincture->firstChild->firstChild);
          break;
      default: // shouldn't happen
        break;
    }
    return($retval);
}

function shiftRGB($hex, $direction="lighter") {
  $factor = 0.33;
  $prefix = '';
  if ($hex[0] == '#') {
    $prefix = '#';
    $hex = substr($hex,1);
  }
  $red = intval('0x' . substr($hex,0,2),16);
  $green = intval('0x' . substr($hex,2,2),16);
  $blue = intval('0x' . substr($hex,4,2),16);
  if ($direction == "lighter") {
    $newRed = (int)($red * (1 - $factor));
    $newGreen = (int)($green * (1 - $factor));
    $newBlue = (int)($blue * (1 - $factor));
  } else {
    $newRed = (int)($red + (255 - $red) * $factor);
    $newGreen = (int)($green + (255 - $green) * $factor);
    $newBlue = (int)($blue + (255 - $blue) * $factor);
  }
  $retval =  sprintf("%s%02x%02x%02x",$prefix,$newRed,$newGreen,$newBlue);
  return $retval;
}

function mixRGB($hex1, $hex2) {
    if ($hex1[0] == '#') {
        $hex1 = substr($hex1,1);
    }
    if ($hex2[0] == '#') {
        $hex2 = substr($hex2,1);
    }
    $red1 = intval('0x' . substr($hex1,0,2),16);
    $green1 = intval('0x' . substr($hex1,2,2),16);
    $blue1 = intval('0x' . substr($hex1,4,2),16);
    $red2 = intval('0x' . substr($hex2,0,2),16);
    $green2 = intval('0x' . substr($hex2,2,2),16);
    $blue2 = intval('0x' . substr($hex2,4,2),16);

    $red3 = intval(($red1 + $red2) / 2);
    $green3 = intval(($green1 + $green2) / 2);
    $blue3 = intval(($blue1 + $blue2) / 2);

    $retval =  sprintf("%s%02x%02x%02x",'#',$red3,$green3,$blue3);

    return $retval;
}

class TinctureData
{
    function __construct($tinctureNode, $rev = false, $inv = false)
    {
        $this->node = resolveReference($tinctureNode);
        if ( !is_object($this->node) || !$this->node->hasChildNodes() )
            $this->type_node = createColourNode('gray','1');
        else
            $this->type_node = $this->node->firstChild;
        $this->type_name = $this->type_node->nodeName;
        $this->rev = $rev;
        $this->inv = $inv;

        $this->pattern_transform = '';
        $this->fill = null;
        $this->defs = [];
        $this->mask = null;
    }

    private function add_def($element, $content)
    {
        list($id, $def) = make_def($element, $content);
        $this->defs[$id] = $def;
        return $id;
    }

    static function fix_url($fill)
    {
        if ( $fill && ($fill[0] == '"' || $fill[0] == "'"))
            return "url('/" . substr($fill, 1, -1) . "')";
        return $fill;
    }

    static function id_url($id)
    {
        return "url(#$id)";
    }

    private function apply_inv(&$data)
    {
        if ( $this->inv )
            $data['body'] = "<g transform=\"translate(0,{$data['height']}) scale(1,-1)\">{$data['body']}</g>";
    }

    private function apply_rev(&$data)
    {
        if ( $this->rev )
            $data['body'] = "<g transform=\"translate({$data['width']},0) scale(-1,1)\">{$data['body']}</g>";
    }

    function parse_fill($layout)
    {
        if ( $this->pattern_transform )
            $patTrans = ' patternTransform="' . $this->pattern_transform . '" ';
        else
            $patTrans = '';

        switch ( $this->type_name )
        {
            case 'colour':
            case 'default': // don't think these two ever occur
            case 'proper':
                if ( $this->type_node->hasAttribute('spec') )
                    $this->fill = $this->type_node->getAttribute('spec');
                else
                    $this->fill = rgb($this->type_node->getAttribute('keyterm'));

                if ( $this->fill )
                {
                    // set if we are to use the fill found
                    if ( substr($this->fill, 0, 4) == 'url(')
                        $this->mask = substr($this->fill, 6, -2);
                }
                break;
            case 'fur':
                $fur_name = $this->type_node->getAttribute('keyterm');
                $fur_data = makeFur( $fur_name );
                // If what we are placing is inverted, we also invert the fill so it appears the "right way up"
                // Don't need to do reversed as all furs are symmetrical
                $this->apply_inv($fur_data);
                $patt_id = $this->add_def(
                    'pattern ' . $patTrans .
                    ' patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="0" y="0" width="' .
                    $fur_data['width'] . '" height="' . $fur_data['height'] . '"',
                    '<g class="fur">' . $fur_data['body'] . '</g>'
                );
                $this->fill = static::id_url($patt_id);
                break;
            case 'treatment':
                // A bit messy, but we handle gradients as a fill, not a pattern fill (could make this its own node type really?)
                if (substr($this->type_node->getAttribute("keyterm"), 0, 5) == 'grady')
                {
                    $this->fill = static::id_url(makeGrady($this->type_node));
                }
                else
                {
                    $treat_data = makeTreatment( $this->type_node, $layout );
                    // If what we are placing is inverted, we also invert the fill so it appears the "right way up"
                    // Don't need to do reversed as all treatments are symmetrical
                    $this->apply_inv($treat_data);
                    //do we need to offset the pattern to center it?
                    $offset = 0;
                    if (array_key_exists('offset', $treat_data))
                        $offset = $treat_data['offset'];

                    $this->fill = static::id_url($this->add_def(
                        'pattern ' . $patTrans .
                        ' patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="' . $offset . '" y="0" width="' .
                        $treat_data['width'] . '" height="' . $treat_data['height'] . '"',
                        '<g class="treatment">' . $treat_data['body'] . '</g>'
                    ));
                }
                break;
            case 'semyde':
                $treat_data = makeSemy($this->type_node, $layout);
                if ($treat_data == null) break;
                // If what we are placing is inverted, we also invert the fill so it appears the "right way up"
                $this->apply_inv($treat_data);
                $this->apply_rev($treat_data);
                $this->fill = static::id_url($this->add_def(
                    'pattern ' . $patTrans .
                    ' patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="0" y="0" width="' .
                    $treat_data['width'] . '" height="' . $treat_data['height'] . '"',
                    $treat_data['body']
                ));
                break;
        }
    }

    function defs()
    {
        if ( count($this->defs) == 0 )
            return "";
        return "<defs>" . implode("", $this->defs) . "</defs>";
    }

}


function apply_tincture ($tinctureNode, $targetSVG, ShieldLayout $layout, ShieldSize $chg_size = null,
        $rot = '0', $rev = false, $inv = false ) {
    global $_SERVER, $messages, $xpath;
    $retval = '';

    if ( is_null($tinctureNode) )
        return $targetSVG; // Maybe an error? Sometimes get here from makeOrdinary()

    if ( !is_object($tinctureNode) || is_null($tinctureNode->parentNode) ) {
        // error_log("bad tincture in " . $options['blazon']);
        return $targetSVG;
    }

    $targetNode = $tinctureNode->parentNode;
    $targetType = $targetNode->nodeName;

    $data = new TinctureData($tinctureNode, $rev, $inv);
    $targetKeyterm = "";
    if ( $targetNode !== null) {
        $targetKeyterm = $targetNode->getAttribute('keyterm');
    } else {
        return $targetSVG;
    }

    if ( $targetType == 'charge' && $chg_size != null && $data->type_name != blazonML::E_COUNTERCHANGED )
    {
        // We make some things a wee bit smaller if on a charge, just for looks
        $chgX = $chg_size->width;
        $chgY = $chg_size->height;
        $scale = max ( $chgX / 1000, $chgY / $layout->size->height ); // multiply here to adjust relative size of fur etc. on a charge
        $data->pattern_transform = ' scale(' . $scale . ') ';
    } elseif ($layout->shape == 'flag') { // adjust size based on flag AR?
        $data->pattern_transform = ' scale(0.5) ';
    }

    // If the object is rotated, we rotate some of the tinctures
    // There are 3 options for the value of the $rot parameter:
    // 1) 0 - no action
    // 2) A number (e.g. 45) - causes a fixed rotation by that number of degrees
    // 3) A string - this is assumed to be a valid SVG transform which is applied to the pattern
    if ( $rot != '0' )
        if (is_int($rot)) {
            $data->pattern_transform .= ' rotate(' . ($rot * -1) . ') ';
        } else {
            $data->pattern_transform .= " $rot ";
        }

    // Nothing to do
    if ( $data->node != null && $data->node->getAttribute('origin') == 'implied' )
        return $targetSVG;

    // stage "raw" means leave charge colours as they are
    // if ( $targetType == 'charge' && array_key_exists('raw', $options)) return $targetSVG;
    if ( $data->node === null || !$data->node->hasChildNodes()
        || $data->node->getAttribute('origin') == 'pending'
        || $data->node->getAttribute('origin') == 'missing') {
        $messages->addMessage ('blazon','Missing colour, Gray substituted.' );
        $data = new TinctureData(createColourNode('gray','1'),$rev, $inv);
    }

    switch ( $data->type_name ) {
        case 'colour':
        case 'default': // don't think these two ever occur
        case 'proper':
        case 'fur':
        case 'treatment':
        case 'semyde':
            $data->parse_fill($layout);

            if ( $data->mask )
            {
                $patt_id = add_def('mask', '<g style="fill:#ffffff;">' . $targetSVG . '</g>');
                $retval .= '<g mask="url(#' . $patt_id . ')"  ><use xlink:href="' . $data->mask . '" /></g>';
                // add the outline and any shading
                if ( $targetType == 'charge' )
                    $retval .= '<g style="fill:none;">' . $targetSVG . '</g>';
            }
            else if ( $data->fill )
            {
                $stroke = '';

                $retval .= $data->defs();

                switch ( $data->type_name )
                {
                    case 'fur':
                        $stroke = ($targetType == 'ordinary') ? 'stroke="' . rgb('sable') . '" stroke-width="5"' : '';
                        break;
                }

                $retval .= "<g $stroke fill=\"{$data->fill}\">$targetSVG</g>";
            }
            else
            {
                $retval .= $targetSVG;
            }
            break;
        case 'division':
            $title = '<title>' . $data->type_node->getAttribute('keyterm') . '</title>';
            // What we do depends on what we are filling...
            switch ($targetType) {
                case 'division':
                    $new_div = makeDivision( $data->type_node, $layout );
                    $mask_id = add_def ( 'mask width="1200" height="' . ($layout->size->height + 200) . '" maskUnits="userSpaceOnUse"',
                    $title . "<desc>Division used to fill $targetKeyterm</desc>" .
                    '<g fill="white">' . $targetSVG . '</g>' );
                    $retval = add_def();
                    $retval .= '<g mask="url(#' . $mask_id . ')"  >' . $new_div . '</g>';
                    break;
                case 'field': // just the field of the shield
                    $retval = '<g>' . $title . '<desc>Division used to fill field</desc>' . makeDivision( $data->type_node, $layout ) . '</g>';
                    break;
                case 'ordinary':
                    if ( strpos($targetKeyterm, 'cross') !== false || strpos($targetKeyterm, 'bend') !== false || strpos($targetKeyterm, 'saltire') !== false ) {
                        $new_div = makeDivision( $data->type_node, $layout );
                        $mask_id = add_def ( 'mask width="1200" height="' . ($layout->size->height + 200) . '" maskUnits="userSpaceOnUse"',
                            $title . "<desc>Division used to fill $targetKeyterm</desc>" .
                            '<g fill="white">' . $targetSVG . '</g>' );
                        $retval = add_def();
                        $retval .= '<g mask="url(#' . $mask_id . ')"  >' . $new_div . '</g>';
                        break;
                    } // else flow through
                case 'charge':
                    $new_div = makeDivision( $data->type_node, ShieldLayout::square() );
                    $height = get_div_height($data->type_node);
                    $patt_id = add_def (
                        'pattern width="100%" height="100%" patternUnits="objectBoundingBox" preserveAspectRatio="none" viewBox="0 0 1000 1000"',
                        $title . "<desc>Division used to fill $targetType</desc>" .
                        $new_div
                    );
                    $retval = add_def();
                    $retval .= '<g fill="url(#' . $patt_id . ')"  >' . $targetSVG . '</g>';
                    break;
            }
            break;
        case 'shield':
            $shield = makeshield( $data->type_node, new ShieldLayout(new ShieldSize(1000, 1000), $layout->shape) );
            $patt_id = add_def ( 'pattern width="100%" height="100%" patternUnits="objectBoundingBox" preserveAspectRatio="none" viewBox="0 0 1000 1000"',
                "<desc>Shield used to fill $targetType</desc>" .
                $shield );
            $retval = add_def();
            $retval .= '<g fill="url(#' . $patt_id . ')"  >' . $targetSVG . '</g>';
            break;
        case 'counterchanged': // Never change anything, just fill with this pattern
            // Try to find a field division
            $division = $xpath->query('preceding::field/tincture/division', $data->node);
            if ( $division->length ) {
                $counterDiv = $division->item(($division->length)-1);
                $div_data = makeDivision( $counterDiv, $layout, true );
                $patt_id = add_def ( 'mask', '<g style="fill:#ffffff;">' . $targetSVG . '</g>' );
                $retval = add_def() . '<g mask="url(#' . $patt_id . ')"  >' . $div_data . '</g>';
            } else {
                $treatment = $xpath->query('preceding::field/tincture/treatment',$data->node);
                if ( $treatment->length ) {
                    $counterTreatment = $treatment->item(($treatment->length) - 1);
                    $treat_data = makeTreatment($counterTreatment, $layout, true);
                    if ( $inv ) $treat_data['body'] = "<g transform=\"translate(0,{$treat_data['height']}) scale(1,-1)\">{$treat_data['body']}</g>";
                    //do we need to offset the pattern to center it?
                    $offset = 0;
                    if (array_key_exists('offset', $treat_data)) {
                        $offset = $treat_data['offset'];
                    }
                    $treatPatt_id = add_def ( 'pattern ' . $data->pattern_transform .
                        ' patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="' . $offset . '" y="0" width="' .
                        $treat_data['width'] . '" height="' . $treat_data['height'] . '"',
                        '<g class="treatment">' . $treat_data['body'] . '</g>' );
                    $fill = '<g fill="url(#' . $treatPatt_id . ')"  ><path d="M0,0 h1000 v' . $layout->size->height . ' h-1000 z" /></g>';
                    $patt_id = add_def('mask', '<g style="fill:#ffffff;">' . $targetSVG . '</g>');
                    $retval = add_def() . '<g mask="url(#' . $patt_id . ')"  >' . $fill . '</g>';
                } else {
                    $fur = $xpath->query('preceding::field/tincture/fur',$data->node);
                    if ($fur->length) {
                        $counterFur = $fur->item(($fur->length) - 1);
                        $fur_data = makeFur($counterFur->getAttribute(blazonML::A_KEYTERM), true);
                        $furPatt_id = add_def ( 'pattern ' . $data->pattern_transform .
                            ' patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="0" y="0" width="' .
                            $fur_data['width'] . '" height="' . $fur_data['height'] . '"',
                            '<g class="fur">' . $fur_data['body'] . '</g>' );
                        $fill = '<g fill="url(#' . $furPatt_id . ')"  ><path d="M0,0 h1000 v' . $layout->size->height. ' h-1000 z" /></g>';
                        $patt_id = add_def('mask', '<g style="fill:#ffffff;">' . $targetSVG . '</g>');
                        $retval = add_def() . '<g mask="url(#' . $patt_id . ')"  >' . $fill . '</g>';
                        } else {
                            $messages->addMessage('error', 'Counterchange requires two colour field');
                            return '<g fill="#050505"><title>Counterchange</title><desc>Not possible</desc>' . $targetSVG . '</g>';
                        }

                }
            }
            break;
        // END of switch
    }
    return $retval;
}
