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

// This function places a list of charges according to the placement string,
// unless modified by something in the mods array. Items in the mods array
// take precedence over the settings of the placement string.
// The placement string is organised as follows:
// 1) Each number of charges has its own substring, separated by '/'
//    * If there is an empty substring the next highest (non-empty)
//      substring will be used and the higher places ignored.
//    * If there is only one substring it will be used for all
//    * If there is no sensible arrangement for this number of charges
//      the substring should be 'x'
// 2) Within the substring the placement of each charge is given by
//    a sub-substring, these are separated by ':'
// 3) The sub-substring may consist of up to 7 numbers, separated by
//    ','. The numbers are as follows:
//    a) The X position of the centre of the charge
//    b) The Y position of the centre of the charge
//    c) The X size of the charge bounding box
//    d) The Y size of the charge bounding box
//    e) The rotation (in degrees) of the charge
//    f) 1 if the charge is inverted, 0 otherwise
//    g) 1 if the charge is reversed, 0 otherwise
// If any of the numbers are ommitted they are assumed to be the
// same as the preceeding entries. If there are no preceeding
// entries they will use default values equivalent to:
//  /500,500,200,200,0,0,0/
// It should be assumed that the charge list will be provided in order,
// left to right then top to bottom

function place_charges ( DOMElement $node, $placement, $layout ) {
  global $messages;
 
  switch ($placement) {
    case '':
    case 'i':
    $messages->addMessage('internal', 'No placement for charge' );
      return '';
    case 'n':
      $messages->addMessage('warning','Not meaningful placement' );
      return '';
  }
//  error_log("Using placement: " . $placement);
  $placement = subPercent($placement, $layout);
//  error_log("Converted to: " . $placement);
  $num_of = $node->getAttribute('number');
  if ( $num_of < 1 ) $num_of = 1; // Just in case...
  $between = null;
  $retval = '';
  /**
   * @var DOMElement $child
   */
  if ( $child = getModifierNodeWithKeyterm($node,'between')) {
    $between = $child->firstChild;
    $between_placement = get_placement($between, $layout);
    // If between has its own placement, use that
    if ($between_placement != 'i' ) {
      $retval .= place_charges( $between, $between_placement, $layout);
      $between = null;
    } else {
      $num_of += (integer)$between->getAttribute('number');
      $half = ($between->getAttribute('number')) / 2;
    }
  } elseif ( $child = getModifierNodeWithKeyterm($node,'above')) {
    $retval .= place_charges( $child->firstChild, get_placement($child->firstChild, $layout), $layout);
  } elseif ( $child = getModifierNodeWithKeyterm($node,'beside')) {
    $retval .= place_charges( $child->firstChild, get_placement($child->firstChild, $layout), $layout);
  }
  $splitArray = preg_split(':/:', $node->getAttribute('keyterm'));
  list($group,$chargeKey) = $splitArray;

  $cx = 500; // default values
  $cy = 500;
  $rot = 0;
  $inv = 0;
  $rev = 0;
  $bbx = 200;
  $bby = 200;
      
    $num_of_list = explode('/', $placement);
    // Choose the placement substring to use
    if ( $num_of > count( $num_of_list ) ) {
      $place_list = 's';
    } else {
      $place_list = $num_of_list[$num_of - 1];
      if ( $place_list == '' ) { // use next highest
        for ( $i = $num_of; $i < count($num_of_list); $i++ )
          if ( $num_of_list[$i] != '' ) $place_list = $num_of_list[$i];
      }
    }
    switch($place_list[0]) {
      case '':
        $messages->addMessage('internal', 'Bad placement');
        return '';
      case 's':
        $messages->addMessage( 'warning', 'Not enough space for charges' );
        return '';
      case 't':
        $messages->addMessage( 'internal', 'Not done charge placement yet (sorry)' );
        return '';
      case 'x':
        $messages->addMessage( 'warning', $node->getAttribute('keyterm') . ' - Wrong number for placement');
        return '';
      case 'c':
        $place_list = ltrim(calcPlace($node,'w','100,100,800,800'),'x/');
        break;
    }
    $place_list = explode(':', $place_list);
    if ( count($place_list) < $num_of ) {
      $messages->addMessage('internal', 'Bad placement list');
      return '';
    }
    
    for ( $count = 0; $count < $num_of; $count++ ) {
      if ( $place_list[$count] == '' ) {
        $messages->addMessage('internal', 'Bad place item' );
        continue;
      }
      $items = explode(',', $place_list[$count]);
      if ( isset($items[0]) && $items[0] != '' && is_numeric($items[0])) {
        $cx = $items[0];
    //    if ( $sinister )$cx = 1000 - $cx;
      }
      if ( isset($items[1]) && $items[1] != '' && is_numeric($items[1]) ) {
        $cy = $items[1];
      //  if ( $inverted) $cy = 1200 - $cy;
    //   if ( $enhanced) $cy -= 200;
    //   if ( $abased) $cy += 200;
      }
      if ( isset($items[2]) && $items[2] != '' ) $bbx = $items[2];
      if ( isset($items[3]) && $items[3] != '' ) $bby = $items[3];
      if ( isset($items[4]) && $items[4] != '' ) {
        $rot = $items[4];
    //    if ( $sinister ) { if ( $rot != 0 ) { $rot += 90; } }
    //    if ( $inverted ) { if ( $rot != 0 ) { $rot -= 90; } }
      }
      if ( isset($items[5]) && $items[5] != '' ) $inv = $items[5];
      if ( isset($items[6]) && $items[6] != '' ) $rev = $items[6];
      $item = $node;
      if ( $between && ( ($count+1) <= $half or ($count+1) > ($num_of - $half))) 
        $item = $between;

      if ($num_of == 1 && $group == 'label') {
        $cx = 500;
        $cy = 150;
        $bbx = 1000;
        $bby = 200;
      }
      $retval .= PlaceCharge( $item, $layout, $bbx, $bby, $cx, $cy, $count + 1,$rot, $inv, $rev );
    }
  return $retval;
}

