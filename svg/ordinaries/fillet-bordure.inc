<?php 

require_once("shield_shape_utils.inc");

$margin = -100;
$width = 130;

if ( strpos($layout->section, "quarter") !== false )
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
