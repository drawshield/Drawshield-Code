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
if ( existModifierWithKeyterm($node, 'inverted')) {
    $div2_data = array('NC' => 'V1X-100Y-100B848H848c1400e1200g1400', 'CH' => 'V1X-100Y100B848H848c1200e1200g1200');
    $div3_data = array('NC' => 'V1X-100Y400B848H848c900e1200g900', 'CH' => 'V1X-100Y500B848H848c1000e1200g1000');
} else {
    $y = $layout->center_point->y - 150 / M_SQRT2;
    $width = 300;
    $length = 2000;

    $builder_2 = path_builder($layout->center_point->x, $y - $width / M_SQRT2)
        ->chevron()
        ->down_right($length)
        ->left_plain($length * M_SQRT2)
        ->up_right($length)
    ;
    $builder_3 = path_builder($layout->center_point->x, $y + $width / M_SQRT2)
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