class PlacementHints
{
    public $offset_x = 0;
    public $offset_y = 0;
    public $angle = 0;
    public $scale_x = 1;
    public $scale_y = 1;

    function adjust($keyterm)
    {
        switch ( $keyterm )
        {
            // the "shown" mods
            case 'rotated-dexter':
                $this->angle = -18;
                break;
            case 'rotated-sinister':
                $this->angle = 18;
                break;
            case 'slightly-rotated-dexter':
                $this->angle = -9;
                break;
            case 'slightly-rotated-sinister':
                $this->angle = 9;
                break;
            case 'much-rotated-dexter':
                $this->angle = -27;
                break;
            case 'much-rotated-sinister':
                $this->angle = 27;
                break;
            case 'very-much-rotated-dexter':
                $this->angle = -36;
                break;
            case 'very-much-rotated-sinister':
                $this->angle = 36;
                break;
            case 'rotated-quarter-dexter':
                $this->angle = -22.5;
                break;
            case 'rotated-quarter-sinister':
                $this->angle = 22.5;
                break;
            case 'rotated-third-dexter':
                $this->angle = -30;
                break;
            case 'rotated-third-sinister':
                $this->angle = 30;
                break;
            case 'rotated-sixth-dexter':
                $this->angle = -15;
                break;
            case 'rotated-sixth-sinister':
                $this->angle = 15;
                break;
            case 'rotated-eighth-dexter':
                $this->angle = -11.25;
                break;
            case 'rotated-eighth-sinister':
                $this->angle = 11.25;
                break;

            case 'smaller':
                $this->scale_x *= 0.8;
                $this->scale_y *= 0.8;
                break;
            case 'larger':
                $this->scale_x *= 1.2;
                $this->scale_y *= 1.2;
                break;
            case 'raised':
                $this->offset_y -= 60;
                break;
            case 'lowered':
                $this->offset_y += 60;
                break;
            case 'dexter':
                $this->offset_x -= 50;
                break;
            case 'sinister':
                $this->offset_x += 50;
                break;
            case 'slightly-smaller':
                $this->scale_x *= 0.9;
                $this->scale_y *= 0.9;
                break;
            case 'slightly-larger':
                $this->scale_x *= 1.1;
                $this->scale_y *= 1.1;
                break;
            case 'slightly-raised':
                $this->offset_y -= 30;
                break;
            case 'slightly-lowered':
                $this->offset_y += 30;
                break;
            case 'slightly-dexter':
                $this->offset_x -= 25;
                break;
            case 'slightly-sinister':
                $this->offset_x += 25;
                break;
            case 'much-smaller':
                $this->scale_x *= 0.6;
                $this->scale_y *= 0.6;
                break;
            case 'much-larger':
                $this->scale_x *= 1.4;
                $this->scale_y *= 1.4;
                break;
            case 'much-raised':
                $this->offset_y -= 120;
                break;
            case 'much-lowered':
                $this->offset_y += 120;
                break;
            case 'much-dexter':
                $this->offset_x -= 100;
                break;
            case 'much-sinister':
                $this->offset_x += 100;
                break;
            case 'very-much-smaller':
                $this->scale_x *= 0.4;
                $this->scale_y *= 0.4;
                break;
            case 'very-much-larger':
                $this->scale_x *= 1.6;
                $this->scale_y *= 1.6;
                break;
            case 'very-much-raised':
                $this->offset_y -= 240;
                break;
            case 'very-much-lowered':
                $this->offset_y += 240;
                break;
            case 'very-much-dexter':
                $this->offset_x -= 200;
                break;
            case 'very-much-sinister':
                $this->offset_x += 200;
                break;
            case 'narrower':
                $this->scale_x *= 0.8;
                break;
            case 'shorter':
                $this->scale_y *= 0.8;
                break;
            case 'taller':
                $this->scale_y *= 1.2;
                break;
            case 'wider':
                $this->scale_x *= 1.2;
                break;
            case 'much-narrower':
                $this->scale_x *= 0.8;
                break;
            case 'much-shorter':
                $this->scale_y *= 0.8;
                break;
            case 'much-taller':
                $this->scale_y *= 1.4;
                break;
            case 'much-wider':
                $this->scale_x *= 1.4;
                break;
            case 'slightly-narrower':
                $this->scale_x *= 0.9;
                break;
            case 'slightly-shorter':
                $this->scale_y *= 0.9;
                break;
            case 'slightly-taller':
                $this->scale_y *= 1.1;
                break;
            case 'slightly-wider':
                $this->scale_x *= 1.1;
                break;
            case 'very-much-narrower':
                $this->scale_x *= 0.6;
                break;
            case 'very-much-shorter':
                $this->scale_y *= 0.6;
                break;
            case 'very-much-taller':
                $this->scale_y *= 1.6;
                break;
            case 'very-much-wider':
                $this->scale_x *= 1.6;
                break;
        }
    }
}


