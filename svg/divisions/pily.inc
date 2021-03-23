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
    [0,0],
    [125,800],
    [250,0],
    [375,1100],
    [500,0],
    [625,1100],
    [750,0],
    [875,800],
    [1000,0],
    [1000,1200]
];

$div_data = "<path d='M 0, $bottom L";
foreach ( $points as $p )
{
    $x = $p[0];
    $y = $p[1]/1200 * ($bottom - $top) + $top;
    $div_data .= " $x,$y";
}
$div_data .=  "' />";

$division['div2'] = $div_data;
