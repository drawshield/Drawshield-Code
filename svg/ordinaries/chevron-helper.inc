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


function build_chevron(ShieldLayout $layout, $width, $offset, PathBuilder $builder, $y_align = 0)
{
    $extent = max($layout->size->width, $layout->size->height);

    return $builder
        ->move($layout->cross_point->x, $layout->cross_point->y + $offset + $y_align * $width * M_SQRT2)
        ->chevron()
        ->down_left($extent)
        ->up_left_plain($width)
        ->up_right($extent + $width)
        ->down_right($extent + $width)
        ->down_left_plain($width)
        ->up_left($extent);
}


function chevron_width(ShieldLayout $layout, $number)
{
    return min(200, $layout->cross_point->y / ($number+0.5));
}
