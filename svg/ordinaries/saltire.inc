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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
 */



$fillet = $node && $node->getAttribute('keyterm') == "fillet-saltire";

$width = 200;

if ( $fillet )
{
    $width = 100;
    $ordinary['feature_size'] = 15;
}

$cx = $layout->cross_point->x;
$cy = $layout->cross_point->y;
$margin = 100;
$arm_length_internal = $margin + max($cx, $cy) * M_SQRT2;
$arm_length_external = $arm_length_internal - $width/2;


if ( existModifierWithKeyterm($node, 'nowy')) // special handling for some linetypes
    $ordinary['body'] = '<path d="m 64.801,-76.6 -141.401,141.401 306.17,306.169 a 300,300 0 0 0 -29.57,129.03 300,300 0 0 0 29.51,129.09 l -386.11,386.11 141.401,141.4 386.169,-386.17 a 300,300 0 0 0 129.03,29.57 300,300 0 0 0 129.09,-29.51 l 386.11,386.11 141.4,-141.4 -386.17,-386.17 a 300,300 0 0 0 29.57,-129.03 300,300 0 0 0 -29.51,-129.09 l 306.11,-306.109 -141.4,-141.401 -306.17,306.17 a 300,300 0 0 0 -129.03,-29.57 300,300 0 0 0 -129.09,29.51 z" />';
elseif ( existModifierWithKeyterm($node, 'quadrate')) // special handling for some linetypes
    $ordinary['body'] = '<path d="m 64.801,-76.6 -141.401,141.401 293.77,293.769 -141.434,141.43 141.434,141.43 -373.77,373.77 141.401,141.4 373.769,-373.77 141.43,141.43 141.43,-141.43 373.77,373.77 141.4,-141.4 -373.77,-373.77 141.43,-141.43 -141.43,-141.43 293.77,-293.769 -141.4,-141.401 -293.77,293.77 -141.43,-141.434 -141.43,141.434 z" />';
elseif ( existModifierWithKeyterm($node, 'square-pierced')) 
    $ordinary['body'] = '<path d="m -141.42068,1000 500,-500 141.421,141.421 L 0,1141.4203 -141.42068,1000 m 1282.84198,0 -141.421,141.42098 -500,-500 141.421,-141.421 500,500 M 1000,-141.42168 1141.4213,0 641.42132,500 l -141.421,-141.421 500,-500 m -500,500 -141.421,141.421 -500,-500 L 0,-141.42168"/>';
else
    $ordinary['shape_spec'] = (string)path_builder($cx - $arm_length_internal / M_SQRT2, $cy - $arm_length_internal / M_SQRT2)
        ->up_right_plain($width/2)
        ->down_right($arm_length_external)
        ->up_right($arm_length_external)
        ->down_right_plain($width)
        ->down_left($arm_length_external)
        ->down_right($arm_length_external)
        ->down_left_plain($width)
        ->up_left($arm_length_external)
        ->down_left($arm_length_external)
        ->up_left_plain($width)
        ->up_right($arm_length_external)
        ->up_left($arm_length_external)
        ->up_right_plain($width/2)

    ;

if ( existModifierWithKeyterm($node,'compony1') )
    $ordinary['body2'] = '<path d="M 941,684 690,935 815,1060 1070,810 z M 314,935 62.7,684 -62.7,810 188,1060 314,935 M 314,57.3 62.7,308 -62.7,183 188,-68.1 314,57.3 m 376,0 L 815,-68.1 1070,183 941,308 z M 439,183 l -63,63 0,-1 -125,126 -63,63 63,62 -63,63 251,251 63,-63 62,63 251,-251 -63,-63 63,-62 -63,-62 1,-1 -126,-126 -1,1 -62,-63 -62,63 -63,-63 z m 63,188 125,125 0,0 -125,126 -126,-126 63,-62 63,-63 z" />';
