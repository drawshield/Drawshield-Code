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


$item = strtolower(getModifierValueByKeyterm($node,'value'));
if ( $item == 'sulfur' ) {
  $charge['file'] = '../alchemical/sulphur.svg';
} elseif ( file_exists( $folder . '../alchemical/' . $item . '.svg')) {
  $charge['file'] = "../alchemical/$item.svg";
} elseif ( file_exists( $folder . '../astronomical/' . $item . '.svg')) {
  $charge['file'] = "../astronomical/$item.svg";
} elseif ( file_exists( $folder . '../zodiac/' . $item . '.svg')) {
  $charge['file'] = "../zodiac/$item.svg";
} else {
  $charge['file'] = "../alchemical/fire.svg";
  $messages->addMessage('warning',"No symbol for $item (using fire)");
}

