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
if ( file_exists( $folder . $item . '.svg'))
  $charge['file'] = "$item.svg";
else {
  $charge['file'] = "aries.svg";
  $messages->addMessage('warning',"No symbol for $item (using aries)");
}

$charge['licence'] = "By Tavmjong - Own work, CC BY 3.0, https://commons.wikimedia.org/w/index.php?curid=3544773";
