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

$div_data = array ( 'NC' => 'O1Q1X500Y500D848g1200B848   O1Q1X500Y500H848c1200F848', 'CH' => 'O1Q1X500Y800D848g1200B848   O1Q1X500Y800H848c1200F848' );
$div3Data = array ( 'NC' => 'O1Q1X500Y500B848c200e1200g200H848', 'CH' => 'O1Q1X500Y800B848e1200H848' );
$div4Data = array ( 'NC' => 'O1Q1X500Y500D848g1200B848', 'CH' => 'O1Q1X500Y800H848c1200F848' );
$division['div2'] = createBody(choose_fit($div_data), getModifierByType($node, 'linetype', false), 60);
$division['div3'] = createBody(choose_fit($div3Data), getModifierByType($node, 'linetype', false), 60);
$division['div4'] = createBody(choose_fit($div4Data), getModifierByType($node, 'linetype', false), 60);

