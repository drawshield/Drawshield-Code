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

$num_of = getModifierValueByKeyterm($node, 'ofnum', true);
if ( $num_of < 1 ) $num_of = 8;
// Each chevron is counted, not just the coloured ones
$num = $num_of / 2;

$NC = '';
$CH = '';
$stepV = 846.5 / $num;
$width = $stepV * 0.707;
$upper = 848.4 + $width;
$stepVC = 700.0 / $num;
$widthC = $stepVC * 0.707;
$upperC = 848.4 + $widthC;
for ( $i = 0; $i < $num; $i++ ) {
  $startY = 1200 - ( $i * 2 * $stepV);
  $NC .= "V1X500Y{$startY}D848f{$width}H{$upper}B{$upper}d{$width}F848     ";
  $startYC = 1200 - ( $i * 2 * $stepVC );
  $CH .= "V1X500Y{$startYC}D848f{$widthC}H{$upperC}B{$upperC}d{$widthC}F848     ";
}
$division['div2'] = createBody(choose_fit( compact ( 'CH', 'NC' )), getModifierByType($node,'linetype'),40);

