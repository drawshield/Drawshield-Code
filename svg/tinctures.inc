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
//                if ($value[0] != '#') {
//                    $value = '#' . $value;
//                }
                $retval[$key] = $value;
            }
        }
    }
    return $retval;
}

function rgb($keyterm) {
    global $options, $messages;

    static $loaded = false;
    static $errorColours = array ('#eeeeee','#dddddd','#cccccc', '#bbbbbb', '#aaaaaa',
                                  '#999999', '#888888', '#777777', '#666666', '#555555');
    static $tinctures = array ();

    if (!$loaded) {
        // always read in the drawshield colours as these are the defaults for any not given
        $tinctures = readTinctureFile('svg/schemes/drawshield.txt');
        $palette = $options['palette'];
        if (!file_exists('svg/schemes/' . $palette . '.txt')) {
          $messages->addMessage('internal', "unknown colour scheme - $palette" );
          $palette = 'drawshield';
        } else {
            $tinctures = array_merge($tinctures, readTinctureFile('svg/schemes/' . $palette . '.txt'));
        }
        if ($options['useWebColours'] == true || $options['shape'] == 'flag') {
          $tinctures = array_merge($tinctures, readTinctureFile('svg/schemes/web.txt'));
        }
        if ($options['useWarhammerColours'] == true || $options['shape'] == 'pauldron') {
          $tinctures = array_merge($tinctures, readTinctureFile('svg/schemes/warhammer.txt'));
        }
        $loaded = true;
    }

    if (!strpos($keyterm, '/')) {
        $keyterm = 'heraldic/' . $keyterm;
    }
    list($type,$name) = explode('/',$keyterm);

    if (array_key_exists($name, $tinctures)) {
        return $tinctures[$name];
    } else {
        switch($type) {
          case 'heraldic': // should not happen
            $messages->addMessage('internal', "unknown colour - $name" );
          break;
          case 'web':
            $messages->addMessage('warning', "$name is not a heraldic colour, to use, enable web colours in preferences" );
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



function apply_tincture ($tinctureNode, $targetSVG, $chg_size = '',
        $rot = '0', $rev = false, $inv = false ) {
  global $_SERVER, $messages, $xpath, $options;
  $retval = '';

  if (is_null($tinctureNode)) return $targetSVG; // Maybe an error? Sometimes get here from makeOrdinary()
  if (!is_object($tinctureNode) || is_null($tinctureNode->parentNode) ) {
    // error_log("bad tincture in " . $options['blazon']);
    return $targetSVG;
  }

  $patTrans = '';
  $targetNode = $tinctureNode->parentNode;
	$targetType = $targetNode->nodeName;
  $targetKeyterm = "";
  if ( $targetNode !== null) {
      $targetKeyterm = $targetNode->getAttribute('keyterm'); 
  } else {
      return $targetSVG;
  }

	if ( $targetType == 'charge' and $chg_size != '' ) // We make some things a wee bit smaller if on a charge, just for looks
	{
	  list($chgX,$chgY) = explode(',', $chg_size);
		$scale = max ( $chgX / 1000, $chgY / 1200 ); // multiply here to adjust relative size of fur etc. on a charge
	  $patTrans = ' scale(' . $scale . ') ';
	} elseif ($options['shape'] == 'flag') { // adjust size based on flag AR?
    $patTrans = ' scale(0.5) ';
  }
	if ( $rot != '0' ) // If the object is rotated, we rotate some of the tinctures to match_number()
	  $patTrans .= ' rotate(' . ($rot * -1) . ') ';
	if ( $patTrans != '' )
	  $patTrans = ' patternTransform="' . $patTrans . '" ';

  if ( $tinctureNode->getAttribute('origin') == 'implied' ) return $targetSVG; // Nothing to do
  // stage "raw" means leave charge colours as they are
  // if ( $targetType == 'charge' && array_key_exists('raw', $options)) return $targetSVG;
  $tinctureNode = resolveReference($tinctureNode);
  if ( $tinctureNode === null || !$tinctureNode->hasChildNodes() 
      || $tinctureNode->getAttribute('origin') == 'pending' 
      || $tinctureNode->getAttribute('origin') == 'missing') {
      $messages->addMessage ('blazon','Missing colour, Gray substituted.' );
      $tinctureNode = createColourNode('gray','1');
  }
  $tinctureTypeNode = $tinctureNode->firstChild;
  $spec = false;
  if (!is_object($tinctureTypeNode)) {
    error_log("non-object tincture in " . $options['blazon']);
    $tinctureTypeNode = createColourNode('gray','1');
  }
  switch ( $tinctureTypeNode->nodeName ) {
    case 'colour':
    case 'default': // don't think these two ever occur
    case 'proper':
      if ($targetType == 'ordinary' && array_key_exists('raw', $options)) {
            $spec = '#999999';
      } elseif ( $tinctureTypeNode->hasAttribute('spec') )  {
        $spec = $tinctureTypeNode->getAttribute('spec');
      } else {
        $spec = rgb($tinctureTypeNode->getAttribute('keyterm'));
      }
      if ($spec !== false) { // set if we are to use the fill found
        $retval = '<g fill="' . $spec . '">' . $targetSVG . '</g>';
      } else {
        $retval =  $targetSVG;
      }
      break;
    case 'fur':
      $fur_name = $tinctureTypeNode->getAttribute('keyterm');
      $fur_data = makeFur( $fur_name );
      // If what we are placing is inverted, we also invert the fill so it appears the "right way up"
      // Don't need to do reversed as all furs are symmetrical
      if ( $inv ) $fur_data['body'] = "<g transform=\"translate(0,{$fur_data['height']}) scale(1,-1)\">{$fur_data['body']}</g>";
      $patt_id = add_def ( 'pattern ' . $patTrans .
         ' patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="0" y="0" width="' .
         $fur_data['width'] . '" height="' . $fur_data['height'] . '"',
         '<g class="fur">' . $fur_data['body'] . '</g>' );
      $retval = add_def();
      $stroke = ($targetType == 'ordinary') ? 'stroke="' . rgb('sable') . '" stroke-width="5"' : '';
      $retval .= '<g ' . $stroke . ' fill="url(#' . $patt_id . ')"  >' . $targetSVG . '</g>';
      break;
    case 'treatment':
      // A bit messy, but we handle gradients as a fill, not a pattern fill (could make this its own node type really?)
      if (substr($tinctureTypeNode->getAttribute("keyterm"), 0, 5) == 'grady') {
        $patt_id = makeGrady( $tinctureTypeNode );
      } else {
        $treat_data = makeTreatment( $tinctureTypeNode );
        // If what we are placing is inverted, we also invert the fill so it appears the "right way up"
        // Don't need to do reversed as all treatments are symmetrical
        if ( $inv ) $treat_data['body'] = "<g transform=\"translate(0,{$treat_data['height']}) scale(1,-1)\">{$treat_data['body']}</g>";
        $patt_id = add_def ( 'pattern ' . $patTrans .
          ' patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="0" y="0" width="' .
          $treat_data['width'] . '" height="' . $treat_data['height'] . '"',
          '<g class="treatment">' . $treat_data['body'] . '</g>' );
      }
      $retval = add_def();
      $retval .= '<g fill="url(#' . $patt_id . ')"  >' . $targetSVG . '</g>';
      break;
    case 'semyde':
      $treat_data = makeSemy( $tinctureTypeNode );
      // If what we are placing is inverted, we also invert the fill so it appears the "right way up"
      if ( $inv ) $treat_data['body'] = "<g transform=\"translate(0,{$treat_data['height']}) scale(1,-1)\">{$treat_data['body']}</g>";
      if ( $rev ) $treat_data['body'] = "<g transform=\"translate({$treat_data['width']},0) scale(-1,1)\">{$treat_data['body']}</g>";
      $patt_id = add_def ( 'pattern ' . $patTrans .
         ' patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="0" y="0" width="' .
         $treat_data['width'] . '" height="' . $treat_data['height'] . '"',
         $treat_data['body'] );
      $retval = add_def();
      $retval .= '<g fill="url(#' . $patt_id . ')"  >' . $targetSVG . '</g>';
      break;
    case 'division': 
		  $title = '<title>' . $tinctureTypeNode->getAttribute('keyterm') . '</title>';
	    // What we do depends on what we are filling...
      switch ($targetType) {
        case 'division':
            $new_div = makeDivision( $tinctureTypeNode );
            $mask_id = add_def ( 'mask width="1200" height="1400" maskUnits="userSpaceOnUse"',
              $title . "<desc>Division used to fill $targetKeyterm</desc>" .
              '<g fill="white">' . $targetSVG . '</g>' );
          $retval = add_def();
          $retval .= '<g mask="url(#' . $mask_id . ')"  >' . $new_div . '</g>';
          break;
        case 'field': // just the field of the shield
				  $retval = '<g>' . $title . '<desc>Division used to fill field</desc>' . makeDivision( $tinctureTypeNode ) . '</g>';
			  	break;
        case 'ordinary':
          if ($targetKeyterm == 'plain-cross' || $targetKeyterm == 'bend') {
            $new_div = makeDivision( $tinctureTypeNode );
             $mask_id = add_def ( 'mask width="1200" height="1400" maskUnits="userSpaceOnUse"',
                $title . "<desc>Division used to fill $targetKeyterm</desc>" .
                '<g fill="white">' . $targetSVG . '</g>' );
            $retval = add_def();
            $retval .= '<g mask="url(#' . $mask_id . ')"  >' . $new_div . '</g>';
            break;
          } // else flow through
        case 'charge':
				  chief('push','NC');
          $new_div = makeDivision( $tinctureTypeNode );
          chief('pop');
					$height = get_div_height($tinctureTypeNode);
          $patt_id = add_def ( 'pattern width="100%" height="100%" patternUnits="objectBoundingBox" preserveAspectRatio="none" viewBox="0 0 1000 ' .   $height . '"',
              $title . "<desc>Division used to fill $targetType</desc>" .
               $new_div ); 
          $retval = add_def();
          $retval .= '<g fill="url(#' . $patt_id . ')"  >' . $targetSVG . '</g>';
				  break;
			}
			break;	
	  case 'shield':
	    $shield = makeshield( $tinctureTypeNode, '10:10' );
	    $patt_id = add_def ( 'pattern width="100%" height="100%" patternUnits="objectBoundingBox" preserveAspectRatio="none" viewBox="0 0 1000 1000"',
              "<desc>Shield used to fill $targetType</desc>" .
               $shield ); 
      $retval = add_def();
      $retval .= '<g fill="url(#' . $patt_id . ')"  >' . $targetSVG . '</g>';
      break;
    case 'counterchanged': // Never change anything, just fill with this pattern
      // if ( $targetType == 'charge' ) {
      //   $messages->addMessage ('alert','Browser bug may not render counterchanged charge correctly, try save file as PNG.' );
      // }
      // Try to find a field division
      $division = $xpath->query('preceding::field/tincture/division',$tinctureNode);
      if ( !$division->length ) {
        $messages->addMessage ('error','Counterchange requires two colour field' );
        return '<g fill="#050505"><title>Counterchange</title><desc>Not possible</desc>' . $targetSVG . '</g>';
      }
      $counterDiv = $division->item(($division->length)-1);
      $div_data = makeDivision( $counterDiv, true );
      if (true) {
        $patt_id = add_def ( 'mask', '<g style="fill:#ffffff;">' . $targetSVG . '</g>' );
        $retval = add_def() . '<g mask="url(#' . $patt_id . ')"  >' . $div_data . '</g>';
      } else {
        $patt_id = add_def ( 'clipPath', $targetSVG );
        $retval = add_def() . '<g clip-path="url(#' . $patt_id . ')"  >' . $div_data . '</g>';
      }
			if ( $targetType == 'charge' ) // add the outline and any shading
			  $retval .= '<g style="fill:none;">' . $targetSVG . '</g>';
      break;
			// END of switch
  }
  return $retval;
}
