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
 * Time: 21:12
 */


$charge['file'] = chose_by_modifier($node, $folder, $chargeKey, array ( 
		'whole-tree' => 'tree',
		'leaf-only' => 'leaf',
		'eradicated' => null
	));
