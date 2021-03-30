<?php

$width = 100;
$margin = 100;
$builder_2 = new MutableShape();
$builder_2->move_to(0, 0)
    ->line_to($layout->center_point->x, $layout->center_point->y)
    ->horizontal($layout->size->width + $margin)
    ->vertical(-$margin)
    ->horizontal(-$margin)
    ->close()
;

$builder_3 = new MutableShape();
$builder_3->move_to(0, $layout->size->height)
    ->line_to($layout->center_point->x, $layout->center_point->y)
    ->horizontal($layout->size->width + $margin)
    ->vertical($layout->size->height + $margin)
    ->horizontal(-$margin)
    ->close()
;

$lineType = getModifierByType($node, 'linetype', false);
$division['div2'] = createBody($builder_2, $lineType, 30, $layout);
$division['div3'] = createBody($builder_3, $lineType, 30, $layout);
