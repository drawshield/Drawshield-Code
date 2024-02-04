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
require_once('size.inc');

class Quarter
{
    function __construct($pos, $size, $scale, $rel_size, $shape, $charge_hint = null, $section = "quarter")
    {
        $this->pos = $pos;
        $this->size = $size;
        $this->scale = $scale;
        $this->rel_size = $rel_size;
        $this->charge_hint = $charge_hint;
        $this->section = "quarter";
        $this->shape = $shape;
        $this->cross_point = null;
    }

    function svg_transform()
    {
        return "transform='translate({$this->pos->x}, {$this->pos->y}) scale({$this->scale})'";
    }

    function to_layout()
    {
        return new ShieldLayout($this->size, $this->shape, $this->cross_point, $this->charge_hint, true, $this->section);
    }
}

class Quartering
{
    public $layout, $quarters;
    private $y;

    function __construct($layout)
    {
        $this->layout = $layout;
        $this->y = 0;
        $this->quarters = [];
    }

    function add_row($n_columns, $height_mult = null)
    {
        if ( $height_mult === null )
            $height_mult = 1 - $this->y;

        $qsize = $this->layout->size->scaled(1 / $n_columns, $height_mult);
        $norm_factor = $qsize->normalize_factor();
        $qsize_norm = $qsize->scaled($norm_factor);

        for ( $i = 0; $i < $n_columns; $i++ )
        {
            array_push($this->quarters, new Quarter(
                new ShieldPos(
                    $i * $qsize->width,
                    $this->y * $this->layout->size->height
                ),
                $qsize_norm,
                1 / $norm_factor,
                $qsize,
                $this->layout->shape
            ));
        }

        $this->y += $height_mult;

    }

    function grid()
    {
        $grid = "<g fill='none' stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\">\n";
        foreach ( $this->quarters as $q )
        {
            if ( $q->pos->x > 0 )
                $grid .= "<path d='M {$q->pos->x} {$q->pos->y} v {$q->rel_size->height}' />";
            if ( $q->pos->y > 0 )
                $grid .= "<path d='M {$q->pos->x} {$q->pos->y} h {$q->rel_size->width}' />";
        }
        $grid .= "</g>\n";
        return $grid;
    }


    function render_svg(DOMNodeList $children, $clips = null)
    {
        global $options;

        $retval = '';

        foreach ( $children as $kid )
        {
            $index = $kid->getAttribute('index');
            $index -= 1; // Arrays are zero based

            // error check
            if (!isset($this->quarters[$index]))
            {
                ds_error_log("bad quarter index in " . $options['blazon'] );
                continue;
            }

            $quarter = $this->quarters[$index];
            $width = $quarter->size->width;
            $height = $quarter->size->height;

            $transform = $quarter->svg_transform();
            $layout = $quarter->to_layout();

            if ( $clips !== null )
            {
                $clipdef = add_def( 'clipPath', "<path d=\"{$clips[$index]}\" />");
                $retval .=
                    "<g clip-path='url(#$clipdef)'><g $transform >"
                    . makeshield($kid, $layout)
                    . "</g></g>\n";
            }
            else
            {
                $clipdef = add_def( 'clipPath', "<path d=\"M0,0 h$width v$height h-$width v-$height Z\" />");
                $retval .=
                    "<g clip-path='url(#$clipdef)' $transform >"
                    . makeshield($kid, $layout)
                    . "</g>\n";
            }
        }

        $retval .= add_def();
        return $retval . ($clips !== null ? "" : $this->grid());
    }

}

function makeQuartered($across, $down, $children, ShieldLayout $layout)
{
    $quartering = new Quartering($layout);

    $h = 1 / $down;
    for ( $y = 0; $y < $down; $y++ )
        $quartering->add_row($across, $h);

    if ( $down == 1 )
    {
        $ratio = $layout->cross_point_ratio();
        foreach ( $quartering->quarters as $quarter )
            $quarter->cross_point = new ShieldPos($ratio->x * $quarter->size->width, $ratio->y * $quarter->size->height);
    }

    return $quartering->render_svg($children);
}

function makequartered4( DOMNodeList $children, ShieldLayout $layout )
{
    $quartering = new Quartering($layout);

    $h = $layout->cross_point_ratio()->y;
    $quartering->add_row(2, $h);
    $quartering->add_row(2);
    $quartering->quarters[0]->section = "quarter-chief-dexter";
    $quartering->quarters[1]->section = "quarter-chief-sinister";
    $quartering->quarters[2]->section = "quarter-base-dexter";
    $quartering->quarters[3]->section = "quarter-base-sinister";

    return $quartering->render_svg($children);
}

