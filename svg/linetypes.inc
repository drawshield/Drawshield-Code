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
   
$subArg = '';

function subSize($str,$size) {
  $S = $size/2;
  switch($str) {
  case '+S': return $S;
  case '-S': return $S * -1;
  case '+H': return $S * 1.414;
  case '-H': return $S * -1.414;
  case '+T': return $S * 0.707;
  case '-T': return $S * -0.707;
  case '+K': return $S * 0.414;
  case '-K': return $S * -0.414;
  case '+L': return $S * 2.414;
  case '-L': return $S * -2.414;
  case '+P': return $S * 0.577;
  case '-P': return $S * -0.577;
  case '+Q': return $S * 1.155;
  case '-Q': return $S * -1.155;
  case '+R': return $S * 1.732;
  case '-R': return $S * -1.732;
  case '+0': return 0;
  }
}

function meetEdges($endDir, $endPos, $startDir, $startPos, $size ) {
  global $subArg;
  $edgeMeets = array (
//                                 endPos = '-'         |                endPos = 'i'                     |            endPos = 'o'
    //        startPos->     'i'                'o'       |      '-'            'i'             'o'         |      '-'            'i'               'o'
    //  angle of      curr                                |                                                 |
    //  edges         next                                |                                                 |
    /* 315 deg 0 */   'X315-i0'=>'-S', 'X315-o0'=>'+S', 'X315i-0'=>'+H', 'X315ii0'=>'+K', 'X315io0'=>'+L', 'X315o-0'=>'-H', 'X315oi0'=>'-L', 'X315oo0'=>'-K',
    /* 315 deg 1 */   'X315-i1'=>'+H', 'X315-o1'=>'-H', 'X315i-1'=>'-S', 'X315ii1'=>'+K', 'X315io1'=>'-L', 'X315o-1'=>'+S', 'X315oi1'=>'+L', 'X315oo1'=>'-K',

    /* 270 deg 0 */   'X270-i0'=>'+S', 'X270-o0'=>'-S', 'X270i-0'=>'+0', 'X270ii0'=>'+S', 'X270io0'=>'-S', 'X270o-0'=>'+0', 'X270oi0'=>'+S', 'X270oo0'=>'-S',
    /* 270 deg 1 */   'X270-i1'=>'+0', 'X270-o1'=>'+0', 'X270i-1'=>'+S', 'X270ii1'=>'+S', 'X270io1'=>'+S', 'X270o-1'=>'-S', 'X270oi1'=>'-S', 'X270oo1'=>'-S',

    /* 135 deg 0 */   'X135-i0'=>'-H', 'X135-o0'=>'+H', 'X135i-0'=>'-S', 'X135ii0'=>'-L', 'X135io0'=>'+K', 'X135o-0'=>'+S', 'X135oi0'=>'-K', 'X135oo0'=>'+L',
    /* 135 deg 1 */   'X135-i1'=>'-S', 'X135-o1'=>'+S', 'X135i-1'=>'-H', 'X135ii1'=>'-L', 'X135io1'=>'-K', 'X135o-1'=>'+H', 'X135oi1'=>'+K', 'X135oo1'=>'+L',

    /* 180 deg n/a */

    /* 225 deg 0 */   'X225-i0'=>'+S', 'X225-o0'=>'-S', 'X225i-0'=>'+H', 'X225ii0'=>'+L', 'X225io0'=>'-K', 'X225o-0'=>'-H', 'X225oi0'=>'-K', 'X225oo0'=>'-L',
    /* 225 deg 1 */   'X225-i1'=>'+H', 'X225-o1'=>'-H', 'X225i-1'=>'+S', 'X225ii1'=>'+L', 'X225io1'=>'-K', 'X225o-1'=>'-S', 'X225oi1'=>'+K', 'X225oo1'=>'-L',

    /*  90 deg 0 */    'X90-i0'=>'-S',  'X90-o0'=>'+S',  'X90i-0'=>'+0',  'X90ii0'=>'-S',  'X90io0'=>'+S',  'X90o-0'=>'+0',  'X90oi0'=>'-S',  'X90oo0'=>'+S',
    /*  90 deg 1 */    'X90-i1'=>'+0',  'X90-o1'=>'+0',  'X90i-1'=>'-S',  'X90ii1'=>'-S',  'X90io1'=>'-S',  'X90o-1'=>'+S',  'X90oi1'=>'+S',  'X90oo1'=>'+S',

    /*  45 deg 0 */    'X45-i0'=>'-H',  'X45-o0'=>'+H',  'X45i-0'=>'+S',  'X45ii0'=>'-K',  'X45io0'=>'+L',  'X45o-0'=>'-S',  'X45oi0'=>'-L',  'X45oo0'=>'+K',
    /*  45 deg 1 */    'X45-i1'=>'+S',  'X45-o1'=>'-S',  'X45i-1'=>'-H',  'X45ii1'=>'-K',  'X45io1'=>'-L',  'X45o-1'=>'+H',  'X45oi1'=>'+L',  'X45oo1'=>'+K',

    /* 0 deg */        'XAo-' => 'v{S*0.5}', 'XAi-' => 'v-{S*0.5}', 'XBo-' => 'l-{S*0.353},{S*0.353}', 'XBi-' => 'l{S*0.353},-{S*0.353}',
    /* 0 deg */        'XCo-' => 'h-{S*0.5}', 'XCi-' => 'h{S*0.5}', 'XDo-' => 'l-{S*0.353},-{S*0.353}', 'XDi-' => 'l{S*0.353},{S*0.353}',
    /* 0 deg */        'XEo-' => 'v-{S*0.5}', 'XEi-' => 'v{S*0.5}', 'XFo-' => 'l{S*0.353},-{S*0.353}', 'XFi-' => 'l-{S*0.353},{S*0.353}',
    /* 0 deg */        'XGo-' => 'h{S*0.5}', 'XGi-' => 'h-{S*0.5}', 'XHo-' => 'l{S*0.353},{S*0.353}', 'XHi-' => 'l-{S*0.353},-{S*0.353}',

    /* 300 deg 0 */   'X300-i0'=>'+P', 'X300-o0'=>'-P', 'X300i-0'=>'+Q', 'X300ii0'=>'+P', 'X300io0'=>'+P', 'X300o-0'=>'+Q', 'X300oi0'=>'-P', 'X300oo0'=>'-P',
    /* 300 deg 0 */   'X300-i1'=>'+Q', 'X300-o1'=>'-Q', 'X300i-1'=>'+R', 'X300ii1'=>'+P', 'X300io1'=>'-P', 'X300o-1'=>'+P', 'X300oi1'=>'+P', 'X300oo1'=>'-P',

    /* 120 deg 0 */   'X120-i0'=>'-P', 'X120-o0'=>'+P', 'X120i-0'=>'+Q', 'X120ii0'=>'-R', 'X120io0'=>'-P', 'X120o-0'=>'+Q', 'X120oi0'=>'+P', 'X120oo0'=>'+R',
    /* 120 deg 0 */   'X120-i1'=>'-Q', 'X120-o1'=>'+Q', 'X120i-1'=>'+R', 'X120ii1'=>'-R', 'X120io1'=>'+P', 'X120o-1'=>'+P', 'X120oi1'=>'-P', 'X120oo1'=>'+R',

    /* 240 deg 0 */   'X240-i0'=>'+P', 'X240-o0'=>'-P', 'X240i-0'=>'-Q', 'X240ii0'=>'+R', 'X240io0'=>'-R', 'X240o-0'=>'+Q', 'X240oi0'=>'+R', 'X240oo0'=>'-R',
    /* 240 deg 0 */   'X240-i1'=>'-Q', 'X240-o1'=>'+Q', 'X240i-1'=>'+P', 'X240ii1'=>'+R', 'X240io1'=>'+R', 'X240o-1'=>'-R', 'X240oi1'=>'-R', 'X240oo1'=>'-R',

    /*  60 deg 0 */    'X60-i0'=>'+P',  'X60-o0'=>'-P',  'X60i-0'=>'-Q',  'X60ii0'=>'-P',  'X60io0'=>'-R',  'X60o-0'=>'+Q',  'X60oi0'=>'+R',  'X60oo0'=>'+P',
    /*  60 deg 0 */    'X60-i1'=>'-Q',  'X60-o1'=>'+Q',  'X60i-1'=>'+P',  'X60ii1'=>'-P',  'X60io1'=>'+R',  'X60o-1'=>'-P',  'X60oi1'=>'-R',  'X60oo1'=>'+P',
  );

  // Calculate the adjustment to the length of the edge to take account of whether they end in the outside, inside or middle
  if ( $endPos == '-' and $startPos == '-' ) return array (0,0); // No action if both are in middle
  $endDir = strtoupper($endDir);
  $startDir = strtoupper($startDir);
  // We are calculating the CLOCKWISE angle through which we would turn to be on the new "heading"
  $dirAngles = array ( 'A' => 0, 'B' => 45, 'C' => 90, 'D' => 135, 'E' => 180, 'F' => 225, 'G' => 270, 'H' => 315 );
  $angle = $dirAngles[$startDir] - $dirAngles[$endDir];
  if ( $angle < 0 ) $angle += 360;
  if ( $endDir == $startDir ) { // For straight connections we can't just adjust the length but need to insert a move
    $index = 'X' . $startDir . $endPos . $startPos;
    if ( !array_key_exists($index, $edgeMeets)) return array (0,0); // we have only done some of the combinations
    $subArg = $size;
    // Replace any references to size (as this is fixed for this path)
    $retval[2] = preg_replace_callback('/{S.*?}/', 'subFunc', $edgeMeets[$index]);
    $retval[0] = 0;
    $retval[1] = 0;
  } else {
    $index = 'X' . $angle . $endPos . $startPos ; // From end of one line to start of next
    if ( !array_key_exists($index . '0', $edgeMeets)) return array (0,0);
    $retval[0] = subSize($edgeMeets[$index . '0'], $size);
    $retval[1] = subSize($edgeMeets[$index . '1'], $size);
  }
  return $retval;
}

