<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see https://www.gnu.org/licenses/. */


$peaks = max((int)getModifierValueByKeyterm($node, 'points', true), 1);
if ( $node->getAttribute('keyterm') == "trimount" )
    $peaks = 3;

$shape = new MutableShape();

$height = $layout->size->height * 0.35;
$tip = $layout->size->height - $height;
$paths_arr = [];


$ystart = $tip + 60;

$arch = (new PolyBezier())
    ->add_point(new ShieldPos(0, $ystart))
    ->quadratic_to(
        new ShieldPos($layout->size->width * 1 / 4, $tip),
        new ShieldPos($layout->size->width * 2 / 4, $tip)
    )
    ->quadratic_to(
        new ShieldPos($layout->size->width * 3 / 4, $tip),
        new ShieldPos($layout->size->width, $ystart)
    )
    ->compile()
;

$max_per_row = 5;
$n_rows = ceil($peaks / $max_per_row);
$rows = [];
while ( $peaks )
{
    $n = ceil($peaks/($n_rows - count($rows)));
    $rows []= $n;
    $peaks -= $n;
}

$ystart = 0;
$dy = $height / 3;
for ( $rown = 0; $rown < count($rows); $rown++ )
{

    $scale = 0.75 ** $rown;
    $num_of = $rows[$rown];
    $target_width = $layout->size->width * $scale;

    $width = $target_width / max(1, $num_of-$rown/6-3);
    if ( $num_of == 4 )
        $width = $target_width * ($rown*.5/4 + .4);

    $dx = $target_width / max(1, $num_of-1);
    $handle = $width/3;

    if ( $num_of == 1 )
    {
        $start = $target_width / 2;
    }
    else if ( $num_of % 2 )
    {
        $start = 0;
    }
    else if ( $rown == 0 )
    {
        $start = $width/4;
        $dx = ($target_width-$width/2) / max(1, $num_of-1);
    }
    else
    {
        $start = 0;
    }

    $start += ($layout->size->width - $target_width) / 2;

    $path_svg = "";

    for ( $i = 0; $i < $num_of / 2; $i++ )
    {
        $path = new MutableShape();
        $tip_x = $start + $i * $dx;
        $tip_pos = new ShieldPos($tip_x, $ystart + $arch->point_at_length($tip_x / $layout->size->width * $arch->length())->y);
        $path->move_to($tip_pos->translated(-$width / 2, $height))
            ->quadratic_to(
                $tip_pos->translated(-$handle, 0),
                $tip_pos
            )
            ->quadratic_to(
                $tip_pos->translated($handle, 0),
                $tip_pos->translated($width / 2, $height)
            )
        ;

        $mirror_trans = new ShieldPos($layout->size->width - 2 * $tip_pos->x, 0);


        if ( $i != floor(($num_of - 0.5) / 2) )
        {
            $path_svg .= $path->to_svg_path();
            $path->translate($mirror_trans);
            $path_svg .= $path->to_svg_path();
        }
        else if ( $num_of % 2 )
        {
            $path_svg .= $path->to_svg_path();
        }
        else
        {
            $points = $path->last_path()->points();
            $arch_back = new CubicBezier($points[1]->pos, $points[1]->tan_out, $points[2]->tan_in, $points[2]->pos);
            $t = $arch_back->line_single_intersection_t(
                new ShieldPos($layout->size->width / 2, 0),
                new ShieldPos($layout->size->width / 2, $layout->size->height)
            );
            $bez = $arch_back->split($t)[0];
            $path->last_path()->points()[2]->pos = $bez->finish;
            $path->last_path()->points()[2]->tan_in = $bez->tan2;
            $path->last_path()->points()[1]->tan_out = $bez->tan1;
            $bez = $bez->translated(-$bez->finish->x, 0);
            $path->cubic_to(
                    $layout->size->width / 2 - $bez->tan2->x, $bez->tan2->y,
                    $layout->size->width / 2 - $bez->tan1->x, $bez->tan1->y,
                    $layout->size->width / 2 - $bez->start->x, $bez->start->y
                )
                ->quadratic_to(
                    $tip_pos->translated($dx+$handle, 0),
                    $tip_pos->translated($dx+$width/2, $height)
                )
            ;
            $path_svg .= $path->to_svg_path();

        }
    }

    array_unshift($paths_arr, $path_svg);
    $dy *= 0.8;
    $ystart -= $dy;
}

$paths = implode("", $paths_arr);

$ordinary['body'] = "<g style='stroke:#808080;stroke-width:3'>$paths</g>";
$ordinary['chargesOn'] = '500,950,500,240/333.33,950,300,200:666.67,950/333.33,900,160,160:666.67,900:500,1050/x/x/x';


$rect = $layout->rect_area()->margin(100, 100, 100, $height * 0.8);
$ordinary['chargesWithin'] = array("h", $rect);
