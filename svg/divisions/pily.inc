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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 06/09/14
 * Time: 19:56
 */


$top = $layout->division_offset->y;
$bottom = $layout->size->height;

$builder = new MutableShape();
$points = [
    [125,800],
    [250,0],
    [375,1100],
    [500,0],
    [625,1100],
    [750,0],
    [875,800],
    [1000,0],
];

$builder->move_to(0, $bottom);
$builder->tag_segment(BezierSegmentFlags::PLAIN);
$builder->line_to(0, 0);
foreach ( $points as $p )
{
    $x = $p[0];
    $y = $p[1]/1200 * ($bottom - $top) + $top;
    $builder->tag_segment(BezierSegmentFlags::NORMAL);
    $builder->line_to($x,$y);
}

$builder->tag_segment(BezierSegmentFlags::PLAIN);
$builder->line_to($layout->size->width+100, $bottom+100);
$builder->close();

$lineType = getModifierByType($node, 'linetype', false);
$division['div2'] = createBody($builder, $lineType, 30, $layout);
