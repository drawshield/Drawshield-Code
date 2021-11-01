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

$builder = new MutableShape();
$builder->move_to($layout->size->width / 2, $layout->size->height)
    ->tag_segment(BezierSegmentFlags::NORMAL)
    ->line_to($layout->size->width, $layout->size->height * .134)
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->line_to($layout->size->width + 100, $layout->size->height + 100)
     ->line_to(-100, $layout->size->height + 100)
    ->line_to(0, $layout->size->height * .134)
    ->tag_segment(BezierSegmentFlags::NORMAL)
    ->line_to($layout->size->width / 2, $layout->size->height)
    ->close();

// $division['div2'] = subPercent('<path d="m500,{%100} l500,-866 0,866 -1000,0 0,-866 500,866z" />', $layout);

$lineType = getModifierByType($node, 'linetype', false);
$division['div2'] = createBody($builder, $lineType, 30, $layout);
