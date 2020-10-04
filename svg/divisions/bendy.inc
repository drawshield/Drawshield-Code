<?php

if ( ($num_of = getModifierValueByKeyterm($node, 'ofnum')) == null ) $num_of = 8;
if ($num_of < 1)
    $num_of = 4;
elseif ($num_of > 24)
    $num_of = 24;
$NC = '';
$CH = '';
$stepH = 1000 / $num_of;
$stepC = 900 / $num_of; // with a chief
$stepV = 1200 / $num_of;
$stepB = 1.414 * $stepH;
if ( existModifierWithKeyterm($node, 'sinister')) {
  for ( $i = 1; $i < $num_of; $i +=2 ) {
    $X1 = 1500 - (($i-1) * $stepH);
    $Y1 = 500 -(($i-1) * $stepH);
    $X2 = 1500 - (($i-1) * $stepC);
    $Y2 = 600 - (($i-1) * $stepC);
    $NC .= "X{$X1}Y{$Y1}D1600f{$stepB}H1600b{$stepB}   ";
      $CH .= "X{$X2}Y{$Y2}D1300f{$stepB}H1300b{$stepB}   ";
    //$CH .= "X{$X2}Y{$Y2}b{$stepB}D1300f{$stepB}H1300   ";
    }
} else {
  for ( $i = 1; $i < $num_of; $i +=2 ) {
    $X1 = (($i-1) * $stepH) - 500;
    $Y1 = 500 - (($i-1) * $stepH);
    $X2 = (($i-1) * $stepC) - 500;
    $Y2 = 600 - (($i-1) * $stepC);
    $NC .= "X{$X1}Y{$Y1}h{$stepB}B1600d{$stepB}F1600   ";
    $CH .= "X{$X2}Y{$Y2}h{$stepB}B1300d{$stepB}F1300   ";
  }
}

$division['div2'] = createBody( choose_fit (compact('NC', 'CH')), getModifierByType($node,'linetype'), 40);
