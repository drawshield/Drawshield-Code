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


$width = 100;
$cx = $layout->center_point->x;
$cy = $layout->center_point->y;
$margin = 100;
$arm_l = $cx - $width / 2 + $margin;
$arm_r = ($layout->size->width - $cx) - $width / 2 + $margin;
$arm_u = $cy - $width / 2 + $margin;
$arm_d = ($layout->size->height - $cy) - $width / 2 + $margin;

$ordinary['shape_spec'] = (string)path_builder(-$margin, $cy-$width/2)
            ->right($arm_l)
            ->up($arm_u)
            ->right_plain($width)
            ->down($arm_u)
            ->right($arm_r)
            ->down_plain($width)
            ->left($arm_r)
            ->down($arm_d)
            ->left_plain($width)
            ->up($arm_d)
            ->left($arm_l)
            ->up_plain($width)
;

$ordinary['feature_size'] = 15;


$ordinary['chargesWithin'] = 'x/250,750,180,180:750,750/x/250,250,180,180:750,250:250,750:750,750/x/x';
