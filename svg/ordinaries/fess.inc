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
  $ordinary['shape_spec'] = 'X100Y350A800c300E800g300';
}
elseif ( existModifierWithKeyterm($node, 'couped-dexter')) {
  $ordinary['shape_spec'] = 'X-8Y350A908c300E908g300';
}
elseif ( existModifierWithKeyterm($node,'couped-sinister')) {
  $ordinary['shape_spec'] = 'X100Y350A908c300E908g300';
}
elseif ( existModifierWithKeyterm($node,'lozengy')) {
  $ordinary['body'] = '<path d="M0,350l83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 v300 l-83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 " />';
} elseif ( existModifierWithKeyterm($node, 'voided')) {
  $ordinary['shape_spec'] = 'X-38Y350A1076c300E1076g300';
} else {
  $ordinary['shape_spec'] = 'X-8Y350A1016c300E1016g300';
}

// Add extra layer if compony
if ( existModifierWithKeyterm($node,'compony1') )
  $ordinary['body2'] = '<path d="M0,300h250v400h-250z M500,300h250v400h-250z" />';
elseif (existModifierWithKeyterm($node,'compony2') )
  $ordinary['body2'] = '<path d="M0,300h250v200h-250z M250,500h250v200h-250z M500,300h250v200h-250z M750,500h250v200h-250z" />';

if (existModifierWithKeyterm($node,'voided'))
  $ordinary['chargesOn'] = array ( 'h','100,430,800,160' );
else
  $ordinary['chargesOn'] = array ( 'h','100,400,800,200' );

$ordinary['chargesWithin'] = 'x/500,150,400,180:500,850,400,300/333,150,200,200:666,150:500,850,400,300/333.33,150,300,180:667.67,150:333.33,850,300,300:667.66,850/250,150,200,180:500,150:750,150:333.33,850,300,300:667.66,850/250,150,200,180:500,150:750,150:250,850,200,300:500,850:750,850';

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',365)">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . $i . ',615)">' . $rivet . '</g>';
  }
  $ordinary['body3'] = $rivetList;
}
