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
 * Time: 23:00
 */

$planet = getModifierValueByKeyterm($node,'value');
if ( file_exists( $folder . $planet . '.svg'))
  $charge['file'] = "$planet.svg";
else {
  $charge['file'] = "sun.svg";
  $messages->addMessage('warning',"No symbol for $planet (using sun)");
}

