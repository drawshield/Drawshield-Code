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
$ordinary['feature_size'] = 40;
$ordinary['shape_spec'] = 'X-{%1}Y-{%1}a{%31}C{%31}E{%31}g{%31}';
if ( $node->getAttribute('number') == '2') $ordinary['shape_spec'] .= '    X{%70}Y-{%1}a{%31}c{%31}E{%31}G{%31}';
$ordinary['chargesOn'] = array ('w', (chiefPresent() ? '20,20,260,260' : '10,10,280,280'));
