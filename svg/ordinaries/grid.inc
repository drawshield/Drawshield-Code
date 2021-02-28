<?php 

$height = (isset($options['flagHeight'])) ? $options['flagHeight'] : 1200;

$retval = '';
// vertical lines
for ($i = 50; $i < 1000; $i += 100) {
    $i1 = $i-1;
    $i2 = $i - 2 + 50;
    // thin line at 50
    $retval .= "<path d=\"M$i1,0 h2 v$height h-2 z\"/>\n"; 
    // thick line at 100
    $retval .= "<path d=\"M$i2,0 h4 v$height h-4 z\"/>\n"; 
}
// horizontal lines
for ($i = 50; $i < $height; $i += 100) {
    $i1 = $i - 1;
    $i2 = $i - 2 + 50;
    // thin line at 50
    $retval .= "<path d=\"M0,$i1 v2 h1000 v-2 z\"/>\n"; 
    // thick line at 100
    $retval .= "<path d=\"M0,$i2 v4 h1000 v-4 z\"/>\n"; 
}

$ordinary['body'] = $retval;

