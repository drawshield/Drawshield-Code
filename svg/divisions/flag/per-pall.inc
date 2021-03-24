<?php

$width = 100;
$margin = 100;
$builder_2 = new SvgPathBuilder();
$builder_2->move(-$margin, -$margin)
    ->line($layout->center_point->x, $layout->center_point->y)
    ->horizontal($layout->size->width + $margin)
    ->vertical(-$margin)
    ->horizontal(-$margin);
;

$builder_3 = new SvgPathBuilder();
$builder_3->move(-$margin, $layout->size->height + $margin)
    ->line($layout->center_point->x, $layout->center_point->y)
    ->horizontal($layout->size->width + $margin)
    ->vertical($layout->size->height + $margin)
    ->horizontal(-$margin);
;

$division['div2'] = $builder_2->to_path();
$division['div3'] = $builder_3->to_path();

