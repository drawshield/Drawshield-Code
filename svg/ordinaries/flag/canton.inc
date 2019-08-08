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


$ordinary['if_ord_on'] = 'scale';
$ordinary['feature_size'] = (int)($calcHeight / 30);;
$ordinary['shape_spec'] = 'X-10Y-{%1}a343C{%51}E343g{%51}';
// 2 cantons not supported on a flag
// if ( $node->getAttribute('number') == '2')
// $ordinary['shape_spec'] .= '    X{%70}Y-{%1}a{%31}c{%31}E{%31}G{%31}';
$qSize = (int)($calcHeight / 2);
$outerEdge = $qSize - 40;
$ordinary['chargesOn'] = array ('w', "20,20,293,$outerEdge");
