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

require_once(__dir__."/../ordinaries/shield_shape_utils.inc");

$num_of = (int)getModifierValueByKeyterm($node, 'ofnum', false);
if ( $num_of == null )
    $num_of = 8;
else if ( $num_of < 6 )
    $num_of = 6;
else if ( $num_of % 2 )
    $num_of += 1;

$builder = new MutableShape();


if ( $num_of == 8 && ShieldShape::from_layout($layout) instanceof RectShieldShape )
{
    $margin = 100;
    $builder->move_to($layout->cross_point->x, $layout->cross_point->y)

        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->line_to($layout->cross_point->x, -$margin)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->line_to($layout->size->width, 0)
        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->close()

        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->line_to($layout->size->width+$margin, $layout->cross_point->y)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->line_to($layout->size->width, $layout->size->height)
        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->close()

        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->line_to($layout->cross_point->x, $layout->size->height + $margin)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->line_to(0, $layout->size->height)
        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->close()

        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->line_to(-$margin, $layout->cross_point->y)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->line_to(0, 0)
        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->close()
    ;
}
else
{
    $angle = M_PI * 2 / $num_of;
    $radius = 2000;
    for ( $i = 0; $i < $num_of; $i += 2 )
    {
        $a1 = $angle * $i;
        $a2 = $angle * ($i+1);
        $builder->move_to($layout->cross_point->x, $layout->cross_point->y)
            ->line_to($layout->cross_point->x + $radius * sin($a1), $layout->cross_point->y - $radius * cos($a1))
            ->tag_segment(BezierSegmentFlags::PLAIN)
            ->line_to($layout->cross_point->x + $radius * sin($a2), $layout->cross_point->y - $radius * cos($a2))
            ->tag_segment(BezierSegmentFlags::NORMAL)
            ->close();
        ;
    }
}

$lineType = getModifierByType($node, 'linetype', false);
$division['div2'] = createBody($builder, $lineType, 40, $layout);
