<?php


if ( existModifierWithKeyterm($node, 'pointed')) {
    $shape = new MutableShape();
    $shape->move_to(-20, $layout->size->height * 1.01)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->vertical(-$layout->size->height*0.25, true)
        ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
        ->line_to($layout->size->width / 2 + 10, -$layout->size->height*0.1, true)
        ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
        ->line_to($layout->size->width / 2 + 10, +$layout->size->height*0.1, true)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->vertical($layout->size->height*0.25, true)
        ->close()
    ;
    $ordinary['shape_spec'] = $shape;
} else {
   $ordinary['shape_spec'] = 'X-8,Y{%75*}A1016c{%26*}e1016g{%26*}';
}

if ($layout->shape == 'flag') {
    $ordinary['chargesOn'] = array('h', '100,{%82*},800,{%15*}');
} else {
   $ordinary['chargesOn'] = '500,{%87*},500,{%21*}/333.33,{%87*},300,{%21*}:666.67,{%87*}/333.33,{%87*},160,{%21*}:500,{%87*}:666.67,{%87*}/x/x/x';
}
if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',915)">' . $rivet . '</g>';
  }
  $ordinary['body3'] = $rivetList;
}
