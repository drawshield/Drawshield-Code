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
if ( file_exists( $folder . 'inkscape/astron-' . $planet . '.svg'))
  $charge['file'] = "inkscape/astron-$planet.svg";
else {
  $charge['file'] = "inkscape/astron-sun.svg";
  $messages->addMessage('warning','Symbol for sun substituted for ' . $planet);
}