function makeOffset ( $dir, $disp, $io, $size ) {
  if ( $disp == 0 and $io == '-' ) return array(0,0);
  $x = 0; $y = 0;
  $size /= 2;
  $dispH = $disp * 0.707;
  $dispT = $disp * 0.866;
  $dispU = $disp * 0.5;
  $sizeH = $size * 0.707;
  $sizeT = $size * 0.866;
  $sizeU = $size * 0.5;
  switch($io) {
    case 'o': $ioF = -1;
              break;
    case 'i': $ioF = 1;
              break;
    case '-': $ioF = 0;
              break;
  }
  switch(strtoupper($dir)){
    case 'A': $x -= $disp; $y += $size * $ioF;
              break;
    case 'B': $x -= $dispH; $y -= $dispH; $x -= $sizeH * $ioF; $y += $sizeH * $ioF;
              break;
    case 'C': $y -= $disp; $x -= $size * $ioF;
              break;
    case 'D': $x += $dispH; $y -= $dispH; $x -= $sizeH * $ioF; $y -= $sizeH * $ioF;
              break;
    case 'E': $x += $disp; $y -= $size * $ioF;
              break;
    case 'F': $x += $dispH; $y += $dispH; $x += $sizeH * $ioF; $y -= $sizeH * $ioF;
              break;
    case 'G': $y += $disp; $x += $size * $ioF;
              break;
    case 'H': $x -= $dispH; $y += $dispH; $x += $sizeH * $ioF; $y += $sizeH * $ioF;
              break;
  }
  return array( $x, $y );
}

