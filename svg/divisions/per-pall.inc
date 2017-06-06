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
if ( existModifierWithKeyterm($node, 'inverted') ) {
  $div2_data = array ( 'NC' => 'O1X500Y0a500c1100f707G600', 'CH' => 'O1X500Y200a500c800f707G400' );
  $div3_data = array ( 'NC' => 'O1X0Y1000H708B707c200e1000g200', 'CH' => 'O1X0Y1000H708B707c200e1000g200' );
} else {
  $div2_data = array ( 'NC' => 'X1000Y0c1200e600g700a100H707', 'CH' => 'X1000Y300c900e600g400a100H707' );
  $div3_data = array ( 'NC' => 'X0Y0B707C700e500g1200', 'CH' => 'X0Y300B707C400e500g900' );
}
$division['div2'] = createBody(choose_fit($div2_data), $lineType, 60);
$division['div3'] = createBody(choose_fit($div3_data), $lineType, 60);


