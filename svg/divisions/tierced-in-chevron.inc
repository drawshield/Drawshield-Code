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
$div2_data = array ( 'NC' => 'V1X-100Y1200g500H848B849c500e1200', 'CH' => 'V1X-100Y1300g400H848B849c400e1200' );
$div3_data = array ( 'NC' => 'V1X-100Y1300g100H848B849c100e1200', 'CH' => 'V1X-100Y1400g100H848B849c100e1200' );
$division['div2'] = createBody(choose_fit($div2_data), $lineType, 60);
$division['div3'] = createBody(choose_fit($div3_data), $lineType, 60);

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


