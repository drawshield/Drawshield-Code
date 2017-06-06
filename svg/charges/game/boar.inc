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
 * Time: 22:05
 */

if (existModifierWithKeyterm($node, 'head-only')) {
    $charge['file'] = 'inkscape/boar-head.svg';
} else if (existModifierWithKeyterm($node, 'rampant')) {
    $charge['file'] = 'inkscape/boar-rampant.svg';
} else {
    $charge['file'] = 'inkscape/boar.svg';
}
