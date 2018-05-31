<?php

$height = (isset($options['flagHeight'])) ? $options['flagHeight'] : 800; // just in case
$size = (int)(min(100,$height/8));
$ordinary['body'] = '<path fill-rule="evenodd" d="M -10,-10 h1020 v' .
	($height + 20) . ' h-1020 v-' . ($height + 20) . ' z M' . $size . ', ' . $size .
	' h ' . (1000 - $size - $size) . ' v ' . ($height - $size - $size) . ' h-' .
	(1000 - $size - $size) . ' z"/>';

