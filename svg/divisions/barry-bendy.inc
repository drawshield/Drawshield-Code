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

if ( existModifierWithKeyterm($node, 'sinister'))
  $div_data = array ( 800, 800, 'm62.5,125h125l62.5,-125h-125z h-62.5v125z m125,0h62.5v125h-125z m-125,250h-62.5v-125h125z h125l-62.5,125h-125z m125,0h62.5v-125z' );
else
  $div_data =  array ( 250, 500, 'l67.5,125h125l-67.5,-125z m0,125v125h125l-62.5,-125z m187.5,0l62.5,125v-125z m0,250h62.5v-125h-125z m-125,0l-62.5,-125v125z l62.5,125h125l-62.5,-125z' );

$retval = '<path d="';
list($width, $height, $pattern ) = $div_data;
for ( $y = 0; $y < 1200; $y += $height )
  for ( $x = 0; $x < 1000; $x += $width )
    $retval .= "M$x,$y $pattern ";
$retval .= '" />';
$division['div2'] = $retval;

