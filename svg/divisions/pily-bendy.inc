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
    [0,923],
    [241,1111],
    [0,750],
    [564,1184],
    [0,500],
    [750,1121],
    [0,170],
    [962,956],
    [180,0],
    [1000,619],
    [503,0],
    [1000,384],
    [750,0],
    [1000,175],
    [902,0],
    [1000,0],
];

$sinister = existModifierWithKeyterm($node, 'sinister');

$x = $sinister ? $layout->size->width : 0;
$builder->move_to($x, $bottom);
foreach ( $points as $p )
{
    $x = $p[0];
    if ( $sinister )
        $x = $layout->size->width - $x;
    $y = $p[1]/1200 * ($bottom - $top) + $top;
    $builder->tag_segment(BezierSegmentFlags::NORMAL);
    $builder->line_to($x, $y);
}

$builder->tag_segment(BezierSegmentFlags::PLAIN);
$builder->line_to($layout->size->width+100, $bottom+100);
$builder->close();

$lineType = getModifierByType($node, 'linetype', false);
$division['div2'] = createBody($builder, $lineType, 30, $layout);


