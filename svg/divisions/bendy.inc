<?php /* Copyright 2010 Karl R. Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

if ( ($num_of = getModifierValueByKeyterm($node, 'ofnum')) == null ) $num_of = 8;
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
    $X2 = 500 - (($i-1) * $stepC);
    $Y2 = 600 - (($i-1) * $stepC);
    $NC .= "X{$X1}Y{$Y1}D1600f{$stepB}H1600b{$stepB}   ";
    $CH .= "X{$X2}Y{$Y2}b{$stepB}D1300f{$stepB}H1300   ";
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
