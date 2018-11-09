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


$ordinary['shape_spec'] = 'X-8,Y{%75*}A1016c{%26*}e1016g{%26*}';

if ($options['drawType'] == 'flag') {
  $ordinary['chargesOn'] = array ('h','100,{%77*},800,{%21*}');
} else {
  $ordinary['chargesOn'] = '500,{%87*},500,{%21*}/333.33,{%87*},300,{%21*}:666.67,{%87*}/333.33,{%87*},160,{%21*}:500,{%87*}:666.67,{%87*}/x/x/x';
}

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',915)">' . $rivet . '</g>';
  }
  $ordinary['body3'] = $rivetList;
}
