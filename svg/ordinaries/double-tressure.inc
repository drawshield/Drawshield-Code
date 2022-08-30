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

require_once("shield_shape_utils.inc");

$margin = 24.2;
$width = 40.4;
$gap = $margin;
$flory_size = 20;

if ( $layout->shape == "flag" )
{
    $width *= 0.8;
    $gap *= 0.8;
    $margin *= 0.7;
    $flory_size *= 0.7;

}

if (!is_null($layout->section) &&  strpos($layout->section, "quarter") !== false )
{
    $width *= 1.3;
    $gap *= 1.5;
    $margin *= 1.3;
}


$flory = existModifierWithKeyterm($node,'flory-counter-flory');
if ( $flory )
    $margin += $flory_size * 3;

$shape = ShieldShape::from_layout($layout);

$path_builder = new MutableShape();

$shape->svg_path($layout, $margin, false, $path_builder);
$shape->svg_path($layout, $margin + $width, true, $path_builder);
$path_builder->last_path()->make_plain();
$shape->svg_path($layout, $gap + $margin + $width, false, $path_builder);
$path_builder->last_path()->make_plain();
$shape->svg_path($layout, $gap + $margin + 2 * $width, true, $path_builder);

if ( $flory )
{
    $min_flories = 1;

    // tag heater tip
    if ( $shape instanceof HeaterShieldShape && !$layout->section )
    {
        $path_builder->sub_paths()[0]->segment_tags[2] = BezierSegmentFlags::NORMAL;
        $path_builder->sub_paths()[3]->segment_tags[3] = BezierSegmentFlags::NORMAL;

        if ( $layout->shape == "french" )
        {
            $path_builder->sub_paths()[0]->segment_tags[1] = BezierSegmentFlags::NORMAL;
            $path_builder->sub_paths()[0]->segment_tags[3] = BezierSegmentFlags::NORMAL;
            $path_builder->sub_paths()[3]->segment_tags[4] = BezierSegmentFlags::NORMAL;
            $path_builder->sub_paths()[3]->segment_tags[2] = BezierSegmentFlags::NORMAL;
            $min_flories = 0;
        }
    }

    $edge = EdgeType::load("flory-counter-flory");
    $flory = new MutableShape();
    $edge->apply($path_builder->compile(), $flory, 400, $flory_size, $min_flories);
    $ordinary['body'] = $flory->to_svg_path();
}
else
{
    $ordinary['shape_spec'] = $path_builder;
}

$ordinary['chargesWithin'] = $layout->rect_area()->relative_box(0.5, 0.45, 0.5, 0.5);



if ( existModifierWithKeyterm($node,'compony1') )
{
    $ordinary['body2'] = $shape->compony($layout, $margin, $width)->to_svg_path();
}


$ordinary['feature_size'] = $width / 2;
