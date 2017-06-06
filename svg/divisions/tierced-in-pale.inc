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

$lineType = getModifierByType($node, 'linetype');
$div2_data = array ( 'NC' => 'X333Y0a667c1200e667G1200',  'CH' => 'X333Y200a667c1100e667G1100' );
$div3_data = array ( 'NC' => 'X666Y0a337c1200e337G1200',  'CH' => 'X666Y200a337c1100e337G1100' );
$division['div2'] = createBody(choose_fit($div2_data), $lineType, 60);
$division['div3'] = createBody(choose_fit($div3_data), $lineType, 60);


if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1200; $i += 80) {
    $rivetList .= '<g transform="translate(298,' . $i . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(348,' . $i . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(631,' . $i . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(681,' . $i . ')">' . $rivet . '</g>';
  }
  $division['div4'] = $rivetList;
}