function makequartered5( $children, ShieldLayout $layout )
{
    $quartering = new Quartering($layout);

    $h = $layout->cross_point_ratio()->y;
    $quartering->add_row(3, $h);
    $quartering->add_row(2);

    $quartering->quarters[0]->section = "quarter-chief-dexter";
    $quartering->quarters[2]->section = "quarter-chief-sinister";
    $quartering->quarters[3]->section = "quarter-base-dexter";
    $quartering->quarters[4]->section = "quarter-base-sinister";

    return $quartering->render_svg($children);
}

function makequartered6( $children, ShieldLayout $layout )
{
    $quartering = new Quartering($layout);

    $h = $layout->cross_point_ratio()->y;
    $quartering->add_row(3, 0.5);
    $quartering->add_row(3);

    return $quartering->render_svg($children);
}

function makequartered8( $children, ShieldLayout $layout )
{
    $quartering = new Quartering($layout);

    if ( $layout->shape == "flag" || $layout->size->height != 1200 )
    {
        $quartering->add_row(3, 1/3);
        $quartering->add_row(3, 1/3);
        $quartering->add_row(2);
    }
    else
    {
        $quartering->add_row(3, 0.3);
        $quartering->add_row(3, 0.3);
        $quartering->add_row(2);
    }

    return $quartering->render_svg($children);
}

function make_quartered_saltire( DOMNodeList $children, ShieldLayout $layout )
{
    $quartering = new Quartering($layout);

    $h = $layout->cross_point_ratio()->y;

    $q1_size = $layout->size->scaled(1, $h);
    $hmargin = $layout->size->height / 1000;

    $quartering->quarters[] = new Quarter(
        new ShieldPos(0, 0),
        $q1_size,
        1,
        $q1_size,
        $layout->size,
        new ShieldRect(
            new ShieldPos($q1_size->width * 0.5, $q1_size->height / 3 + 30 * $hmargin),
            $q1_size->scaled(0.4, 0.6)
        )
    );

    $side_size = $layout->size->scaled(0.5, $h * 2);
    $norm_factor = $side_size->normalize_factor();
    $side_size_norm = $side_size->scaled($norm_factor);

    $quartering->quarters[] = new Quarter(
        new ShieldPos(0, 0),
        $side_size_norm,
        1/$norm_factor,
        $side_size,
        $layout->size,
        new ShieldRect(
            new ShieldPos($side_size_norm->width / 3 + 50, $side_size_norm->height / 2),
            $side_size_norm->scaled(2/3, 0.4)
        )
    );
    $quartering->quarters[] = new Quarter(
        new ShieldPos($layout->size->width / 2, 0),
        $side_size_norm,
        1/$norm_factor,
        $side_size,
        $layout->size,
        new ShieldRect(
            new ShieldPos($side_size_norm->width * 2 / 3 - 50, $side_size_norm->height / 2),
            $side_size_norm->scaled(2/3, 0.4)
        )
    );

    $q4_size = $layout->size->scaled(1, 1-$h);
    $quartering->quarters[] = new Quarter(
        new ShieldPos(0, $h * $layout->size->height),
        $q4_size,
        1,
        $q4_size,
        $layout->size,
        new ShieldRect(
            new ShieldPos($q4_size->width * 0.5, $q4_size->height * 2 / 3 - 50 * $hmargin),
            $q4_size->scaled(0.4, 2/3)
        )
    );

    $center_x = $layout->size->width / 2;
    $center_y = $layout->size->height * $h;
    $bottom_s = $layout->size->height * $h * 2;
    $bottom = $layout->size->height;
    $right = $layout->size->width;
    $clips = [
        "M 0 0 L $center_x $center_y $right 0 z",
        "M 0 0 L $center_x $center_y 0 $bottom_s z",
        "M $right 0 L $center_x $center_y $right $bottom_s z",
        "M $center_x $center_y L $right $bottom_s $right $bottom 0 $bottom 0 $bottom_s z",
    ];

    $grid = "<g fill='none' stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\">\n";
    $grid .= "<path d='M 0 0 L $right $bottom_s M $right 0 L 0 $bottom_s'/>\n";
    $grid .= "</g>";
    return $quartering->render_svg($children, $clips) . $grid;
}
