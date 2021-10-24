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

$diagonal = (int)($calcHeight * 0.707 + 141);
$return = $calcHeight + 200;
$ordinary['shape_spec'] =  "V1X-100Y-100B${diagonal}D{$diagonal}g${return}";


$ordinary['chargesOn'] = // TODO Fix this...
  '500,400,100,100/250,625,100,100,45:750,625,,,-45/250,625,100,100,45:500,400,,,0:750,625,,,-45/200,675,100,100,45:400,475:600,475,,,-45:800,675/200,675,100,100,45:350,525:500,400,,,0:650,525,,,-45:800,675/150,725,100,100,45:250,625:350,525:650,525,,,-45:750,625:850,725/150,725,100,100,45:250,625:350,525:500,400,100,100,0:650,525,,,-45:750,625:850,725';

