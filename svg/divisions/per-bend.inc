<?php

$y = $layout->division_offset->y - 106;

$div_data = array (
  'per-bend' => "X-106Y{$y}B1714c300e1212g1512",
  'per-bend-fracted' => "X200Y{$layout->division_offset->y}B717e500B1131c100e1010g1410",
  'per-bend-sinister' => "X1000Y{$layout->division_offset->y}c1400e1000g400H1414",
  'per-bend-sinister-fracted' => "X800Y{$layout->division_offset->y}a200c1300e1000H1131e500H717",
);

$item = 'per-bend';
if ( existModifierWithKeyterm($node, 'sinister')) $item .= '-sinister';
if ( existModifierWithKeyterm($node, 'fracted')) $item .= '-fracted';
$division['div2'] = createBody($div_data[$item], getModifierByType($node, 'linetype', false),60, $layout);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $offset = $layout->division_offset->y;
  for ($i = 995; $i > 0; $i -= 80) {
    if ( $item == 'per-bend-sinister' ) {
      $rivetList .= '<g transform="translate(' . (930 - $i) . ',' . ($i  + $offset). ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (980 - $i) . ',' . ($i + 50 + $offset) . ')">' . $rivet . '</g>';
    } else {
      $rivetList .= '<g transform="translate(' . ($i + 50 - $offset) . ',' . $i . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i - $offset) . ',' . ($i + 50) . ')">' . $rivet . '</g>';
    }
  }
  $division['divx'] = $rivetList;
}

