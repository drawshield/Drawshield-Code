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

if ( existModifierWithKeyterm($node,'rampant')) {
  $charge['file'] = 'inkscape/bear-rampant.svg';
} else { // default bear
  $charge['file'] = 'inkscape/bear-passant.svg';
  existModifierWithKeyterm($node,'passant',true); // mark used
}
