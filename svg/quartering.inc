<?php
require_once('size.inc');

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
    global $options;
    $saveShape = $options['shape'];
    $saveHeight = $options['flagHeight'] ?? null;
    $options['shape'] = 'quarter';
    $options['flagHeight'] = 1200;
	$retval = '';
	$subWidth = 1000 / $across;
	$subHeight = 1200 / $down;
	$aspect = (($subWidth / $subHeight) > 0.84) ? 'square' : 'shield';
	$widthScale = 1 / $across;
	$heightScale = 1 / $down;
	$yPos = 0;
	$childCount = 0;
    $grid = "<g stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\">\n";
	for ( $aCount = 0; $aCount < $across; $aCount++ ) {
		$xPos = 0;
        $grid .= "<path d=\"M 0 " . ($yPos - 1) . " h 1000\"/>\n";
		for ( $dCount = 0; $dCount < $down; $dCount++ ) {
			$retval .= make1Quarter($children[$childCount++], $widthScale, $heightScale, $xPos, $yPos, $aspect);
            if ($yPos == 0) { // first time only
                $grid .= "<path d=\"M " . ($xPos - 1) . " 0 v 1200\"/>\n";
            }
			$xPos += $subWidth;
		}
		$yPos += $subHeight;
    }
    $grid .= "</g>\n";
    $retval .= $grid;
    $options['shape'] = $saveShape;
    if ( $saveHeight )
        $options['flagHeight'] = $saveHeight;
	return $retval;
}

function makequarters( DOMNodeList $children, $child_ARs, $child_Scales, $translates, $this_AR) {
    global $scale_factor, $options;

    $retval = '';
    $saveShape = $options['shape'];
    $saveHeight = $options['flagHeight'] ?? null;
    $ymult = get_size_multiplier()[1];

    if ( $options['shape'] != 'flag' )
        $height_ratio = calculateAR($this_AR);
    else
        $height_ratio = 1;
    $options['shape'] = 'quarter';

    $scale_factor_old = $scale_factor;
    foreach ( $children as $kid ) {
        $index = $kid->getAttribute('index');
        $index -= 1; // Arrays are zero based

        // error check
        if (!isset($child_ARs[$index])) {
            error_log("bad quarter index in " . $options['blazon'] );
            continue;
        }

        region('push','Q' . $index ); // Can lose this soon I hope...

        $child_AR = $child_ARs[$index];

        $scale = $child_Scales[$index];
        $scale_factor = $scale; // Set the global (used in furs and treatments)
        if ( $scale != '' )
        {
            if ( strpos($scale, ",") !== false )
                list($xscale, $yscale) = explode(",", $scale);
            else
                $xscale = $yscale = $scale;
            $yscale *= $ymult;
            $scale = $xscale;
            $svg_scale = " scale($scale) ";
        }
        else
        {
            $xscale = $yscale = $scale;
            $svg_scale = " scale($scale) ";
        }
        $width = 1000;
        $height = 1000 / $height_ratio * $yscale / $xscale;
        $clipdef = add_def( 'clipPath', "<path d=\"M0,0 h$width v$height h-$width v-$height Z\" />");

        $options['flagHeight'] = $height;

        $translate = $translates[$index];
        $transform = " transform=\" $translate $svg_scale \" ";
        $retval .=
            "<g clip-path='url(#$clipdef)' $transform _w='$width' _h='$height' >"
            . makeshield( $kid, $child_AR )
            . "</g>\n";
        region('pop');
    }
    $scale_factor = $scale_factor_old;
    $options['shape'] = $saveShape;
    if ( $saveHeight )
        $options['flagHeight'] = $saveHeight;
    $retval .= add_def();
    return $retval;
}

function makequartered5( $children, $this_AR ) {
  global $halfHeights, $messages, $options;

  // Default values ( for 10:12 )
  $child_ARs = array ( '10:14', '10:14', '10:14', '10:14', '10:14' );
  $child_Scales = array ( '0.333,0.417', '0.333,0.417', '0.333,0.417', '0.5,0.5843', '0.5,0.5843' );
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
  if ( $options["shape"] == "flag" )
  {
    $child_Scales = array ( '0.387,0.417', '0.387,0.417', '0.387,0.417', '0.5843,0.417', '0.5843,0.417' );
    $halfHeight *= get_size_multiplier()[1];
  }
  else if ( $options["shape"] == "quarter" )
  {
    $halfHeight *= get_size_multiplier()[1];
  }

  $translates = array (
    " translate(0,0) ",
    " translate(333,0) ",
    " translate(666,0) ",
    " translate(0,$halfHeight) ",
    " translate(500,$halfHeight) ",
  );
  $grid = "<g stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\">\n";
  $grid .= "<path d=\"M 332 0 v " . $halfHeight . "\" />\n";
  $grid .= "<path d=\"M 665 0 v " . $halfHeight . "\" />\n";
  $grid .= "<path d=\"M 0 " . ($halfHeight - 1) . " h 1000\" />\n";
  $grid .= "<path d=\"M 500 " . $halfHeight . " v " . $halfHeight . "\" />\n";
  $grid .= "</g>\n";

  return makequarters( $children, $child_ARs, $child_Scales, $translates, $this_AR ) . $grid;
}

