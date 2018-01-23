<?php

function make1Quarter($child, $xScale, $yScale, $xOffset, $yOffset, $aspect ) {
	global $scale_factor;

	$retval = '';
	$path = ($aspect == 'square') ? '<path d="M0,0 h1000 v1000 h-1000 v-1000 Z" />' :
		'<path d="M0,0 h1000 v1200 h-1000 v-1200 Z" />';
	$clipdef = add_def( 'clipPath', $path );
	$scale_factor = $xScale; // Set the global (used in furs and treatments)
	$scale = " scale($xScale,$yScale) ";
	$translate = " translate($xOffset,$yOffset) ";
	$transform = " transform=\" $translate $scale \" ";
	$retval .= '<g clip-path="url(#' . $clipdef . ')" ' . $transform . ' >' . makeshield( $child, '10:12' ) . "</g>\n";
	$retval .= add_def();
	return $retval;	
}

function makeQuartered($across, $down, $children) {
	$retval = '';
	$subWidth = 1000 / $across;
	$subHeight = 1200 / $down;
	$aspect = (($subWidth / $subHeight) > 0.84) ? 'square' : 'shield';
	$widthScale = 1 / $across;
	$heightScale = 1 / $down;
	$yPos = 0;
	$childCount = 0;
	for ( $aCount = 0; $aCount < $across; $aCount++ ) {
		$xPos = 0;
		for ( $dCount = 0; $dCount < $down; $dCount++ ) {
			$retval .= make1Quarter($children[$childCount++], $widthScale, $heightScale, $xPos, $yPos, $aspect);
			$xPos += $subWidth;
		}
		$yPos += $subHeight;
	}
	return $retval;
}

function makequarters( DOMNodeList $children, $child_ARs, $child_Scales, $translates) {
  global $scale_factor;
  
  $retval = '';
  /**
   * @var DOMElement $kid
   */
  foreach ( $children as $kid ) {
    $index = $kid->getAttribute('index');
    region('push','Q' . $index ); // Can lose this soon I hope...
    $index -= 1; // Arrays are zero based
    $child_AR = $child_ARs[$index];
    $clippath = $xOffset = $yOffset = null; // Prevent error message
    extract( get_clipdata ($child_AR)  ); // Creates clippath, xOffset, yOffset
    $clipdef = add_def( 'clipPath', $clippath );
    $scale = $child_Scales[$index];
    $scale_factor = $scale; // Set the global (used in furs and treatments)
    if ( $scale != '' ) $scale = " scale($scale) ";
    $scaleTrans = '';
    if ( $xOffset != 0 ) $xOffset = "-$xOffset";
    if ( $yOffset != 0 ) $yOffset = "-$yOffset";
    if ( $xOffset or $yOffset ) $scaleTrans = " translate($xOffset, $yOffset) ";
    $translate = $translates[$index];
    $transform = " transform=\" $translate $scale $scaleTrans \" ";
    $retval .= '<g clip-path="url(#' . $clipdef . ')" ' . $transform . ' >' . makeshield( $kid, $child_AR ) . "</g>\n";
    region('pop');
  }
  $retval .= add_def();
  return $retval;
}

