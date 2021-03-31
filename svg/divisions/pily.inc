<?php
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
