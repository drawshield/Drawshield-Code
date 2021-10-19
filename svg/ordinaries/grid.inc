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

$height = $layout->size->height;

$retval = '';
// vertical lines
for ($i = 50; $i < 1000; $i += 100) {
    $i1 = $i-1;
    $i2 = $i - 2 + 50;
    // thin line at 50
    $retval .= "<path d=\"M$i1,0 h2 v$height h-2 z\"/>\n"; 
    // thick line at 100
    $retval .= "<path d=\"M$i2,0 h4 v$height h-4 z\"/>\n"; 
}
// horizontal lines
for ($i = 50; $i < $height; $i += 100) {
    $i1 = $i - 1;
    $i2 = $i - 2 + 50;
    // thin line at 50
    $retval .= "<path d=\"M0,$i1 v2 h1000 v-2 z\"/>\n"; 
    // thick line at 100
    $retval .= "<path d=\"M0,$i2 v4 h1000 v-4 z\"/>\n"; 
}

$ordinary['body'] = $retval;

