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

$fillet = $node && $node->getAttribute('keyterm') == "fillet-saltire";

$width = 200;

if ( $fillet )
{
    $width = 100;
    $ordinary['feature_size'] = 15;
}

$angle = atan2($layout->size->height, $layout->size->width);

$top = $left = 0;
$bottom = $layout->size->height;
$right = $layout->size->width;

$cx = $layout->size->width / 2;
$cy = $layout->size->height / 2;
$dx = $width * cos($angle);
$dy = $width * sin($angle);

$path = new MutableShape();
$path->move_to($cx,             $cy - $dy)
      ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
     ->line_to($right,          $top - $dy)
     ->tag_segment(BezierSegmentFlags::PLAIN)
     ->line_to($right + $dx,    $top)
     ->tag_segment(BezierSegmentFlags::UNDERSIDE|BezierSegmentFlags::STRAIGHT)
     ->line_to($cx + $dx,       $cy)
     ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
     ->line_to($right + $dx,    $bottom)
     ->tag_segment(BezierSegmentFlags::PLAIN)
     ->line_to($right,          $bottom + $dy)
     ->tag_segment(BezierSegmentFlags::UNDERSIDE|BezierSegmentFlags::STRAIGHT)
     ->line_to($cx,             $cy + $dy)
     ->tag_segment(BezierSegmentFlags::UNDERSIDE|BezierSegmentFlags::STRAIGHT)
     ->line_to($left,           $bottom + $dy)
     ->tag_segment(BezierSegmentFlags::PLAIN)
     ->line_to($left - $dx,     $bottom)
     ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
     ->line_to($cx - $dx,       $cy)
     ->tag_segment(BezierSegmentFlags::UNDERSIDE|BezierSegmentFlags::STRAIGHT)
     ->line_to($left - $dx,     $top)
     ->tag_segment(BezierSegmentFlags::PLAIN)
     ->line_to($left,           $top - $dy)
     ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
     ->line_to($cx,             $cy - $dy)
     ->close();


$ordinary['shape_spec'] = $path;

