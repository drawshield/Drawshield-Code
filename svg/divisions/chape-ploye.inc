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

$y = $layout->division_offset->y;
if (existModifierWithKeyterm($node, "enhanced")) {
    $division['div2'] = "<path d='m 500,$y c 0,165.71429 327.14285,500 500,500 v -500 z m 0,0 c 0,167.14286 -330,500 -500,500 v -500 z'/>";
} else {
    $division['div2'] = "<path d='m 500,0 c 0,165.71429 271.44693,679.66835 500,788.4 V 0 Z m 0,0 C 500,167.14286 256.34664,679.66835 0,788.4 V 0 Z' />";
}


