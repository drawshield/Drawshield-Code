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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
 */
   
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

  if ( substr($match[0],2,1) == '}' ) {
    return $subArg;
  } else {
    $val = floatVal(substr($match[0],3));
    switch ( substr($match[0],2,1) ) {
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
    'angled-upper' => '0:100:o:i:h50v{S}h50:O1',
    'arched' => '1:{D}:i:i:c {D/2},-{S*2} {D/2},-{S*2} {D},0:R1',
    'bowed' => '1:{D}:i:i:c {D/2},{S*2} {D/2},{S*2} {D},0:R1',
    'battled-ghibelline' => '*:{S*4}:i:i:h{S} v-{S*2} c0,0 {S},{S*0.37} {S},{S*0.9} 0,-{S*0.58} {S},-{S*0.90} {S},-{S*0.9} v{S*2} h{S}:U1',
    'embattled-arrondi' => '*:{S*4}:i:i:h{S} v-{S} c0,-{S} {S*2},-{S} {S*2},0 v{S} h{S}:U1',
    'cupolaed' => '*:{S*4}:i:i:h{S} v-{S} h{S/2} c0,-{S/2} {S},-{S/2} {S},0 h{S/2} v{S} h{S}:U1',
    'gabled' => '*:{S*4}:i:i:h{S} v-{S} h{S/2} l{S/2},-{S/2} l{S/2},{S/2} h{S/2} v{S} h{S}:U1',
    'battled-mine-dumps' => '*:{S*7}:i:i:h{S*1.5} l{S/2},{S} h{S*3} l{S/2},-{S} h{S*1.5}:U1',
    'battled-counter' => '*:{S*4}:i:i:h{S}v-{S}h{S*2}v{S}h{S}:R1',
    'battled-brettesse' => '*:{S*4}:i:i:h{S}v-{S}h{S*2}v{S}h{S}',
    'battled-embattled' => '*:{S*4}:i:i:l{S*0.34},0 0,-{S/3} {S*0.66},0 0,-{S/3} {S*0.67},0 0,-{S/3} {S*0.66},0 0,{S/3} {S*0.67},0 0,{S/3} {S*0.66},0 0,{S/3} {S*.34},0:U1',
    'bevilled' => '0:{S}:i:o:h{S}l-{S},-{S}h{S}:O1',
    'bevilled-upper' => '0:{S}:o:i:h{S}l-{S},{S}h{S}:O1',
    'thorny' => '*:{S*3}:-:-:c 0,-{S*1.08} -{S*0.32},-{S*1.08} -{S*0.32},-{S*1.08} h {S*1.5} c 0,0 {S*0.32},0 {S*0.32},{S*1.08} 0,{S*1.08} -{S*0.32},{S*1.08} -{S*0.32},{S*1.08} h {S*1.5} c 0,0 {S*0.32},0 {S*0.32},-{S*1.08} ',
    'indented-pommetty' => '*:{S*1.8}:-:-:l
{S*0.5},-{S*1.80}
c
-{S*0.10},0
-{S*0.20},-{S*0.10}
-{S*0.20},-{S*0.20}
0,-{S*0.10}
{S*0.10},-{S*0.20}
{S*0.20},-{S*0.20}
{S*0.10},0
{S*0.20},{S*0.10}
{S*0.20},{S*0.20}
0,{S*0.10}
-{S*0.10},{S*0.20}
-{S*0.20},{S*0.20}
l
{S*0.80},{S*3.60}
c
-{S*0.10},0
-{S*0.20},{S*0.10}
-{S*0.20},{S*0.20}
0,{S*0.10}
{S*0.10},{S*0.20}
{S*0.20},{S*0.20}
{S*0.10},0
{S*0.20},-{S*0.10}
{S*0.20},-{S*0.20}
0,-{S*0.10}
-{S*0.10},-{S*0.20}
-{S*0.20},-{S*0.20}
l
{S*0.5},-{S*1.80}',
    'trefle' => '*:{S*2.16}:-:-:c
{S*0.1974684},{S*0.0549}
{S*0.3983884},{S*0.1098}
{S*0.4820684},-{S*0.2125}
l
-{S*0.04647},-{S*0.0232}
c
-{S*0.05227},{S*0.0967}
-{S*0.15391},{S*0.1626}
-{S*0.2708},{S*0.1626}
-{S*0.170061},0
-{S*0.308},-{S*0.1392}
-{S*0.308},-{S*0.3107}
0,-{S*0.1717}
{S*0.137939},-{S*0.3108}
{S*0.308},-{S*0.3108}
{S*0.05318},0
{S*0.10309},{S*0.0136}
{S*0.14684},{S*0.0374}
l
{S*0.03085},-{S*0.021}
c
-{S*0.05572},-{S*0.0563}
-{S*0.0902},-{S*0.134}
-{S*0.0902},-{S*0.2198}
0,-{S*0.1716}
{S*0.13776},-{S*0.3106}
{S*0.30782},-{S*0.3106}
{S*0.17025},0
{S*0.30801},{S*0.139}
{S*0.30801},{S*0.3106}
0,{S*0.0913}
-{S*0.03884},{S*0.1731}
-{S*0.10091},{S*0.2299}
l
{S*0.03158},{S*0.0109}
c
{S*0.04374},-{S*0.0238}
{S*0.0824},-{S*0.0374}
{S*0.13556},-{S*0.0374}
{S*0.17006},0
{S*0.30796},{S*0.1391}
{S*0.30796},{S*0.3108}
0,{S*0.1715}
-{S*0.1379},{S*0.3107}
-{S*0.30796},{S*0.3107}
-{S*0.10634},0
-20,-{S*0.0545}
-{S*0.25535},-{S*0.137}
l
-{S*0.03866},{S*0.0183}
c
{S*0.18259},{S*0.5837}
{S*0.70417},-{S*0.1997}
{S*0.86427},{S*0.3955}
l
-{S*0.0448},{S*0.0062}
c
-{S*0.0558},-{S*0.0563}
-{S*0.1327},-{S*0.091}
-{S*0.2178},-{S*0.091}
-{S*0.1701},0
-{S*0.30796},{S*0.1391}
-{S*0.30796},{S*0.3108}
0,{S*0.1715}
{S*0.13786},{S*0.3105}
{S*0.30796},{S*0.3105}
{S*0.0531},0
{S*0.1031},-{S*0.0134}
{S*0.1468},-{S*0.0374}
l
{S*0.0044},{S*0.0018}
c
-{S*0.0685},{S*0.057}
-{S*0.1122},{S*0.1432}
-{S*0.1122},{S*0.2398}
0,{S*0.1715}
{S*0.1379},{S*0.3107}
{S*0.308},{S*0.3107}
{S*0.1701},0
{S*0.3081},-{S*0.1392}
{S*0.3081},-{S*0.3107}
0,-{S*0.0966}
-{S*0.0436},-{S*0.1828}
-{S*0.1123},-{S*0.2398}
l
{S*0.0132},-{S*0.0085}
c
{S*0.0464},{S*0.0285}
{S*0.1011},{S*0.0448}
{S*0.1595},{S*0.0448}
{S*0.1701},0
{S*0.308},-{S*0.139}
{S*0.308},-{S*0.3105}
0,-{S*0.1717}
-{S*0.1379},-{S*0.3108}
-{S*0.308},-{S*0.3108}
-{S*0.0849},0
-{S*0.1619},{S*0.0347}
-{S*0.2177},{S*0.091}
l
-{S*0.0443},{S*0.0027}
c
{S*0.0697},-{S*0.3222}
{S*0.270},-{S*0.2673}
{S*0.4738},-{S*0.2125}',
    'fir-tree-topped' => '*:{S*3}:-:-:l
{S*0.397022},-{S*0.6288}
-{S*0.69447},{S*0.2902}
{S*1.14526},-{S*1.6169}
{S*1.20623},{S*1.6169}
-{S*0.5931},-{S*0.2902}
{S*0.8217},{S*1.2574}
-{S*0.727},-{S*0.2902}
{S*0.9499},{S*1.2574}
{S*0.8892},-{S*1.2574}
-{S*0.621},{S*0.2902}
{S*0.4286},-{S*0.6286}',
    'liljakoro' => '*:{S*2}:-:-:c
-{S*0.000524},-{S*0.1511}
{S*0.0893876},-{S*0.2784}
{S*0.2493766},-{S*0.2768}
{S*0.016},{S*0.002}
{S*0.16161},{S*0.1682}
{S*0.31457},{S*0.112}
-{S*0.03798},-{S*0.4685}
-{S*0.43945},-{S*0.4418}
-{S*0.615358},-{S*0.2477}
-{S*0.054997},-{S*0.2281}
{S*0.109767},-{S*0.391}
{S*0.312168},-{S*0.3922}
{S*0.20242},-{S*0.002}
{S*0.24169},{S*0.1825}
{S*0.32832},{S*0.1631}
-{S*0.2617},-{S*0.5081}
-{S*0.22113},-{S*0.945}
{S*0.16809},-{S*1.3099}
{S*0.38919},{S*0.3649}
{S*0.43039},{S*0.8018}
{S*0.16858},{S*1.3099}
{S*0.08661},{S*0.0194}
{S*0.12591},-{S*0.1644}
{S*0.32841},-{S*0.1631}
{S*0.2023},{S*0.0019}
{S*0.3671},{S*0.1641}
{S*0.3121},{S*0.3922}
-{S*0.1758},-{S*0.1941}
-{S*0.57738},-{S*0.2208}
-{S*0.61535},{S*0.2477}
{S*0.15295},{S*0.0562}
{S*0.15465},-{S*0.1105}
{S*0.31455},-{S*0.112}
{S*0.016},-{S*0.0019}
{S*0.2499},{S*0.1257}
{S*0.2494},{S*0.2768}
-{S*0.0012},{S*0.1433}
-{S*0.1263},{S*0.2581}
-{S*0.2935},{S*0.2538}
-{S*0.1672},-{S*0.0046}
-{S*0.1217},-{S*0.1679}
-{S*0.26624},-{S*0.1439}
-{S*0.08877},{S*0.3546}
{S*0.26674},{S*0.5856}
{S*0.57344},{S*0.3659}
-{S*0.1887},{S*0.4147}
-{S*0.1255},{S*0.984}
{S*0.2213},{S*1.1611}
{S*0.3468},-{S*0.1771}
{S*0.4173},-{S*0.7464}
{S*0.2285},-{S*1.1611}
{S*0.3068},{S*0.2197}
{S*0.6623},-{S*0.0113}
{S*0.5735},-{S*0.3659}
-{S*0.1446},-{S*0.024}
-{S*0.099},{S*0.1393}
-{S*0.2664},{S*0.1439}
-{S*0.167},{S*0.0043}
-{S*0.2928},-{S*0.1105}
-{S*0.2934},-{S*0.2538}',
    'vallikoro' => '*:{S*2}:-:-:l-{S/4},-{S/4} {S*0.75},-{S} {S*0.75},{S} -{S/2},{S/2} {S*0.75},{S} {S*0.75},-{S} -{S/4},-{S/4}:O1',
    'hakulikoro' => '*:{S*1.5}:-:-:l-{S/8},-{S/2} {S/2},-{S/2} {S/2},{S/2} -{S/4},{S} {S/2},{S/2} {S/2},-{S/2} -{S/8},-{S/2}:O1',
    'ristikoro' => '*:{S*2}:-:-:v-{S/2}h{S/2}v-{S/2}h{S/2}v{S/2}h{S/2}v{S/2}h-{S/2}v{S/2}h{S/2}v{S/2}h{S/2}v-{S/2}h{S/2}v-{S/2}h-{S/2}:O1',
    'dancetty' => '*:{S*4}:i:i:l{S*2},-{S*2} {S*2},{S*2}:R1',
    'dancetty-floretty' => '*:160:-:-:l32.7,-52.1 c 0,0 -11.4,-7.2 -30,14.3 -3.371,-32.4 21.5,-30.1 32.2,-22.2 10.7,7.8 -29.84,-18 5.4,-43 35.3,25 -5.2,50.8 5.5,43 10.7,-7.9 35.6,-10.2 32.2,22.2 -18.6,-21.5 -30,-14.3 -30,-14.3 21.6,34.8 43.1,69.6 65,104.2 0,0 -12,7.2 -30.3,-14.3 -3.4,32.4 21.3,30.1 32.3,22.2 11,-7.8 -29.9,18 5,43 36,-25 -5,-50.8 6,-43 11,7.9 35,10.2 32,-22.2 -19,21.5 -30,14.3 -30,14.3 l32,-52.1',
    'double-arched' => '2:{D/2}:i:i:q {D/4},-{S*2} {D/2},0:R1',
    // 'dovetailed' => '*:{S*4}:i:i:l{S*1.9},0 -{S*1.8},-{S*2} {S*3.8},0 -{S*1.8},{S*2} {S*1.9},0',
    'dovetailed' => '*:{S*4}:i:i:l{S*1.5},0 -{S},-{S*2} {S*3},0 -{S},{S*2} {S*1.5},0',
    'embattled' => '*:{S*4}:i:i:h{S}v-{S}h{S*2}v{S}h{S}:U1',
//    'engrailed' => '*:{S*2}:o:o:a{S*2},{S*5},0,0,0 {S*2},0',
    'engrailed' => '*:{S*4}:o:o:a{S*4},{S*10},0,0,0 {S*4},0',
    'escartelly' => '0:{S*2}:i:i:v-{S}h{S*2}v{S}:R1',
//    'indented' => '*:{S*2}:-:-:l{S/2},-{S/2} {S},{S} {S/2},-{S/2}',
    'indented' => '*:{S*4}:-:-:l{S},-{S} {S*2},{S*2} {S},-{S}',
//    'invected' => '*:{S*2}:i:i:a{S*2},{S*5},0,0,1,{S*2},0',
    'invected' => '*:{S*4}:i:i:a{S*4},{S*10},0,0,1,{S*4},0',
    // 'nebuly' => '*:{S*3.5}:-:-:a{S*0.95},{S/2},0,1,0,{S*0.95},0a{S*0.95},{S/2},0,1,1,{S*0.95},0a{S*0.95},{S/2},0,1,0,{S*0.95},0a{S*0.95},{S/2},0,1,1,{S*0.95},0',
    'nebuly' => '*:{S*3}:i:i:c{S*0.4},0,{S*0.96},0,{S*0.96},-{S*0.36}c0,-{S*0.24},-{S*0.4},-{S*0.24},-{S*0.4},-{S*0.4}c0,-{S*0.24},{S*0.36},-{S*0.36},{S*0.96},-{S*0.36}c0,0,{S*0.96},0,{S*0.96},{S*0.36}c0,{S*0.24},-{S*0.4},{S*0.24},-{S*0.4},{S*0.4}c0,{S*0.24},{S*0.36},{S*0.36},{S*0.96},{S*0.36}',
    'none' => '0:{D}:-:-:h{D}', // dummy values, not actually used
    'nowy' => '0:{S*3}:i:i:q{S*1.5},-{S*2} {S*3},0:R1',
//    'potenty' => '*:{S*2}:-:-:h-{S/2}v-{S/2}h{S*1.5}v{S/2}h-{S/2}v{S/2}h{S*1.5}v-{S/2}',
    'potenty' => '*:{S*4}:-:-:h-{S}v-{S}h{S*3}v{S}h-{S}v{S}h{S*3}v-{S}',
//    'raguly' => '*:{S*2}:i:i:l{S*0.5},0 -{S*0.2},-{S} {S},0 {S*0.2},{S} {S*0.5},0',
    'raguly' => '*:{S*4}:i:i:l{S},0 -{S*0.4},-{S*2} {S*2},0 {S*0.4},{S*2} {S},0',
//    'rayonny' => '*:{S}:o:o:a{S/4},{S*0.4}-30,0,0,{S/4},{S/2}a{S/4},{S*0.4}-30,0,1,{S/4},{S/2}a{S/4},{S*0.4},30,0,0,{S/4},-{S/2}a{S/4},{S*0.4},30,0,1,{S/4},-{S/2}',
    'rayonny' => '*:{S*2}:o:o:a{S/2},{S*0.8}-30,0,0,{S/2},{S}a{S/2},{S*0.8}-30,0,1,{S/2},{S}a{S/2},{S*0.8},30,0,0,{S/2},-{S}a{S/2},{S*0.8},30,0,1,{S/2},-{S}',
    'urdy' => '*:{S*2}:-:-:v{S/2}l{S/2},{S/2} l{S/2},-{S/2}v-{S}l{S/2},-{S/2} l{S/2},{S/2}v{S/2}',
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
    'erable' => '*:{S*2.78}:-:-:l{S*0.90},0
    0,{S*-0.30}
    {S*-0.4},{S*0.10}
    {S*0.10},{S*-0.2}
    {S*-0.25},{S*-0.30}
    {S*0.10},{S*-0.10}
    {S*-0.2},{S*-0.30}
    {S*0.30},{S*-0.10}
    {S*0.30},{S*0.10}
    {S*-0.2},{S*-0.4}
    {S*0.2},{S*0.10}
    {S*0.30},{S*-0.30}
    {S*0.30},{S*0.30}
    {S*0.2},{S*-0.10}
    {S*-0.2},{S*0.4}
    {S*0.30},{S*-0.10}
    {S*0.30},{S*0.10}
    {S*-0.2},{S*0.30}
    {S*0.10},{S*0.10}
    {S*-0.25},{S*0.30}
    {S*0.10},{S*0.2}
    {S*-0.4},{S*-0.10}
    0,{S*0.30}
    {S*0.90},0
    0,{S*0.30}
    {S*-0.4},{S*-0.10}
    {S*0.10},{S*0.2}
    {S*-0.25},{S*0.30}
    {S*0.10},{S*0.10}
    {S*-0.2},{S*0.30}
    {S*0.30},{S*0.10}
    {S*0.30},{S*-0.10}
    {S*-0.2},{S*0.4}
    {S*0.2},{S*-0.10}
    {S*0.30},{S*0.30}
    {S*0.30},{S*-0.30}
    {S*0.2},{S*0.10}
    {S*-0.2},{S*-0.4}
    {S*0.30},{S*0.10}
    {S*0.30},{S*-0.10}
    {S*-0.2},{S*-0.30}
    {S*0.10},{S*-0.10}
    {S*-0.25},{S*-0.30}
    {S*0.10},{S*-0.2}
    {S*-0.4},{S*0.10}
    0,{S*-0.30}',
  'meander' => '*:{S*2}:-:-:
  h{S*1.75}v-{S*1.5}h-{S*1.25}
  v{S}h{S*0.75}v-{S*0.5}
  h-{S*0.25}v{S*0.25}h-{S*0.25}
  v-{S*0.5}h{S*0.75}v{S}
  h-{S*1.25}v-{S*1.5}h{S*1.75}
  v{S*1.75}'
  // 'meander' => '*:{S}:-:-:
  // h{S*0.875}v-{S*0.75}h-{S*0.625}
  // v{S*0.5}h{S*0.375}v-{S*0.25}
  // h-{S*0.125}v{S*0.125}h-{S*0.125}
  // v-{S*0.25}h{S*0.375}v{S*0.5}
  // h-{S*0.625}v-{S*0.75}h{S*0.875}
  // v{S*0.875}'
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
    switch ( $option[0] ) {
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
    $dir = $edgeSpec[0];
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
    if ($lineType == 'none' || 
        ctype_lower($dir) || 
        (($upperOnly == true ) && ($angle >45) && ($angle < 315))) {
      $dir = strtolower($dir);
      $start = '-';
      $end = '-';
    }

    $edges[] = compact('dir','dist', /* 'flag', */  'start','end','doMirror');
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
    if ($len > $dist ) { // straight, or cannot fit this path into distance
      $dir = strtolower($dir);
      $start = '-';
      $end = '-';
    }
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
    $dir = $match[0];
    $dist = substr($match,1);
    if ( strpos ( "XYZQVOU", $dir ) !== false ) {
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
    $dir = $match[0];
    $dist = floatval(substr($match,1));
    // Get the line spec for this direction
    if ( $dir == 'X' ) {
      $Xoff = $dist;
      continue;
    } elseif ( $dir == 'Y' ) {
      $Yoff = $dist;
      continue;
    } elseif ( strpos("ZQVOU", $dir) !== false ) {
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

class PathBuilder
{
    public $spec = "";

    function next()
    {
        if ( $this->spec != "" && $this->spec[strlen($this->spec)-1] != " ")
            $this->spec .= "   ";
    }

    function move($x, $y)
    {
        $this->next();
        $this->spec .= "X{$x}Y$y";
        return $this;
    }

    function __toString()
    {
        return $this->spec;
    }

    function right($length)
    {
        $this->spec .= "A$length";
        return $this;
    }

    function right_plain($length)
    {
        $this->spec .= "a$length";
        return $this;
    }

    function down_right($length)
    {
        $this->spec .= "B$length";
        return $this;
    }

    function down_right_plain($length)
    {
        $this->spec .= "b$length";
        return $this;
    }

    function down($length)
    {
        $this->spec .= "C$length";
        return $this;
    }

    function down_plain($length)
    {
        $this->spec .= "c$length";
        return $this;
    }

    function down_left($length)
    {
        $this->spec .= "D$length";
        return $this;
    }

    function down_left_plain($length)
    {
        $this->spec .= "d$length";
        return $this;
    }

    function left($length)
    {
        $this->spec .= "E$length";
        return $this;
    }

    function left_plain($length)
    {
        $this->spec .= "e$length";
        return $this;
    }

    function up_left($length)
    {
        $this->spec .= "F$length";
        return $this;
    }

    function up_left_plain($length)
    {
        $this->spec .= "f$length";
        return $this;
    }

    function up($length)
    {
        $this->spec .= "G$length";
        return $this;
    }

    function up_plain($length)
    {
        $this->spec .= "g$length";
        return $this;
    }

    function up_right($length)
    {
        $this->spec .= "H$length";
        return $this;
    }

    function up_right_plain($length)
    {
        $this->spec .= "h$length";
        return $this;
    }

    function close($flag)
    {
        $this->spec .= "Z" . ($flag ? 1 : 0);
        return $this;
    }

    function draw_all($flag=true)
    {
        $this->spec .= "U" . ($flag ? 1 : 0);
        return $this;
    }

    function offset($flag=true)
    {
        $this->spec .= "O" . (!$flag ? 1 : 0);
        return $this;
    }

    function quarterly($flag=true)
    {
        $this->spec .= "Q" . ($flag ? 1 : 0);
        return $this;
    }

    function chevron($flag=true)
    {
        $this->spec .= "V" . ($flag ? 1 : 0);
        return $this;
    }
}


function path_builder($x, $y)
{
    return (new PathBuilder())->move($x, $y);
}
