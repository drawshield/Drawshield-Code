<?php 

include 'svg/ordinaries/bar.inc';
if ( existModifierWithKeyterm($node,'couped') ) {
    $width = 200;
    $ordinary['shape_spec'] = (string)build_bend($layout, $width, 200);
}
require_once("bend-helper.inc");

$ordinary['chargesWithin'] = bend_within($layout, 200);
$ordinary['chargesOn'] = placement_in_bend($layout->rect_area(), false, 45, 0.17);
