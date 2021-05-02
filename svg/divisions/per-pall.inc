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

$lineType = getModifierByType($node, 'linetype', false);
if ( $rev = getModifierNodeWithKeyterm($node,'reversed',true)) // reversed means inverted
    $rev->setAttribute('keyterm','inverted');

$builder_2 = path_builder($layout->cross_point->x, $layout->cross_point->y);
$builder_3 = path_builder($layout->cross_point->x, $layout->cross_point->y);
$margin = 100;
$up = (max($layout->cross_point->x, $layout->cross_point->y) + $margin)  * M_SQRT2;
$down = $layout->size->height - $layout->cross_point->y + $margin;
$side = $up / M_SQRT2;

if ( existModifierWithKeyterm($node, 'inverted') )
{
    $down = $layout->cross_point->y + $margin;
    $builder_2
        ->offset(false)
        ->down_right($up)
        ->up_plain($down + $side)
        ->left_plain($side)
        ->down($down)
    ;
    $builder_3
        ->offset(false)
        ->down_right($up)
        ->left_plain(2 * $side)
        ->up_right($up)
    ;
}
else
{
    $builder_2
        ->up_right($up)
        ->down_plain($down + $side)
        ->left_plain($side + $margin)
        ->up_plain($down)
        ->right_plain($margin)
    ;
    $builder_3
        ->up_left($up)
        ->down_plain($down + $side)
        ->right_plain($side)
        ->up($down)
    ;
}
$division['div2'] = createBody((string)$builder_2, $lineType, 60, $layout);
$division['div3'] = createBody((string)$builder_3, $lineType, 60, $layout);


