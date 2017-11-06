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

function apply_tincture ( DOMElement $tincture, $object, $chg_size = '',
        $rot = '0', $rev = false, $inv = false ) {
  global $_SERVER, $messages, $xpath;
  $retval = '';

	$patTrans = '';
	$object_type = $tincture->parentNode->nodeName;
  $objectKeyterm = "";
  if ( $tincture->parentNode !== null) {
      $objectKeyterm = $tincture->parentNode->getAttribute('keyterm'); 
  } else {
      return $object;
  }

	if ( $object_type == 'charge' and $chg_size != '' ) // We make some things a wee bit smaller if on a charge, just for looks
	{
	  list($chgX,$chgY) = explode(',', $chg_size);
		$scale = max ( $chgX / 1000, $chgY / 1200 ); // multiply here to adjust relative size of fur etc. on a charge
	  $patTrans = ' scale(' . $scale . ') ';
	}
	if ( $rot != '0' ) // If the object is rotated, we rotate some of the tinctures to match_number()
	  $patTrans .= ' rotate(' . ($rot * -1) . ') ';
	if ( $patTrans != '' )
	  $patTrans = ' patternTransform="' . $patTrans . '" ';

  if ( $tincture->getAttribute('origin') == 'implied' ) return $object; // Nothing to do
  $tincture = resolveReference($tincture);
  if ( ! $tincture->hasChildNodes() ) {
    if ($tincture->getAttribute('origin') == 'pending') {
      $messages->addMessage ('blazon','Missing tincture? Gray substituted.' );
      $tincture = createColourNode('gray','1');
    } else {
      $messages->addMessage('internal', 'Malformed tincture ID=' . $tincture->getAttribute('ID'));
      return '';
    }
  }
  $child = $tincture->firstChild;
  switch ( $child->nodeName ) {
    case 'colour':
    case 'default': // don't think these two ever occur
    case 'proper':
      if ( $child->hasAttribute('spec') )  {
        $spec = $child->getAttribute('spec');
      } else {
        $spec = rgb($child->getAttribute('keyterm'));
      }
      $retval = '<g fill="' . $spec . '">' . $object . '</g>';
      break;
    case 'fur':
      $fur_name = $child->getAttribute('keyterm');
      $fur_data = makeFur( $fur_name );
      // If what we are placing is inverted, we also invert the fill so it appears the "right way up"
      // Don't need to do reversed as all furs are symmetrical
      if ( $inv ) $fur_data['body'] = "<g transform=\"translate(0,{$fur_data[1]}) scale(1,-1)\">{$fur_data['body']}</g>";
      $patt_id = add_def ( 'pattern ' . $patTrans .
         ' patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="0" y="0" width="' .
         $fur_data['width'] . '" height="' . $fur_data['height'] . '"',
         '<g class="fur">' . $fur_data['body'] . '</g>' );
      $retval = add_def();
      $stroke = ($object_type == 'ordinary') ? 'stroke="' . rgb('sable') . '" stroke-width="5"' : '';
      $retval .= '<g ' . $stroke . ' fill="url(#' . $patt_id . ')"  >' . $object . '</g>';
      break;
    case 'treatment':
      $treat_data = makeTreatment( $child );
      // If what we are placing is inverted, we also invert the fill so it appears the "right way up"
      // Don't need to do reversed as all treatments are symmetrical
      if ( $inv ) $treat_data['width'] = "<g transform=\"translate(0,{$treat_data['height']}) scale(1,-1)\">{$treat_data['body']}</g>";
      $patt_id = add_def ( 'pattern ' . $patTrans .
         ' patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="0" y="0" width="' .
         $treat_data['width'] . '" height="' . $treat_data['height'] . '"',
        '<g class="treatment">' . $treat_data['body'] . '</g>' );
      $retval = add_def();
      $retval .= '<g fill="url(#' . $patt_id . ')"  >' . $object . '</g>';
      break;
    case 'semyde':
      $treat_data = makeSemy( $child );
      // If what we are placing is inverted, we also invert the fill so it appears the "right way up"
      if ( $inv ) $treat_data['body'] = "<g transform=\"translate(0,{$treat_data['height']}) scale(1,-1)\">{$treat_data['body']}</g>";
      if ( $rev ) $treat_data['body'] = "<g transform=\"translate({$treat_data['width']},0) scale(-1,1)\">{$treat_data['body']}</g>";
      $patt_id = add_def ( 'pattern ' . $patTrans .
         ' patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="0" y="0" width="' .
         $treat_data['width'] . '" height="' . $treat_data['height'] . '"',
         $treat_data['body'] );
      $retval = add_def();
      $retval .= '<g fill="url(#' . $patt_id . ')"  >' . $object . '</g>';
      break;
    case 'division': 
		  $title = '<title>' . $child->getAttribute('keyterm') . '</title>';
	    // What we do depends on what we are filling...
      switch ($object_type) {
        case 'division':
        case 'field': // just the field of the shield
				  $retval = '<g>' . $title . '<desc>Division used to fill field</desc>' . makeDivision( $child ) . '</g>';
			  	break;
        case 'ordinary':
          if ($objectKeyterm == 'plain-cross' || $objectKeyterm == 'bend') {
            $new_div = makeDivision( $child );
             $mask_id = add_def ( 'mask width="1200" height="1400" maskUnits="userSpaceOnUse"',
                $title . "<desc>Division used to fill $objectKeyterm</desc>" .
                '<g fill="white">' . $object . '</g>' );
            $retval = add_def();
            $retval .= '<g mask="url(#' . $mask_id . ')"  >' . $new_div . '</g>';
            break;
          } // else flow through
        case 'charge':
				  chief('push','NC');
          $new_div = makeDivision( $child );
          chief('pop');
					$height = get_div_height($child);
          $patt_id = add_def ( 'pattern width="100%" height="100%" patternUnits="objectBoundingBox" preserveAspectRatio="none" viewBox="0 0 1000 ' .   $height . '"',
              $title . "<desc>Division used to fill $object_type</desc>" .
               $new_div ); 
          $retval = add_def();
          $retval .= '<g fill="url(#' . $patt_id . ')"  >' . $object . '</g>';
				  break;
			}
			break;	
	  case 'shield':
	    $shield = makeshield( $child, '10:10' );
	    $patt_id = add_def ( 'pattern width="100%" height="100%" patternUnits="objectBoundingBox" preserveAspectRatio="none" viewBox="0 0 1000 1000"',
              "<desc>Shield used to fill $object_type</desc>" .
               $shield ); 
      $retval = add_def();
      $retval .= '<g fill="url(#' . $patt_id . ')"  >' . $object . '</g>';
      break;
    case 'counterchanged': // Never change anything, just fill with this pattern
      // Try to find a field division
      $division = $xpath->query('preceding::field/tincture/division|ancestor::field/tincture/division',$tincture);
      if ( !$division->length )
			  return '<g fill="#050505"><title>Counterchange</title><desc>Not possible</desc>' . $object . '</g>';
			// else go ahead
      $div_data = makeDivision( $division->item(($division->length)-1), true );
      if ( $object_type == 'charge' )
        $messages->addMessage ('warning','Browser bug may not render counterchanged charge correctly, try save file as PNG.' );
      $patt_id = add_def ( 'clipPath', $object );
      $retval = add_def() . '<g clip-path="url(#' . $patt_id . ')"  >' . $div_data . '</g>';
			if ( $object_type == 'charge' ) // add the outline and any shading
			   $retval .= '<g style="fill:none;">' . $object . '</g>';
      break;
			// END of switch
  }
  return $retval;
}

