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


$old_ordinary = $ordinary;
include "plain-cross.inc";
$old_ordinary["chargesOn"] = $ordinary["chargesOn"];
$old_ordinary["chargesWithin"] = $ordinary["chargesWithin"];
$ordinary = $old_ordinary;

$tan_out_1 = 136.88444;
$tan_out_2 = 91.07927;
$tan_in_1 = 9;
$tan_in_2 = 166;
$width_top = $layout->size->height / 1.2 * 0.618;
$width_side = $layout->size->width * 0.618;
$width_bottom = $layout->size->height *  0.718;
$margin = 18;
$center_off = 103;

$ordinary['shape_spec'] = (new MutableShape)
    ->move_to($layout->cross_point->x - $width_top / 2, -$margin)
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->line_to($layout->cross_point->x + $width_top / 2, -$margin)
    ->tag_segment(BezierSegmentFlags::NORMAL)
    ->cubic_to(
        $layout->cross_point->x + $width_top / 2 - $tan_out_1, -$margin + $tan_out_2,
        $layout->cross_point->x + $center_off - $tan_in_1, $layout->cross_point->y - $center_off - $tan_in_2,
        $layout->cross_point->x + $center_off, $layout->cross_point->y - $center_off
    )
    ->tag_segment(BezierSegmentFlags::MIRROR)
    ->cubic_to(
        $layout->cross_point->x + $center_off + $tan_in_2, $layout->cross_point->y - $center_off - $tan_in_1,
        $layout->size->width + $margin - $tan_out_2, $layout->cross_point->y - $width_side / 2 + $tan_out_1,
        $layout->size->width + $margin, $layout->cross_point->y - $width_side / 2
    )
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->line_to($layout->size->width + $margin, $layout->cross_point->y + $width_side / 2)
    ->tag_segment(BezierSegmentFlags::NORMAL)
    ->cubic_to(
        $layout->size->width + $margin - $tan_out_2, $layout->cross_point->y + $width_side / 2 - $tan_out_1,
        $layout->cross_point->x + $center_off + $tan_in_2, $layout->cross_point->y + $center_off - $tan_in_1,
        $layout->cross_point->x + $center_off, $layout->cross_point->y + $center_off
    )
    ->tag_segment(BezierSegmentFlags::MIRROR)
    ->cubic_to(
        $layout->cross_point->x + $center_off - $tan_in_1, $layout->cross_point->y + $center_off + $tan_in_2,
        $layout->cross_point->x + $width_bottom / 2 - $tan_out_1 * 1.5, $layout->size->height + $margin - $tan_out_2 / 1.5,
        $layout->cross_point->x + $width_bottom / 2, $layout->size->height + $margin
    )
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->line_to($layout->cross_point->x - $width_bottom / 2, $layout->size->height + $margin)
    ->tag_segment(BezierSegmentFlags::NORMAL)
    ->cubic_to(
        $layout->cross_point->x - $width_bottom / 2 + $tan_out_1 * 1.5, $layout->size->height + $margin - $tan_out_2 / 1.5,
        $layout->cross_point->x - $center_off + $tan_in_1, $layout->cross_point->y + $center_off + $tan_in_2,
        $layout->cross_point->x - $center_off, $layout->cross_point->y + $center_off
    )
    ->tag_segment(BezierSegmentFlags::MIRROR)
    ->cubic_to(
        $layout->cross_point->x - $center_off - $tan_in_2, $layout->cross_point->y + $center_off -  $tan_in_1,
        -$margin + $tan_out_2, $layout->cross_point->y + $width_side / 2 - $tan_out_1,
        -$margin, $layout->cross_point->y + $width_side / 2
    )
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->line_to(-$margin, $layout->cross_point->y - $width_side / 2)
    ->tag_segment(BezierSegmentFlags::NORMAL)
    ->cubic_to(
        -$margin + $tan_out_2, $layout->cross_point->y - $width_side / 2 + $tan_out_1,
        $layout->cross_point->x - $center_off - $tan_in_2, $layout->cross_point->y - $center_off + $tan_in_1,
        $layout->cross_point->x - $center_off, $layout->cross_point->y - $center_off
    )
    ->tag_segment(BezierSegmentFlags::MIRROR)
    ->cubic_to(
        $layout->cross_point->x - $center_off + $tan_in_1, $layout->cross_point->y - $center_off - $tan_in_2,
        $layout->cross_point->x - $width_top / 2 + $tan_out_1, -$margin + $tan_out_2,
        $layout->cross_point->x - $width_top / 2, -$margin
    )
    ->close();
;
