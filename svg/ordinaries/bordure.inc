<?php /* Copyright 2010 Karl R. Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */


require_once("shield_shape_utils.inc");

$margin = -100;
$width = 180;

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



if ( existModifierWithKeyterm($node,'compony1') )
{
    $ordinary['body2'] = $shape->compony($layout, 0, $width + $margin)->to_svg_path();
}
elseif (existModifierWithKeyterm($node,'compony2') )
{
    $body2 = $shape->compony($layout, 0, $width + $margin);
    $shape->svg_path($layout, ($width + $margin) / 2, true, $body2);
    $ordinary['body2'] = $body2->to_svg_path();
}

$ordinary['chargesWithin'] = $layout->rect_area()->margin(250, 0, 250, 350);
$ordinary['chargesOn'] = $shape->charge_placement($layout, 0, $width + $margin);
$ordinary['feature_size'] = 30;