function makequartered5( $children, $this_AR ) {
  global $halfHeights, $messages;

  // Default values ( for 10:12 )
  $child_ARs = array ( '10:14', '10:14', '10:14', '10:14', '10:14' );
  $child_Scales = array ( '0.387,0.417', '0.387,0.417', '0.387,0.417', '0.5843', '0.5843' );
  switch ( $this_AR ) {
  case '10:12':
    // same as default
    break;
  case '10:10': // TODO Finish these
    $child_ARs = array ( '10:10', '10:10', '10:10', '10:10' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 0;
    break;         
  case '10:14':
    $child_ARs = array ( '10:14', '10:14', '10:14', '10:14' );
    $child_Scales = array ( '0.5834', '0.5834', '0.5834', '0.5834' );
    $xShift = 70;
    break;  
  case '10:20':
    $child_ARs = array ( '10:20', '10:20', '10:20', '10:20' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 250;
    break;
  case '10:24':
    $child_ARs = array ( '10:24', '10:24', '10:24', '10:24' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 250;
    break;         
  case '10:28':
    $child_ARs = array ( '10:28', '10:28', '10:28', '10:28' );
    $child_Scales = array ( '0.5', '0.5', '0.5834', '0.5834' );
    $xShift = 285;
    break; 
  default:
    $messages->addMessage ( 'internal', 'Unknown aspect ratio for quartered ' . $this_AR );
  }
  $halfHeight = $halfHeights[$this_AR];
  $translates = array (
    " translate(0,0) ",
    " translate(333,0) ",
    " translate(666,0) ",
    " translate(0,$halfHeight) ",
    " translate(500,$halfHeight) ",
  );

  return makequarters( $children, $child_ARs, $child_Scales, $translates );
}

function makequartered4( DOMNodeList $children, $this_AR ) {
  global $halfHeights, $messages;

  // Default values ( for 10:12 )
  $child_ARs = array ( '10:10', '10:10', '10:14', '10:14' );
  $child_Scales = array ( '0.5', '0.5', '0.5843', '0.5843' );
  $xShift = 0;

  switch ( $this_AR ) {
  case '10:12':
    // same as default
    break;
  case '10:10':
    $child_ARs = array ( '10:10', '10:10', '10:10', '10:10' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 0;
    break;         
  case '10:14':
    $child_ARs = array ( '10:14', '10:14', '10:14', '10:14' );
    $child_Scales = array ( '0.5834', '0.5834', '0.5834', '0.5834' );
    $xShift = 70;
    break;  
  case '10:20':
    $child_ARs = array ( '10:20', '10:20', '10:20', '10:20' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 250;
    break;
  case '10:24':
    $child_ARs = array ( '10:24', '10:24', '10:24', '10:24' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 250;
    break;         
  case '10:28':
    $child_ARs = array ( '10:28', '10:28', '10:28', '10:28' );
    $child_Scales = array ( '0.5', '0.5', '0.5834', '0.5834' );
    $xShift = 285;
    break; 
  default:
    $messages->addMessage ( 'internal', 'Unknown aspect ratio for quartered ' . $this_AR );
  }
  $halfHeight = $halfHeights[$this_AR];
  $translates = array (
    " translate($xShift,0) ",
    " translate(500,0) ",
    " translate($xShift,$halfHeight) ",
    " translate(500,$halfHeight) ",
  );

  return makequarters( $children, $child_ARs, $child_Scales, $translates );
}

function makequartered6( $children, $this_AR ) {
  global $halfHeights, $messages;
  
  switch ( $this_AR ) {
  case '10:12':
    $child_ARs = array ( '10:14', '10:14', '10:14', '10:20', '10:20', '10:20' );
    $child_Scales = array ( '0.387,0.417', '0.387,0.417', '0.387,0.417', '0.667,0.7', '0.667,0.7', '0.667,0.7' );
    $xShift0 = 0;
    $xShift1 = 333;
    $xShift2 = 666;
    break; // TODO finish this function!
  case '10:10':
    $child_ARs = array ( '10:10', '10:10', '10:10', '10:10' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 0;
    break;         
  case '10:14':
    $child_ARs = array ( '10:14', '10:14', '10:14', '10:14' );
    $child_Scales = array ( '0.5834', '0.5834', '0.58333', '0.58333' );
    $xShift = 70;
    break;  
  case '10:20':
    $child_ARs = array ( '10:20', '10:20', '10:20', '10:20' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 250;
    break;
  case '10:24':
    $child_ARs = array ( '10:24', '10:24', '10:24', '10:24' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 250;
    break;         
  case '10:28':
    $child_ARs = array ( '10:28', '10:28', '10:28', '10:28' );
    $child_Scales = array ( '0.5', '0.5', '0.58333', '0.58333' );
    $xShift = 285;
    break; 
  default:
    $messages->addMessage ( 'internal', 'Unknown aspect ratio for quartered ' . $this_AR );
  }
  $halfHeight = $halfHeights[$this_AR];
  $translates = array (
    " translate($xShift0,0) ",
    " translate($xShift1,0) ",
    " translate($xShift2,0) ",
    " translate($xShift0,$halfHeight) ",
    " translate($xShift1,$halfHeight) ",
    " translate($xShift2,$halfHeight) ",
  );

  return makequarters( $children, $child_ARs, $child_Scales, $translates );
  return $retval;
}

function makequartered8( $children, $this_AR ) {
  global $thirdHeights, $messages;
  
  switch ( $this_AR ) {
  case '10:12':
    $child_ARs = array ( '10:12', '10:12', '10:12', '10:12', '10:12', '10:12', '10:10', '10:10'  );
    $child_Scales = array ( '0.3333,0.3333', '0.3333,0.3333', '0.3333,0.3333', '0.3333,0.3333', '0.3333,0.3333', '0.3333,0.3333', '0.5,0.45', '0.5,0.45' );
    $xShift0 = 0;
    $xShift1 = 333;
    $xShift2 = 666;
    break; // TODO finish this function!
  case '10:10':
    $child_ARs = array ( '10:10', '10:10', '10:10', '10:10' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 0;
    break;         
  case '10:14':
    $child_ARs = array ( '10:14', '10:14', '10:14', '10:14' );
    $child_Scales = array ( '0.5834', '0.5834', '0.58333', '0.58333' );
    $xShift = 70;
    break;  
  case '10:20':
    $child_ARs = array ( '10:20', '10:20', '10:20', '10:20' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 250;
    break;
  case '10:24':
    $child_ARs = array ( '10:24', '10:24', '10:24', '10:24' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 250;
    break;         
  case '10:28':
    $child_ARs = array ( '10:28', '10:28', '10:28', '10:28' );
    $child_Scales = array ( '0.5', '0.5', '0.58333', '0.58333' );
    $xShift = 285;
    break; 
  default:
    $messages->addMessage ( 'internal', 'Unknown aspect ratio for quartered ' . $this_AR );
  }
  $thirdHeight = $thirdHeights[$this_AR];
  $twoThirds = $thirdHeight * 2;
  $translates = array (
    " translate($xShift0,0) ",
    " translate($xShift1,0) ",
    " translate($xShift2,0) ",
    " translate($xShift0,$thirdHeight) ",
    " translate($xShift1,$thirdHeight) ",
    " translate($xShift2,$thirdHeight) ",
    " translate(0,$twoThirds) ",
    " translate(500,$twoThirds) ",
  );

  return makequarters( $children, $child_ARs, $child_Scales, $translates );
  return $retval;
}