function rotatePoint( $x, $y, $theta ) {
  switch ( $theta ) {
    case 360:
    case 0: // should not happen, but just in case...
      $x2 = $x;
      $y2 = $y;
      break;
    case 90:
      $x2 = $y * -1;
      $y2 = $x;
      break;
    case 180:
      $x2 = $x * -1;
      $y2 = $y * -1;
      break;
    case 270:
      $x2 = $y;
      $y2 = $x * -1;
      break;
    default:
      $cos = cos(deg2rad($theta));
      $sin = sin(deg2rad($theta));
      $x2 = ($cos * $x) - ($sin * $y);
      $y2 = ($cos * $y) + ($sin * $x);
      break;
  }
  $retval = sprintf('%.4g,%.4g',$x2,$y2);
  return $retval;
}

function getNumbers ( $argString ) {
  global $messages;
  $argString = preg_replace('/-/',' -', $argString);
  $argArray = array();
  $length = strlen($argString);
  $numStr = '';
  $expStr = '';
  $component = 'man';
  
  for ( $count = 0; $count < $length; $count++ ) {
    $char = $argString{$count};
    if ( $char == 'e' ) {
      $component = 'exp';
      $messages->addMessage('warning',"Found exponent");
    } elseif ( ctype_digit($char) or $char == '.' or $char == '-' ) {
      if ( $component == 'man' ) 
        $numStr .= $char;
      else
        $expStr .= $char;
    } else { // Got a complete number
      if ( strlen($numStr) ) {
        $number = (float) $numStr;
        if ( $component == 'exp' ) {
          $number = 0.0;
        }
        $argArray[] = $number;
        $numStr = '';
        $expStr = '';
        $component = 'man';
      }
    }
  } // Process last number
  if ( strlen($numStr) ) {
    $number = (float) $numStr;
    if ( $component == 'exp' ) {
      $number = 0.0;
    }
    $argArray[] = $number;
  }
  return $argArray;
}

