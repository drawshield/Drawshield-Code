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

require_once(__dir__ . "/../ordinaries/chevron-helper.inc");

$lineType = getModifierByType($node, 'linetype', false);


$width = 300;
$length = 2000;

if ( existModifierWithKeyterm($node, 'inverted'))
{
    $y = $layout->cross_point->y;

    $builder_2 = path_builder($layout->cross_point->x, $y + $width * M_SQRT2)
        ->chevron()
        ->up_right($length)
        ->left_plain($length * M_SQRT2)
        ->down_right($length)
    ;
    $builder_3 = path_builder($layout->cross_point->x, $y)
        ->chevron()
        ->up_right($length)
        ->left_plain($length * M_SQRT2)
        ->down_right($length)
    ;
}
else
{
    $ordinary_width = chevron_width($layout, 1);;
    $y = $layout->cross_point->y - $ordinary_width / M_SQRT2;

    $builder_2 = path_builder($layout->cross_point->x, $y - $width / M_SQRT2)
        ->chevron()
        ->down_right($length)
        ->left_plain($length * M_SQRT2)
        ->up_right($length)
    ;
    $builder_3 = path_builder($layout->cross_point->x, $y + $width / M_SQRT2)
        ->chevron()
        ->down_right($length)
        ->left_plain($length * M_SQRT2)
        ->up_right($length)
    ;
}

$division['div2'] = createBody((string)$builder_2, $lineType, 60, $layout);
$division['div3'] = createBody((string)$builder_3, $lineType, 60, $layout);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $ref = $y + 150;
  for ($i = 5; $i < 450; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',' . (155 + $ref) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (970 - $i) . ',' . (155 + $ref) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . $i . ',' . (55 + $ref) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (970 - $i) . ',' . (55 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . ($i + 20) . ',' . (465 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . (950 - $i) . ',' . (465 + $ref) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i + 20) . ',' . (575 + $ref) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (950 - $i) . ',' . (575 + $ref) . ')">' . $rivet . '</g>';
    $ref -= 80;
  }
    $rivetList .= '<g transform="translate(485,'.($ref+80).')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(485,'.($ref+180).')">' . $rivet . '</g>';
    $division['divx'] = $rivetList;
}


