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
 * Time: 21:41
 */

if ( existModifierWithKeyterm($node,'pierced') and file_exists($folder . 'inkscape/' . $chargeKey . '-pierced.svg'))
  $charge['file'] = "inkscape/$chargeKey-pierced.svg";
elseif ( existModifierWithKeyterm($node,'voided') and file_exists($folder . 'inkscape/' . $chargeKey . '-voided.svg'))
  $charge['file'] = "inkscape/$chargeKey-voided.svg";
elseif ( existModifierWithKeyterm($node,'fitchy') and file_exists($folder . 'inkscape/' . $chargeKey . '-fitchy.svg'))
  $charge['file'] = "inkscape/$chargeKey-fitchy.svg";
elseif ( existModifierWithKeyterm($node,'fitchy-foot') and file_exists($folder . 'inkscape/' . $chargeKey . '-fitchy-foot.svg'))
  $charge['file'] = "inkscape/$chargeKey-fitchy-foot.svg";

$charge['charged-with'] = 'c/c/c/500,200,200,200:200,500:800,500:500,800/c/c'; // special placement for 4 things on us, otherwise just use normal layout


