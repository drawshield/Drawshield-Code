<?php

$height = (isset($options['flagHeight'])) ? $options['flagHeight'] : 1200;

$midX = "500";
$midY = sprintf('%.2f',($height/2));
$topCenterY = sprintf('%.2f',($height/2)-141.4);
$lowerCenterY = sprintf('%.2f',($height/2)+141.4);
$lowerY = sprintf('%.2f',$height + 141.4);
$leftX = "358.6";
$rightX = "641.4";
$path = "<path d=\"M 0,-141.4 ";
$path .= "L $midX,$topCenterY 1000,-141.4 1141.4,0 $rightX,$midY 1141.4,$height ";
$path .= " 1000,$lowerY $midX,$lowerCenterY 0,$lowerY -141.4,$height ";
$path .= "$leftX,$midY -141.4,0 z\" />";

$ordinary['body'] = $path;

