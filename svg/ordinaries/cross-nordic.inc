<?php

$height = (isset($options['flagHeight'])) ? $options['flagHeight'] : 1200;
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
