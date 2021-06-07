<?php
$div_data = 'X500Y0a500c{%100*}e500G{%100*}';
$division['div2'] = createBody($div_data, getModifierByType($node, 'linetype', false), 60, $layout);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1200; $i += 80) {
    $rivetList .= '<g transform="translate(465,' . $i . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(515,' . $i . ')">' . $rivet . '</g>';
  }
  $division['divx'] = $rivetList;
}


if ( count($tinctures) > 2 )
    include "multi-bar.inc";
