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
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 06/09/14
 * Time: 19:56
 */

$lineType = getModifierByType($node, 'linetype', false);
if ( $rev = getModifierNodeWithKeyterm($node,'reversed',true)) // reversed means inverted
    $rev->setAttribute('keyterm','inverted');

$builder_2 = path_builder($layout->cross_point->x, $layout->cross_point->y);
$builder_3 = path_builder($layout->cross_point->x, $layout->cross_point->y);
$margin = 100;
$up = (max($layout->cross_point->x, $layout->cross_point->y) + $margin)  * M_SQRT2;
$down = $layout->size->height - $layout->cross_point->y + $margin;
$side = $up / M_SQRT2;

if ( existModifierWithKeyterm($node, 'inverted') )
{
    $down = $layout->cross_point->y + $margin;
    $builder_2
        ->offset(false)
        ->down_right($up)
        ->up_plain($down + $side)
        ->left_plain($side)
        ->down($down)
    ;
    $builder_3
        ->offset(false)
        ->down_right($up)
        ->down($margin)
        ->left_plain(2 * $side)
        ->up($margin)
        ->up_right($up)
    ;
}
else
{
    $builder_2
        ->up_right($up)
        ->down_plain($down + $side)
        ->left_plain($side + $margin)
        ->up_plain($down)
        ->right_plain($margin)
    ;
    $builder_3
        ->up_left($up)
        ->down_plain($down + $side)
        ->right_plain($side)
        ->up($down)
    ;
}
$division['div2'] = createBody((string)$builder_2, $lineType, 60, $layout);
$division['div3'] = createBody((string)$builder_3, $lineType, 60, $layout);


