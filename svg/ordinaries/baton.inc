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


$base = min($layout->cross_point->x, $layout->cross_point->y) - 100;
$length = $base * M_SQRT2 * 2;
$width = 56;

$ordinary['shape_spec'] = (string)
    path_builder($layout->cross_point->x + $base, $layout->cross_point->y - $base)
    ->down_right_plain($width/2)
    ->down_left($length)
    ->up_left_plain($width)
    ->up_right($length)
    ->down_right_plain($width/2)
;
$ordinary['feature_size'] = 18;

