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


$num_of = getModifierValueByKeyterm($node, 'ofnum', true);
if ( $num_of < 4 )
    $num_of = 6;
elseif ( $num_of > 10 )
    $num_of = 10;

$suffix = '-of' . $num_of;



$file = getBestMatchFilename(__dir__, 'star', $suffix);
if ($file != '')
    $charge['file'] = $file;


