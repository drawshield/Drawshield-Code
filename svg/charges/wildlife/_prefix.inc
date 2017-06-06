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
 * Time: 14:47
 */

// If head only, switch to appropriate SVG file
if ( existModifierWithKeyterm($node, 'head-only' )) {
  $file =  'inkscape/' . $chargeKey . '-head.svg';
  if ( file_exists($folder . $file)) {
    existModifierWithKeyterm($node,'head-only', true); // mark as used
    $charge['file'] = $file;
  }
}