function makequartered4( DOMNodeList $children, $this_AR ) {
  global $halfHeights, $messages, $options;
  // Default values ( for 10:12 )
  $child_ARs = array ( '10:12', '10:12', '10:14', '10:14' );
  $child_Scales = array ( '0.5,0.417', '0.5,0.417', '0.5,0.5843', '0.5,0.5843' );
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
    break;
  case '10:20':
    $child_ARs = array ( '10:20', '10:20', '10:20', '10:20' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    break;
  case '10:24':
    $child_ARs = array ( '10:24', '10:24', '10:24', '10:24' );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    break;
  case '10:28':
    $child_ARs = array ( '10:28', '10:28', '10:28', '10:28' );
    $child_Scales = array ( '0.5', '0.5', '0.5834', '0.5834' );
    break;
  default:
    $messages->addMessage ( 'internal', 'Unknown aspect ratio for quartered ' . $this_AR );
  }

  $halfHeight = $halfHeights[$this_AR];
  if ( $options["shape"] == "flag" )
  {
    $child_ARs = array ( $this_AR, $this_AR, $this_AR, $this_AR );
    $child_Scales = array ( '0.5', '0.5', '0.5', '0.5' );
    $xShift = 0;
    $halfHeight *= get_size_multiplier()[1];
  }
  else if ( $options["shape"] == "quarter" )
  {
    $halfHeight *= get_size_multiplier()[1];
  }


  $translates = array (
    " translate($xShift,0) ",
    " translate(500,0) ",
    " translate($xShift,$halfHeight) ",
    " translate(500,$halfHeight) ",
  );

  $grid = "<g stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\">\n";
  $grid .= "<path d=\"M 499 0 v 1200\" />\n";
  $grid .= "<path d=\"M 0 " . ($halfHeight - 1) . " h 1000\" />\n";
  $grid .= "</g>\n";

  return makequarters( $children, $child_ARs, $child_Scales, $translates, $this_AR ) . $grid;
}

function makequartered6( $children, $this_AR ) {
  global $halfHeights, $messages, $options;

  $xShift0 = 0; // fudge until finished properly
  $xShift1 = 333;
  $xShift2 = 666;
  switch ( $this_AR ) {
  case '10:12':
    $child_ARs = array ( '10:14', '10:14', '10:14', '10:20', '10:20', '10:20' );
    $child_Scales = array ( '0.333,0.417', '0.334,0.417', '0.333,0.417', '0.333,0.583', '0.334,0.583', '0.333,0.583' );
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
  if ( $options["shape"] == "flag" )
  {
    $halfHeight *= get_size_multiplier()[1];
    $child_Scales = array ( '0.3333,0.5', '0.3333,0.5', '0.3333,0.5', '0.3333,0.5', '0.3333,0.5', '0.3333,0.5' );
  }
  else if ( $options["shape"] == "quarter" )
  {
    $halfHeight *= get_size_multiplier()[1];
  }
  $translates = array (
    " translate($xShift0,0) ",
    " translate($xShift1,0) ",
    " translate($xShift2,0) ",
    " translate($xShift0,$halfHeight) ",
    " translate($xShift1,$halfHeight) ",
    " translate($xShift2,$halfHeight) ",
  );

  $grid = "<g stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\">\n";
  $grid .= "<path d=\"M 332 0 v 1200\" />\n";
  $grid .= "<path d=\"M 665 0 v 1200\" />\n";
  $grid .= "<path d=\"M 0 " . ($halfHeight - 1) . " h 1000\" />\n";
  $grid .= "</g>\n";

  return makequarters( $children, $child_ARs, $child_Scales, $translates, $this_AR ) . $grid;
}

function makequartered8( $children, $this_AR ) {
  global $thirdHeights, $messages, $options;

  $thirdHeight = $thirdHeights[$this_AR];

  switch ( $this_AR ) {
  case '10:12':
    $child_ARs = array ( '10:12', '10:12', '10:12', '10:12', '10:12', '10:12', '10:10', '10:10'  );
    $child_Scales = array ( '0.3333,0.3', '0.3333,0.3', '0.3333,0.3', '0.3333,0.3', '0.3333,0.3', '0.3333,0.3', '0.5,0.4', '0.5,0.4' );
    $xShift0 = 0;
    $xShift1 = 333;
    $xShift2 = 666;
    $thirdHeight = 1200 * 0.3;
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

  if ( $options["shape"] == "flag" ) {
    $thirdHeight = get_size()[1] / 3;
    $child_Scales = array ( '0.3333,0.3333', '0.3333,0.3333', '0.3333,0.3333', '0.3333,0.3333', '0.3333,0.3333', '0.3333,0.3333', '0.5,0.3333', '0.5,0.3333' );
  }
  else if ( $options["shape"] == "quarter" )
  {
    $thirdHeight *= get_size_multiplier()[1];
  }

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

  $grid = "<g stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\">\n";
  $grid .= "<path d=\"M 332 0 v " . ($thirdHeight * 2) . "\" />\n";
  $grid .= "<path d=\"M 665 0 v " . ($thirdHeight * 2) . "\" />\n";
  $grid .= "<path d=\"M 0 " . ($thirdHeight - 1) . " h 1000\" />\n";
  $grid .= "<path d=\"M 0 " . (($thirdHeight * 2) - 1) . " h 1000\" />\n";
  $grid .= "<path d=\"M 500 " . ($thirdHeight * 2) . " v $thirdHeight\" />\n";
  $grid .= "</g>\n";
  return makequarters( $children, $child_ARs, $child_Scales, $translates, $this_AR ) . $grid;
}

