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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */


$ordinary['body'] ='<path d="m -8,-8 48,0 c 0,0 194,32 195,608 1,580 -195,610 -195,610 l -48,0 z" />';

if ( $node->getAttribute('number') == '2') $ordinary['body'] .= '<path d="m 1015.0038,-8 -47.99999,0 c 0,0 -194,32 -195,608 -1,580 195,610 195,610 l 47.99999,0 z" />';

$ordinary['chargesWithin'] = array ('n','300,300,400,600');
