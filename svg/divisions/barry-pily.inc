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

if ( ($num_of = getModifierValueByKeyterm($node, 'ofnum')) == null ) $num_of = 8;
$svg = '';
$stepV = $calcHeight / $num_of;
if ( existModifierWithKeyterm($node, 'sinister')) {
  for ( $i = 1; $i < $num_of; $i +=2 ) {
      $Y1 = ($i - 1) * $stepV;
      $svg .= "M1000,{$Y1}l-1000,{$stepV} 1000,{$stepV}z ";
    }
} else {
  for ( $i = 1; $i < $num_of; $i +=2 ) {
      $Y1 = ($i - 1) * $stepV;
      $svg .= "M0,{$Y1}l1000,{$stepV} -1000,{$stepV}z ";
  }
}

$division['div2'] = '<path d="' . $svg . '" />';
