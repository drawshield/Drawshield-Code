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

$div_data = array (
  'per-bend' => array ( 'NC' => 'X-106Y-106B1714c100e1212g1312', 'CH' => 'X0Y300B1414e1000g900'),
  'per-bend-fracted' => array ( 'NC' => 'X200Y0B717e500B1131c100e1010g1410', 'CH' => 'X200Y300B717e500B717e1010g1310' ),
  'per-bend-sinister' => array ( 'NC' => 'X1000Y0c1200e1000g200H1414', 'CH' => 'X1000Y300c1000e1000H1414' ),
  'per-bend-sinister-fracted' => array ( 'NC' => 'X800Y0a200c1300e1000H1131e500H717', 'CH' => 'X800Y300a200c1000e800H717e500H717' ),
);

$item = 'per-bend';
if ( existModifierWithKeyterm($node, 'sinister')) $item .= '-sinister';
if ( existModifierWithKeyterm($node, 'fracted')) $item .= '-fracted';
$division['div2'] = createBody(choose_fit($div_data[$item]),getModifierByType($node, 'linetype'),60);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $offset = chiefPresent() ? 300 : 0;
  for ($i = 995; $i > 0; $i -= 80) {
    if ( $item == 'per-bend-sinister' ) {
      $rivetList .= '<g transform="translate(' . (930 - $i) . ',' . ($i  + $offset). ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (980 - $i) . ',' . ($i + 50 + $offset) . ')">' . $rivet . '</g>';
    } else {
      $rivetList .= '<g transform="translate(' . ($i + 50 - $offset) . ',' . $i . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i - $offset) . ',' . ($i + 50) . ')">' . $rivet . '</g>';
    }
  }
  $division['div4'] = $rivetList;
}

