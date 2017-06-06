<?php
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 09/09/14
 * Time: 15:28
 */

if (!array_key_exists('file',$charge)) {
  $charge['file'] = 'inkscape/knight.svg';
  $messages->addMessage('warning','Knights helm substituted for ' . $chargeKey);
}
