<?php

$height = (isset($options['flagHeight'])) ? $options['flagHeight'] : 1200;
$h100 = sprintf("%.2f",$height/100);

$ordinary['body'] = "<path d=\"M-10,-$h100 L500,$height 1010,-$h100 z\" />\n";


