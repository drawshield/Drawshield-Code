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
 * Time: 23:58
 */

$charge['orient_to_ordinary'] = true;
if (!array_key_exists('file',$charge)) {
  $charge['file'] = 'inkscape/chough.svg';
  $messages->addMessage('warning','chough substituted for ' . $chargeKey );
}
