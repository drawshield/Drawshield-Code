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


if ( ($num_of = getModifierValueByKeyterm($node, 'ofnum', false)) == null ) $num_of = 8;

$NC = '';
$CH = '';
$stepH = 1000 / $num_of;
$height = $layout->size->height + 200;
for ( $i = 1; $i < $num_of; $i +=2 ) {
      $X1 = $i * $stepH;
      $NC .= "X{$X1}Y-100a{$stepH}C{$height}e{$stepH}G{$height}   ";
}
$division['div2'] = createBody( $NC, getModifierByType($node,'linetype', false), 40, $layout);


if ( count($tinctures) > 2 )
    include "multi-bar.inc";
