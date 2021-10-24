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

$y = $layout->division_offset->y - 106;

$div_data = array (
  'per-bend' => "X-106Y{$y}B1714c300e1212g1512",
  'per-bend-fracted' => "X200Y{$layout->division_offset->y}B717e500B1131c100e1010g1410",
  'per-bend-sinister' => "X1000Y{$layout->division_offset->y}c1400e1000g400H1414",
  'per-bend-sinister-fracted' => "X800Y{$layout->division_offset->y}a200c1300e1000H1131e500H717",
);

$item = 'per-bend';
if ( existModifierWithKeyterm($node, 'sinister')) $item .= '-sinister';
if ( existModifierWithKeyterm($node, 'fracted')) $item .= '-fracted';
$division['div2'] = createBody($div_data[$item], getModifierByType($node, 'linetype', false),60, $layout);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $offset = $layout->division_offset->y;
  for ($i = 995; $i > 0; $i -= 80) {
    if ( $item == 'per-bend-sinister' ) {
      $rivetList .= '<g transform="translate(' . (930 - $i) . ',' . ($i  + $offset). ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (980 - $i) . ',' . ($i + 50 + $offset) . ')">' . $rivet . '</g>';
    } else {
      $rivetList .= '<g transform="translate(' . ($i + 50 - $offset) . ',' . $i . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i - $offset) . ',' . ($i + 50) . ')">' . $rivet . '</g>';
    }
  }
  $division['divx'] = $rivetList;
}

