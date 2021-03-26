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


$num_of = (int)getModifierValueByKeyterm($node, 'ofnum', false);
if ( $num_of == null )
    $num_of = 8;
else if ( $num_of < 6 )
    $num_of = 6;
else if ( $num_of % 2 )
    $num_of += 1;

$builder = new SvgPathBuilder();

$angle = M_PI * 2 / $num_of;
$radius = 2000;
for ( $i = 0; $i < $num_of; $i += 2 )
{
    $a1 = $angle * $i;
    $a2 = $angle * ($i+1);
    $builder->move($layout->center_point->x, $layout->center_point->y)
        ->line($layout->center_point->x + $radius * sin($a1), $layout->center_point->y - $radius * cos($a1))
        ->line($layout->center_point->x + $radius * sin($a2), $layout->center_point->y - $radius * cos($a2))
        ->close();
    ;
}

$division['div2'] = $builder->to_path();
