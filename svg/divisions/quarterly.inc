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

$margin = 100;
$cx = $layout->cross_point->x;
$cy = $layout->cross_point->y;
$down = $layout->size->height - $layout->cross_point->y + $margin;
$right = $layout->size->width - $layout->cross_point->x + $margin;
$up = $cy + $margin;
$left = $cx + $margin;
$modifier = getModifierByType($node, 'linetype', false);

$division['div2'] = createBody(
    (string)(new PathBuilder)
        ->move($cx, $cy)
        ->offset(false)
        ->quarterly()
        ->down($down)
        ->left_plain($left)
        ->up_plain($down)
        ->right($left)

        ->move($cx, $cy)
        ->offset(false)
        ->quarterly()
        ->up($up)
        ->right_plain($right)
        ->down_plain($up)
        ->left($right)
    ,
    $modifier, 60, $layout
);
$division['div3'] = createBody(
    (string)path_builder($cx, $cy)
        ->offset(false)
        ->quarterly()
        ->down($down)
        ->right_plain($right)
        ->up_plain($down)
        ->left($right)
    ,
    $modifier, 60, $layout
);

$division['div4'] = createBody(
    (string)path_builder($cx, $cy)
        ->offset(false)
        ->quarterly()
        ->down($down)
        ->left_plain($left)
        ->up_plain($down)
        ->right($left)
    ,
    $modifier, 60, $layout
);