function PlaceCharge ( $node,  // images
                            $layout,
                            $boundX, $boundY,  // size of bounding box
                            $centreX, $centreY,// centre of bounding box
                            $index = 1,
                            $rotation = 0,
                            $inverted = false,
                            $reversed = false ) {

  $charge = getCharge( $node, $layout );
  if ($charge == null) return '';
  $orientation = null;
  $tinctures = [];
  $Xflex = array_key_exists('wflex', $charge) ? $charge['wflex'] : 1;
  $Yflex = array_key_exists('hflex', $charge) ? $charge['hflex'] : 1;
  $inArrangement = false;
  $shownRotation = 0;
  $situation = $charge['situation'] ?? 'palewise';

    $hints = new PlacementHints();
  foreach ( $node->childNodes as $child ) {
    if ( $child->nodeName == 'modifier' ) {
      if ($child->getAttribute('type') == 'arrangement') {
        $inArrangement = true;
      }
      $keyterm = $child->getAttribute('keyterm');
      switch ( $keyterm ) {
        case 'inverted':
            $inverted = !$inverted;
            break;
        case 'reversed':
            $reversed = !$reversed;
            break;
        case 'bendwise':
        case 'bendsinwise':
        case 'fesswise':
        case 'palewise':
        case 'haurient':
            $orientation = $keyterm;
            break;
        case 'entire':
        case 'conjoined':
            $Xflex = $Yflex = 9999;
            break;
        // the "shown" mods
        default:
            $hints->adjust($keyterm);
            break;
      }
    } elseif ( $child->nodeName == 'tincture' ) {
      $tinctures[intval($child->getAttribute(blazonML::A_INDEX))] = $child;
    }
  }
  // Now try to map the index we are given to the list of tinctures provided
    if (isset($tinctures[$index])) { // exact match
        $tincture = $tinctures[$index];
    } else {
       $indexes = array_keys($tinctures);
       $tincture = $tinctures[$indexes[($index - 1) % count($indexes)]];
    }

  // sometimes we can ignore the orientation as we are already "that way up"
  if ($charge['group'] != 'quadrate' && $orientation == $situation) $orientation = null;

    $shownRotation = $hints->angle;

  $BoundAR = 1;
  if ($boundY != 0) {
    $BoundAR = $boundX / $boundY;
  }
  $retval = $charge['body'];
  $chargeX = max(1,$charge['width']); // protect against zero
  $chargeY = max(1,$charge['height']);

  // when a charge is turned (fesswise or palewise) then inverted and reversed swap meanings
  if ($inverted && !$reversed && ($orientation == 'fesswise' || $orientation == 'palewise')) {
    $inverted = false;
    $reversed = true;
  } elseif ($reversed && !$inverted && ($orientation == 'fesswise' || $orientation == 'palewise')) {
    $reversed = false;
    $inverted = true;
  }
  if ( $inverted ) {
    $retval = "<g transform=\"translate(0,$chargeY) scale(1,-1)\"><desc>inverted charge</desc>$retval</g>\n";
  }
  if ( $reversed ) {
    $retval = "<g transform=\"translate($chargeX,0) scale(-1,1)\"><desc>reversed charge</desc>$retval</g>\n";
  }
  if ( $orientation ) {
    $rotation = 0; // An explicit charge orientation overrides a rotation from the arrangement
    switch ( $orientation ) {
      case 'fesswise':
          $retval = "<g transform=\"translate(0,$chargeX) rotate(-90)\"><desc>fesswise charge</desc>$retval</g>\n";
          $temp = $chargeX; $chargeX = $chargeY; $chargeY = $temp;
          $temp = $Xflex; $Xflex = $Yflex; $Yflex = $temp;
        break;
      case 'bendwise':
      case 'bendsinwise':
        if ( $situation == 'fesswise' ) {
          $angle = ($orientation == 'bendwise') ? "45" : "135";
        } else {
          $angle = ($orientation == 'bendwise') ? "-45" : "45";
        }
			  $X2 = $chargeX / 2; $Y2 = $chargeY / 2;
				$chargeX = ($chargeX + $chargeY) * 0.707;
				$XY2 = $chargeX / 2;
        $chargeY = $chargeX;
        $retval = "<g transform=\"translate($XY2,$XY2) rotate($angle) translate(-$X2,-$Y2)\"><desc>bendwise</desc>$retval</g>\n";
        break;
      case 'palewise':
          $retval = "<g transform=\"translate($chargeY,0) rotate(90)\"><desc>palewise charge</desc>$retval</g>\n";
          $temp = $chargeX; $chargeX = $chargeY; $chargeY = $temp;
          $temp = $Xflex; $Xflex = $Yflex; $Yflex = $temp;
        break;
      case 'haurient':
        $retval = "<g transform=\"translate($chargeY,0) rotate(90)\"><desc>erect charge</desc>$retval</g>\n";
        $temp = $chargeX; $chargeX = $chargeY; $chargeY = $temp;
        $temp = $Xflex; $Xflex = $Yflex; $Yflex = $temp;
        break;
    }
  }
  if ($rotation != 0) {
    if ( array_key_exists('horizontal', $charge)) {
      // Swap over bounding box sizes
      $temp = $boundX;
      $boundX = $boundY;
      $boundY = $temp;
    }
  }

  // If rotation is set this is because we need to turn to fit onto an ordinary
  // If we CANNOT turn then assume that the bounding box is square. This makes sure we fit even if not turned
  if ( $node->parentNode->getAttribute('keyterm') == 'on' /* || $inArrangement */) {
    if ( $rotation != 0 ) {
      if ( ! array_key_exists('orient_to_ordinary', $charge) ) {
        $boundX = min( $boundX, $boundY );
        $boundY = $boundX;
        $rotation = 0 ;
      }
      // If we CAN turn then set the appropriate direction (depends on charge)
      else  { // Set to 1 for normal, -1 if needs to turn other way
        if ( $charge['orient_to_ordinary'] == -1) {
          $rotation *= -1;
        }
      }
    }
  }

  // Calculate the size of the image to fit into the bounding box,
  $Xscale = $boundX / $chargeX;
  $Yscale = $boundY / $chargeY;
  if ( $Xflex <= 1 && $Yflex <=1 ) { // no flexing, fixed AR
    $Xscale = $Yscale = min ( $Xscale, $Yscale );
  } else {
    $chargeAR = $chargeX / $chargeY;
    $MaxAR = $chargeAR * $Xflex;
    $MinAR = $chargeAR / $Yflex;
    if ( $BoundAR > $MaxAR ) { // BB is wider than we can be
      $limit = $MaxAR;
    } elseif ( $BoundAR < $MinAR ) { // BB is higher than we can be
      $limit = $MinAR;
    } else {
    $limit = 0;  // We can flex as required to fill whole BB
    // $Xscale and $Yscale are already set to the correct values
    }
    if ( $limit > 0 ) {
      if ( $Xscale < $Yscale ) {
        $Yscale = ($chargeX*$Xscale)/($limit*$chargeY);
      } else {
        $Xscale = ($limit * $chargeY * $Yscale) / $chargeX;
      }
    }
  }
    $Xscale *= $hints->scale_x;
    $Yscale *= $hints->scale_y;
    $centreY += $hints->offset_y;
    $centreX += $hints->offset_x;
  $sizeX = $chargeX * $Xscale;
  $sizeY = $chargeY * $Yscale;
  $posX = $centreX - ($sizeX / 2);
  $posY = $centreY - ($sizeY / 2);

  $transform = '';
  if ( $rotation != 0 ) {
    $transform .= 'rotate(' . $rotation . ',' . $centreX . ',' . $centreY . ') ';
  }
  $overrideRotation = '';
  if ($shownRotation != 0) {
      $rotateX = $chargeX / 2;
      $rotateY = $chargeY / 2;
      $overrideRotation = "rotate($shownRotation,$rotateX,$rotateY)";
  }
  $transform .= " translate($posX,$posY) scale($Xscale, $Yscale) $overrideRotation";
  if (!isset($charge['title'])) $charge['title'] = 'a charge';
  // $retval = "<g transform=\"$transform\" >\n<title>" . htmlentities(transliterate($charge['title'])) . '</title>' . $retval . "</g>\n";
  //$retval = "<g transform=\"$transform\" >\n<title>" . htmlentities($charge['title'], ENT_HTML5) . '</title>' . $retval . "</g>\n";
  $retval = "<g class=\"PlaceCharge\" transform=\"$transform\" >\n<title>" . $charge['title'] . '</title>' . $retval . "</g>\n";

  $retval = apply_tincture( $tincture, $retval, $layout, new ShieldSize($chargeX, $chargeY), $rotation, $reversed, $inverted );
  return $retval;
}

