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


foreach ( $node->childNodes as $child ) {
  if ( $child->nodeName == 'tincture') {
    $child->setAttribute('origin', 'implied');
  }
}
 // TODO bring rainbow in here for colouring
