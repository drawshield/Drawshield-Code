<?php


$builder_2 = new MutableShape();
$builder_2->move_to(-$layout->cross_point->x, -$layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
    ->line_to($layout->cross_point->x, $layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
    ->line_to($layout->size->width + $layout->cross_point->x, $layout->size->height + $layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->vertical(-$layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::MIRROR|BezierSegmentFlags::STRAIGHT)
    ->line_to($layout->cross_point->x, $layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::MIRROR|BezierSegmentFlags::STRAIGHT)
    ->line_to(-$layout->cross_point->x, $layout->size->height + $layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->close()
;

$builder_3 = new MutableShape();
$builder_3->move_to(-$layout->cross_point->x, $layout->size->height + $layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
    ->line_to($layout->cross_point->x, $layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
    ->line_to($layout->size->width + $layout->cross_point->x, $layout->size->height + $layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->close()
;

$builder_4 = new MutableShape();
$builder_4->move_to($layout->size->width + $layout->cross_point->x, -$layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
    ->line_to($layout->cross_point->x, $layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
    ->line_to($layout->size->width + $layout->cross_point->x, $layout->size->height + $layout->cross_point->y)
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->close()
;

$lineType = getModifierByType($node, 'linetype', false);
$division['div2'] = createBody($builder_2, $lineType, 30, $layout);
$division['div3'] = createBody($builder_3, $lineType, 30, $layout);
$division['div4'] = createBody($builder_4, $lineType, 30, $layout);

