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


$builder = new MutableShape();

$margin = 8;
$w = $layout->size->width / 3;
$h = $layout->cross_point->y * 2;
$builder
    ->move_to(-$margin, -$margin)
    ->line_to($w, $layout->cross_point->y)
    ->line_to(-$margin, $h + $margin)
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->close()
;

$charge = new ShieldRect(new ShieldPos($w / 3, $layout->cross_point->y), new ShieldSize($w * 2/3, $h/3));
$ordinary['chargesOn'] = (string)$charge;

if ( $node->getAttribute('number') == '2')
{
    $ordinary['chargesOn'] .= "/$charge:" . $charge->translated($layout->size->width - $charge->size->width, 0);
    $builder
        ->move_to($layout->size->width + $margin, -$margin)
        ->line_to($layout->size->width * 2 / 3, $layout->cross_point->y)
        ->line_to($layout->size->width + $margin, $layout->cross_point->y * 2 + $margin)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->close()
    ;
}

$ordinary['shape_spec'] = $builder;

// $ordinary['chargesWithin'] = array ('n','300,300,400,600');
