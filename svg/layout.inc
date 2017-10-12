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

function place_charges ( DOMElement $node, $placement ) {
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
  $num_of = $node->getAttribute('number');
  if ( $num_of < 1 ) $num_of = 1; // Just in case...
  $between = null;
  $retval = '';
  /**
   * @var DOMElement $child
   */
  if ( $child = getModifierNodeWithKeyterm($node,'between')) {
    $between = $child->firstChild;
    $between_placement = get_placement($between);
    // If between has its own placement, use that
    if ($between_placement != 'i' ) {
      $retval .= place_charges( $between, $between_placement);
      $between = null;
    } else {
      $num_of += (integer)$between->getAttribute('number');
      $half = ($between->getAttribute('number')) / 2;
    }
  }

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
  switch($place_list{0}) {
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
    if ( isset($items[0]) and $items[0] != '' ) {
      $cx = $items[0];
  //    if ( $sinister )$cx = 1000 - $cx;
    }
    if ( isset($items[1]) and $items[1] != '' ) {
      $cy = $items[1];
    //  if ( $inverted) $cy = 1200 - $cy;
   //   if ( $enhanced) $cy -= 200;
   //   if ( $abased) $cy += 200;
    }
    if ( isset($items[2]) and $items[3] != '' ) $bbx = $items[2];
    if ( isset($items[3]) and $items[2] != '' ) $bby = $items[3];
    if ( isset($items[4]) and $items[4] != '' ) {
      $rot = $items[4];
  //    if ( $sinister ) { if ( $rot != 0 ) { $rot += 90; } }
  //    if ( $inverted ) { if ( $rot != 0 ) { $rot -= 90; } }
    }
    if ( isset($items[5]) and $items[5] != '' ) $inv = $items[5];
    if ( isset($items[6]) and $items[6] != '' ) $rev = $items[6];
    $item = $node;
    if ( $between and ( ($count+1) <= $half or ($count+1) > ($num_of - $half))) 
      $item = $between;
    $retval .= PlaceCharge( $item, $bbx, $bby, $cx, $cy, $rot, $inv, $rev );
  }
  return $retval;
}


