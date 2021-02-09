<?php

function get_size()
{
    global $aspect_ratio;
    global $options;

    $sx = 1000;
    if ( isset($options["flagHeight"]) && $options["flagHeight"] && $aspect_ratio != "square" )
        $sy = $options["flagHeight"];
    else
        $sy = 1000;

    return [$sx, $sy];
}


function get_size_multiplier()
{
    list($sx, $sy) = get_size();
    return [$sx/1000., $sy/1000.];
}
