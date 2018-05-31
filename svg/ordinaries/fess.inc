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



// Set basic body shape
if ( existModifierWithKeyterm($node,'couped') ) {
  $ordinary['shape_spec'] = 'X100Y{%35}A800c{%30}E800g{%30}';
}
elseif ( existModifierWithKeyterm($node, 'couped-dexter')) {
  $ordinary['shape_spec'] = 'X-8Y{%35}A908c{%30}E908g{%30}';
}
elseif ( existModifierWithKeyterm($node,'couped-sinister')) {
  $ordinary['shape_spec'] = 'X100Y{%35}A908c{%30}E908g{%30}';
} elseif ( existModifierWithKeyterm($node,'fracted') ) {
  $ordinary['shape_spec'] = 'X-100Y{%50}A600g{%15}A600c{%15}E600c{%15}E600g{%15}';
} elseif ( existModifierWithKeyterm($node,'lozengy')) {
  $ordinary['body'] = '<path d="M0,{%35}l83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 v{%30} l-83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 " />';
} elseif ( existModifierWithKeyterm($node,'dancetty')) { // TODO Finish these weird shapes as %ages
  $ordinary['body'] = '<path d="M-100,450L100,250 {%30},450 500,250 700,450 900,250 1100,450 1100,750 900,550 700,750 500,550 {%30},750 100,550 -100,750Z" />';
} elseif ( existModifierWithKeyterm($node, 'voided')) {
  $ordinary['shape_spec'] = 'X-100Y{%35}A1200c{%30}E1200g{%30}';
} elseif ( existModifierWithKeyterm($node, 'nowy')) { // special handling for some linetypes
  $ordinary['body'] = '<path d="m 500,200 a 297.98,297.98 0 0 1 257.05,147.98 h 300 v 300 h -292.88 a 297.98,297.98 0 0 1 -257.12,147.98 297.98,297.98 0 0 1 -257.05,-147.98 h -292.95 v -300 h 300 a 297.98,297.98 0 0 1 257.12,-150 z" />';
} elseif ( existModifierWithKeyterm($node, 'quadrate')) { 
  $ordinary['body'] = '<path d="m 250,250 v 100 h -300 v 300 h 300 v 100 h 500 v -100 h 300 v -300 h -300 v -100 z" />';
} else {
  $ordinary['shape_spec'] = 'X-100Y{%35}A1200c{%30}E1200g{%30}';
}

// Add extra layer if compony
if ( existModifierWithKeyterm($node,'compony1') )
  $ordinary['body2'] = '<path d="M0,{%30}h250v400h-250z M500,{%30}h250v400h-250z" />';
elseif (existModifierWithKeyterm($node,'compony2') )
  $ordinary['body2'] = '<path d="M0,{%30}h250v200h-250z M250,500h250v200h-250z M500,{%30}h250v200h-250z M750,500h250v200h-250z" />';

if (existModifierWithKeyterm($node,'voided'))
  $ordinary['chargesOn'] = array ( 'h','100,{%43},800,{%16}' );
else
  $ordinary['chargesOn'] = array ( 'h','100,{%40},800,{%20}' );

$ordinary['chargesWithin'] = 'x/500,150,400,180:500,850,400,{%30}/333,150,200,200:666,150:500,850,400,300/333.33,150,300,180:667.67,150:333.33,850,300,300:667.66,850/250,150,200,180:500,150:750,150:333.33,850,300,300:667.66,850/250,150,200,180:500,150:750,150:250,850,200,300:500,850:750,850';

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',365)">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . $i . ',615)">' . $rivet . '</g>';
  }
  $ordinary['body3'] = $rivetList;
}
