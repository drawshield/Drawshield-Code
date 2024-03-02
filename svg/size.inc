<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

     DrawShield is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */

#[AllowDynamicProperties]
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

#[AllowDynamicProperties]
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

    function angle_to($other)
    {
        return atan2($other->y - $this->y, $other->x - $this->x);
    }

    function rotated($angle, ShieldPos $origin = null)
    {
        if ( !$origin )
            $origin = new ShieldPos(0, 0);

        $r = $this->distance($origin);
        $a = $origin->angle_to($this) + $angle;
        return new ShieldPos(
            $origin->x + $r * cos($a),
            $origin->y + $r * sin($a)
        );
    }

    function scaled($dx, $dy = null)
    {
        if ( $dy === null )
            $dy = $dx;
        return new ShieldPos($this->x * $dx, $this->y * $dy);
    }

    function inverted()
    {
        return $this->scaled(-1);
    }

    function __toString()
    {
        return "{$this->x},{$this->y}";
    }
}

#[AllowDynamicProperties]
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

    function svg_rect()
    {
        return "<rect x='{$this->left()}' y='{$this->top()}' width='{$this->size->width}' height='{$this->size->height}' />";
    }

    function combined(ShieldRect $other)
    {
        return ShieldRect::from_corners(
            min($other->left(), $this->left()),
            min($other->top(), $this->top()),
            max($other->right(), $this->right()),
            max($other->bottom(), $this->bottom())
        );
    }

    function combine(ShieldRect $other)
    {
        $comb = $this->combined($other);
        $this->pos = $comb->pos;
        $this->size = $comb->size;
    }
}

#[AllowDynamicProperties]
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

    function __construct(ShieldSize $size, $shape, ShieldPos $cross_point = null, ShieldRect $charge_hint = null, $chief = true, $section = null)
    {
        // shield size 1000xheight
        $this->size = $size;
        // shape name
        $this->shape = $shape;
        // cross/quarters center point
        if ( $cross_point === null )
            $this->auto_center();
        else
            $this->cross_point = $cross_point;
        // rect for custom positioning
        $this->charge_hint = $charge_hint;
        // Should use chief spacing
        $this->chief = $chief;
        if ( $this->chief === true )
            $this->auto_chief();

        // To determine the border area
        $this->section = $section;

        $this->division_offset = new ShieldPos(0, 0);

    }

    function auto_center()
    {
        # This sometimes generates message "Illegal offset type in isset or empty", not sure why...
        if ( (!is_null($this->shape) && isset(static::$custom_size[$this->shape])) || $this->size->height >= 1500 || $this->size->height < 900 )
            $this->cross_point = new ShieldPos($this->size->width / 2, $this->size->height / 2);
        else
            $this->cross_point = new ShieldPos($this->size->width / 2, 500);
    }

    function auto_chief()
    {
        $width = $this->size->width;
        $height = $this->size->height * 0.3;
        $this->chief = new ShieldRect(
            new ShieldPos($width / 2, $height / 2),
            new ShieldSize($width, $height)
        );
    }

    static function best_height($shape)
    {
        if ( !empty(self::$custom_size[$shape]) )
            return self::$custom_size[$shape];
        if ( $shape == "flag" )
            return 600;
        return 1200;
    }

    static function from_options($options)
    {
        $width = 1000;
        $height = 1200;
        $cross_point = new ShieldPos(500, 500);
        $shape = $options["shape"];

        if ( isset($options["flagHeight"]) )
        {
            $height = $options["flagHeight"];
            $cross_point = new ShieldPos(500, $height / 2);
        }
        else if ( isset(static::$custom_size[$shape]) )
        {
            $height = static::$custom_size[$shape];
            $cross_point = new ShieldPos(500, $height / 2);
        }

        $charge_hint = null;

        $size = new ShieldSize($width, $height);

        switch ( $shape )
        {
            case 'pauldron':
                $charge_hint = new ShieldRect(new ShieldPos(500, 605), new ShieldSize(510, 580));
                break;
            case 'lozenge':
                $charge_hint = new ShieldRect(clone $cross_point, $size->scaled(0.8));
                break;
            case 'scutum':
            case 'horsehead':
            case 'african':
                $charge_hint = new ShieldRect(new ShieldPos(500, 600), new ShieldSize(640, 1200));
                $cross_point = new ShieldPos(500, 600);
                break;
            default:
            break;
        }

        return new ShieldLayout(
            $size,
            $shape,
            $cross_point,
            $charge_hint,
            true
        );
    }

    static function square()
    {
        $width = 1000;
        $height = 1000;
        $cross_point = new ShieldPos(500, 500);
        $shape = 'square';

        $charge_hint = null;

        $size = new ShieldSize($width, $height);

        return new ShieldLayout(
            $size,
            $shape,
            $cross_point,
            $charge_hint,
            true
        );
    }

    
    function cross_point_ratio()
    {
        return new ShieldPos(
            $this->cross_point->x / $this->size->width,
            $this->cross_point->y / $this->size->height
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
