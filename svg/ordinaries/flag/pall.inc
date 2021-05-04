<?php

$width = 100;
$angle = atan2($layout->cross_point->y, $layout->cross_point->x);
$length = hypot($layout->cross_point->y, $layout->cross_point->x);

$builder = new MutableShape();

$dx = $width / 2 / sin($angle);
$dy = cos($angle) * $width / 2;
$fy = $width/2;
$fx = -($dy - $fy) / tan($angle);

$builder->move_to(-$dx, 0)
    ->line_to($layout->cross_point->x - $dx, $layout->cross_point->y)
    ->line_to(-$dx, $layout->size->height)
    ->line_to($fx, $layout->size->height + $fy)
    ->line_to($layout->cross_point->x + $fx, $layout->cross_point->y + $fy)
    ->line_to($layout->size->width + 100, $layout->cross_point->y + $fy)
    ->line_to($layout->size->width + 100, $layout->cross_point->y - $fy)
    ->line_to($layout->cross_point->x + $fx, $layout->cross_point->y - $fy)
    ->line_to($fx, -$fy)
;

$ordinary['shape_spec'] = $builder;
