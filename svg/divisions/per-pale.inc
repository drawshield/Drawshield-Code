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

$div_data = array ( 'NC' => 'X500Y0a500c1200e500G1200', 'CH' => 'X500Y200a500c1100e500G1100' );
$division['div2'] = createBody(choose_fit($div_data), getModifierByType($node, 'linetype'), 60);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1200; $i += 80) {
    $rivetList .= '<g transform="translate(465,' . $i . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(515,' . $i . ')">' . $rivet . '</g>';
  }
  $division['div4'] = $rivetList;
}



