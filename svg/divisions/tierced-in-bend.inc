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

$base = max($layout->size->width, $layout->size->height);
$length = $base * M_SQRT2 * 2;
$width = 400;

if ( $sinister = existModifierWithKeyterm($node, 'sinister') )
{{
    $builder_2 = path_builder($layout->cross_point->x + $base, $layout->cross_point->y - $base)
        ->up_left_plain($width/2)
        ->down_left($length)
        ->right_plain(($length+$width/2) / M_SQRT2)
        ->up_plain(($length-$width/2) / M_SQRT2)
    ;

    $builder_3 = path_builder($layout->cross_point->x + $base, $layout->cross_point->y - $base)
        ->down_right_plain($width/2)
        ->down_left($length)
        ->right_plain(($length-$width/2) / M_SQRT2)
        ->up_plain(($length-$width/2) / M_SQRT2)
    ;
}
}
else
{
    $builder_2 = path_builder($layout->cross_point->x - $base, $layout->cross_point->y - $base)
        ->down_left_plain($width/2)
        ->down_right($length)
        ->up_plain(($length+$width/2) / M_SQRT2)
        ->left_plain(($length-$width/2) / M_SQRT2)
    ;

    $builder_3 = path_builder($layout->cross_point->x - $base, $layout->cross_point->y - $base)
        ->up_right_plain($width/2)
        ->down_right($length)
        ->up_plain(($length-$width/2) / M_SQRT2)
        ->left_plain(($length-$width/2) / M_SQRT2)
    ;
}


$division['div2'] = createBody((string)$builder_2, $lineType, 60, $layout);
$division['div3'] = createBody((string)$builder_3, $lineType, 60, $layout);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $offset = $layout->cross_point->y - 500;
  for ($i = 1075; $i > -80; $i -= 80) {
    if ( $sinister ) {
      $rivetList .= '<g transform="translate(' . (630 - $i) . ',' . ($i  + $offset). ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (680 - $i) . ',' . ($i + 50 + $offset) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (930 - $i) . ',' . ($i +300 + $offset). ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (980 - $i) . ',' . ($i + 350 + $offset) . ')">' . $rivet . '</g>';
    } else {
      $rivetList .= '<g transform="translate(' . ($i + 350 - $offset) . ',' . $i . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i + 300 - $offset) . ',' . ($i + 50) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i + 50 - $offset) . ',' . ($i + 300) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i - $offset) . ',' . ($i + 350) . ')">' . $rivet . '</g>';
    }
  }
  $division['divx'] = $rivetList;
}



