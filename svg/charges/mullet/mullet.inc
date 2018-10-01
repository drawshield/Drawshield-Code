<?php
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 12/08/14
 * Time: 18:50
 */

$suffix = '';
if (existModifierWithKeyterm($node,'pierced')) {
  $suffix = '-pierced';
}

$num_of = getModifierValueByKeyterm($node, 'of-num', true);
if ( $num_of < 4 )
  $num_of = 5;
elseif ( $num_of > 10 )
  $num_of = 10;

$charge['file'] = 'mullet-of' . $num_of . $suffix . '.svg';