function quickPlace($svg, $chargeX, $chargeY, $boundX, $boundY, $centreX, $centreY, $fill = null, $adjustments = [] ) {
    $rotate = 0;
    // Apply the adjustments first
    if ($adjustments != null && count($adjustments)) {
        $hints = new PlacementHints();
        foreach ($adjustments as $adjustment) {
            $hints->adjust($adjustment);
        }
        $boundX *= $hints->scale_x;
        $boundY *= $hints->scale_y;
        $rotate += $hints->angle;
        $centreY += $hints->offset_y;
        $centreX += $hints->offset_x;
    }
  // Calculate the size of the image to fit into the bounding box,
  $Xscale = $boundX / $chargeX;
  $Yscale = $boundY / $chargeY;
  $Xscale = $Yscale = min ( $Xscale, $Yscale );
  $sizeX = $chargeX * $Xscale;
  $sizeY = $chargeY * $Yscale;
  $posX = $centreX - ($sizeX / 2);
  $posY = $centreY - ($sizeY / 2);

  $rotation = '';
  if ($rotate) {
      $rotateX = $chargeX / 2;
      $rotateY = $chargeY / 2;
      $rotation = "rotate($rotate,$rotateX,$rotateY)";
  }
    $attribs = "transform=\"translate($posX,$posY) scale($Xscale, $Yscale) $rotation\"";
  if ($fill)
    $attribs .= " fill=\"$fill\" ";
    

  return "<g $attribs>\n" .  $svg . "\n</g>\n";
}
