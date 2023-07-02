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

if ( existModifierWithKeyterm($node,'couped') ) {
  $ordinary['shape_spec'] = 'X100Y-8U1a800c208E800g208';
  $ordinary['chargesOn'] = array ('h','200,20,600,160');
} elseif ( existModifierWithKeyterm($node, 'couped-dexter')) {
  $ordinary['shape_spec'] = 'X-8Y-8U1a908c208E908g208';
  $ordinary['chargesOn'] = array ('h','200,20,600,160');
} elseif ( existModifierWithKeyterm($node,'couped-sinister')) {
  $ordinary['shape_spec'] = 'X100Y-8U1a908c208E908g208';
  $ordinary['chargesOn'] = array ('h','200,20,600,160');
} elseif ( existModifierWithKeyterm($node, 'voided')) {
  $ordinary['shape_spec'] = 'X0Y0U1a1000c208E1000g208';
  $ordinary['chargesOn'] = array ('h','100,30,800,160');
} else {
  $ordinary['shape_spec'] = (string)path_builder(-100, -8)
    ->draw_all()
    ->right_plain($layout->size->width+200)
    ->down_plain(141)
    ->left($layout->size->width+200)
    ->up_plain(141);

    $ordinary['chargesOn'] = array("h", "100, 20, 800, 110");
}

