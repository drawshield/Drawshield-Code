<?php

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

class ShieldPos
{
    function __construct($x, $y)
    {
        $this->x = $x;
        $this->y = $y;
    }
}

class ShieldRect
{
    function __construct($pos, $size)
    {
        $this->pos = $pos;
        $this->size = $size;
    }
}

class ShieldLayout
{
    private static $custom_size = [
        "lozenge" => 1000,
        "circle" => 1000,
        "square" => 1000,
        // these use flagHeight
        "flag" => 0,
        "quarter" => 0,
    ];

    function __construct(ShieldSize $size, $shape, ShieldPos $center_point = null, ShieldRect $charge_hint = null)
    {
        // shield size 1000xheight
        $this->size = $size;
        // shape name
        $this->shape = $shape;
        // cross/quarters center point
        if ( $center_point === null )
            $this->auto_center();
        else
            $this->center_point = $center_point;
        // rect for custom positioning
        $this->charge_hint = $charge_hint;
    }

    function auto_center()
    {
        if ( isset(static::$custom_size[$this->shape]) )
            $this->center_point = new ShieldPos($this->size->width / 2, $this->size->height / 2);
        else
            $this->center_point = new ShieldPos(500, 500);
    }

    static function from_options($options)
    {
        $width = 1000;
        $height = 1200;
        $center_point = new ShieldPos(500, 500);
        $shape = $options["shape"];

        if ( isset($options["flagHeight"]) )
        {
            $height = $options["flagHeight"];
            $center_point = new ShieldPos(500, $height / 2);
        }
        else if ( isset(static::$custom_size[$shape]) )
        {
            $height = static::$custom_size[$shape];
            $center_point = new ShieldPos(500, $height / 2);
        }

        return new ShieldLayout(
            new ShieldSize($width, $height),
            $shape,
            $center_point
        );
    }

    function center_point_ratio()
    {
        return new ShieldPos(
            $this->center_point->x / $this->size->width,
            $this->center_point->y / $this->size->height
        );
    }
}
