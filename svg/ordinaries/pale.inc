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

if ( existModifierWithKeyterm($node,'couped') )
  $ordinary['shape_spec'] = array ( 'NC' => 'X350Y100a300C{%83*}e300G{%83*}', 'CH' => 'X350Y400a300C700e300G700' );
elseif ( existModifierWithKeyterm($node,'couped-chief') )
  $ordinary['shape_spec'] = array ( 'NC' => 'X350Y100a300C{%92*}e300G{%92*}', 'CH' => 'X350Y400a300C808e300G808' );
elseif ( existModifierWithKeyterm($node,'couped-base') )
  $ordinary['shape_spec'] = array ( 'NC' => 'X350Y-8a300C{%92*}e300G{%92*}', 'CH' => 'X350Y200a300C900e300G900' );
elseif ( existModifierWithKeyterm($node,'fracted') )
  $ordinary['shape_spec'] = 'X350Y-{%2}a150C{%52*}a150C{%52*}e150G{%52*}e150G{%52*}';
elseif ( existModifierWithKeyterm($node,'lozengy')) // TODO size flag, and compony
  $ordinary['body'] = '<path d="M350,150l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150  M350,450l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150   M350,750l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150  M350,1050l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150" />';
elseif ( existModifierWithKeyterm($node,'voided'))
  $ordinary['shape_spec'] = array ( 'NC' => 'X350Y-{%2}a300C{%104*}e300G{%104*}', 'CH' => 'X350Y262a300C972e300G972' );
elseif ( existModifierWithKeyterm($node, 'nowy')) // special handling for some linetypes
  $ordinary['body'] = '<path d="m 202.02,500 c 0.15,-105.89 56.48,-203.75 147.98,-257.05 v -352.95 h 300 v 352.88 c 91.52,53.32 147.85,151.2 147.98,257.12 -0.15,105.89 -56.48,203.75 -147.98,257.05 v 552.95 h -300 v -552.88 c -91.52,-53.32 -147.85,-151.2 -147.98,-257.12 z" />';
elseif ( existModifierWithKeyterm($node, 'quadrate')) // special handling for some linetypes
  $ordinary['body'] = '<path d="m 350,-100 h 300 v 350 h 100 v 500 h -100 v 550 h -300 v -550 h -100 v -500 h 100 z" />';
else
  $ordinary['shape_spec'] = array ( 'NC' => 'X350Y-{%2}a300C{%104*}e300G{%104*}', 'CH' => 'X350Y292a300C916e300G916' );

if ( existModifierWithKeyterm($node,'compony1') )
   $ordinary['body2'] = '<path d="M250,0h500v300h-500z M250,600h500v300h-500z M250,600h500v300h-500z " />';
  elseif (existModifierWithKeyterm($node,'compony2') )
    $ordinary['body2'] = '<path d="M250,0h250v150h-250z M500,150h250v150h-250z M250,300h250v150h-250z M500,450h250v150h-250z M250,600h250v150h-250z M500,750h250v150h-250z M250,900h250v150h-250z M500,1050h250v150h-250z " />';

$ordinary['chargesOn'] = array ('v', chiefPresent() ? '400,350,200,750' : '400,{%10},200,{%84*}');

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
