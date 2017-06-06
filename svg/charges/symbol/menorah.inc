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


$charge['file'] = 'inkscape/menorah-of-7.svg';

if ( getModifierValueByKeyterm($node, 'of-num') == 9) {
  $charge['file'] = 'inkscape/menorah-of-9.svg';
}
