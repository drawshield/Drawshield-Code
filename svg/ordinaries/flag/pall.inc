<?php

$width = 100;
$margin = 0;
$angle = atan2($layout->center_point->y, $layout->center_point->x);
$length = hypot($layout->center_point->y, $layout->center_point->x);

$builder = new SvgPathBuilder();

$dx = $width / 2 / sin($angle);
$dy = cos($angle) * $width / 2;
$fy = $width/2;
$fx = -($dy - $fy) / tan($angle);

$builder->move(-$dx, 0)
    ->line($layout->center_point->x - $dx, $layout->center_point->y)
    ->line(-$dx, $layout->size->height)
    ->line($fx, $layout->size->height + $fy)
    ->line($layout->center_point->x + $fx, $layout->center_point->y + $fy)
    ->line($layout->size->width + 100, $layout->center_point->y + $fy)
    ->line($layout->size->width + 100, $layout->center_point->y - $fy)
    ->line($layout->center_point->x + $fx, $layout->center_point->y - $fy)
    ->line($fx, -$fy)
;

$ordinary['body'] = $builder->to_path();
