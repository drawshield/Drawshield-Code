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

if ( ($num_of = getModifierValueByKeyterm($node, 'ofnum')) == null ) $num_of = 8;
if ($num_of < 1)
    $num_of = 4;
elseif ($num_of > 24)
    $num_of = 24;


$diag_off1 = 0;
$diag_off2 = 0;
if ( $layout->section != "quarter" )
{
    switch ( $layout->shape )
    {
        case "heater":  $diag_off1 = 100; break;
        case "french":  $diag_off1 = 70; break;
        case "oval":    $diag_off1 = $diag_off2 = 170; break;
        case "lozenge": $diag_off1 = $diag_off2 = $layout->size->width / 4; break;
        case "italian": $diag_off1 = 190; $diag_off2 = 110; break;
        case "swiss":   $diag_off1 = 220; break;
        case "english": $diag_off1 = 100; break;
        case "german":  $diag_off1 = 150; $diag_off2 = 40; break;
        case "polish":  $diag_off1 = 170; $diag_off2 = 140; break;
        case "spanish": $diag_off1 = 120; break;
        case "circle":  $diag_off1 = $diag_off2 = 130; break;
        case "sca":   $diag_off1 = 220; break;
        case "pauldron":  $diag_off1 = 125; $diag_off2 = 260; break;
    }
}

$diag_off2 = max(0, $diag_off2-$layout->division_offset->y);

$area_width = $layout->size->width - $diag_off1 - $diag_off2;
$area_height = $layout->size->height - $layout->division_offset->y - $diag_off1 - $diag_off2;
$area_diag = hypot($area_width, $area_height);
$bend_width = $area_diag / $num_of;
$bend_length = max($layout->size->width, $layout->size->height) * M_SQRT2 + 100;
$bend_diag_off = $bend_length / M_SQRT2 / 2;

$builder = new PathBuilder();

if ( !existModifierWithKeyterm($node, 'sinister') )
{
    for ( $i = 0; $i < $num_of; $i += 2 )
    {
        $x = $area_width / $num_of * $i;
        $y = $area_height / $num_of * $i;
        $builder
            ->move($x - $bend_diag_off + $diag_off1, $layout->size->height - $y - $bend_diag_off - $diag_off1)
            ->down_right($bend_length)
            ->up_right_plain($bend_width)
            ->up_left($bend_length)
            ->down_left_plain($bend_width)
        ;
    }
}
else
{
    for ( $i = 0; $i < $num_of; $i += 2 )
    {
        $x = $area_width / $num_of * $i;
        $y = $area_height / $num_of * $i;
        $builder
            ->move($layout->size->width - $x + $bend_diag_off - $diag_off1, $layout->size->height - $y - $bend_diag_off - $diag_off1)
            ->down_left($bend_length)
            ->up_left_plain($bend_width)
            ->up_right($bend_length)
            ->down_right_plain($bend_width)
        ;
    }
}

$division['div2'] = createBody((string)$builder, getModifierByType($node,'linetype', false), 40, $layout);
