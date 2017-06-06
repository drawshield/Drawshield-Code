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


$ordinary['body'] ='<path d="m -8,-8 48,0 c 0,0 194,32 195,608 1,580 -195,610 -195,610 l -48,0 z" />';

if ( $node->getAttribute('number') == '2') $ordinary['body'] .= '<path d="m 1015.0038,-8 -47.99999,0 c 0,0 -194,32 -195,608 -1,580 195,610 195,610 l 47.99999,0 z" />';

$ordinary['chargesWithin'] = array ('n','300,300,400,600');
