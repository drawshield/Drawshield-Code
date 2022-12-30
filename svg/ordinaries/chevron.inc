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

require_once("chevron-helper.inc");


if ( existModifierWithKeyterm($node,'throughout') )
{
    $delta_y = $layout->size->height * 0.28;
    $ordinary['shape_spec'] = (new MutableShape())
        ->move_to($layout->size->width/2, 0)
        ->line_to($layout->size->width, $layout->size->height - $delta_y)
        ->line_to($layout->size->width, $layout->size->height)
        ->line_to($layout->size->width/2, $delta_y)
        ->line_to(0, $layout->size->height)
        ->line_to(0, $layout->size->height - $delta_y)
        ->close()
    ;
}
elseif ( existModifierWithKeyterm($node,'rompu') )
{
    $y = max(0, $layout->size->height - 1000);
    $ordinary['shape_spec'] = "V1X500Y{$y}B100c282B800d200F600g282f100d100c282D600f200H800g282H100";
}
elseif ( existModifierWithKeyterm($node,'burst') )
{
    $ordinary['shape_spec'] = (string)path_builder($layout->cross_point->x, $layout->cross_point->y)
        ->up_right_plain(200)
        ->down_right(800)
        ->down_left_plain(200)
        ->up_left(800)
        ->down_left(800)
        ->up_left_plain(200)
        ->up_right(800)
        ->down_right_plain(200)
    ;
}
elseif ( existModifierWithKeyterm($node,'ploye') )
{
    $y = max(0, $layout->size->height - 1000);
  $ordinary['body'] = "<path
  d='M 500,$y c -72.76993,254.89798 -279.24912,466.23116 -532.396698,544.87 0,85.24966 0,170.49933 0,255.74899 203.487968,-47.94427 391.259858,-158.79523 532.396698,-312.91399 141.09407,154.18423 328.93916,264.91397 532.40702,312.91399 0,-85.24966 0,-170.49933 0,-255.74899 -253.66335,-79.02684 -459.28469,-289.37357 -532.40702,-544.87 z'
  />";
}
elseif ( existModifierWithKeyterm($node,'fracted') )
{
    $y = $layout->cross_point->y;
    $ordinary['body'] = "<path d='M 500,$y l -100,100 v 231.67 l 100,-91.67 100,100 v -240 z m -100,-140 -500,500 v 240 l 500,-500 z m 200,0 v 240 l 500,500 v -240 z' />";
}
elseif ( existModifierWithKeyterm($node,'disjointed') )
{
    $y = $layout->cross_point->y - 140;
    $ordinary['body'] = "<path d='m 400,$y l -500,500 v 240 l 500,-500 z m 200,0 v 240 l 500,500 v -240 z' />";
}
elseif ( existModifierWithKeyterm($node,'removed') )
{
    $y = $layout->cross_point->y;
    $ordinary['body'] = "<path d='m 500,$y -600,600 v 240 l 600,-600 z m 0,-240 v 240 l 600,600 v -240 z' />";
}
elseif ( existModifierWithKeyterm($node,'inarched') )
{
    $y = $layout->cross_point->y - 283;
    $ordinary['body'] = "<path fill-rule='evenodd'
    d='m 500,$y -707,707 141.4,141.4 369,-369 c -15,61.6 -23,130.78424 -23.3,203.3 0,300 100,500 220,500 120,0 220,-200 220,-500 0,-72.5 -8.4,-141.7 -23.3,-203.3 l 369,369 141.4,-141.4 -707,-707 m 0,323 c 80,0 140,150 140,350 0,210 -60,310 -140,310 -80,0 -140,-100 -140,-310 0,-200 60,-350 140,-350 z'
    />";
}
else
{

    $mutable = new MutableShape();
    $number = (int)$node->getAttribute('number');
    $width = chevron_width($layout, $number);
    switch ( $number )
    {
        case '2':
            $sep = $width * 2;
            build_chevron($layout, $width, -$sep/4, $mutable);
            build_chevron($layout, $width, $sep*3/4, $mutable);
            break;
        case '3':
            $sep = $layout->cross_point->y / M_SQRT2;
            build_chevron($layout, $width, -$sep + 50, $mutable);
            build_chevron($layout, $width, 50, $mutable);
            build_chevron($layout, $width, $sep + 50, $mutable);
            break;
        default: // just do one
            build_chevron($layout, $width, 0, $mutable);
            break;
    }
    $ordinary['shape_spec'] = $mutable;

    if ( existModifierWithKeyterm($node,'compony1') )
    {
        $y = $layout->cross_point->y + 42;
        $ordinary['body2'] = "<path d='m 957,$y -285,286 125,125 283,-285 z m -912,0 286,286 -126,125 -285,-285 z m 456,-45 -205,-205 -125,125 285,285 45,-45 45,45 286,-285 -126,-125 -205,205' />";
    }
    elseif (existModifierWithKeyterm($node,'compony2') )
    {
        $y = $layout->cross_point->y - 130;
        $ordinary['body2'] = "<path d='m 500,$y -565,565 -125,-125 125,-125 250,250 125,-125 -250,-250 126,-127 314,313 313,-313 127,127 -260,260 130,130 260,-260 120,120 -125,125 -565,-565 m 0,125 -188,-188 188,-187 188,187 z' />";
    }

    if ( division_to_modifier($node, 'quarterly') )
        $ordinary['body2'] = 'V1X500Y100C259D848g259h848   V1X1100Y958c259f849G259B849';
}

