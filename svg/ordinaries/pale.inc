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

if ( existModifierWithKeyterm($node,'couped') ) {
    $ordinary['shape_spec'] = 'X350Y{%8.5*}a300C{%83*}e300G{%83*}';
} elseif ( existModifierWithKeyterm($node,'couped-chief') ) {
  $ordinary['shape_spec'] = 'X350Y100a300C{%92*}e300G{%92*}';
} elseif ( existModifierWithKeyterm($node,'couped-base') ) {
  $ordinary['shape_spec'] = 'X350Y-8a300C{%92*}e300G{%92*}';
} elseif ( existModifierWithKeyterm($node,'fracted') ) {
  $ordinary['shape_spec'] = 'X350Y-{%2}a150C{%52*}a150C{%52*}e150G{%52*}e150G{%52*}';
} elseif ( existModifierWithKeyterm($node,'lozengy')) { // TODO size flag, and compony
    // lozengy can have 1 colour, in which case it is just conjoined lozenges in that colour
    // or two colours, in which case:
    // The first colour is a normal, straight edged fess
    // The second colour is the conjoined lozenges superimposed
    $numCols = 0;
foreach($node->childNodes as $childNode) {
    if ($childNode->nodeName == blazonML::E_TINCTURE) $numCols++;
    }
    if ($numCols < 2) {
        $ordinary['body'] = '<path d="M350,150l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150  M350,450l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150   M350,750l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150  M350,1050l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150" />';
    } else {
        $ordinary['body'] = '<path d="M350,-50 h 300 v 1300 h -300 v -1300 z" />';
        $ordinary['body2'] = '<path d="M350,150l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150  M350,450l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150   M350,750l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150  M350,1050l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150" />';
    }
} elseif ( existModifierWithKeyterm($node,'voided')) {
  $ordinary['shape_spec'] = 'X350Y-{%4}a300C{%106*}e300G{%106*}';
//} elseif ( existModifierWithKeyterm($node, 'nowy'))  {// special handling for some linetypes
//  $ordinary['body'] = '<path d="m 202.02,500 c 0.15,-105.89 56.48,-203.75 147.98,-257.05 v -352.95 h 300 v 352.88 c 91.52,53.32 147.85,151.2 147.98,257.12 -0.15,105.89 -56.48,203.75 -147.98,257.05 v 552.95 h -300 v -552.88 c -91.52,-53.32 -147.85,-151.2 -147.98,-257.12 z" />';
} elseif ( existModifierWithKeyterm($node, 'quadrate')) { // special handling for some linetypes
  $ordinary['body'] = '<path d="m 350,-100 h 300 v 350 h 100 v 500 h -100 v 550 h -300 v -550 h -100 v -500 h 100 z" />';
} else {
    $ordinary['shape_spec'] = 'X350Y-{%2}a300C{%104*}e300G{%104*}';
}

if ( existModifierWithKeyterm($node,'compony1') )
   $ordinary['body2'] = '<path d="M250,0h500v300h-500z M250,600h500v300h-500z M250,600h500v300h-500z " />';
  elseif (existModifierWithKeyterm($node,'compony2') )
    $ordinary['body2'] = '<path d="M250,0h250v150h-250z M500,150h250v150h-250z M250,300h250v150h-250z M500,450h250v150h-250z M250,600h250v150h-250z M500,750h250v150h-250z M250,900h250v150h-250z M500,1050h250v150h-250z " />';

$ordinary['chargesOn'] = array('v', '400,{%10},200,{%84*}');

$ordinary['chargesWithin'] = 'x/175,{%46*},300,{%75}:825,{%46*}/x/175,383.33,300,350:175,716.67:825,383.33:825,716.67/x/175,300,250,200:175,{%46*}:175,{%75}:825,300:825,{%46*}:825,{%75}'; // TODO Finish placements

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1200; $i += 80) {
    $rivetList .= '<g transform="translate(365,' . $i . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(615,' . $i . ')">' . $rivet . '</g>';
  }
  $ordinary['body3'] = $rivetList;
}
