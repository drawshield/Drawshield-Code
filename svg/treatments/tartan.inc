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

$tartanPattern = [];

foreach ($children as $child) {
    if ($child->nodeName == 'colour') {
        $hex = rgb($child->getAttribute('keyterm'));
        $mod = $child->firstChild;
        $size = $mod->getAttribute('value');
        $tartanPattern[] = array($hex, $size);
    }
}

$numThreads = count($tartanPattern);
if (!$numThreads) { // something wrong, no colours
    $tartanPattern = [
        ['#0f0f0f', 25],
        ['#f0f0f0', 25],
    ];
    $numThreads = 2;
    $messages->addMessage('parser',"Tartan requires colours and thread counts");
}

// calculate the repeated area size
$repeatCount = 0;
for ($k = 0; $k < $numThreads; $k++) {
    // can't reproduce this, but it happens sometimes...
    if (!isset($tartanPattern[$k][1]))
        $tartanPattern[$k][1] = 25;
    $repeatCount += $tartanPattern[$k][1];
}

$threadScale = 2; // default
if ($repeatCount > 100)
    $threadScale = 1;
elseif ($repeatCount < 21)
    $threadScale = 3;

$repeatCount *= $threadScale;

if ($reverse)  $repeatCount *= 2;

$svg = "<g>\n";

$y = 0;
for ($k = 0; $k < $numThreads; $k++) {
    $x = 0;
    $ySize = $tartanPattern[$k][1] * $threadScale;
    for ($j = 0; $j < $numThreads; $j++ ) {
        $xSize = $tartanPattern[$j][1] * $threadScale;
        $svg .= "<rect x=\"$x\" y=\"$y\" width=\"$xSize\" height=\"$ySize\" fill=\"" . mixRGB($tartanPattern[$k][0], $tartanPattern[$j][0]) . "\" />\n";
        $x += $xSize;
    }
    // $y += $ySize;
    if ($reverse) {
        for ($j = $numThreads - 1; $j >= 0; $j--) {
            $xSize = $tartanPattern[$j][1] * $threadScale;
            $svg .= "<rect x=\"$x\" y=\"$y\" width=\"$xSize\" height=\"$ySize\" fill=\"" . mixRGB($tartanPattern[$k][0], $tartanPattern[$j][0]) . "\" />\n";
            $x += $xSize;
        }
    }
    $y += $ySize;
}
if ($reverse) {
    for ($k = $numThreads - 1; $k >= 0 ; $k--) {
        $x = 0;
        $ySize = $tartanPattern[$k][1] * $threadScale;
        for ($j = 0; $j < $numThreads; $j++ ) {
            $xSize = $tartanPattern[$j][1] * $threadScale;
            $svg .= "<rect x=\"$x\" y=\"$y\" width=\"$xSize\" height=\"$ySize\" fill=\"" . mixRGB($tartanPattern[$k][0], $tartanPattern[$j][0]) . "\" />\n";
            $x += $xSize;
        }
        // $y += $ySize;
        if ($reverse) {
            for ($j = $numThreads - 1; $j >= 0; $j--) {
                $xSize = $tartanPattern[$j][1] * $threadScale;
                $svg .= "<rect x=\"$x\" y=\"$y\" width=\"$xSize\" height=\"$ySize\" fill=\"" . mixRGB($tartanPattern[$k][0], $tartanPattern[$j][0]) . "\" />\n";
                $x += $xSize;
            }
        }
        $y += $ySize;
    }
}
$svg .= "</g>\n";

$treatment_data['height'] = $repeatCount;
$treatment_data['width'] = $repeatCount;
$treatment_data['pattern'] = $svg;

