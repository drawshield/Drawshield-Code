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

function build_bend_fracted($layout, $width = 200, $count = 2, $path_builder = null) {
    $unit = $layout->size->height / 5;
    if ($unit > 200) $unit = 200;

    $finalDistance = ($unit + $unit + ($layout->size->height - (5 * $unit))) * M_SQRT2;
    $forwardLeg = $unit * M_SQRT2;
    $backwardLeg = ($unit/2) * M_SQRT2;

    if ( $path_builder == null )
        $path_builder = new PathBuilder();

    return $path_builder
        ->move( $width + ($unit / 2), 0)
        ->down_right($forwardLeg)
        ->down_left($backwardLeg)
        ->down_right($forwardLeg)
        ->down_left($backwardLeg)
        ->down_right($finalDistance)
        ->left_plain($width)
        ->up_left($finalDistance)
        ->up_right($backwardLeg)
        ->up_left($forwardLeg)
        ->up_right($backwardLeg)
        ->up_left($forwardLeg)
        ->right_plain($width)
        ;
}

function build_bend($layout, $width, $margin, $offset = 0, $path_builder = null)
{
    $base = min($layout->cross_point->x, $layout->cross_point->y) - $margin;
    $length = $base * M_SQRT2 * 2;
    $offset_comp = $offset / M_SQRT2;
    if (!$layout->chief)
        $offset_comp -= 50;

    if ( $path_builder == null )
        $path_builder = new PathBuilder();

    return $path_builder
        ->move($layout->cross_point->x - $base + $offset_comp, $layout->cross_point->y - $base - $offset_comp)
        ->up_right_plain($width/2)
        ->down_right($length)
        ->down_left_plain($width)
        ->up_left($length)
        ->up_right_plain($width/2)
    ;
}


function placement_in_bend($boundingBox, $sinister, $angle, $scale = 0.23)
{
    $placement_arr = [];
    $mult = $sinister ? 1 : -1;
    // Special handling for a single charge
    $plac = [];
    $plac[] = $boundingBox->size->width / 2 . ',' . $boundingBox->size->width / 2 . ',' .
                $boundingBox->size->width * 0.8 * 0.23 . ',' . $boundingBox->size->height * 0.8 . ',45,0,0';
    $placement_arr[] = $plac;
    // for the rest, ensure we are working with a square
    $min_extent = min($boundingBox->size->width, $boundingBox->size->height);
    if ( $boundingBox->size->width > $boundingBox->size->height )
        $boundingBox->pos->x += $mult * ($boundingBox->size->width - $min_extent) / 2;
    else
        $boundingBox->pos->y -= ($boundingBox->size->height - $min_extent) / 2;

    $boundingBox->size->width = $boundingBox->size->height = $min_extent;

    for ( $n = 2; $n <= 8; $n++ )
    {
        $plac = [];
        $size = 1 / $n * 0.8;
        $start = 0.1 + $size / 2;
        for ( $i = 0; $i < $n; $i++ )
        {
            $pos = $start + $size * $i;
            $plac[] = box_placement(
                $boundingBox->relative_box(
                    $sinister ? 1 - $pos : $pos,
                    $pos,
                    $scale,
                    $scale
                ),
                $angle
            );
        }

        $placement_arr[] = $plac;

    }
    return array_to_placement($placement_arr);
}

function bend_within($layout, $width)
{

    $cx = $layout->cross_point->x;
    $cy = $layout->cross_point->y;
    $lw = $layout->size->width;
    $lh = min($layout->size->height, $layout->size->width);
    $width_delta = $width / 2 / M_SQRT2 + 10;
    $within_margin = 50;
    $within_2 = [
        ShieldRect::from_corners($cx + $width_delta, $within_margin, $lw - $within_margin, $cy - $width_delta),
        ShieldRect::from_corners($within_margin, $cy + $width_delta, $cx - $width_delta, $lh - $within_margin)
    ];
    $within_4 = [
        ShieldRect::from_corners($cx, $within_margin, $within_2[0]->left() + $within_2[0]->size->width / 3, $within_2[0]->bottom() - $within_2[0]->size->height / 3),
        ShieldRect::from_corners($within_2[0]->left() + $within_2[0]->size->width / 3, $within_2[0]->bottom() - $within_2[0]->size->height / 3, $lw - $within_margin, $cy),

        ShieldRect::from_corners($within_margin, $cy, $within_2[1]->right() - $within_2[1]->size->width / 3, $within_2[1]->top() + $within_2[1]->size->height / 3),
        ShieldRect::from_corners($within_2[1]->right() - $within_2[1]->size->width / 3, $within_2[1]->top() + $within_2[1]->size->height / 3, $cx, $lh - $within_margin),
    ];

    return array_to_placement([
        "",
        $within_2,
        [
            $within_2[0],
            $within_4[2],
            $within_4[3],
        ],
        $within_4,
        "",
        [
            new ShieldRect($within_4[0]->pos, $within_4[0]->size->scaled(0.8)),
            new ShieldRect($within_4[1]->pos, $within_4[1]->size->scaled(0.8)),
            new ShieldRect(new ShieldPos($within_4[1]->pos->x, $within_4[0]->pos->y), $within_4[0]->size->scaled(0.8)),

            new ShieldRect($within_4[2]->pos, $within_4[2]->size->scaled(0.8)),
            new ShieldRect($within_4[3]->pos, $within_4[3]->size->scaled(0.8)),
            new ShieldRect(new ShieldPos($within_4[2]->pos->x, $within_4[3]->pos->y), $within_4[0]->size->scaled(0.8)),
        ]

    ]);
}
