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

$margin = 100;

$num_of = (int)getModifierValueByKeyterm($node, 'ofnum', false);
if ( $num_of == 0 )
    $num_of = count($tinctures);

$pale = strpos($node->getAttribute('keyterm'), "pal") !== false;
if ( $pale )
{
    $thickness = $layout->size->width / $num_of;
}
else
{
    $thickness = $layout->size->height / $num_of;
}

$lineType = getModifierByType($node, 'linetype', false);

for ( $i = 0; $i < $num_of; $i++ )
{
    $p = $thickness * $i;
    $builder = new PathBuilder();
    $plain_end = $i + 1 == $num_of ? "_plain" : "";

    if ( $pale )
    {
        $end = "down$plain_end";
        $builder->move($p, -$margin)
            ->right_plain($thickness)
            ->$end($layout->size->height + $margin * 2)
            ->left_plain($thickness)
            ->up($layout->size->height + $margin * 2);
    }
    else
    {
        $end = "left$plain_end";
        $builder->move(-$margin, $p)
            ->right($layout->size->width + $margin * 2)
            ->down_plain($thickness)
            ->$end($layout->size->width + $margin * 2)
            ->up_plain($thickness);
    }

    $division["div".($i+1)] = createBody((string)$builder, $lineType, 60, $layout);
}

if ( count($tinctures) < $num_of )
{
    $copy = $tinctures;
    while ( count($tinctures) < $num_of )
        $tinctures = array_merge($tinctures, $copy);

    if ( count($tinctures) > $num_of )
        $tinctures = array_slice($tinctures, 0, $num_of);
}
