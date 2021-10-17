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
    d='m 698,415 57,57 28,28 -283,283 -85,-85 57,-57 28,29 170,-170 -29,-28 57,-57 z m -339,113 -57,57 -85,-85 283,-283 85,85 -57,57 -28,-29 -170,170 29,28 z M 118,825 415,528 471,585 175,882 118,825 z M 528,415 825,118 882,175 585,472 528,415 z M 415,472 472,415 585,528 528,585 415,472 z M 641,698 698,641 884,827 827,884 641,698 z M 118,175 175,118 358,302 302,358 118,175 z'
/>";

