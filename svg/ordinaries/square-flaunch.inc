<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see https://www.gnu.org/licenses/.
 */


$builder = new MutableShape();

$margin = 8;
$w = $layout->size->width / 3;
$h = $layout->cross_point->y * 2;
$builder
    ->move_to(-$margin, -$margin)
    ->line_to($w, $layout->cross_point->y)
    ->line_to(-$margin, $h + $margin)
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->close()
;

$charge = new ShieldRect(new ShieldPos($w / 3, $layout->cross_point->y), new ShieldSize($w * 2/3, $h/3));
$ordinary['chargesOn'] = (string)$charge;

if ( $node->getAttribute('number') == '2')
{
    $ordinary['chargesOn'] .= "/$charge:" . $charge->translated($layout->size->width - $charge->size->width, 0);
    $builder
        ->move_to($layout->size->width + $margin, -$margin)
        ->line_to($layout->size->width * 2 / 3, $layout->cross_point->y)
        ->line_to($layout->size->width + $margin, $layout->cross_point->y * 2 + $margin)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->close()
    ;
}

$ordinary['shape_spec'] = $builder;

// $ordinary['chargesWithin'] = array ('n','300,300,400,600');
