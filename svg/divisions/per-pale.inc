<?php
$div_data = array ( 'NC' => 'X500Y0a500c{%100*}e500G{%100*}', 'CH' => 'X500Y0a500c1300e500G1100g200' );
$division['div2'] = createBody(choose_fit($div_data), getModifierByType($node, 'linetype', false), 60, $layout);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1200; $i += 80) {
    $rivetList .= '<g transform="translate(465,' . $i . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(515,' . $i . ')">' . $rivet . '</g>';
  }
  $division['divx'] = $rivetList;
}



