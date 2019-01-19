<?php


function readTinctureFile($filename) {
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
                if ($value[0] != '#') {
                    $value = '#' . $value;
                }
                $retval[$key] = $value;
            }
        }
    }
    return $retval;
}

function rgb($name) {
    global $options, $messages;

    static $loaded = false;
    static $tinctures = array ();

    if (!$loaded) {
        // always read in the drawshield colours as these are the defaults for any not given
        $tinctures = readTinctureFile('svg/schemes/drawshield.txt');
        $palette = $options['palette'];
        if (!file_exists('svg/schemes/' . $palette . '.txt')) {
            $palette = 'drawshield';
            $messages->addMessage('internal', "unknown colour scheme - $palette" );
        } else {
            $tinctures = array_merge($tinctures, readTinctureFile('svg/schemes/' . $palette . '.txt'));
        }
        $loaded = true;
    }
    if ($options['drawType'] == 'flag') {
      $tinctures = array_merge($tinctures, readTinctureFile('svg/schemes/flags.txt'));
    }

    if (array_key_exists($name, $tinctures)) {
        return $tinctures[$name];
    } else {
        $messages->addMessage('internal', "unknown colour - $name" );
    }
    return '#ffffff';

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



function apply_tincture ( DOMElement $tincture, $object, $chg_size = '',
        $rot = '0', $rev = false, $inv = false ) {
  global $_SERVER, $messages, $xpath, $options;
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
	} elseif ($options['drawType'] == 'flag') { // adjust size based on flag AR?
    $patTrans = ' scale(0.5) ';
  }
	if ( $rot != '0' ) // If the object is rotated, we rotate some of the tinctures to match_number()
	  $patTrans .= ' rotate(' . ($rot * -1) . ') ';
	if ( $patTrans != '' )
	  $patTrans = ' patternTransform="' . $patTrans . '" ';

  if ( $tincture->getAttribute('origin') == 'implied' ) return $object; // Nothing to do
  // stage "raw" means leave charge colours as they are
  if ( $object_type == 'charge' && array_key_exists('raw', $options)) return $object;
  $tincture = resolveReference($tincture);
  if ( ! $tincture->hasChildNodes() ) {
    if ($tincture->getAttribute('origin') == 'pending' || $tincture->getAttribute('origin') == 'missing') {
      $messages->addMessage ('blazon','Gray substituted.' );
      $tincture = createColourNode('gray','1');
    } /* else {
      $messages->addMessage('internal', 'Malformed tincture ID=' . $tincture->getAttribute('ID'));
      return '';
    } */
  }
  $child = $tincture->firstChild;
  switch ( $child->nodeName ) {
    case 'colour':
    case 'default': // don't think these two ever occur
    case 'proper':
      if (array_key_exists('raw', $options)) {
        switch ($object_type) {
          case 'charge':
            $spec = 'none';
            break;
          case 'ordinary':
            $spec = '#999999';
            break;
          default:
            break;
        }
      } elseif ( $child->hasAttribute('spec') )  {
        $spec = $child->getAttribute('spec');
      } else {
        $spec = rgb($child->getAttribute('keyterm'));
      }
      if ($spec !== false) { // set if we are to use the fill found
        $retval = '<g fill="' . $spec . '">' . $object . '</g>';
      } else {
        $retval =  $object;
      }
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
      // A bit messy, but we handle gradients as a fill, not a pattern fill (could make this its own node type really?)
      if (substr($child->getAttribute("keyterm"), 0, 5) == 'grady') {
        $patt_id = makeGrady( $child );
      } else {
        $treat_data = makeTreatment( $child );
        // If what we are placing is inverted, we also invert the fill so it appears the "right way up"
        // Don't need to do reversed as all treatments are symmetrical
        if ( $inv ) $treat_data['width'] = "<g transform=\"translate(0,{$treat_data['height']}) scale(1,-1)\">{$treat_data['body']}</g>";
        $patt_id = add_def ( 'pattern ' . $patTrans .
          ' patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="0" y="0" width="' .
          $treat_data['width'] . '" height="' . $treat_data['height'] . '"',
          '<g class="treatment">' . $treat_data['body'] . '</g>' );
      }
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