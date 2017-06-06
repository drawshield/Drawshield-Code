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

$div_data = array ( 'NC' => 'V1X0Y0a1000c500F708D707g500', 'CH' => 'V1X0Y300a1000c500F708D707g500' );
$division['div2'] = createBody(choose_fit($div_data), getModifierByType($node, 'linetype'), 60);


