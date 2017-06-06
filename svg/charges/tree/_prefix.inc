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

if ( $chargeKey != 'tree' and existModifierWithKeyterm($node,'whole-tree') and file_exists($folder . 'inkscape/' . $chargeKey . '-tree.svg')) {
  $charge['file'] =  'inkscape/' . $chargeKey . '-tree.svg';
}

if ( existModifierWithKeyterm($node,'leaf-only') and file_exists($folder . 'inkscape/' . $chargeKey . '-leaf.svg')) {
  $charge['file'] = 'inkscape/' . $chargeKey . '-leaf.svg';
}