function PlaceCharge ( DOMElement $node,  // images
                            $boundX, $boundY,  // size of bounding box
                            $centreX, $centreY,// centre of bounding box
                            $rotation = '0',
                            $inverted = '0',
                            $reversed = '0' ) {
  global $dom;

  $charge = getCharge( $node );
  $orientation = null;
  $tincture = null;
  $Xflex = array_key_exists('wflex', $charge) ? $charge['wflex'] : 1;
  $Yflex = array_key_exists('hflex', $charge) ? $charge['hflex'] : 1;
//  $parent = $node->parentNode;
  foreach ( $node->childNodes as $child ) {
    if ( $child->nodeName == 'modifier' ) {
      switch ( $child->getAttribute('keyterm') ) {
        case 'inverted':
          $inverted = true;
          break;
        case 'reversed':
          $reversed = true;
          break;
        case 'bendwise':
        case 'bendsinwise':
        case 'fesswise':
        case 'palewise':
          $orientation = $child->getAttribute('keyterm');
          break;
        case 'entire':
          $Xflex = $Yflex = 9999;
          break;
      }
    } elseif ( $child->nodeName == 'tincture' ) {
      $tincture = $child;
    }
  }
  if ( $tincture == null ) {
    $col = $dom->createElement('colour');
    $col->setAttribute( 'name', 'gules');
    $tincture = $dom->createElement('tincture');
    $tincture->appendChild ( $col);
  }
  // If there are multiple charges, choose the one with the best match to the bounding box
  $BoundAR = 1;
  if ($boundY != 0) {
    $BoundAR = $boundX / $boundY;
  }
  //if ( ! array_key_exists('body', $charge) ) { // must be an array of different charge shapes
  //  $closestAR = 9999;
  //  $bestMatch = 0;
  //  foreach ( $charge as $chg ) {
  //    $thisAR = $chg['width'] / $chg['height'];
  //    if ( abs($thisAR - $boundAR) < $closestAR ) {
  //      $closestAR = abs($thisAR - $boundAR);
  //     $bestMatch = $chg;
  //    }
  //  }
  //  $charge = $bestMatch;
  //}
  $retval = $charge['body'];
  $chargeX = $charge['width'];
  $chargeY = $charge['height'];

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
        $do_turn = $chargeY > $chargeX;
        if ( array_key_exists('turn_fesswise', $charge) ) { $do_turn = $charge['turn_fesswise']; }
        if ($do_turn ) {
          $retval = "<g transform=\"translate($chargeY,0) rotate(90)\"><desc>fesswise charge</desc>$retval</g>\n";
          $temp = $chargeX; $chargeX = $chargeY; $chargeY = $temp;
          $temp = $Xflex; $Xflex = $Yflex; $Yflex = $temp;
        }
        break;
      case 'bendwise':
      case 'bendsinwise':
        if ( array_key_exists('horizontal', $charge)  ) {
          $angle = ($orientation == 'bendwise') ? "45" : "-45";
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
        $do_turn = $chargeX >= $chargeY;
        if ( array_key_exists('horizontal', $charge)  ) {
          $retval = "<g transform=\"translate(0,$chargeX) rotate(-90)\"><desc>palewise charge</desc>$retval</g>\n";
          $temp = $chargeX; $chargeX = $chargeY; $chargeY = $temp;
          $temp = $Xflex; $Xflex = $Yflex; $Yflex = $temp;
        }
        break;
      case 'erect': // TODO is this still used?
        $retval = "<g transform=\"translate($chargeY,0) rotate(90)\"><desc>erect charge</desc>$retval</g>\n";
        $temp = $chargeX; $chargeX = $chargeY; $chargeY = $temp;
        $temp = $Xflex; $Xflex = $Yflex; $Yflex = $temp;
        break;
    }
  }
  // Fix some older charges that had orient_to_ordinary as a boolean
  if ( array_key_exists('orient_to_ordinary', $charge) ) {
    if ( $charge['orient_to_ordinary'] === false )
      unset( $charge['orient_to_ordinary'] );
    elseif ( $charge['orient_to_ordinary'] === true )
      $charge['orient_to_ordinary'] = 1;
  }

  // If rotation is set this is because we need to turn to fit onto an ordinary
  // If we CANNOT turn then assume that the bounding box is square. This makes sure we fit even if not turned
  if ( $rotation != 0 ) { // TODO apply this logic only if the item really is on an ordinary!
    if ( ! array_key_exists('orient_to_ordinary', $charge) ) {
      $boundX = min( $boundX, $boundY );
      $boundY = $boundX;
      $rotation = 0 ;
    }
    // If we CAN turn then set the appropriate direction (depends on charge)
    else  { // Set to 1 for normal, -1 if needs to turn other way
      $rotation *= $charge['orient_to_ordinary'];
    }
  }

  // Calculate the size of the image to fit into the bounding box,
  $Xscale = $boundX / $chargeX;
  $Yscale = $boundY / $chargeY;
  if ( $Xflex <= 1 and $Yflex <=1 ) { // no flexing, fixed AR
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
  $sizeX = $chargeX * $Xscale;
  $sizeY = $chargeY * $Yscale;
  $posX = $centreX - ($sizeX / 2);
  $posY = $centreY - ($sizeY / 2);
  $transform = '';
  if ( $rotation != 0 ) {
    $transform .= 'rotate(' . $rotation . ',' . $centreX . ',' . $centreY . ') ';
  }
  $transform .= " translate($posX,$posY) scale($Xscale, $Yscale) ";
  $retval = "<g transform=\"$transform\" >\n<title>" . htmlentities($charge['title']) . '</title>' . $retval . "</g>\n";

  $retval = apply_tincture( $tincture, $retval, "$chargeX,$chargeY", $rotation, $reversed, $inverted );
  return $retval;
}