function subFunc( $match ) {
  global $subArg;

  if ( $match[0]{2} == '}' ) {
    return $subArg;
  } else {
    $val = floatVal(substr($match[0],3));
    switch ( $match[0]{2} ) {
      case '*': return sprintf("%.3f", ($subArg * $val));
      case '/': return sprintf("%.3f", ($subArg / $val));
    }
  }
  return $match; // should not happen
}

// Given a shapeSpec, a lineType and a feature size, return the appropriate
// SVG movements to create the path using the linetype. This is the bit
// inside the 'd' parameter of an SVG path element
function makePath ( $shapeSpec, $lineType, $size = 60 ) {
  if ( !$lineType ) $lineType = 'none';
  $retval = '';
  $matches = preg_split('/   */', $shapeSpec);
  foreach ( $matches as $match ) {
    $retval .= makePath2(  $match, $lineType, $size );
  }
  return $retval;
}

function dir2angle ( $dir ) {
  $rotations = array ( 'A' => 0, 'B' => 45, 'C' => 90, 'D' => 135, 'E' => 180, 'F' => 225, 'G' => 270, 'H' => 315 );
  
  return $rotations[strtoupper($dir)];
}

function mirrorAction ( $angle, $condition ) {
  $retval = false;
  switch ( $condition ) {
    case '90-270':
      if (( $angle > 90 ) and ($angle <= 270))
        $retval = true;
      break;
    case '45-135&225-315':
      if ((( $angle > 45) and ($angle <=135)) or (($angle>225) and ($angle <= 315)))
        $retval = true;
      break;
    case '90-180&270-360':
      if ((( $angle > 90) and ($angle <=180)) or (($angle>270) and ($angle <= 360)))
         $retval = true;
      break;
  }
  return $retval;
}

