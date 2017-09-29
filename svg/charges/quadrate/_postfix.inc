<?php
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 11/09/14
 * Time: 21:41
 */


$charge['file'] = chose_by_modifier($node, $folder, $chargeKey, array ( 
		'pierced' => null,
		'fitchy-foot' => null,
		'fitchy' => null,
		'voided' => null
	));

$charge['charged-with'] = 'c/c/c/500,200,200,200:200,500:800,500:500,800/c/c'; // special placement for 4 things on us, otherwise just use normal layout


