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