function makePath2 ( $shapeSpec, $lineType, $size = 60 ) {
  global $subArg;

  $lineSpecs = array (
    'angled' => '0:100:i:o:h50v-{S}h50:O1',
    'arched' => '1:{D}:i:i:q {D/2},-{S*2} {D},0:R1',
    'battled-counter' => '*:{S*4}:i:i:h{S}v-{S}h{S*2}v{S}h{S}:R1',
    'battled-brettesse' => '*:{S*4}:i:i:h{S}v-{S}h{S*2}v{S}h{S}',
    'battled-embattled' => '*:{S*4}:i:i:l{S*0.34},0 0,-{S/3} {S*0.66},0 0,-{S/3} {S*0.67},0 0,-{S/3} {S*0.66},0 0,{S/3} {S*0.67},0 0,{S/3} {S*0.66},0 0,{S/3} {S*.34},0:U1',
    'bevilled' => '0:{S}:i:o:h{S}l-{S},-{S}h{S}:O1',
    // 'bevilled' => '0:{S}:o:i:h{S}l-{S},{S}h{S}:O1',
    'dancetty' => '*:{S*3}:i:i:l{S*1.5},-{S} {S*1.5},{S}:R1',
    'dancetty-floretty' => '*:160:-:-:l32.7,-52.1 c 0,0 -11.4,-7.2 -30,14.3 -3.371,-32.4 21.5,-30.1 32.2,-22.2 10.7,7.8 -29.84,-18 5.4,-43 35.3,25 -5.2,50.8 5.5,43 10.7,-7.9 35.6,-10.2 32.2,22.2 -18.6,-21.5 -30,-14.3 -30,-14.3 21.6,34.8 43.1,69.6 65,104.2 0,0 -12,7.2 -30.3,-14.3 -3.4,32.4 21.3,30.1 32.3,22.2 11,-7.8 -29.9,18 5,43 36,-25 -5,-50.8 6,-43 11,7.9 35,10.2 32,-22.2 -19,21.5 -30,14.3 -30,14.3 l32,-52.1',
    'double-arched' => '2:{D/2}:i:i:q {D/4},-{S*2} {D/2},0:R1',
    'dovetailed' => '*:{S*2}:i:i:l{S*0.95},0 -{S*0.9},-{S} {S*1.9},0 -{S*0.9},{S} {S*0.95},0',
    'embattled-arrondi' => '*:{S*4}:i:i:h{S}c0,-{S} {S*2},-{S} {S*2},0h{S}:U1',
    'embattled' => '*:{S*4}:i:i:h{S}v-{S}h{S*2}v{S}h{S}:U1',
    'engrailed' => '*:{S*2}:o:o:a{S*2},{S*5},0,0,0 {S*2},0',
    'escartelly' => '0:{S*2}:i:i:v-{S}h{S*2}v{S}:R1',
    'indented' => '*:{S*2}:-:-:l{S/2},-{S/2} {S},{S} {S/2},-{S/2}',
    'invected' => '*:{S*2}:i:i:a{S*2},{S*5},0,0,1,{S*2},0',
    'nebuly' => '*:{S*4}:-:-:a{S*0.95},{S/2},0,1,0,{S*0.95},0a{S*0.95},{S/2},0,1,1,{S*0.95},0a{S*0.95},{S/2},0,1,0,{S*0.95},0a{S*0.95},{S/2},0,1,1,{S*0.95},0',
    'none' => '0:{D}:-:-:h{D}', // dummy values, not actually used
    'nowy' => '0:{S*3}:i:i:q{S*1.5},-{S*2} {S*3},0:R1',
    'potenty' => '*:{S*2}:-:-:h-{S/2}v-{S/2}h{S*1.5}v{S/2}h-{S/2},v{S/2}h{S*1.5}v-{S/2}',
    'raguly' => '*:{S*2}:i:i:l{S*0.5},0 -{S*0.2},-{S} {S},0 {S*0.2},{S} {S*0.5},0',
    'rayonny' => '*:{S}:o:o:a{S/4},{S*0.4}-30,0,0,{S/4},{S/2}a{S/4},{S*0.4}-30,0,1,{S/4},{S/2}a{S/4},{S*0.4},30,0,0,{S/4},-{S/2}a{S/4},{S*0.4},30,0,1,{S/4},-{S/2}',
    'urdy' => '*:{S}:-:-:v{S/4}l{S/4},{S/4} l{S/4},-{S/4}v-{S/2}l{S/4},-{S/4} l{S/4},{S/4}v{S/4}',
    'wavy' => '*:{S*3}:-:-:q{S*0.75},-{S} {S*1.5},0 q{S*0.75},{S} {S*1.5},0',
    'twigged' => '*:{S*1.5}:-:-:
    l-{S/4},-{S/4}
    c-{S/8},-{S/8} {S/8},-{S*0.375} {S/4},-{S/4}
    l{S*0.198},{S*0.198}
    l0,-{S*0.544}
    c0,-{S*0.177} {S*0.354},-{S*0.177} {S*0.354},0
    l0,{S*0.544}
    l{S*0.198},-{S*0.198}
    c{S/8},-{S/8} {S*0.375},{S*0.177} {S/4},{S/4}
    l-{S/2},{S/2}
    c-{S/8},{S/8} {S/8},{S*0.375} {S/4},{S/4}
    l{S*0.198},-{S*0.198}
    l0,{S*0.544}
    c0,{S*0.177} {S*0.354},{S*0.177} {S*0.354},0
    l0,-{S*0.544}
    l{S*0.198},{S*0.198}
    c{S/8},{S/8} {S*0.375},-{S/8} {S/4},-{S/4}
    l-{S/4},-{S/4}
    ',
  );

  if ( $shapeSpec == '' ) return '';
  $close_path = true;
  $retval = '';
  $Xstart = 0;
  $Ystart = 0;
  $whenRotated = 'overlay'; // Flag for linetype, result when rotated 180 degreees
  $doOffset = true;
  $upperOnly = false;
  $mirrorConds = array (
    'offset' => 'none',
    'reflected' => '90-270',
    'overlay' => 'none',
  );
  
  // Get the line specification
  if ($lineType == null or  ! (array_key_exists($lineType, $lineSpecs)) ) $lineType = 'none';
  $lineSpec= $lineSpecs[$lineType];
  // Size is fixed for this path, so can substitute now
  $subArg = $size;
  $lineSpec = preg_replace_callback('/{S.*?}/', 'subFunc', $lineSpec);
  list($repeat,$length,$defaultStart,$defaultEnd,$spec,$options) = explode(':',$lineSpec . ': ' ); // options are optional(!)
  // Process any flags for this lineSpec
  foreach ( str_split($options,2) as $option ) {
    switch ( $option{0} ) {
      case 'R':
        $whenRotated = 'reflected';
        break;
      case 'O':
        $whenRotated = 'offset';
        break;
      case 'U':
        $upperOnly = true;
        break;
    }
  }
  
  // Chop up the shapeSpec into parts, extracting X,Y and other flags
  if ( preg_match_all ( '/[A-Za-z][0-9\.\-,<>]*/', $shapeSpec, $edgeSpecs ) === false ) return '';
  $edges = array();
  foreach ($edgeSpecs[0] as $edgeSpec) {
    $doMirror = false;
    $dir = $edgeSpec{0};
    $dist = floatval(substr($edgeSpec,1));
    if ( $dir == 'X' ) {
      $Xstart = $dist;
      continue;
    } elseif ( $dir == 'Y' ) {
      $Ystart = $dist;
      continue;
    } elseif ( $dir == 'Z' ) {
      $close_path = ( $dist != 0 );
      continue;
    } elseif ( $dir == 'Q' ) { // mirroring for quartering
      $mirrorConds['reflected'] = '45-135&225-315';
      if ( ($defaultStart == $defaultEnd) and ($defaultStart != '-')) 
        $whenRotated = 'reflected';
      continue;
    } elseif ( $dir == 'V' ) { // mirroring for chevron like things
      $mirrorConds['offset'] = '90-180&270-360';
      $mirrorConds['reflected'] = '90-180&270-360';
      if ( $defaultStart == '-' and $defaultEnd == '-') 
        $whenRotated = 'reflected';
      continue;
    } elseif ( $dir == 'U' ) {
      $upperOnly = ($dist == 0);
      continue;
    } elseif ( $dir == 'O' ) {
      $doOffset = false;
      continue;
    }
    $angle = dir2angle($dir);
    $start = $defaultStart;
    $end = $defaultEnd;
    $doMirror = mirrorAction ( $angle, $mirrorConds[$whenRotated] );
    if ( $doMirror ) {
      if ( $start != '-' ) $start = ($start == 'o') ? 'i' : 'o';
      if ( $end != '-' ) $end = ($end == 'o') ? 'i' : 'o';
    }
    if ($lineType == 'none' or 
        ctype_lower($dir) or 
        (($upperOnly == true ) and ($angle >45) and ($angle < 315)) or
        $length > $dist ) { // straight, or cannot fit this path into distance
      $dir = strtolower($dir);
      $start = '-';
      $end = '-';
    }

    $edges[] = compact('dir','dist','flag','start','end','doMirror');
  }
  // We now have all the edge directions and lengths in an array, we calculate any
  // adjustments to the lengths to take account of how they meet (inner to outer or whatever)
  $numEdges = count($edges);
  for ( $i = 0; $i < $numEdges; $i++ ) {
    $next = ($i+1)%$numEdges;
    $adjust = meetEdges($edges[$i]['dir'], $edges[$i]['end'], $edges[$next]['dir'], $edges[$next]['start'], $size );
    $edges[$i]['dist'] += $adjust[0];
    $edges[$next]['dist'] += $adjust[1];
  }
  
  // Start building the SVG path
  // Move to the offset
  $retval .= "M$Xstart,$Ystart";
  // The last adjustment from above also gives us the offset from the current position in the SVG path
  // Calculate the offset for the first segment
  if ( $doOffset ) {
    $offs = makeOffset ( $edges[0]['dir'], $adjust[1], $edges[0]['start'], $size );
    if ( $offs[0] != 0 or $offs[1] != 0 ) $retval .= 'm' . $offs[0] . ',' . $offs[1];
  }
  
  // We can now go along each edge, adding to the path
  for ( $i = 0; $i < $numEdges; $i++ ) {
    extract($edges[$i]); // creates $dir and $dist etc.
    $angle = dir2angle($dir);
    if ( ctype_lower($dir) ) { // Just a straight line, no need to look up lineSpec
      $retval .= lineStraight($angle,$dist);
      continue;
    }
    // We know the distance of this edge, so we can substitute any 'D's
    $subArg = $dist;
    $path = preg_replace_callback('/{D.*?}/', 'subFunc', $spec);
    $len = preg_replace_callback('/{D.*?}/', 'subFunc', $length);
    $angle = dir2angle($dir);
    if ( $doMirror )
      $path = mirrorPath($path,'x');
    // Stretch if required to fit
    if ( $repeat == '*' ) {
      $reps = (int)($dist/$len);
      $left = $dist - ($reps * $len);
      // if space left, expand the pattern to fit exactly
      if ( $left > 0 ) {
        $path = scalePath ( $path, (($dist/$reps)/$len), 1 );
      }
    }
    // If necessary, rotate the path
    $path = rotatePath($path, $angle);
    switch ( $repeat ) { // type of line
    case '*': // insert as many times as will fit
      $retval .= str_repeat($path,$reps);
      break;
    case '0': // insert once, at a fixed size
      $left = $dist - $len;
      if ( $left > 0 ) $retval .= lineStraight($angle,$left/2);
      $retval .= $path;
      if ( $left > 0 ) $retval .= lineStraight($angle,$left/2);
      break;
    default: // repeat this number of times
      $retval .= str_repeat($path, $repeat);
      break;
    }
  }
  if ( $close_path ) $retval .= 'z';
  return $retval;
}


