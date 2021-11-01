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

$lineType = getModifierByType($node, 'linetype', false);


$margin = 100;
$height = ($layout->size->height - $layout->division_offset->y) / 3;
$y1 = $layout->division_offset->y + $height;
$y2 = $y1 + $height;
$width = $layout->size->width + 2 * $margin;

$builder_2 = path_builder(-$margin, $y1)
    ->right($width)
    ->down_plain(2*$height + $margin)
    ->left_plain($width)
    ->up_plain(2*$height + $margin)
;
$builder_3 = path_builder(-$margin, $y2)
    ->right($width)
    ->down_plain($height + $margin)
    ->left_plain($width)
    ->up_plain($height + $margin)
;

$division['div2'] = createBody((string)$builder_2, $lineType, 60, $layout);
$division['div3'] = createBody((string)$builder_3, $lineType, 60, $layout);
if ( existModifierWithKeyterm($node, 'riveted')) {
	include '_rivet.inc';
	$rivetList = '';
	for ($i = 5; $i < 1000; $i += 80) {
		$rivetList .= '<g transform="translate(' . $i . ',365)">' . $rivet . '</g>';
		$rivetList .= '<g transform="translate(' . $i . ',415)">' . $rivet . '</g>';
		$rivetList .= '<g transform="translate(' . $i . ',765)">' . $rivet . '</g>';
		$rivetList .= '<g transform="translate(' . $i . ',815)">' . $rivet . '</g>';
	}
	$division['divx'] = $rivetList;
}
