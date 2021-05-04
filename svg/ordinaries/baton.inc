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


$base = min($layout->cross_point->x, $layout->cross_point->y) - 100;
$length = $base * M_SQRT2 * 2;
$width = 56;

$ordinary['shape_spec'] = (string)
    path_builder($layout->cross_point->x + $base, $layout->cross_point->y - $base)
    ->down_right_plain($width/2)
    ->down_left($length)
    ->up_left_plain($width)
    ->up_right($length)
    ->down_right_plain($width/2)
;
$ordinary['feature_size'] = 18;

