<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

     DrawShield is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
 */

// Ford
// although really I am a base with wavy bars

foreach ( $node->childNodes as $child ) {
  if ( $child->nodeName == 'tincture') {
    $child->setAttribute('origin', 'implied');
  }
}
$messages->addMessage('licence','boss - https://commons.wikimedia.org/wiki/File:Viking_shield.svg');
// boss is 121 x 121 units
$centerX = 500 - 121;
$centerY = ($layout->size->height / 2) - 121;
$ordinary['body'] = "  <g transform=\"translate($centerX, $centerY) scale(2) \">" .
    preg_replace('/<\?xml.*?\?>/i', '', file_get_contents('svg/ordinaries/boss.svg'))  .
  '</g>';
