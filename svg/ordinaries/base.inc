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


if ( existModifierWithKeyterm($node, 'pointed')) {
    $shape = new MutableShape();
    $shape->move_to(-20, $layout->size->height * 1.01)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->vertical(-$layout->size->height*0.25, true)
        ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
        ->line_to($layout->size->width / 2 + 10, -$layout->size->height*0.1, true)
        ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
        ->line_to($layout->size->width / 2 + 10, +$layout->size->height*0.1, true)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->vertical($layout->size->height*0.25, true)
        ->close()
    ;
    $ordinary['shape_spec'] = $shape;
} else {
   $ordinary['shape_spec'] = 'X-100,Y{%75*}A1200c{%26*}e1200g{%26*}';
}

if ($layout->shape == 'flag') {
    $ordinary['chargesOn'] = array('h', '100,{%82*},800,{%15*}');
} else {
   $ordinary['chargesOn'] = '500,{%87*},500,{%21*}/333.33,{%87*},300,{%21*}:666.67,{%87*}/333.33,{%87*},160,{%21*}:500,{%87*}:666.67,{%87*}/x/x/x';
}
if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',915)">' . $rivet . '</g>';
  }
  $ordinary['body3'] = $rivetList;
}
