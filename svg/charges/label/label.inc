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

$suffix = '-straight';
if (existModifierWithKeyterm($node,'dovetailed')) {
  $suffix = '-dovetailed';
} elseif (existModifierWithKeyterm($node,'pattee')) {
  $suffix = '-pattee';
}

$num_of = getModifierValueByKeyterm($node, 'ofnum');
if ( $num_of < 3 )
  $num_of = 3;
elseif ( $num_of > 7 )
  $num_of = 7;


$charge['file'] = 'label-of-' . $num_of . $suffix . '.svg';
switch($num_of) {
  case 3:
    $charge['placement'] = '1000,200,230,360/240,200,230,360:1075,150/1000,200,230,360:240,200:1766,200';
    break;
  case 4:
    $charge['placement'] = 'x/725,200,230,260:1275,200/x/200,200,230,360:725,200:1275,200:1805,200';
    break;
  case 5:
    $charge['placement'] = '1000,200,230,360/600,200,230,360:1400,200/600,200,230,360:1000,200:1400,200/x/200,200,230,360:600,200:1000,200:1400,200:1800,200';
    break;
  case 6:
    $charge['placement'] = 'x/820,200,180,360:1180,200/x/460,200,180,360:820,200:1180,200:1540,200/x/120,200,180,360:460,200:820,200:1180,200:1540,200:1880,200';
    break;
  case 7:
    $charge['placement'] = '1000,150,140,180/705,150,140,180:1300,150/705,150,140,180:1000,150:1300,150/x/405,150,140,180:705,150:1000,150:1300,150:1600,150/x/110,150,140,180:405,150:705,150:1000,150:1300,150:1600,150:1890,150';
    break;
  default:
    $charge['placement'] = 'x';
    break;
}

