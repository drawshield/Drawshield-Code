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
    d='M 400,0 L 440,0 L 440,390 L 400,390 L 400,0 z M 560,0 L 600,0 L 600,390 L 560,390 L 560,0 z M 400,0 L 440,0 L 440,390 L 400,390 L 400,0 z M 400,450 L 440,450 L 440,550 L 400,550 L 400,450 z M 560,450 L 600,450 L 600,550 L 560,550 L 560,450 z M 400,610 L 440,610 L 440,1200 L 400,1200 L 400,610 z M 560,610 L 600,610 L 600,1200 L 560,1200 L 560,610 z M 480,0 L 520,0 L 520,470 L 480,470 L 480,0 z M 480,530 L 520,530 L 520,1200 L 480,1200 L 480,530 z M 0,400 L 470,400 L 470,440 L 0,440 L 0,400 z M 0,560 L 470,560 L 470,600 L 0,600 L 0,560 z M 0,480 L 390,480 L 390,520 L 0,520 L 0,480 z M 450,480 L 550,480 L 550,520 L 450,520 L 450,480 z M 530,400 L 1000,400 L 1000,440 L 530,440 L 530,400 z M 530,560 L 1000,560 L 1000,600 L 530,600 L 530,560 z M 610,480 L 1000,480 L 1000,520 L 610,520 L 610,480 z'
/>";




$old_ordinary = $ordinary;
include "plain-cross.inc";
$old_ordinary["chargesOn"] = $ordinary["chargesOn"];
$old_ordinary["chargesWithin"] = $ordinary["chargesWithin"];
$ordinary = $old_ordinary;
