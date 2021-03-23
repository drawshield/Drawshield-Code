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

$y = $layout->center_point->y - 150 / M_SQRT2;
$extent = max($layout->size->width, $layout->size->height);

$div_data = path_builder($layout->center_point->x, $y)->chevron();

if ( existModifierWithKeyterm($node, 'inverted') )
    $div_data->up_right($extent)->left_plain($extent*M_SQRT2)->down_right($extent);
else
    $div_data->down_right($extent)->left_plain($extent*M_SQRT2)->up_right($extent);


$division['div2'] = createBody((string)$div_data, $lineType, 60, $layout);


if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $ref = $y + -25;
  for ($i = 5; $i < 450; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',' . (455 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . ($i + 40) . ',' . (495 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . (970 - $i) . ',' . (455 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . (930 - $i) . ',' . (495 + $ref) . ')">' . $rivet . '</g>';
    $ref -= 80;
  }
  $division['divx'] = $rivetList;
}