$chevron_width = 200;
//$ordinary['rotate_pattern'] = 45;

$ordinary['rotate_pattern'] = 'translate(4,5) rotate(-45) scale(0.6) ';

$ordinary['chargesOn'] = placement_chevron($layout, $chevron_width, 0.6, $node);

$within_margin = 80;

$within_top_height = ($layout->cross_point->y - $chevron_width * M_SQRT2);
$within_top_margin = $within_top_height > 200 ? $within_margin / 2 : 0;
$within_top = new ShieldRect(
    new ShieldPos($layout->cross_point->x, ($within_top_height - $within_top_margin) / 2 + $within_top_margin),
    new ShieldSize($layout->size->width * 0.3, $within_top_height - $within_top_margin)
);

$within_bottom_height = ($layout->size->height - $layout->cross_point->y - $within_margin) * 2/3;
$within_bottom = new ShieldRect(
    new ShieldPos($layout->cross_point->x, $layout->cross_point->y + $within_bottom_height),
    new ShieldSize($within_bottom_height, $within_bottom_height)
);
$side_off = $chevron_width / M_SQRT2 + $within_margin / 2 / M_SQRT2;
$within_left = ShieldRect::from_corners($within_margin, $within_margin, $layout->cross_point->x - $side_off, $layout->cross_point->y - $side_off);
$within_right = ShieldRect::from_corners($layout->cross_point->x + $side_off, $within_margin, $layout->size->width - $within_margin, $layout->cross_point->y - $side_off);

$ordinary['chargesWithin'] = array_to_placement([
    [],
    [$within_top, $within_bottom],
    [$within_left, $within_right, $within_bottom],
    [$within_top, $within_left, $within_right, $within_bottom],
    [$within_top, $within_left, $within_right, $within_bottom->relative_box(0.25, 0.5, 0.45, 1), $within_bottom->relative_box(0.75, 0.5, 0.45, 1)],
    [$within_top, $within_left, $within_right, $within_bottom->relative_box(0.25, 0.7, 0.45, 0.45), $within_bottom->relative_box(0.75, 0.7, 0.45, 0.45), $within_bottom->relative_box(0.5, 0.2, 0.5, 0.45)]
]);


if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $ref = 475;
  for ($i = 5; $i < 450; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',' . (245 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . $i . ',' . (455 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . (970 - $i) . ',' . (245 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . (970 - $i) . ',' . (455 + $ref) . ')">' . $rivet . '</g>';
    $ref -= 80;
  }
  $ref = 0;
  $rivetList .= '<g transform="translate(485,' . (245 + $ref) . ')">' . $rivet . '</g>';
  $rivetList .= '<g transform="translate(485,' . (455 + $ref) . ')">' . $rivet . '</g>';
  $ordinary['body3'] = $rivetList;
}
