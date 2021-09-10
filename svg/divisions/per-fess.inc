<?php 

$lineType = getModifierByType($node, 'linetype', false);


$margin = 100;
$down = $layout->size->height - $layout->cross_point->y + $margin;

$builder = new PathBuilder();
$builder->move(-$margin, $layout->cross_point->y)
    ->right($layout->size->width + $margin * 2)
    ->down_plain($down)
    ->left_plain($layout->size->width + $margin * 2)
    ->up_plain($down);

$division['div2'] = createBody((string)$builder, $lineType, 60, $layout);


if ( count($tinctures) > 2 )
    include "multi-bar.inc";

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $y1 = $layout->cross_point->y - 35;
  $y2 = $layout->cross_point->y + 15;
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',' . $y1 . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . $i . ',' . $y2 . ')">' . $rivet . '</g>';
  }
  $division['divx'] = $rivetList;
}



