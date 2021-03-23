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
$width = 200;

if ( strpos($layout->section, "quarter") !== false )
{
    $width /= 1.3;
    $margin /= 2;
}

if ( $layout->section == "quarter" )
    $shape = new RectShieldShape();
else
    $shape = ShieldShape::from_name($layout->shape);

$path_builder = new SvgPathBuilder();

$shape->svg_path($layout, $margin, $layout->section, false, $path_builder);
$shape->svg_path($layout, $margin + $width, $layout->section, true, $path_builder);

$ordinary['body'] = $path_builder->to_path();
