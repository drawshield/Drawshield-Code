<?php

$height = (isset($options['flagHeight'])) ? $options['flagHeight'] : 1200;
$armSize = (int)$height / 5;
$armHeight = $armSize * 2;
$armWidth = (1000 - $armSize) / 2; // actually whatever is left over

$shape = 'X-10Y{';
// top left
$shape = sprintf('X-10Y%dA%dG%da%d', $armHeight, $armWidth + 10, $armHeight + 10, $armSize);
// top right
$shape .= sprintf('C%dA%dc%d', $armHeight + 10, $armWidth + 10, $armSize);
// bottom right
$shape .= sprintf('E%dC%de%d', $armWidth + 10, $armHeight + 10, $armSize);
// bottom left
$shape .= sprintf('G%dE%sg%d', $armHeight + 10, $armWidth + 10, $armSize);

$ordinary['shape_spec'] = $shape;

$size = (int)($armSize * 0.9);
$midway = $armHeight + ($armSize / 2);
// single charge
$chargesOn = sprintf('%d,%d,%d,%d', $midway, $midway, $size, $size );
// two & 3 charges
$chargesOn .= '/X/X/';
// four charges
$chargesOn .= sprintf('%d,%d,%d,%d:%d,%d:%d,%d:%d,%d', 
	$midway, $armHeight / 2, $size, $size, 
	$armHeight / 2, $midway, 
	1000 - $midway, $midway,
	$midway, $height - $midway
);
// 5 charges
$chargesOn .= sprintf('%d,%d,%d,%d:%d,%d:%d,%d:%d,%d,%d,%d', 
	$midway, $midway,
	$midway, $armHeight / 2, $size, $size, 
	$armHeight / 2, $midway, 
	1000 - $midway, $midway,
	$midway, $height - $midway
);
// 6 charges
$chargesOn .= '/X';

$ordinary['charges_on'] = $chargesOn;
