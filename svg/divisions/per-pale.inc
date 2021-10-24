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
$div_data = 'X500Y0a500c{%100*}e500G{%100*}';
$division['div2'] = createBody($div_data, getModifierByType($node, 'linetype', false), 60, $layout);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1200; $i += 80) {
    $rivetList .= '<g transform="translate(465,' . $i . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(515,' . $i . ')">' . $rivet . '</g>';
  }
  $division['divx'] = $rivetList;
}


if ( count($tinctures) > 2 )
    include "multi-bar.inc";
