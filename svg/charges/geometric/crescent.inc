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


$charge['orient_to_ordinary'] = true;

if (($pendent = getModifierNodeWithKeyterm($node,'pendent')) != null)
  $pendent->setAttribute('keyterm','inverted');
