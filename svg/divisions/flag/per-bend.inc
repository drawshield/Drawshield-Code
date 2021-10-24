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

$h = $layout->size->height;
$d = intval(sqrt(1000000 + ($h * $h))); // diagonal length
$feature_size = $h / 20;
$theta = atan($h/1000);

$startX = intval(-100 * cos($theta));
$startY = intval(-100 * sin($theta));

if ( existModifierWithKeyterm($node, 'sinister'))
    $shape_spec = "X${startX}Y${startY}A" . ($d + 200) . "c${h}e" . ($d + 200) . "g${h}";
else
    $shape_spec = "X${startX}Y${startY}A" . ($d + 200) . "c${h}e" . ($d + 200) . "g${h}";
$shape_spec = subPercent($shape_spec, $layout);
// turn that into an SVG path
$path_spec = makePath($shape_spec, getModifierByType($node,'linetype', false), $feature_size);
// rotate that path to run down the diagonal

if ( existModifierWithKeyterm($node, 'sinister')) {
    $div_path = rotatePath($path_spec,rad2deg($theta)*-1);
    $div_path = translatePath($div_path,0,$h + 100);

} else {
    $div_path = rotatePath($path_spec,rad2deg($theta));
}

$division['div2'] = "<path fill-rule=\"evenodd\" d=\"$div_path\"/>";
