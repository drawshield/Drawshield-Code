<?php

$lineType = getModifierByType($node, 'linetype', false);
if ( $lineType == 'nebuly' || $lineType == 'potenty' )
  $div_data = 'X-100Y-100a1200c400E1200g400';
else
  $div_data = 'X-8Y-8a1016c308E1016g308';

$division['div2'] = createBody($div_data, $lineType, 60);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',265)">' . $rivet . '</g>';
  }
  $division['div4'] = $rivetList;
}



