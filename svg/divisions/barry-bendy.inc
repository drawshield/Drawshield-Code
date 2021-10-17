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

if ( existModifierWithKeyterm($node, 'sinister'))
  $div_data = array ( 250, 500, 'm62.5,125h125l62.5,-125h-125z h-62.5v125z m125,0h62.5v125h-125z m-125,250h-62.5v-125h125z h125l-62.5,125h-125z m125,0h62.5v-125z' );
else
  $div_data =  array ( 250, 500, 'l67.5,125h125l-67.5,-125z m0,125v125h125l-62.5,-125z m187.5,0l62.5,125v-125z m0,250h62.5v-125h-125z m-125,0l-62.5,-125v125z l62.5,125h125l-62.5,-125z' );

$retval = '<path d="';
list($width, $height, $pattern ) = $div_data;
for ( $y = 0; $y < $calcHeight; $y += $height )
  for ( $x = 0; $x < 1000; $x += $width )
    $retval .= "M$x,$y $pattern ";
$retval .= '" />';
$division['div2'] = $retval;

