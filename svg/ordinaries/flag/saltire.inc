<?php


$width = 200;

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