elseif (existModifierWithKeyterm($node,'compony2') )
    $ordinary['body2'] = '<path d="M 941,57.3 815,-68.1 941,-194 1070,-68.1 941,57.3 M 62.7,57.3 -62.7,-68.1 62.7,-194 188,-68.1 z M 941,935 815,1060 941,1190 1070,1060 z M 815,183 941,308 1070,183 941,57.3 815,183 M 690,308 564,183 690,57.3 815,183 z M 188,183 62.7,308 -62.7,183 62.7,57.3 z M 314,308 439,183 314,57.3 188,183 z M 815,810 941,684 1070,810 941,935 z M 690,684 564,810 690,935 815,810 z m 0,-376 -126,126 63,62 -63,63 L 690,684 815,559 752,496 815,434 690,308 z M 62.7,935 188,1060 62.7,1190 -62.7,1060 62.7,935 M 188,810 62.7,684 -62.7,810 62.7,935 z M 314,684 439,810 314,935 188,810 314,684 m 0,-376 -126,126 63,62 -63,63 126,125 125,-125 -63,-63 63,-62 -125,-126 z m 188,188 -63,63 63,63 62,-63 z m -63,-62 63,62 62,-62 -62,-63 -63,63" />';



if ( !$fillet )
{
    $on_size = new ShieldSize($width, $width);
    $on_center = new ShieldRect(clone $layout->cross_point, $on_size);

    $d = $width + 50/M_SQRT2;
    $d1 = ($width + 50)/M_SQRT2;
    $angle = 45;

    $ordinary['chargesOn'] = array_to_placement([
        [$on_center],
        "",
        "",
        "",
        [
            box_placement($on_center->translated(-$d, $d), $angle),
            box_placement($on_center->translated($d, -$d), $angle),
            box_placement($on_center->translated(-$d, -$d), -$angle),
            box_placement($on_center->translated($d, $d), -$angle),
            box_placement($on_center, 0),
        ],
        "",
        "",
        "",
        [
            box_placement($on_center->translated(-$d1, $d1), $angle),
            box_placement($on_center->translated($d1, -$d1), $angle),
            box_placement($on_center->translated(-$d1, -$d1), -$angle),
            box_placement($on_center->translated($d1, $d1), -$angle),
            box_placement($on_center, 0),
            box_placement($on_center->translated(-$d1*2, $d1*2), $angle),
            box_placement($on_center->translated($d1*2, -$d1*2), $angle),
            box_placement($on_center->translated(-$d1*2, -$d1*2), -$angle),
            box_placement($on_center->translated($d1*2, $d1*2), -$angle),
        ]
    ]);

    $ordinary['feature_size'] = 30;
}



/*
Algebra time!
To find the optimal position/size of a charge on the side of the saltire:
We consider the side outside the saltire ("half" of the shield minus width / sqrt(2))
We want a square with its center point on the line passing through the saltire center point
We want this square's edge to touch the edge of the shield, and two of its corners touching the saltire
Since the saltire might be off-center on the y axis, we get the smaller side between top, bottom, and left
(This assumes the saltire is centered horizontally).
The size of this square is 2/3rds of the available side

Rough visual example for the left side:


C is $cx - $width / sqrt(2)
HC is the segment we have available
P is the center of the square
PACB forms another square
_a_ is half the side of the square we want to find
since _a_ is the height of the triangle PAB, it must be half the diagonal of the square PACB
Therefore 2*_a_ is the distance between P and C
and 3*_a_ == HC
so the size of the square we want is 2/3 HC,
which is also the distance between P and C
(the distance between P and $cx needs + $width / sqrt(2))

 |`.
 |  `.
 |    `.
 |_______.A
 |       |`.
H|_a_._a_|_a_.C  $cx
 |   P   | ,'
 |_______|'
 |     ,' B
 |   ,'
 | ,'
 :'

*/
$within_left = $cx - $width / M_SQRT2;
$within_top = $cy - $width / M_SQRT2;
$within_bottom = $layout->size->height - $cy - $width / M_SQRT2;
$within_length = min([$within_left, $within_top, $within_bottom]) * 2 / 3;
$within_delta = $within_length + $width / M_SQRT2;
$within_size = new ShieldSize($within_length, $within_length);
$within_center = new ShieldRect(clone $layout->cross_point, $within_size);


$ordinary['chargesWithin'] = array_to_placement([
    [],
    "",
    "",
    "",
    [
        $within_center->translated(-$within_delta, 0),
        $within_center->translated(0, -$within_delta),
        $within_center->translated($within_delta, 0),
        $within_center->translated(0, $within_delta),
    ]
]);

$ordinary['rotate_pattern'] = 'translate(4,5) rotate(-45) scale(0.6) ';
