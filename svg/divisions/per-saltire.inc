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

$line_type = getModifierByType($node, 'linetype', false);
$margin = 100;
$arm = (max($layout->cross_point->x, $layout->cross_point->y) + $margin) * M_SQRT2;
$side = $arm * M_SQRT2;

$builder_2 = path_builder($layout->cross_point->x, $layout->cross_point->y)->offset(false)->quarterly();
$builder_3 = path_builder($layout->cross_point->x, $layout->cross_point->y)->offset(false)->quarterly();
$builder_4 = path_builder($layout->cross_point->x, $layout->cross_point->y)->offset(false)->quarterly();


$builder_2->up_right($arm)->down_plain($side)->up_left($arm);
$builder_2->move($layout->cross_point->x, $layout->cross_point->y)->offset(false)->quarterly();
$builder_2->up_left($arm)->down_plain($side)->up_right($arm);
$builder_3->down_right($arm)->left_plain($side)->up_right($arm);
$builder_4->up_right($arm)->down_plain($side)->up_left($arm);


$division['div2'] = createBody((string)$builder_2, $line_type, 60, $layout);
$division['div3'] = createBody((string)$builder_3, $line_type, 60, $layout);
$division['div4'] = createBody((string)$builder_4, $line_type, 60, $layout);

