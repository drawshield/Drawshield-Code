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

$ordinary['if_ord_on'] = 'scale';
$ordinary['feature_size'] = 40;
$ordinary['shape_spec'] = 'X-{%1}Y-{%1}a{%31}C{%31}E{%31}g{%31}';
if ( $node->getAttribute('number') == '2') 
    $ordinary['shape_spec'] .=  '    X{%101}Y-{%1}c{%31}E{%31}G{%31}a{%31}';
$ordinary['chargePlacement'] = subPercent("{%3},{%3},{%24},{%24}", $layout);
