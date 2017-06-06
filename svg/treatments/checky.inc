<?php
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 13/09/14
 * Time: 00:04
 */

  // Checky is rotated if on some ordinaries
  if ( $parent->nodeName == 'ordinary' ) {
        $ord_type = $parent->getAttribute('keyterm');
        if ( $ord_type == 'saltire' or substr($ord_type,0,6) == 'chevron' ) {
                $filename = 'checky45.svg';
        }
  }
