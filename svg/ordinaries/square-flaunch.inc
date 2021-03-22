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


$builder = new SvgPathBuilder();

$margin = 8;
$builder
    ->move(-$margin, -$margin)
    ->line($layout->size->width / 3, $layout->center_point->y)
    ->line(-$margin, $layout->center_point->y * 2 + $margin)
    ->close()
;

if ( $node->getAttribute('number') == '2')
{
    $builder
        ->move($layout->size->width + $margin, -$margin)
        ->line($layout->size->width * 2 / 3, $layout->center_point->y)
        ->line($layout->size->width + $margin, $layout->center_point->y * 2 + $margin)
        ->close()
    ;
}

$ordinary['body'] = $builder->to_path();

$ordinary['chargesWithin'] = array ('n','300,300,400,600');
