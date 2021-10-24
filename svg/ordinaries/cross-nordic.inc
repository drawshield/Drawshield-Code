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

$height = $layout->size->height;
$unit = (int)$height / 7;
$unit3 = $unit * 3;
$unit5 = 1000 - $unit - $unit3; // actually whatever is left over

$shape = 'X-10Y{';
// top left
$shape = sprintf('X-10Y%dA%dG%da%d', $unit3, $unit3 + 10, $unit3 + 10, $unit);
// top right
$shape .= sprintf('C%dA%dc%d', $unit3 + 10, $unit5 + 10, $unit);
// bottom right
$shape .= sprintf('E%dC%de%d', $unit5 + 10, $unit3 + 10, $unit);
// bottom left
$shape .= sprintf('G%dE%sg%d', $unit3 + 10, $unit3 + 10, $unit);

$ordinary['shape_spec'] = $shape;

$size = (int)($unit * 0.9);
$midway = $unit3 + ($unit / 2);
// single charge
$chargesOn = sprintf('%d,%d,%d,%d', $midway, $midway, $size, $size );
// two & 3 charges
$chargesOn .= '/X/X/';
// four charges
$chargesOn .= sprintf('%d,%d,%d,%d:%d,%d:%d,%d:%d,%d', 
	$midway, $unit3 / 2, $size, $size, 
	$unit3 / 2, $midway, 
	1000 - $midway, $midway,
	$midway, $height - $midway
);
// 5 charges
$chargesOn .= sprintf('%d,%d,%d,%d:%d,%d:%d,%d:%d,%d,%d,%d', 
	$midway, $midway,
	$midway, $unit3 / 2, $size, $size, 
	$unit3 / 2, $midway, 
	1000 - $midway, $midway,
	$midway, $height - $midway
);
// 6 charges
$chargesOn .= '/X';

$ordinary['charges_on'] = $chargesOn;
