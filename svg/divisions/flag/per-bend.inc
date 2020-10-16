<?php

$h = $options['flagHeight'];
$d = intval(sqrt(1000000 + ($h * $h))); // diagonal length
$feature_size = $h / 20;
$theta = atan($h/1000);

$startX = intval(-100 * cos($theta));
$startY = intval(-100 * sin($theta));

if ( existModifierWithKeyterm($node, 'sinister'))
    $shape_spec = "X${startX}Y${startY}A" . ($d + 200) . "c${h}e" . ($d + 200) . "g${h}";
else
    $shape_spec = "X${startX}Y${startY}A" . ($d + 200) . "c${h}e" . ($d + 200) . "g${h}";
$shape_spec = subPercent($shape_spec);
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
