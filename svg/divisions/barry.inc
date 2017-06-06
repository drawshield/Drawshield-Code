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


if ( ($num_of = getModifierValueByKeyterm($node, 'ofnum', true)) == null ) $num_of = 8;

$NC = '';
$CH = '';
$stepC = 900 / $num_of; // with a chief
$stepV = 1200 / $num_of;
for ( $i = 1; $i < $num_of; $i +=2 ) {
  $Y1 = $i * $stepV;
  $YC = 300 + $i * $stepC;
  $NC .= "X-100Y{$Y1}A1200c{$stepV}E1200g{$stepV}   ";
  $CH .= "X-100Y{$YC}A1200c{$stepC}E1200g{$stepC}   ";
}
$division['div2'] = createBody( choose_fit (compact('NC', 'CH')), getModifierByType($node,'linetype'), 40);

