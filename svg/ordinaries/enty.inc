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


$y = ($layout->size->height - 1200);
$builder = SvgDParser::parse_d("M 888.70158,1124.8309 C 558.99143,923.1401 499.23001,705.74879 499.23001,705.74879 c 0,0 -59.76141,217.39131 -389.47156,419.08211 l 389.47156,112.6087 z");
$builder->translate(0, $y);
$ordinary['shape_spec'] = $builder;
$ordinary['chargesOn'] = '500,950,500,240/333.33,950,300,200:666.67,950/333.33,900,160,160:666.67,900:500,1050/x/x/x';
