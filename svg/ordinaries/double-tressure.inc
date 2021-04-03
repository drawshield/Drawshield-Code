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

if ( strpos($layout->section, "quarter") !== false )
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
