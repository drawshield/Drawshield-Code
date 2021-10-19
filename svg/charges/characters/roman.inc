<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see https://www.gnu.org/licenses/.
 */

// Roman letter

$charge['default_colour'] = 'sable';

$item = getModifierValueByKeyterm( $node, 'value' );
if ( $item == null )
  $item = '?';
else
  $item = strtoupper($item{0});
$charge['svgCode'] = '<svg height="44" width="25"><text stroke="none" x="12.5" y="44" text-anchor="middle" font-size="44" font-family="serif">' .
      $item . '</text></svg>';


