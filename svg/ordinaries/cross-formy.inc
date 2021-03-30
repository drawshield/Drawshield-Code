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
    ->move_to($layout->center_point->x - $width_top / 2, -$margin)
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->line_to($layout->center_point->x + $width_top / 2, -$margin)
    ->tag_segment(BezierSegmentFlags::NORMAL)
    ->cubic_to(
        $layout->center_point->x + $width_top / 2 - $tan_out_1, -$margin + $tan_out_2,
        $layout->center_point->x + $center_off - $tan_in_1, $layout->center_point->y - $center_off - $tan_in_2,
        $layout->center_point->x + $center_off, $layout->center_point->y - $center_off
    )
    ->tag_segment(BezierSegmentFlags::MIRROR)
    ->cubic_to(
        $layout->center_point->x + $center_off + $tan_in_2, $layout->center_point->y - $center_off - $tan_in_1,
        $layout->size->width + $margin - $tan_out_2, $layout->center_point->y - $width_side / 2 + $tan_out_1,
        $layout->size->width + $margin, $layout->center_point->y - $width_side / 2
    )
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->line_to($layout->size->width + $margin, $layout->center_point->y + $width_side / 2)
    ->tag_segment(BezierSegmentFlags::NORMAL)
    ->cubic_to(
        $layout->size->width + $margin - $tan_out_2, $layout->center_point->y + $width_side / 2 - $tan_out_1,
        $layout->center_point->x + $center_off + $tan_in_2, $layout->center_point->y + $center_off - $tan_in_1,
        $layout->center_point->x + $center_off, $layout->center_point->y + $center_off
    )
    ->tag_segment(BezierSegmentFlags::MIRROR)
    ->cubic_to(
        $layout->center_point->x + $center_off - $tan_in_1, $layout->center_point->y + $center_off + $tan_in_2,
        $layout->center_point->x + $width_bottom / 2 - $tan_out_1 * 1.5, $layout->size->height + $margin - $tan_out_2 / 1.5,
        $layout->center_point->x + $width_bottom / 2, $layout->size->height + $margin
    )
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->line_to($layout->center_point->x - $width_bottom / 2, $layout->size->height + $margin)
    ->tag_segment(BezierSegmentFlags::NORMAL)
    ->cubic_to(
        $layout->center_point->x - $width_bottom / 2 + $tan_out_1 * 1.5, $layout->size->height + $margin - $tan_out_2 / 1.5,
        $layout->center_point->x - $center_off + $tan_in_1, $layout->center_point->y + $center_off + $tan_in_2,
        $layout->center_point->x - $center_off, $layout->center_point->y + $center_off
    )
    ->tag_segment(BezierSegmentFlags::MIRROR)
    ->cubic_to(
        $layout->center_point->x - $center_off - $tan_in_2, $layout->center_point->y + $center_off -  $tan_in_1,
        -$margin + $tan_out_2, $layout->center_point->y + $width_side / 2 - $tan_out_1,
        -$margin, $layout->center_point->y + $width_side / 2
    )
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->line_to(-$margin, $layout->center_point->y - $width_side / 2)
    ->tag_segment(BezierSegmentFlags::NORMAL)
    ->cubic_to(
        -$margin + $tan_out_2, $layout->center_point->y - $width_side / 2 + $tan_out_1,
        $layout->center_point->x - $center_off - $tan_in_2, $layout->center_point->y - $center_off + $tan_in_1,
        $layout->center_point->x - $center_off, $layout->center_point->y - $center_off
    )
    ->tag_segment(BezierSegmentFlags::MIRROR)
    ->cubic_to(
        $layout->center_point->x - $center_off + $tan_in_1, $layout->center_point->y - $center_off - $tan_in_2,
        $layout->center_point->x - $width_top / 2 + $tan_out_1, -$margin + $tan_out_2,
        $layout->center_point->x - $width_top / 2, -$margin
    )
    ->close();
;
