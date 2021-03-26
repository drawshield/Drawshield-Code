<?php 
// TODO make the version with a chief smaller also

require_once("shield_shape_utils.inc");


$margin = 200;

if ($node->parentNode->parentNode->nodeName == 'overall')
    $margin  = 300;

$shape = ShieldShape::from_name($layout->shape);

$ordinary['body'] = $shape->svg_path($layout, $margin)->to_path();

$ordinary['chargesOn'] = $layout->rect_area()->margin($margin+(400-$margin)/2);
