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


class ShieldSize
{
    function __construct($width, $height)
    {
        $this->width = $width;
        $this->height = $height;
    }

    static function current()
    {
        global $aspect_ratio;
        global $options;

        $w = 1000;

        if ( $aspect_ratio == "square" )
            $h = $w;
        else if ( isset($options["flagHeight"]) && $options["flagHeight"] )
            $h = $options["flagHeight"];
        else
            $h = 1200;

        return new ShieldSize($w, $h);
    }

    function scaled($x, $y = null)
    {
        if ( $y === null )
            $y = $x;

        return new ShieldSize($this->width * $x, $this->height * $y);
    }

    function normalize_factor()
    {
        return 1000 / $this->width;
    }
}
