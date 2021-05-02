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


$y = ($layout->cross_point->y - 500);

$ordinary['body'] = "<path
    transform='translate(0, $y)'
    d='M 410,0 L 410,400 L 470,400 L 470,0 L 410,0 z M 530,0 L 530,520 L 590,520 L 590,0 L 530,0 z M 410,480 L 410,1200 L 470,1200 L 470,480 L 410,480 z M 530,600 L 530,1200 L 590,1200 L 590,600 L 530,600 z M 0,410 L 520,410 L 520,470 L 0,470 L 0,410 z M 600,410 L 1000,410 L 1000,470 L 600,470 L 600,410 z M 0,530 L 400,530 L 400,590 L 0,590 L 0,530 z M 480,530 L 1000,530 L 1000,590 L 480,590 L 480,530 z'
/>";




$old_ordinary = $ordinary;
include "plain-cross.inc";
$old_ordinary["chargesOn"] = $ordinary["chargesOn"];
$old_ordinary["chargesWithin"] = $ordinary["chargesWithin"];
$ordinary = $old_ordinary;
