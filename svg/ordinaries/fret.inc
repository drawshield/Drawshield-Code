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
    d='m 585,698 -57,57 -28,28 -283,-283 85,-85 57,57 -29,28 170,170 28,-29 57,57 z M 472,359 415,302 500,217 783,500 698,585 641,528 670,500 500,330 472,359 z M -16.6,-73.2 472,415 415,472 -73.2,-16.6 -16.6,-73.2 z M 585,528 l 495,492 -60,60 -492,-495 57,-57 z M 528,415 585,472 472,585 415,528 528,415 z M 302,641 358,698 -17,1070 -73.5,1020 302,641 z m 718,-714.5 50,56.5 -372,375 -57,-56 379,-375.5 z'
/>";
