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
    along with Foobar.  If not, see https://www.gnu.org/licenses/. */

$lineType = getModifierByType($node, 'linetype', false);


$margin = 100;
$down = $layout->size->height - $layout->cross_point->y + $margin;

$builder = new PathBuilder();
$builder->move(-$margin, $layout->cross_point->y)
    ->right($layout->size->width + $margin * 2)
    ->down_plain($down)
    ->left_plain($layout->size->width + $margin * 2)
    ->up_plain($down);

$division['div2'] = createBody((string)$builder, $lineType, 60, $layout);


if ( count($tinctures) > 2 )
    include "multi-bar.inc";

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $y1 = $layout->cross_point->y - 35;
  $y2 = $layout->cross_point->y + 15;
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',' . $y1 . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . $i . ',' . $y2 . ')">' . $rivet . '</g>';
  }
  $division['divx'] = $rivetList;
}



