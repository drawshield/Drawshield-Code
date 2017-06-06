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

if ( existModifierWithKeyterm($node,'head-only'))
  $charge['file'] = 'inkscape/horse-head.svg';
else {
  $charge['file'] = 'inkscape/horse-rampant.svg';
  existModifierWithKeyterm($node, 'rampant'); // Set as used
  // TODO horses in different poses!
}