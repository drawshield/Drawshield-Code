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
$div2_data = 'X333Y0a667c{%100*}e667G{%100*}';
$div3_data = 'X666Y0a337c{%100*}e337G{%100*}';
$division['div2'] = createBody($div2_data, $lineType, 60, $layout);
$division['div3'] = createBody($div3_data, $lineType, 60, $layout);


if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1200; $i += 80) {
    $rivetList .= '<g transform="translate(298,' . $i . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(348,' . $i . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(631,' . $i . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(681,' . $i . ')">' . $rivet . '</g>';
  }
  $division['divx'] = $rivetList;
}