function rotatePath( $path, $theta ) {
  if ( $theta == 0 ) return $path;
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) )
      $numbers = getNumbers( $chunks[$i+1] );
    switch ( strtolower($command) ) {
      case 'h':
        $x = $numbers[0];
        if ($theta == 270 )
          $newPath .= sprintf('v%.4g', ($x * -1));
        elseif ( $theta == 180 )
          $newPath .= sprintf('h%.4g', ($x * -1));
        elseif ( $theta == 90 )
          $newPath .= 'v' . $x;
        else
          $newPath .= 'l' . rotatePoint($x,0, $theta);
        break;
      case 'v':
        $y = $numbers[0];
        if ($theta == 270 )
          $newPath .= 'h' . $y;
        elseif ( $theta == 180 )
          $newPath .= sprintf('v%.4g', ($y * -1));
        elseif ( $theta == 90 )
          $newPath .= sprintf('h%.4g', ($y * -1));
        else
          $newPath .= 'l' . rotatePoint(0,$y, $theta);
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$sweep,$x,$y) = $numbers;
        $newPath .= sprintf('a%.4g,%.4g %.4g,%d,%d %s', $rx,$ry, (($rot + $theta)%360),$arc,$sweep, rotatePoint($x,$y, $theta));
        break;
      case 'l':
      case 'q':
      case 'c':
      case 'm':
        $newPath .= $command;
        if ($command == 'M') {
          $newPath .= $numbers[0] . ',' . $numbers[1];
        } else {
          for ( $j = 0; $j < count($numbers); $j += 2 )
            $newPath .= rotatePoint($numbers[$j], $numbers[$j+1], $theta ) . ' ';
          }
        break;
      case 'z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}

function translatePath( $path, $xTrans, $yTrans ) {
  if ( $xTrans == 0 and $yTrans == 0 ) return $path;
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  $firstMove = true;
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) )
      $numbers = getNumbers( $chunks[$i+1] );
    switch ( $command ) {
      case 'H':
        $newPath .= sprintf('H%.4g', $numbers[0] + $xTrans);
        break;
      case 'V':
        $newPath .= sprintf('V%.4g', $numbers[0] + $yTrans);
        break;
      case 'M':
      case 'L':
      case 'C':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $newPath .= sprintf('%.4g,', ($numbers[$j] + $xTrans));
          $newPath .= sprintf('%.4g ', ($numbers[$j+1] + $yTrans));
        }
        if ( $command == 'M' ) $firstMove = false;
        break;
      case 'm':
        $newPath .= 'm';
        if ( $firstMove ) {
          $newPath .= sprintf('%.4g,', ($numbers[0] + $xTrans));
          $newPath .= sprintf('%.4g ', ($numbers[1] + $yTrans));
          for ( $j = 2; $j < count($numbers); $j += 2 ) {
            $newPath .= sprintf('%.4g,', ($numbers[$j]));
            $newPath .= sprintf('%.4g ', ($numbers[$j+1]));
          }
          $firstMove = false;
        } else {  
          for ( $j = 0; $j < count($numbers); $j += 2 ) {
            $newPath .= sprintf('%.4g,', ($numbers[$j]));
            $newPath .= sprintf('%.4g ', ($numbers[$j+1]));
          }
        }
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$sweep,$x,$y) = $numbers;
        $newPath .= sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry, $rot,$arc,$sweep, $x,$y);
        break;
      case 'h':
      case 'v': // Pass all unchanged
        $newPath .= $command . sprintf('%.4g', $numbers[0]);
        break;
      case 'l':
      case 'q':
      case 'c':
        $newPath .= $command;
        $odd = true;
        foreach ( $numbers as $number ) {
          $newPath .= (sprintf('%.4g', $number)) . ($odd ? ',':' ');
          $odd = !$odd;
        }
        break;
      case 'Z':
      case 'z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}

