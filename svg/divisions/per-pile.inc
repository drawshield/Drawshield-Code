<?php
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 06/09/14
 * Time: 19:56
 */

if ( existModifierWithKeyterm($node, 'reversed') or  existModifierWithKeyterm($node, 'reversed') ) {
  $division['div2'] = '<polygon points="0,1400 500,0 1000,1400" />';
  $division['div3'] = '<polygon points="500,-100 500,0 1000,1400 1100,1400 1100,-100" />';
} else {
  $division['div2'] = '<polygon points="0,-200 500,1200 1000,-200" />';
  $division['div3'] = '<polygon points="1000,-200 500,1200 500,1300 1100,1300 1100,-200" />';
}


