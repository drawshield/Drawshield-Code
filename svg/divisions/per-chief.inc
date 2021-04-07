<?php

$lineType = getModifierByType($node, 'linetype', false);

if ( $layout->chief )
    $y = $layout->chief->bottom();
else if ( $layout->division_offset->y > 0 )
    $y = $layout->division_offset->y;
else
    $y = $layout->size->height * 0.3;

$margin = 100;

$builder = new PathBuilder();
$builder->move(-$margin, -$margin)
    ->right_plain($layout->size->width + $margin * 2)
    ->down_plain($y + $margin)
    ->left($layout->size->width + $margin * 2)
    ->up_plain($y + $margin);

$division['div2'] = createBody((string)$builder, $lineType, 60, $layout);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',' . ($y - 35) .')">' . $rivet . '</g>';
  }
  $division['divx'] = $rivetList;
}



