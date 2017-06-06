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


$ordinary['shape_spec'] = 'X-8,Y900A1016c308e1016g308';

$ordinary['chargesOn'] = '500,950,500,240/333.33,950,300,200:666.67,950/333.33,900,160,160:666.67,900:500,1050/x/x/x';

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',915)">' . $rivet . '</g>';
  }
  $ordinary['body3'] = $rivetList;
}