function matMul ( $a, $b, $c, $d, $e, $f, $x, $y ) {
  return array ( (($a * $x) + ($c * $y) + $e), (($b * $x) + ($d * $y) + $f) );
}

function matrixPath( $path, $a, $b, $c, $d, $e, $f ) {
  global $messages;
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  $firstMove = true;
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) )
      $numbers = getNumbers( $chunks[$i+1] );
    switch ( $command ) {
      case 'H':
        $coords =  matMul($a, $b, $c, $d, $e, $f, $numbers[0], 0);
        $newPath .= sprintf('l%.4g,%.4g ', $coords[0], $coords[1]);
        break;
      case 'V':
        $coords =  matMul($a, $b, $c, $d, $e, $f, 0, $numbers[0]);
        $newPath .= sprintf('l%.4g,%.4g ', $coords[0], $coords[1]);
        break;
      case 'M':
      case 'L':
      case 'C':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $coords =  matMul($a, $b, $c, $d, $e, $f, $numbers[$j], $numbers[$j+1]);
          $newPath .= sprintf('%.4g,', $coords[0]);
          $newPath .= sprintf('%.4g ', $coords[1]);
        }
        if ( $command == 'M' ) $firstMove = false;
        break;
      case 'm':
        $newPath .= 'm';
        if ( $firstMove ) {
          $coords = matMul($a, $b, $c, $d, $e, $f, $numbers[0], $numbers[1]);
          $newPath .= sprintf('%.4g, %.4g ', $coords[0], $coords[1]);
          for ( $j = 2; $j < count($numbers); $j += 2 ) {
            $coords = matMul($a, $b, $c, $d, 0, 0, $numbers[$j], $numbers[$j+1]);
            $newPath .= sprintf('%.4g,', ($coords[0]));
            $newPath .= sprintf('%.4g ', ($coords[1]));
          }
          $firstMove = false;
        } else {  
          for ( $j = 0; $j < count($numbers); $j += 2 ) {
            $coords = matMul($a, $b, $c, $d, 0, 0, $numbers[$j], $numbers[$j+1]);
            $newPath .= sprintf('%.4g,', ($coords[0]));
            $newPath .= sprintf('%.4g ', ($coords[1]));
          }
        }
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$sweep,$x,$y) = $numbers; /* TODO extract rotation & apply */
        $newPath .= sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry, $rot,$arc,$sweep, $x,$y);
        $messages->addMessage('internal','no matrix apply for arc');
        break;
      case 'h':
        $coords =  matMul($a, $b, $c, $d, 0, 0, $numbers[0], 0);
        $newPath .= sprintf('l%.4g,%.4g ', $coords[0], $coords[1]);
        break;
      case 'v':
        $coords =  matMul($a, $b, $c, $d, 0, 0, 0, $numbers[0]);
        $newPath .= sprintf('l%.4g,%.4g ', $coords[0], $coords[1]);
        break;
      case 'l':
      case 'q':
      case 'c':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $coords = matMul($a, $b, $c, $d, 0, 0, $numbers[$j], $numbers[$j+1]);
          $newPath .= sprintf('%.4g,', ($coords[0]));
          $newPath .= sprintf('%.4g ', ($coords[1]));
        }
        break;
      case 'Z':
      case 'z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}

