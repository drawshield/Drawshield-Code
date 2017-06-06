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
$svg = '';
$stepV = 1200 / $num_of;
if ( existModifierWithKeyterm($node, 'sinister')) {
  for ( $i = 1; $i < $num_of; $i +=2 ) {
      $Y1 = ($i - 1) * $stepV;
      $svg .= "M1000,{$Y1}l-1000,{$stepV} 1000,{$stepV}z ";
    }
} else {
  for ( $i = 1; $i < $num_of; $i +=2 ) {
      $Y1 = ($i - 1) * $stepV;
      $svg .= "M0,{$Y1}l1000,{$stepV} -1000,{$stepV}z ";
  }
}

$division['div2'] = '<path d="' . $svg . '" />';
