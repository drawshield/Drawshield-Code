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


    $n = 3;
// default number of labels
if ( existModifierWithKeyterm($node, 'points') ) {
    $n = getModifierValueByKeyterm($node, 'points', true);
}
    if ( $n < 1 ) { $n = 1; }
    if ( $n > 10 ) { $n = 10; }
    $gapsize = (1000 - $n * 10) / $n;

    $body = '<path d="M1000,180h100v-80h-1200v80h100 ';
    $label = 'l-45,100h100l-45-100';
    $body .= sprintf ("h%4f", $gapsize / 2);
    for ( $i = 0; $i < $n; $i++ ) {
        $body .= $label;
        if ( $i != $n - 1 ) { $body .= 'h' . $gapsize; }
    }
    $body .= sprintf( "h%4fz\" />", $gapsize / 2);
    $ordinary['body'] = $body;