function mirrorPath ( $path, $axis = 'x' ) {
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) ) {
      $numbers = getNumbers($chunks[$i+1]);
    }
    switch ( strtolower($command) ) {
      case 'h':
        $x = $numbers[0];
        if ($axis == 'x')
          $newPath .= sprintf('h%.4g',$x);
        else
          $newPath .= sprintf('h%.4g',($x * -1));
        break;
      case 'v':
        $y = $numbers[0];
        if ($axis == 'x')
          $newPath .= sprintf('v%.4g',($y * -1));
        else
          $newPath .= sprintf('v%.4g',$y);
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        if ( $axis == 'x' ) {
          $swap = $swap == 1? 0 : 1;
          $newPath .= sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry, (($rot + 180)%360),$arc,$swap, $x,$y);
        } else {
          $x *= -1;
          $newPath .= sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry,(($rot + 180)%360),$arc,$swap, $x,$y);
        }
        break;
      case 'l':
      case 'q':
      case 'c':
      case 's':
      case 'm':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          if ( $command == 'M' ) {
            $newX = $numbers[$j];
            $newY = $numbers[$j+1];
          } elseif ( $axis == 'x' ) {
            $newX = $numbers[$j];
            $newY = $numbers[$j+1] * -1;
          } else {
            $newX = $numbers[$j] * -1;
            $newY = $numbers[$j+1];
          }
          $newPath .= sprintf ('%.4g,%.4g ', $newX, $newY);
        }
        break;
      case 'z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}

function scalePath ( $path, $xScale = 1, $yScale = 1 ) {
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) ) {
      $numbers = getNumbers($chunks[$i+1]);
    }
    switch ( strtolower($command) ) {
      case 'h':
        $x = $numbers[0];
        if ($xScale == 1)
          $newPath .= sprintf('h%.4g',$x);
        else
          $newPath .= sprintf('h%.4g',($x * $xScale));
        break;
      case 'v':
        $y = $numbers[0];
        if ($yScale == 1)
          $newPath .= sprintf('v%.4g',$y);
        else
          $newPath .= sprintf('v%.4g',($y * $yScale));
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        if ( $xScale != 1 ) {
          $rx *= $xScale;
          $x *= $xScale;
        }
        if ( $yScale != 1 ) {
          $ry *= $yScale;
          $y *= $yScale;
        }
        $newPath .=  sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry, $rot,$arc,$swap, $x,$y);
        break;
      case 'c':
      case 'l':
      case 'q':
      case 's':
      case 'm':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $newY = $numbers[$j+1];
          $newX = $numbers[$j];
          if ( $xScale != 1 ) {
            $newX *= $xScale;
          }
          if ( $yScale != 1 ) {
            $newY *= $yScale;
          }
          $newPath .= sprintf ('%.4g,%.4g ', $newX, $newY);
        }
        break;
      case 'z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}

