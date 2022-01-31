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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */

if ( existModifierWithKeyterm($node,'couped') ) {
  $ordinary['shape_spec'] = 'X100Y-8U1a800c408E800g408';
  $ordinary['chargesOn'] = array ('h','200,50,600,200');
} elseif ( existModifierWithKeyterm($node, 'couped-dexter')) {
  $ordinary['shape_spec'] = 'X-8Y-8U1a908c408E908g408';
  $ordinary['chargesOn'] = array ('h','200,50,600,200');
} elseif ( existModifierWithKeyterm($node,'couped-sinister')) {
  $ordinary['shape_spec'] = 'X100Y-8U1a908c408E908g408';
  $ordinary['chargesOn'] = array ('h','200,50,600,200');
} elseif ( existModifierWithKeyterm($node,'lozengy')) {
    // lozengy can have 1 colour, in which case it is just conjoined lozenges in that colour
    // or two colours, in which case:
    // The first colour is a normal, straight edged fess
    // The second colour is the conjoined lozenges superimposed
    $numCols = 0;
    foreach($node->childNodes as $childNode) {
        if ($childNode->nodeName == blazonML::E_TINCTURE) $numCols++;
    }
    if ($numCols < 2) {
        $ordinary['body'] = '<path d="M0,0 l83.3,{%60} 83.3,-{%60} 83.3,{%60} 83.3,-{%60} 83.3,{%60} 83.3,-{%60} 83.3,{%60} 83.3,-{%60} 83.3,{%60} 83.3,-{%60} 83.3,{%60} 83.3,-{%60} v{%120} l-83.3,-{%60} -83.3,{%60} -83.3,-{%60} -83.3,{%60} -83.3,-{%60} -83.3,{%60} -83.3,-{%60} -83.3,{%60} -83.3,-{%60} -83.3,{%60} -83.3,-{%60} -83.3,{%60} " />';
    } else {
        $ordinary['body'] = '<path d="M-50,0 h 1300 v{%120} h-1300 z" />';
        $ordinary['body2'] = '<path d="M0,0 l83.3,{%60} 83.3,-{%60} 83.3,{%60} 83.3,-{%60} 83.3,{%60} 83.3,-{%60} 83.3,{%60} 83.3,-{%60} 83.3,{%60} 83.3,-{%60} 83.3,{%60} 83.3,-{%60} v{%120} l-83.3,-{%60} -83.3,{%60} -83.3,-{%60} -83.3,{%60} -83.3,-{%60} -83.3,{%60} -83.3,-{%60} -83.3,{%60} -83.3,-{%60} -83.3,{%60} -83.3,-{%60} -83.3,{%60} " />';
    }
  $ordinary['chargesOn'] = array ('h','100,50,800,200'); // Or an error?
} elseif ( existModifierWithKeyterm($node, 'voided')) {
  $ordinary['shape_spec'] = 'X-100Y300U1a1200c40E1200g40';
  $ordinary['chargesOn'] = array ('h','100,30,800,180');
} elseif ( existModifierWithKeyterm($node, 'pointed')) {
  $ordinary['body'] = '<path d="M -20,250 500,350 1020,250 V -20 H -20 Z" />';
  $ordinary['chargesOn'] = array ('h','100,50,800,200');
} else {
  $ordinary['shape_spec'] = (string)path_builder(-100, -8)
    ->draw_all()
    ->right_plain($layout->size->width+200)
    ->down_plain($layout->size->height+8)
    ->left($layout->size->width+200)
    ->up_plain($layout->size->height+8);

    $rect = $layout->rect_area()->margin($layout->size->height / 6);
    $ordinary['chargesOn'] = array("h", $rect);
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
