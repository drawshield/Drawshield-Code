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

$builder = new SvgPathBuilder();
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
    [1000,1200]
];

$sinister = existModifierWithKeyterm($node, 'sinister');

$x = $sinister ? $layout->size->width : 0;
$div_data = "<path d='M $x, $bottom L";
foreach ( $points as $p )
{
    $x = $p[0];
    if ( $sinister )
        $x = $layout->size->width - $x;
    $y = $p[1]/1200 * ($bottom - $top) + $top;
    $div_data .= " $x,$y";
}
$div_data .=  "' />";

$division['div2'] = $div_data;


