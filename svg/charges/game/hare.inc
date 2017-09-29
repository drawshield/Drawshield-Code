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
 * Time: 22:05
 */

if ( existModifierWithKeyterm($node,'courant'))
  $charge['file'] = 'hare-courant.svg';
  elseif ( existModifierWithKeyterm($node,'sejant'))
    $charge['file'] = 'hare-sejant.svg';
elseif ( existModifierWithKeyterm($node,'salient'))
  $charge['file'] = 'hare-salient.svg';
else {
  $charge['file'] = 'hare-statant.svg';
  existModifierWithKeyterm($node, 'statant'); // Set as used
}