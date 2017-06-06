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

if ( existModifierWithKeyterm($node,'couped') ) {
  $ordinary['shape_spec'] = 'X100Y-8U1a800c408E800g408';
  $ordinary['chargesOn'] = array ('h','200,50,600,200');
} elseif ( existModifierWithKeyterm($node, 'couped-dexter')) {
  $ordinary['shape_spec'] = 'X-8Y-8U1a908c408E908g408';
  $ordinary['chargesOn'] = array ('h','200,50,600,200');
} elseif ( existModifierWithKeyterm($node,'couped-sinister')) {
  $ordinary['shape_spec'] = 'X100Y-8U1a908c408E908g408';
  $ordinary['chargesOn'] = array ('h','200,50,600,200');
} elseif ( existModifierWithKeyterm($node,'triangular')) {
  $ordinary['shape_spec'] = 'V1X-8Y-8U1a1016D719F719';
  $ordinary['chargesOn'] = '500,200,200,200/375,180,180,180:625,180/325,140,160,180:500,240:675,140/x/x/x';
} elseif ( existModifierWithKeyterm($node,'lozengy')) {
  $ordinary['body'] = '<path d="M0,0l83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 v300 l-83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 " />';
  $ordinary['chargesOn'] = array ('h','100,50,800,200'); // Or an error?
} elseif ( existModifierWithKeyterm($node, 'voided')) {
  $ordinary['shape_spec'] = 'X-40Y300U1a1080c40E1080g40';
  $ordinary['chargesOn'] = array ('h','100,30,800,180');
} else {
  $ordinary['shape_spec'] = 'X-8Y-8U1a1016c308E1016g308';
  $ordinary['chargesOn'] = array ('h','100,50,800,200');
}

 if ( existModifierWithKeyterm($node,'compony1') )
  $ordinary['body2'] = '<path d="M0,0h250v450h-250z M500,0h250v450h-250z" />';
elseif (existModifierWithKeyterm($node,'compony2') )
  $ordinary['body2'] =  '<path d="M0,0h250v150h-250z M250,150h250v300h-250z M500,0h250v150h-250z M750,150h250v300h-250z" />';

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',265)">' . $rivet . '</g>';
  }
  $ordinary['body3'] = $rivetList;
}
