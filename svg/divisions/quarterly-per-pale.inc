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

$division['div2'] = createBody( 'O1Q1X500Y500C800e600g800a600   O1Q1X500Y500G600a600c600e600',
  getModifierByType($node,'linetype', false),60, $layout);
$division['div4'] = createBody( 'O1Q1X500Y{%50}C{%80}e600g{%80}a600',
  getModifierByType($node,'linetype', false),60, $layout);
$division['div3'] = createBody( 'O1Q1X500Y{%50}a600C{%80}e600G{%80}',
  getModifierByType($node,'linetype', false),60, $layout);
