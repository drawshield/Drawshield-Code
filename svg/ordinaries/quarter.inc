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

$margin = 100;
$ordinary['shape_spec'] = (string)path_builder(-$margin, -$margin)
    ->right_plain($layout->cross_point->x + $margin)
    ->down($layout->cross_point->y + $margin)
    ->left($layout->cross_point->x + $margin)
    ->up_plain($layout->cross_point->y + $margin)
;

$ordinary['if_ord_on'] = 'scale';
$area = new ShieldRect(new ShieldPos($margin, $margin), new ShieldSize ( $layout->cross_point->x - $margin - $margin,
    $layout->cross_point->y - $margin - $margin));
$ordinary['chargePlacement'] = (string)$area;

