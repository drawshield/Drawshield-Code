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

$line_type = getModifierByType($node, 'linetype', false);
$margin = 100;
$arm = max($layout->center_point->x, $layout->center_point->y) + $margin;
$side = $arm * M_SQRT2;

$builder_2 = path_builder($layout->center_point->x, $layout->center_point->y)->offset(false)->quarterly();
$builder_3 = path_builder($layout->center_point->x, $layout->center_point->y)->offset(false)->quarterly();
$builder_4 = path_builder($layout->center_point->x, $layout->center_point->y)->offset(false)->quarterly();


$builder_2->up_right($arm)->down_plain($side)->up_left($arm);
$builder_2->move($layout->center_point->x, $layout->center_point->y)->offset(false)->quarterly();
$builder_2->up_left($arm)->down_plain($side)->up_right($arm);
$builder_3->down_right($arm)->left_plain($side)->up_right($arm);
$builder_4->up_right($arm)->down_plain($side)->up_left($arm);



// $div3Data = array ( 'NC' => 'O1Q1X500Y500B848c200e1200g200H848', 'CH' => 'O1Q1X500Y800B848e1200H848' );
// $div4Data = array ( 'NC' => 'O1Q1X500Y500D848g1200B848', 'CH' => 'O1Q1X500Y800H848c1200F848' );
$division['div2'] = createBody((string)$builder_2, $line_type, 60, $layout);
$division['div3'] = createBody((string)$builder_3, $line_type, 60, $layout);
$division['div4'] = createBody((string)$builder_4, $line_type, 60, $layout);

