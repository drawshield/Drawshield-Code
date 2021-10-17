<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see https://www.gnu.org/licenses/.
 */


$ordinary['shape_spec'] = (new MutableShape())
    ->move_to($layout->cross_point->x, 0)
    ->tag_segment(BezierSegmentFlags::SYMMETRICAL)
    ->line_to($layout->size->width, $layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::SYMMETRICAL)
    ->line_to($layout->cross_point->x, $layout->size->height)
    ->tag_segment(BezierSegmentFlags::SYMMETRICAL)
    ->line_to(0, $layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::SYMMETRICAL)
    ->close()
;


