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
    along with Foobar.  If not, see https://www.gnu.org/licenses/. */

$width = 100;
$angle = atan2($layout->cross_point->y, $layout->cross_point->x);
$length = hypot($layout->cross_point->y, $layout->cross_point->x);

$builder = new MutableShape();

$dx = $width / 2 / sin($angle);
$dy = cos($angle) * $width / 2;
$fy = $width/2;
$fx = -($dy - $fy) / tan($angle);

$builder->move_to(-$dx, 0)
    ->line_to($layout->cross_point->x - $dx, $layout->cross_point->y)
    ->line_to(-$dx, $layout->size->height)
    ->line_to($fx, $layout->size->height + $fy)
    ->line_to($layout->cross_point->x + $fx, $layout->cross_point->y + $fy)
    ->line_to($layout->size->width + 100, $layout->cross_point->y + $fy)
    ->line_to($layout->size->width + 100, $layout->cross_point->y - $fy)
    ->line_to($layout->cross_point->x + $fx, $layout->cross_point->y - $fy)
    ->line_to($fx, -$fy)
;

$ordinary['shape_spec'] = $builder;
