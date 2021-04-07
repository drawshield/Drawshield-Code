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

$num_of = getModifierValueByKeyterm($node, 'ofnum', false);
if ( $num_of < 1 ) $num_of = 8;
// Each chevron is counted, not just the coloured ones
$num = $num_of / 2;

$stepV = ($layout->size->height - $layout->division_offset->y / 2) / 2 / ($num-1);
$width = $stepV / M_SQRT2;
$upper = $layout->size->width / 2 * M_SQRT2 + $width;
$down = $upper - $width;

$builder = new PathBuilder();

for ( $i = 0; $i < $num; $i++ ) {
  $startY = $layout->division_offset->y / 2 + ( $i * 2 * $stepV);
  $builder->move($layout->center_point->x, $startY)
    ->chevron()
    ->down_left($down)
    ->up_left_plain($width)
    ->up_right($upper)
    ->down_right($upper)
    ->down_left($width)
    ->up_left($down)
  ;

//   $NC .= "V1X500Y{$startY}D{$down}f{$width}H{$upper}B{$upper}d{$width}F{$down}     ";
}
$division['div2'] = createBody((string)$builder, getModifierByType($node,'linetype', false),40, $layout);

