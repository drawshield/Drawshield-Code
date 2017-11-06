<?php
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 08/09/14
 * Time: 22:18
 */

if ( existModifierWithKeyterm($node,'encircled')) {  // i.e. encircled
  $node->setAttribute('number','1'); // Always 3 in a single charge
  $charge['file'] = 'squirrel-circle.svg';
}
