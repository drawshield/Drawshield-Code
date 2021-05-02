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

$margin = 100;
$cx = $layout->cross_point->x;
$cy = $layout->cross_point->y;
$down = $layout->size->height - $layout->cross_point->y + $margin;
$right = $layout->size->width - $layout->cross_point->x + $margin;
$up = $cy + $margin;
$left = $cx + $margin;
$modifier = getModifierByType($node, 'linetype', false);

$division['div2'] = createBody(
    (string)(new PathBuilder)
        ->move($cx, $cy)
        ->offset(false)
        ->quarterly()
        ->down($down)
        ->left_plain($left)
        ->up_plain($down)
        ->right($left)

        ->move($cx, $cy)
        ->offset(false)
        ->quarterly()
        ->up($up)
        ->right_plain($right)
        ->down_plain($up)
        ->left($right)
    ,
    $modifier, 60, $layout
);
$division['div3'] = createBody(
    (string)path_builder($cx, $cy)
        ->offset(false)
        ->quarterly()
        ->down($down)
        ->right_plain($right)
        ->up_plain($down)
        ->left($right)
    ,
    $modifier, 60, $layout
);

$division['div4'] = createBody(
    (string)path_builder($cx, $cy)
        ->offset(false)
        ->quarterly()
        ->down($down)
        ->left_plain($left)
        ->up_plain($down)
        ->right($left)
    ,
    $modifier, 60, $layout
);
