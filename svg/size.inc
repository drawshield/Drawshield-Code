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
    private static $ars = [
        '10:12' => 1.2,
        '10:10' => 1,
        '10:14' => 1.4,
        '10:24' => 2.4,
        '10:20' => 2,
        '10:28' => 2.8,
        '10:6' => 0.6,
    ];

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

    function closest_aspect_ratio()
    {
        $rel_h = $this->height / $this->width;
        $min_diff = 1000;
        $best = null;

        foreach ( static::$ars as $name => $h )
        {
            $diff = abs($rel_h - $h);
            if ( $diff < $min_diff )
            {
                $min_diff = $diff;
                $best = $name;
            }
        }

        return $best;
    }
}
