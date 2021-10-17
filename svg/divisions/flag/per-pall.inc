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
$margin = 100;
$builder_2 = new MutableShape();
$builder_2->move_to(0, 0)
    ->line_to($layout->cross_point->x, $layout->cross_point->y)
    ->horizontal($layout->size->width + $margin)
    ->vertical(-$margin)
    ->horizontal(-$margin)
    ->close()
;

$builder_3 = new MutableShape();
$builder_3->move_to(0, $layout->size->height)
    ->line_to($layout->cross_point->x, $layout->cross_point->y)
    ->horizontal($layout->size->width + $margin)
    ->vertical($layout->size->height + $margin)
    ->horizontal(-$margin)
    ->close()
;

$lineType = getModifierByType($node, 'linetype', false);
$division['div2'] = createBody($builder_2, $lineType, 30, $layout);
$division['div3'] = createBody($builder_3, $lineType, 30, $layout);
