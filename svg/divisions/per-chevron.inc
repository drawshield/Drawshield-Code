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

if ( $rev = getModifierNodeWithKeyterm($node,'reversed',true)) // reversed means inverted
    $rev->setAttribute('keyterm','inverted');

$extent = max($layout->size->width, $layout->size->height);

if ( existModifierWithKeyterm($node, 'inverted') )
{
    $y = $layout->cross_point->y + 300 / M_SQRT2;
    $div_data = path_builder($layout->cross_point->x, $y)->chevron();
    $div_data->up_right($extent)->down_plain(2000)->left_plain($extent*M_SQRT2)->up_plain(2000)->down_right($extent);
}
elseif ( existModifierWithKeyterm($node, 'throughout') && existModifierWithKeyterm($node, 'inverted')) {
    $y = $layout->size->height;
    $div_data = path_builder($layout->cross_point->x, $y)->chevron();
    $div_data->up_right($extent)->down_plain(2000)->left_plain($extent*M_SQRT2)->up_plain(2000)->down_right($extent);
}
elseif ( existModifierWithKeyterm($node, 'throughout')) {
    $y = 0;
    $div_data = path_builder($layout->cross_point->x, $y)->chevron();
    $div_data->down_right($extent)->down($layout->size->height - 500)->left_plain($extent * M_SQRT2)->up($layout->size->height - 500)->up_right($extent);
}
else
{
    $y = $layout->cross_point->y - 150 / M_SQRT2;
    $div_data = path_builder($layout->cross_point->x, $y)->chevron();
    $div_data->down_right($extent)->left_plain($extent * M_SQRT2)->up_right($extent);
}


$division['div2'] = createBody((string)$div_data, $lineType, 60, $layout);


if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $ref = $y + -25;
  for ($i = 5; $i < 450; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',' . (455 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . ($i + 40) . ',' . (495 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . (970 - $i) . ',' . (455 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . (930 - $i) . ',' . (495 + $ref) . ')">' . $rivet . '</g>';
    $ref -= 80;
  }
  $division['divx'] = $rivetList;
}

