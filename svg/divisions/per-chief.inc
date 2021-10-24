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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */

$lineType = getModifierByType($node, 'linetype', false);

if ( $layout->chief )
    $y = $layout->chief->bottom();
else if ( $layout->division_offset->y > 0 )
    $y = $layout->division_offset->y;
else
    $y = $layout->size->height * 0.3;

$margin = 100;

$builder = new PathBuilder();
$builder->move(-$margin, -$margin)
    ->right_plain($layout->size->width + $margin * 2)
    ->down_plain($y + $margin)
    ->left($layout->size->width + $margin * 2)
    ->up_plain($y + $margin);

$division['div2'] = createBody((string)$builder, $lineType, 60, $layout);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',' . ($y - 35) .')">' . $rivet . '</g>';
  }
  $division['divx'] = $rivetList;
}



