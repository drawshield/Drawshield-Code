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


$ordinary['body'] = '<path d="m -42.8,94.3 c 0,0 94.2,405.7 273.8,485.7 -165.6,135 9,650 9,650 l -288.5,-10 z" />';
if ( $node->getAttribute('number') == '2') $ordinary['body'] .= '<path d="m 1034.3,94.3 c 0,0 -94.2,405.7 -273.8,485.7 165.6,135 -9,650 -9,650 l 288.5,-10 z" />';
$ordinary['chargesOn'] = '130,700,160,160/130,600,140,140:130,800/s/s/s/s';
