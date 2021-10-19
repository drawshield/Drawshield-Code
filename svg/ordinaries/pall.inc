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


// This shape cannot be simply inverted, we actually need a different shaped to match per-pall inverted.
if ( ($inv = getModifierNodeWithKeyterm($node,'inverted')) != null ) {
    $node->removeChild($inv);
    $width = 141;
    $height = $layout->cross_point->y - $width / M_SQRT2;
    $arm_l = 1000;
    $ordinary['shape_spec'] = (string)path_builder($layout->cross_point->x, $layout->cross_point->y)
        ->down_left($arm_l - $width / 2 / M_SQRT2)
        ->up_left_plain($width)
        ->up_right(991) // arm sides look like the same length but are not really
        ->up($height)
        ->right_plain($width)
        ->down($height)
        ->down_right(991)
        ->down_left_plain($width)
        ->up_left($arm_l - $width / 2 / M_SQRT2)
    ;
    $within_margin = 80;
    $within_bottom_height = ($layout->size->height - $layout->cross_point->y - $within_margin) * 2/3;
    $within_bottom = new ShieldRect(
        new ShieldPos($layout->cross_point->x, $layout->cross_point->y + $within_bottom_height),
        new ShieldSize($within_bottom_height, $within_bottom_height)
    );
    $side_off = $width / M_SQRT2;
    $within_left = ShieldRect::from_corners($within_margin, $within_margin, $layout->cross_point->x - $side_off * 2, $layout->cross_point->y - $side_off / 2);
    $within_right = ShieldRect::from_corners($layout->cross_point->x + $side_off * 2, $within_margin, $layout->size->width - $within_margin, $layout->cross_point->y - $side_off / 2);

    $ordinary['chargesWithin'] = array_to_placement([
        [],
        [$within_left, $within_right],
        [$within_left, $within_right, $within_bottom],
        [
            $within_left->relative_box(0.5, 0.2, 1, 0.5),
            $within_left->relative_box(0.5, 0.8, 1, 0.5),
            $within_right->relative_box(0.5, 0.2, 1, 0.5),
            $within_right->relative_box(0.5, 0.8, 1, 0.5),
        ],
        [
            $within_left->relative_box(0.5, 0.2, 1, 0.5),
            $within_left->relative_box(0.5, 0.8, 1, 0.5),
            $within_right->relative_box(0.5, 0.2, 1, 0.5),
            $within_right->relative_box(0.5, 0.8, 1, 0.5),
            $within_bottom
        ],
        [
            $within_left->relative_box(0.5, 0.2, 1, 0.5),
            $within_left->relative_box(0.5, 0.8, 1, 0.5),
            $within_right->relative_box(0.5, 0.2, 1, 0.5),
            $within_right->relative_box(0.5, 0.8, 1, 0.5),
            $within_bottom->relative_box(0.5, 0.2, 1, 0.5),
            $within_bottom->relative_box(0.5, 0.8, 1, 0.5),
        ]
    ]);
} else {
  $ordinary['shape_spec'] = 'X92Y-8B577H577a100c100D618C870e141G870F618g100a100';
  $ordinary['chargesWithin'] =  'x/220,760,240,400:780,760/220,760,240,400:500,150,180,180:780,760,240,400/220,600,200,180:780,600:220,840:780,840/220,600,200,180:780,600:500,150,180,180:220,840,200,180:780,840/x';
}



$ordinary['feature_size'] = 30;


