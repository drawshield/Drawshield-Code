<?php 
// TODO make the version with a chief smaller also

require_once("shield_shape_utils.inc");


$margin = 200;

if ($node->parentNode->parentNode->nodeName == 'overall')
    $margin  = 300;

if ( $layout->section == "quarter" )
    $shape = new RectShieldShape();
else
    $shape = ShieldShape::from_name($layout->shape);

$ordinary['body'] = $shape->svg_path($layout, $margin, $layout->section)->to_path();

$ordinary['chargesOn'] = $layout->rect_area()->margin($margin+(400-$margin)/2);
