<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

     DrawShield is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
 */

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
