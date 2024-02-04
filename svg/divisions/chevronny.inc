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

$num_of = getModifierValueByKeyterm($node, 'ofnum', false);
if ( $num_of <= 1 ) $num_of = 8;
// Each chevron is counted, not just the coloured ones
$num = $num_of / 2;

$stepV = ($layout->size->height - $layout->division_offset->y / 2) / 2 / ($num-1);
$width = $stepV / M_SQRT2;
$upper = $layout->size->width / 2 * M_SQRT2 + $width;
$down = $upper - $width;

$builder = new PathBuilder();

for ( $i = 0; $i < $num; $i++ ) {
  $startY = $layout->division_offset->y / 2 + ( $i * 2 * $stepV);
  $builder->move($layout->cross_point->x, $startY)
    ->chevron()
    ->down_left($down)
    ->up_left_plain($width)
    ->up_right($upper)
    ->down_right($upper)
    ->down_left($width)
    ->up_left($down)
  ;

//   $NC .= "V1X500Y{$startY}D{$down}f{$width}H{$upper}B{$upper}d{$width}F{$down}     ";
}
$division['div2'] = createBody((string)$builder, getModifierByType($node,'linetype', false),40, $layout);

