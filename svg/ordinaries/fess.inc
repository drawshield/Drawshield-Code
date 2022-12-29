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


if ( ($num_of = getModifierValueByKeyterm($node, 'points')) == null ) $num_of = 3;
// Set basic body shape
if ( existModifierWithKeyterm($node,'fracted') ) {
  $ordinary['shape_spec'] = 'X-100Y{%50}A600g{%15}A600c{%15}E600c{%15}E600g{%15}';
} elseif ( existModifierWithKeyterm($node,'lozengy')) {
    // lozengy can have 1 colour, in which case it is just conjoined lozenges in that colour
    // or two colours, in which case:
    // The first colour is a normal, straight edged fess
    // The second colour is the conjoined lozenges superimposed
    $numCols = 0;
    foreach($node->childNodes as $childNode) {
        if ($childNode->nodeName == blazonML::E_TINCTURE) $numCols++;
    }
    $yStart = $layout->cross_point->y - (($layout->size->height * 0.3) / 2);
    if ($numCols < 2) {
        $ordinary['body'] = '<path d="M0,' . $yStart . 'l83.3,{%15} 83.3,-{%15} 83.3,{%15} 83.3,-{%15} 83.3,{%15} 83.3,-{%15} 83.3,{%15} 83.3,-{%15} 83.3,{%15} 83.3,-{%15} 83.3,{%15} 83.3,-{%15} v{%30} l-83.3,-{%15} -83.3,{%15} -83.3,-{%15} -83.3,{%15} -83.3,-{%15} -83.3,{%15} -83.3,-{%15} -83.3,{%15} -83.3,-{%15} -83.3,{%15} -83.3,-{%15} -83.3,{%15} " />';
    } else {
        $ordinary['body'] = '<path d="M-50,' . $yStart . ' h 1300 v{%30} h-1300 z" />';
        $ordinary['body2'] = '<path d="M0,' . $yStart . 'l83.3,{%15} 83.3,-{%15} 83.3,{%15} 83.3,-{%15} 83.3,{%15} 83.3,-{%15} 83.3,{%15} 83.3,-{%15} 83.3,{%15} 83.3,-{%15} 83.3,{%15} 83.3,-{%15} v{%30} l-83.3,-{%15} -83.3,{%15} -83.3,-{%15} -83.3,{%15} -83.3,-{%15} -83.3,{%15} -83.3,-{%15} -83.3,{%15} -83.3,-{%15} -83.3,{%15} -83.3,-{%15} -83.3,{%15} " />';
    }
} elseif ( existModifierWithKeyterm($node,'dancetty') && $num_of == 2) {
  $ordinary['body'] = '<path d="M -10.528194,490.00143 247.10288,290.00714 504.73396,490.00143 762.36503,290.00714 1019.9961,490.00143 V 789.99287 L 762.36503,589.99858 504.73396,789.99287 247.10288,589.99858 -10.528194,789.99287 Z" />';
} elseif ( existModifierWithKeyterm($node,'dancetty')) { // TODO Finish these weird shapes as %ages
  $ordinary['body'] = '<path d="M-100,450L100,250 {%30},450 500,250 700,450 900,250 1100,450 1100,750 900,550 700,750 500,550 {%30},750 100,550 -100,750Z" />';
// } elseif ( existModifierWithKeyterm($node, 'nowy')) { // special handling for some linetypes
//   $ordinary['body'] = '<path d="m 500,200 a 297.98,297.98 0 0 1 257.05,147.98 h 300 v 300 h -292.88 a 297.98,297.98 0 0 1 -257.12,147.98 297.98,297.98 0 0 1 -257.05,-147.98 h -292.95 v -300 h 300 a 297.98,297.98 0 0 1 257.12,-150 z" />';
} elseif ( existModifierWithKeyterm($node, 'quadrate')) { 
  $ordinary['body'] = '<path d="m 250,250 v 100 h -300 v 300 h 300 v 100 h 500 v -100 h 300 v -300 h -300 v -100 z" />';
} else {
    $margin = 100;
    $left = -$margin;
    $right = $layout->size->width + $margin;

    $couped_size = $layout->size->width / 10;

    $couped = existModifierWithKeyterm($node,'couped');
    if ( $couped || existModifierWithKeyterm($node, 'couped-dexter'))
        $left = $couped_size;

    if ( $couped || existModifierWithKeyterm($node, 'couped-sinister'))
        $right = $layout->size->width - $couped_size;

    $width = $layout->size->height * 0.3;
    $builder = new MutableShape();
    $builder->move_to($left, $layout->cross_point->y - $width / 2)
        ->tag_segment(BezierSegmentFlags::STRAIGHT|BezierSegmentFlags::SYMMETRICAL)
        ->horizontal($right)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->vertical($width, true)
        ->tag_segment(BezierSegmentFlags::STRAIGHT|BezierSegmentFlags::SYMMETRICAL|BezierSegmentFlags::UNDERSIDE)
        ->horizontal($left)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->vertical(-$width, true)
        ->close()
    ;

  $ordinary['shape_spec'] = $builder;
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
