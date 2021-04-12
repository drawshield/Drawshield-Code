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
$ordinary['shape_spec'] = (string)path_builder(-$margin, -$margin)
    ->right_plain($layout->center_point->x + $margin)
    ->down($layout->center_point->y + $margin)
    ->left($layout->center_point->x + $margin)
    ->up_plain($layout->center_point->y + $margin)
;

$ordinary['if_ord_on'] = 'scale';
// $qSize = (int)($calcHeight / 2);
// $outerEdge = $qSize - 40;
$on_margin = 50;
$ordinary['chargesOn'] = array("h", ShieldRect::from_corners($on_margin, $on_margin, $layout->center_point->x - $on_margin, $layout->center_point->y - $on_margin));

