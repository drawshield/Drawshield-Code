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

require_once("shield_shape_utils.inc");

$margin = -100;
$width = 130;

if ( !is_null($layout->section) && strpos($layout->section, "quarter") !== false )
{
    $width /= 1.3;
    $margin /= 2;
}

$shape = ShieldShape::from_layout($layout);

$path_builder = new MutableShape();

$shape->svg_path($layout, $margin, false, $path_builder);
$shape->svg_path($layout, $margin + $width, true, $path_builder);

$ordinary['shape_spec'] = $path_builder;


$ordinary['chargesWithin'] = $layout->rect_area()->margin(250, 0, 250, 350);
$ordinary['chargesOn'] = $shape->charge_placement($layout, 0, $width + $margin);
$ordinary['feature_size'] = 25;
