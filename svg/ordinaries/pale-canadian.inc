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
  $ordinary['shape_spec'] = array ( 'NC' => 'X250Y100a500C{%83}e500G{%83*}', 'CH' => 'X250Y400a500C700e500G700' );
elseif ( existModifierWithKeyterm($node,'couped-chief') )
  $ordinary['shape_spec'] = array ( 'NC' => 'X250Y100a500C{%93}e500G{%93*}', 'CH' => 'X250Y400a500C808e500G808' );
elseif ( existModifierWithKeyterm($node,'couped-base') )
  $ordinary['shape_spec'] = array ( 'NC' => 'X250Y-8a500C{%93}e500G{%93*}', 'CH' => 'X250Y200a500C900e500G900' );
elseif ( existModifierWithKeyterm($node,'lozengy'))
  $ordinary['body'] = '<path d="M250,150l125,-150 125,150 125,-150 125,150 -125,150 -125,-150 -125,150 -125,-150  M250,450l125,-150 125,150 125,-150 125,150 -125,150 -125,-150 -125,150 -125,-150   M250,750l125,-150 125,150 125,-150 125,150 -125,150 -125,-150 -125,150 -125,-150  M250,1050l125,-150 125,150 125,-150 125,150 -125,150 -125,-150 -125,150 -125,-150" />';
elseif ( existModifierWithKeyterm($node,'voided'))
  $ordinary['shape_spec'] = array ( 'NC' => 'X250Y-{%2}a500C{%104*}e500G{%104*}', 'CH' => 'X250Y262a500C972e500G972' );
else
  $ordinary['shape_spec'] = array ( 'NC' => 'X250Y-{%2}a500C{%104*}e500G{%104*}', 'CH' => 'X250Y292a500C916e500G916' );

if ( existModifierWithKeyterm($node,'compony1') )
   $ordinary['body2'] = '<path d="M150,0h700v300h-700z M150,600h700v300h-700z M150,600h700v300h-700z " />';
  elseif (existModifierWithKeyterm($node,'compony2') )
    $ordinary['body2'] = '<path d="M150,0h350v150h-350z M500,150h350v150h-350z M150,300h350v150h-350z M500,450h350v150h-350z M150,600h350v150h-350z M500,750h350v150h-350z M150,900h350v150h-350z M500,1050h350v150h-350z " />';

$ordinary['chargesOn'] = array ('v', chiefPresent() ? '300,350,400,750' : '300,{%10},400,{%80*}');

// TODO for flags
$ordinary['chargesWithin'] = 'x/125,550,200,800:875,550/x/125,383.33,200,350:125,716.67:875,383.33:875,716.67/x/125,200,250,200:125,550:125,800:875,200:875,550:875,800';
