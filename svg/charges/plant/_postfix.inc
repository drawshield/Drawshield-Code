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
 * Time: 21:28
 */
$charge['orient_to_ordinary'] = true;

if (!array_key_exists('file',$charge)) {
  $charge['file'] = 'inkscape/thistle.svg';
  $messages->addMessage('warning','thistle substituted for ' . $chargeKey );
}
