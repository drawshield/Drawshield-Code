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
// TODO make the version with a chief smaller also

require_once("shield_shape_utils.inc");


$margin = 200;

if (!is_null($node->parentNode->parentNode) && $node->parentNode->parentNode->nodeName == 'overall')
    $margin  = 300;

$shape = ShieldShape::from_layout($layout);

$ordinary['body'] = $shape->svg_path($layout, $margin)->to_svg_path();

$ordinary['chargesOn'] = ["w", $layout->rect_area()->margin($margin+(400-$margin)/2)];
