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

if ( existModifierWithKeyterm($node,'couped') )
  $ordinary['shape_spec'] = 'X250Y{%8.5*}a500C{%83}e500G{%83*}';
elseif ( existModifierWithKeyterm($node,'couped-chief') )
  $ordinary['shape_spec'] = 'X250Y100a500C{%93}e500G{%93*}';
elseif ( existModifierWithKeyterm($node,'couped-base') )
  $ordinary['shape_spec'] = 'X250Y-8a500C{%93}e500G{%93*}';
elseif ( existModifierWithKeyterm($node,'lozengy'))
  $ordinary['body'] = '<path d="M250,150l125,-150 125,150 125,-150 125,150 -125,150 -125,-150 -125,150 -125,-150  M250,450l125,-150 125,150 125,-150 125,150 -125,150 -125,-150 -125,150 -125,-150   M250,750l125,-150 125,150 125,-150 125,150 -125,150 -125,-150 -125,150 -125,-150  M250,1050l125,-150 125,150 125,-150 125,150 -125,150 -125,-150 -125,150 -125,-150" />';
elseif ( existModifierWithKeyterm($node,'voided'))
  $ordinary['shape_spec'] = 'X250Y-{%2}a500C{%104*}e500G{%104*}';
else
  $ordinary['shape_spec'] = 'X250Y-{%2}a500C{%104*}e500G{%104*}';

if ( existModifierWithKeyterm($node,'compony1') )
   $ordinary['body2'] = '<path d="M150,0h700v300h-700z M150,600h700v300h-700z M150,600h700v300h-700z " />';
  elseif (existModifierWithKeyterm($node,'compony2') )
    $ordinary['body2'] = '<path d="M150,0h350v150h-350z M500,150h350v150h-350z M150,300h350v150h-350z M500,450h350v150h-350z M150,600h350v150h-350z M500,750h350v150h-350z M150,900h350v150h-350z M500,1050h350v150h-350z " />';

$ordinary['chargesOn'] = array('v', '300,{%10},400,{%80*}');

// TODO for flags
$ordinary['chargesWithin'] = 'x/125,550,200,800:875,550/x/125,383.33,200,350:125,716.67:875,383.33:875,716.67/x/125,200,250,200:125,550:125,800:875,200:875,550:875,800';