// This maps lines and curves to their opposite directions, for spec reversal
$toReverse = array ( 'A' => 'E', 'B' => 'F', 'C' => 'G', 'D' => 'H', 'E' => 'A', 'F' => 'B', 'G' => 'C', 'H' => 'D', 
                     'a' => 'e', 'b' => 'f', 'c' => 'g', 'd' => 'h', 'e' => 'a', 'f' => 'b', 'g' => 'c', 'h' => 'd', );

Function reverseSpec ( $shapeSpec ) {
  global $toReverse;

  $retval = '';
  $matches = array(); // Chop the shape specification into "direction+distance" components
  if ( preg_match_all ( '/[A-Za-z]-?[0-9\.\-<>,]*/', $shapeSpec, $matches ) === false ) return '';
  $shape = array();
  foreach ( array_reverse($matches[0]) as $match ) {
    // Get direction and distance
    $dir = $match{0};
    $dist = substr($match,1);
    if ( strpos ( "XYZQVO", $dir ) !== false ) {
      $retval .= $match;
    } else {
      $retval .= $toReverse[$dir] . $dist;
    }
  }
  return $retval;
}

// Given a shapeSpec, return another shapeSpec that is exactly parallel to the first but
// inside ($io = 'i') or outside ($io = 'o') and offset by $size
function parallelSpec ( $shapeSpec, $io, $size ) {

  $retval = '';
  $matches = array(); // Chop the shape specification into "direction+distance" components
  if ( preg_match_all ( '/[A-Za-z]-?[0-9\.\-<>,]*/', $shapeSpec, $matches ) === false ) return '';
  $shape = array();
  $count = 0;
  $Xoff = 0; $Yoff = 0;
  $flags = '';
  foreach ( $matches[0] as $match ) {
    // Get direction and distance
    $dir = $match{0};
    $dist = floatval(substr($match,1));
    // Get the line spec for this direction
    if ( $dir == 'X' ) {
      $Xoff = $dist;
      continue;
    } elseif ( $dir == 'Y' ) {
      $Yoff = $dist;
      continue;
    } elseif ( strpos("ZQVO", $dir) !== false ) {
      $flags .= $dir . $dist;
      continue;
    }
    // Split the line spec into its parts
    $shape[$count++] = compact('dir','dist');
  }
  $numSegments = count($shape);
  // Adjust interior edges to meet
  $prevDir = strtoupper($shape[($numSegments-1)]['dir']);
  for ( $i = 0; $i < $numSegments; $i++ ) {
    extract($shape[$i]); // creates $dir and $dist
    $next = ($i+1)%$numSegments;
    $nextDir = $shape[$next]['dir'];
    $adjust = meetEdges($dir, $io, $nextDir, $io, $size*2 );
    $shape[$i]['dist'] += $adjust[0];
    $shape[$next]['dist'] += $adjust[1];
    $prevDir = strtoupper($dir);
  }
  // The last adjustment also gives us the offset from the current position in the SVG path
  // Calculate the offset for the first segment
  $offs = makeOffset ( $shape[0]['dir'], $adjust[1], $io, $size*2 );
  $retval .= 'X' . ($Xoff + $offs[0]) . 'Y' . ($Yoff + $offs[1]);

  // Create the inner edge
  for ( $i = 0; $i < $numSegments; $i++ ) {
    extract($shape[$i]); // creates $dir and $dist
    $retval .= $dir . $dist;
  }
  return $flags . $retval;
}
