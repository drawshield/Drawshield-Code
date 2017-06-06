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
  $div_data = array ( 800, 800, 'm200,200l100,-300 300,-100 -100,300z m200,200l100,-300 300,-100 -100,300z m200,200l100,-300 300,-100 -100,300z m-400,400l100,-300 300,-100 -100,300z m-200,-200l100,-300 300,-100 -100,300z m-200,-200l100,-300 300,-100 -100,300z',);
else
  $div_data =  array ( 800, 800, 'm-200,200 l300,100 100,300 -300,-100z m200,-200l300,100 100,300 -300,-100z m200,-200l300,100 100,300 -300,-100z m400,400l300,100 100,300 -300,-100z m-200,200l300,100 100,300 -300,-100z m-200,200l300,100 100,300 -300,-100z', );

$retval = '<path d="';
list($width, $height, $pattern ) = $div_data;
for ( $y = 0; $y < 1200; $y += $height )
  for ( $x = 0; $x < 1000; $x += $width )
    $retval .= "M$x,$y $pattern ";
$retval .= '" />';
$division['div2'] = $retval;
