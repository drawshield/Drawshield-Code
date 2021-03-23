<?php

if ( ($num_of = getModifierValueByKeyterm($node, 'ofnum')) == null ) $num_of = 8;
if ($num_of < 1)
    $num_of = 4;
elseif ($num_of > 24)
    $num_of = 24;
$NC = '';
$stepH = 1000 / $num_of;
$stepC = floor($calcHeight / $num_of); // with a chief
$stepV = floor($calcHeight / $num_of);
$stepB = 1.414 * $stepH;
if ( existModifierWithKeyterm($node, 'sinister')) {
  for ( $i = 1; $i < $num_of; $i +=2 ) {
    $X1 = 1500 - (($i-1) * $stepH);
    $Y1 = 500 -(($i-1) * $stepH);
    $X2 = 1500 - (($i-1) * $stepC);
    $Y2 = 600 - (($i-1) * $stepC);
    $NC .= "X{$X1}Y{$Y1}D1700f{$stepB}H1700b{$stepB}   ";
    }
} else {
  for ( $i = 1; $i < $num_of; $i +=2 ) {
    $X1 = (($i-1) * $stepH) - 500;
    $Y1 = 500 - (($i-1) * $stepH);
    $X2 = (($i-1) * $stepC) - 500;
    $Y2 = 600 - (($i-1) * $stepC);
    $NC .= "X{$X1}Y{$Y1}h{$stepB}B1700d{$stepB}F1700   ";
  }
}

$division['div2'] = createBody( $NC, getModifierByType($node,'linetype', false), 40, $layout);
