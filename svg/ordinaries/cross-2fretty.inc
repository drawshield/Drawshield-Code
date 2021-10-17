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
    along with Foobar.  If not, see https://www.gnu.org/licenses/.
 */


$y = ($layout->cross_point->y - 500);

$ordinary['body'] = "<path
    transform='translate(0, $y)'
    d='M 410,0 L 410,400 L 470,400 L 470,0 L 410,0 z M 530,0 L 530,520 L 590,520 L 590,0 L 530,0 z M 410,480 L 410,1200 L 470,1200 L 470,480 L 410,480 z M 530,600 L 530,1200 L 590,1200 L 590,600 L 530,600 z M 0,410 L 520,410 L 520,470 L 0,470 L 0,410 z M 600,410 L 1000,410 L 1000,470 L 600,470 L 600,410 z M 0,530 L 400,530 L 400,590 L 0,590 L 0,530 z M 480,530 L 1000,530 L 1000,590 L 480,590 L 480,530 z'
/>";




$old_ordinary = $ordinary;
include "plain-cross.inc";
$old_ordinary["chargesOn"] = $ordinary["chargesOn"];
$old_ordinary["chargesWithin"] = $ordinary["chargesWithin"];
$ordinary = $old_ordinary;
