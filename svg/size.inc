<?php

class ShieldSize
{
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
}

function lerp($a, $b, $factor)
{
    return $a * (1-$factor) + $b * $factor;
}

class ShieldPos
{
    function __construct($x, $y)
    {
        $this->x = $x;
        $this->y = $y;
    }

    function lerp(ShieldPos $other, $factor_x, $factor_y = null)
    {
        if ( $factor_y === null )
            $factor_y = $factor_x;

        return new ShieldPos(
            lerp($this->x, $other->x, $factor_x),
            lerp($this->y, $other->y, $factor_y)
        );
    }

    function translated($dx, $dy)
    {
        return new ShieldPos($this->x + $dx, $this->y + $dy);
    }

    function distance($other)
    {
        return hypot($other->x - $this->x, $other->y - $this->y);

    }
}

class ShieldRect
{
    function __construct(ShieldPos $pos, ShieldSize $size)
    {
        $this->pos = $pos;
        $this->size = $size;
    }

    function left()
    {
        return $this->pos->x - $this->size->width / 2;
    }

    function right()
    {
        return $this->pos->x + $this->size->width / 2;
    }

    function top()
    {
        return $this->pos->y - $this->size->height / 2;
    }

    function bottom()
    {
        return $this->pos->y + $this->size->height / 2;
    }

    function __toString()
    {
        return "{$this->pos->x},{$this->pos->y},{$this->size->width},{$this->size->height}";
    }

    /**
     * Returns a box relative to the current one
     * The parameters define the new box and are scaling factors
     * $x and $y are defined as interpolation factors between top-left and bottom-right
     */
    function relative_box($x, $y, $width, $height)
    {
        return new ShieldRect(
            new ShieldPos(
                lerp($this->left(), $this->right(), $x),
                lerp($this->top(), $this->bottom(), $y)
            ),
            $this->size->scaled($width, $height)
        );
    }

    function margin($x1, $y1 = null, $x2 = null, $y2 = null)
    {
        if ( $y1 === null )
            $y1 = $x1;

        if ( $x2 === null )
            $x2 = $x1;

        if ( $y2 === null )
            $y2 = $y1;

        $l = $this->left() + $x1;
        $r = $this->right() - $x2;
        $t = $this->top() + $y1;
        $b = $this->bottom() - $y2;

        return new ShieldRect(
            new ShieldPos(($l+$r)/2, ($t+$b)/2),
            new ShieldSize($r-$l, $b-$t)
        );
    }

    function translated($x, $y)
    {
        return new ShieldRect(new ShieldPos($x + $this->pos->x, $y + $this->pos->y), clone $this->size);
    }

    static function from_corners($x1, $y1, $x2, $y2)
    {
        return new ShieldRect(
            new ShieldPos(($x1 + $x2) / 2, ($y1 + $y2) / 2),
            new ShieldSize($x2 - $x1, $y2 - $y1)
        );
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

    function __construct(ShieldSize $size, $shape, ShieldPos $center_point = null, ShieldRect $charge_hint = null, $chief = true, $section = null)
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
        // Should use chief spacing
        $this->chief = $chief;
        if ( $this->chief === true )
            $this->auto_chief();

        // To determine the border area
        $this->section = $section;

    }

    function auto_center()
    {
        if ( isset(static::$custom_size[$this->shape]) )
            $this->center_point = new ShieldPos($this->size->width / 2, $this->size->height / 2);
        else
            $this->center_point = new ShieldPos($this->size->width / 2, 500);
    }

    function auto_chief()
    {
        $width = $this->size->width;
        $height = min($this->size->height * 0.3, 300);
        $this->chief = new ShieldRect(
            new ShieldPos($width / 2, $height / 2),
            new ShieldSize($width, $height)
        );
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

        $charge_hint = null;

        $size = new ShieldSize($width, $height);

        switch ( $shape )
        {
            case 'pauldron':
                $charge_hint = new ShieldRect(new ShieldPos(245, 305), new ShieldSize(510, 580));
                break;
            case 'lozenge':
                $charge_hint = new ShieldRect(clone $center_point, $size->scaled(0.8));
                break;
            break;
        }

        return new ShieldLayout(
            $size,
            $shape,
            $center_point,
            $charge_hint,
            true
        );
    }

    function center_point_ratio()
    {
        return new ShieldPos(
            $this->center_point->x / $this->size->width,
            $this->center_point->y / $this->size->height
        );
    }

    function rect_area()
    {
        return new ShieldRect(
            new ShieldPos($this->size->width / 2, $this->size->height / 2),
            clone $this->size
        );
    }
}
