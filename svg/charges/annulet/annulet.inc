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

// annulet

$annuletMods = array('concentric', 'interlaced', 'braced', 'conjunct',);
$num = $node->getAttribute('number');
if ( $num > 10 ) $num = 10;

$modded = '';
foreach($annuletMods as $aMod) {
  if (existModifierWithKeyterm($node, $aMod)) {
    if ($modded) {
      $messages->addMessage('parser',"Can only apply one modifier to annulets");
      break;
    }
    $modded = "-$aMod"; // only use first mod
  }   
}

$of_num = '';
if (existModifierWithKeyterm($node, 'ofnum')) {
  $of_num = '-of' . getModifierValueByKeyterm($node, 'ofnum');
  if (!$modded) { // set a default mod if "ofnum" is present alone
    $modded = "-concentric";
  }
}

if ($modded && $of_num == '') {  // if of_num not present, num of charges becomes of_num
  if ($num > 1) {
    $node->setAttribute('number','1');
  }
  $of_num = "-of$num";
}
// only set the filename if we have found relevant modifiers
if ($modded || $of_num) $charge['file'] = "annulet$modded$of_num.svg";
