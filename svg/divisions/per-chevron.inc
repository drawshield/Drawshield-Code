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
if ( $rev = getModifierNodeWithKeyterm($node,'reversed',true)) // reversed means inverted
  $rev->setAttribute('keyterm','inverted');
if ( existModifierWithKeyterm($node, 'inverted') )
  $div_data = array ( 'NC' => 'V1X-100Y300a100B707H708c900a100e1200g900', 'CH' => 'V1X-100Y500a100B707H708c900a100e1200g900' );
else
  $div_data = array (  'NC' => 'V1X500Y500B848c500e1200g500H848', 'CH' => 'V1X0Y1275g200H707B708c200e1000' );

$division['div2'] = createBody(choose_fit($div_data), $lineType, 60);


if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $ref = chiefPresent() ? 545 : 475;
  for ($i = 5; $i < 450; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',' . (455 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . ($i + 40) . ',' . (495 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . (970 - $i) . ',' . (455 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . (930 - $i) . ',' . (495 + $ref) . ')">' . $rivet . '</g>';
    $ref -= 80;
  }
  $division['div4'] = $rivetList;
}

