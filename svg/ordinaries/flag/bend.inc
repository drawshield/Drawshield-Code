<?php

$h = $layout->size->height;
$d = intval(sqrt(1000000 + ($h * $h))); // diagonal length
$width = $h * 0.25;
$w2 = $width/-2;
$feature_size = $h / 20;
$theta = atan($h/1000);
// draw a long fess first
$startX = intval((-100 * cos($theta)) - ($w2 * sin($theta)));
$startY = intval(($w2 * cos($theta)) + (-100 * sin($theta)));
// Calculate start points, by rotating the point -100,bend width/2
$shape_spec = "X${startX}Y${startY}A" . ($d + 200) . 'c{%25}E' . ($d + 200) . 'g{%25}';
$shape_spec = subPercent($shape_spec, $layout);
// turn that into an SVG path
$path_spec = makePath($shape_spec, getModifierByType($node,'linetype'), $feature_size);
// rotate that path to run down the diagonal

$bend_path = rotatePath($path_spec,rad2deg($theta));

$ordinary['body'] = "<path fill-rule=\"evenodd\" d=\"$bend_path\"/>";