function calcPlace( $node, $aspect, $bb = '100,100,800,900'  ) {
    global $messages;
    
  $wide_rows = array (
    '0',          // 0
    '1',          // 1
    '2',          // 2
    '2,1',        // 3
    '3,1',        // 4
    '3,2',        // 5
    '3,2,1',      // 6
    '3,3,1',      // 7
    '3,3,2',      // 8
    '3,3,3',      // 9
    '4,3,2,1',    // 10
    '4,3,3,1',    // 11
    '4,3,3,2',    // 12
    '4,3,3,3',    // 13
    '5,4,3,2',    // 14
    '5,4,3,2,1',  // 15
    '5,4,3,3,1',  // 16
    '5,4,3,3,2',  // 17
    '6,5,4,2,1',  // 18
    '6,5,4,3,1',  // 19
    '6,5,4,3,2',  // 20
  );
  $narrow_rows = array (
    '0',
    '1',
    '1,1',
    '1,1,1',
    '2,2',
    '2,2,1',
    '2,2,2',
    '3,2,2',
    '3,3,2',
    '3,3,3',
    '4,3,3',
    '4,4,3',
    '4,4,4',      // 12
    '4,4,3,2',    // 13
    '4,4,3,2,1',  // 14
    '4,4,4,3',    // 15
    '4,4,4,4',    // 16
    '4,4,4,4,1',  // 17
    '4,4,4,4,2',  // 18
    '4,4,4,4,3',  // 19
    '4,4,4,4,4',  // 20
  );
  $number = getNumberOfCharges($node);
 // $number = $node->getAttribute('number');
  $conjoined = existModifierWithKeyterm($node,'conjoined');
  $facing_in = existModifierWithKeyterm($node,'facing-in');
  $facing_out = existModifierWithKeyterm($node,'facing-out');
  if ( $number > 20 ) return $number;
  elseif ( $number < 1 ) $number = 1;
  if ( $rows = getModifierValueByKeyterm($node,'arrange-by-rows') )
    $row_list = explode(',', $rows);
  else {
    switch ( $aspect ) {
      case 'h': // single horizontal row
        $row_list = array($number);
        break;
      case 'v': // single vertical row
        $row_list = array_fill(0,$number,'1');
        break;
      case 'w': // wide box
        $row_list = explode(',',$wide_rows[$number]);
        break;
      case 'n': // narrow box
        $row_list = explode(',',$narrow_rows[$number]);
        break;
    }
  }

  if ( $number < 1 ) $number = 1; // Fixes bug elsewhere?
  $placement = str_repeat('x/', $number-1);
  $space_pct = 0.2;
  $max_row = max($row_list);
  $num_rows = count($row_list);
  list($bb_x,$bb_y,$bb_w,$bb_h) = explode(',',$bb);
  if ( $conjoined == 'true' ) {
    // Calculate the charge bounding box (same size for all)
    $cbb_height = $bb_h / $num_rows;
    $cbb_width = $bb_w / $max_row;
    for ( $i = 0; $i < $num_rows; $i++ ) {
      $centre_y = $bb_y + ($cbb_height * $i) + ($cbb_height / 2);
      for ( $j = 0; $j < $row_list[$i]; $j++ ) {
        if ( $row_list[$i] == $max_row and $max_row != 1 ) {
          $centre_x = $bb_x + ($cbb_width * $j) + ($cbb_width/2);
        } else {
          $centre_x = $bb_x + ((1000 - $bb_x - $bb_x - ($cbb_width * $row_list[$i]))/2) + ($cbb_width * $j) + ($cbb_width/2);
        }
        $placement .= sprintf ( "%.2f,%.2f,%.2f,%.2f:",$centre_x,$centre_y,$cbb_width,$cbb_height);
      }
    }
 // } elseif ($number == 1) {
 //   $placement = sprintf ( "%.2f,%.2f,%.2f,%.2f:",($bb_x + ($bb_w*0.5)), ($bb_y + ($bb_h*0.5)), ($bb_w*0.8), ($bb_h*0.8));
  } else {
    // Calculate the charge bounding box (same size for all)
    $vert_gap = ($num_rows == 1) ? 0 : ($bb_h * $space_pct) / ($num_rows-1);
    $cbb_height = ($num_rows == 1) ? $bb_h : ($bb_h * (1-$space_pct)) / $num_rows;
    $cbb_width = ($max_row <= 1 ) ? $bb_w : ($bb_w * (1-$space_pct)) / $max_row;
    $horiz_gap = ($bb_w * $space_pct) / max(1,($max_row - 1));
    for ( $i = 0; $i < $num_rows; $i++ ) {
      $centre_y = $bb_y + (($cbb_height + $vert_gap) * $i) + ($cbb_height / 2);
      $offset = ($bb_w - ($cbb_width * $row_list[$i]) - ($horiz_gap * ($row_list[$i]-1)))/2;
      for ( $j = 0; $j < $row_list[$i]; $j++ ) {
        if ( $row_list[$i] != 1 ) {
          $centre_x = $bb_x + $offset + (($cbb_width + $horiz_gap) * $j) + ($cbb_width/2);
        } else {
          $centre_x = $bb_x + ($bb_w/2);
        }
        $placement .= sprintf ( "%.2f,%.2f,%.2f,%.2f",$centre_x,$centre_y,$cbb_width,$cbb_height);
        if ( $facing_in ) {
           if (($j % 2) == 0 ) { // flip alternate charges if facing in ("combattant")
                $placement .= ",0,0,1";
            } else {
                $placement .= ",0,0,0";
            }
        }
        if ( $facing_out ) {
           if (($j % 2) == 1 ) { // flip alternate charges if facing in ("combattant")
                $placement .= ",0,0,1";
            } else {
                $placement .= ",0,0,0";
            }
        }
        $placement .= ":";
      }
    }
  }
  return $placement;
}
