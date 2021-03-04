<?php


$num_of = getModifierValueByKeyterm($node, 'of-num', true);
if ( $num_of < 4 )
    $num_of = 6;
elseif ( $num_of > 10 )
    $num_of = 10;

$suffix = '-of' . $num_of;



$file = getBestMatchFilename(__dir__, 'star', $suffix);
if ($file != '')
    $charge['file'] = $file;


