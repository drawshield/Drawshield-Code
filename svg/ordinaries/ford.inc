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
$ordinary['body'] = subPercent('  <g>
    <g style="fill:' . rgb('azure') . '">
      <path d="m -10 {%80} h1020 v410 h-1020 z"/>
    </g>
    <g style="fill:' . rgb('argent') . '">
      <path d="m-100, {%87}   c20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0   l0,84.79   c-20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0   z m0,169.6   c20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0 20,-7.537 40,-7.537 60,0 20,7.537 40,7.537 60,0   l0,84.79   c-20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0 -20,7.537 -40,7.537 -60,0 -20,-7.537 -40,-7.537 -60,0   z " style="fill-rule:evenodd"/>
    </g>
  </g>', $layout);
