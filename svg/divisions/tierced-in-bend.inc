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
if ( ($sinister = existModifierWithKeyterm($node, 'sinister'))) {
  $div2_data = array ( 'NC' => 'X-100Y800H1696c1700e1200g500', 'CH' => 'X-100Y1000H1696c1700e1200g500' );
  $div3_data = array ( 'NC' => 'X1100Y200c1100e1100H1696', 'CH' => 'X1100Y400c1100e1100H1696' );
} else {
  $div2_data = array ( 'NC' => 'X-100Y-100a1200c1500F1696g300', 'CH' => 'X-100Y100a1200c1500F1696g300' );
  $div3_data = array ( 'NC' => 'X200Y-100a1000c1000F1414',  'CH' => 'X200Y100a1000c1000F1414' );
}
$division['div2'] = createBody(choose_fit($div2_data), $lineType, 60);
$division['div3'] = createBody(choose_fit($div3_data), $lineType, 60);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $offset = chiefPresent() ? 200 : 0;
  for ($i = 1075; $i > -80; $i -= 80) {
    if ( $sinister ) {
      $rivetList .= '<g transform="translate(' . (630 - $i) . ',' . ($i  + $offset). ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (680 - $i) . ',' . ($i + 50 + $offset) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (930 - $i) . ',' . ($i +300 + $offset). ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (980 - $i) . ',' . ($i + 350 + $offset) . ')">' . $rivet . '</g>';
    } else {
      $rivetList .= '<g transform="translate(' . ($i + 350 - $offset) . ',' . $i . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i + 300 - $offset) . ',' . ($i + 50) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i + 50 - $offset) . ',' . ($i + 300) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i - $offset) . ',' . ($i + 350) . ')">' . $rivet . '</g>';
    }
  }
  $division['div4'] = $rivetList;
}