function makeRelative ( $path, $Xoff = 0, $Yoff = 0 ) {
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  $curX = $Xoff; $curY = $Yoff; 
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) ) {
      $numbers = getNumbers($chunks[$i+1]);
    }
    switch ( $command ) {
      case 'h':
        $x = $numbers[0];
        $curX += $x;
        $newPath .= sprintf('h%.4g',$x);
        break;
      case 'H':
        $x = $numbers[0];
        $newPath .= sprintf('h%.4g',($x - $curX));
        $curX = $x;
        break;
      case 'v':
        $y = $numbers[0];
        $curY += $y;
        $newPath .= sprintf('v%.4g',$y);
        break;
      case 'V':
        $y = $numbers[0];
        $newPath .= sprintf('v%.4g',($y - $curY));
        $curY = $y;
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        $newPath =  sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry, $rot,$arc,$swap, $x,$y);
        $curX += $x;
        $curY += $y;
        break;
      case 'A':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        $newPath =  sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry,$rot,$arc,$swap, ($x - $curX),($y - $curY));
        $curX = $x;
        $curY = $y;
        break;
      case 'c':
        $tempPath = 'c';
        for ( $j = 0; $j < count($numbers); $j += 6 ) {
          $cpStartX = $numbers[$j+0];
          $cpStartY = $numbers[$j+1];
          $cpEndX = $numbers[$j+2];
          $cpEndY = $numbers[$j+3];
          $endX = $numbers[$j+4];
          $endY = $numbers[$j+5];
          $tempPath .= sprintf('%.4g,%.4g %.4g,%.4g %.4g,%.4g ',$cpStartX,$cpStartY, $cpEndX,$cpEndY, $endX,$endY);
          $curX += $endX;
          $curY += $endY;
        }
        $newPath .= $tempPath;
        break;
      case 'C': // $newPath .= "($curX,$curY)";
        $tempPath = 'c';
        for ( $j = 0; $j < count($numbers); $j += 6 ) {
          $cpStartX = $numbers[$j+0];
          $cpStartY = $numbers[$j+1];
          $cpEndX = $numbers[$j+2];
          $cpEndY = $numbers[$j+3];
          $endX = $numbers[$j+4];
          $endY = $numbers[$j+5];
          $tempPath .= sprintf('%.4g,%.4g %.4g,%.4g %.4g,%.4g ', ($cpStartX - $curX), ($cpStartY - $curY),($cpEndX - $curX),($cpEndY - $curY),($endX - $curX),($endY - $curY));
          $curX = $endX;
          $curY = $endY;
        }
        $newPath .= $tempPath;
        break;
      case 'l':
      case 'm':
        $tempPath = $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $y = $numbers[$j+1];
          $x = $numbers[$j];
          $tempPath .= sprintf('%.4g,%.4g ',$x,$y);
          $curX += $x;
          $curY += $y;
        }
        $newPath .= $tempPath;
        break;
      case 'L':
      case 'M':
        $tempPath = strtolower($command);
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $y = $numbers[$j+1];
          $x = $numbers[$j];
          $tempPath .= sprintf('%.4g,%.4g ', ($x - $curX),($y - $curY));
          $curX = $x;
          $curY = $y;
        }
        $newPath .= $tempPath;
        break;
      case 'z':
      case 'Z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}

function reversePath ( $path ) {
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) ) {
      $numbers = getNumbers($chunks[$i+1]);
    }
    switch ( strtolower($command) ) {
      case 'h':
        $x = $numbers[0];
        $newPath = sprintf('h%.4g',($x * -1)) . $newPath;
        break;
      case 'v':
        $y = $numbers[0];
        $newPath = sprintf('v%.4g',($y * -1)) . $newPath;
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        $newPath =   sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g',$rx,$ry, $rot,$arc,$swap,($x * -1),($y * -1)) . $newPath;
        break;
      case 'c':
        $tempPath = '';
        for ( $j = 0; $j < count($numbers); $j += 6 ) {
          $cpStartX = $numbers[$j+0];
          $cpStartY = $numbers[$j+1];
          $cpEndX = $numbers[$j+2];
          $cpEndY = $numbers[$j+3];
          $endX = $numbers[$j+4];
          $endY = $numbers[$j+5];
          $newCPStart = sprintf( '%.4g,%.4g ', ($cpEndX - $endX),($cpEndY - $endY));
          $newCPEnd = sprintf( '%.4g,%.4g ', ($cpStartX - $endX),($cpStartY - $endY));
          $newEnd = sprintf( '%.4g,%.4g ', ($endX * -1),($endY * -1));
          $tempPath = $newCPStart . $newCPEnd . $newEnd . $tempPath;
        }
        $tempPath = $command . $tempPath;
        $newPath = $tempPath . $newPath;
        break;
      case 'l':
      case 'q':
      case 's':
        $tempPath = '';
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $newY = $numbers[$j+1] * -1;
          $newX = $numbers[$j] * -1;
          $tempPath = sprintf('%.4g,%.4g ', $newX,$newY) . $tempPath;
        }
        $tempPath = $command . $tempPath;
        $newPath = $tempPath . $newPath;
        break;
      case 'z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}


function lineStraight( $angle, $dist ) {
  return rotatePath('h'.$dist, $angle);
}

?>