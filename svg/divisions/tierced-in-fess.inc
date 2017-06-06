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
$div2_data = array ( 'NC' => 'X0Y400A1000c800e1000g800',  'CH' => 'X0Y600A1000c600e1000g600' );
$div3_data = array ( 'NC' => 'X0Y800A1000c400e1000g400',  'CH' => 'X0Y900A1000c300e1000g300' );
$division['div2'] = createBody(choose_fit($div2_data), $lineType, 60);
$division['div3'] = createBody(choose_fit($div3_data), $lineType, 60);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',365)">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . $i . ',415)">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . $i . ',765)">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . $i . ',815)">' . $rivet . '</g>';
  }
  $division['div4'] = $rivetList;
}



