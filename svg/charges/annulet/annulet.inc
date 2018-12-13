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

// annulet

$annuletMods = array('concentric', 'interlaced', 'braced', 'conjunct',);

$modded = false;
foreach($annuletMods as $aMod) {
  if (existModifierWithKeyterm($node, $aMod)) {
    if ($modded) {
      $messages->addMessage('parser',"Can only apply one modifier to annulets");
      break;
    }
    $num = $node->getAttribute('number');
    if ($num > 1) {
      $node->setAttribute('number','1');
      if ( $num > 10 ) $num = 10;
      $charge['file'] = "annulet-$aMod-of$num.svg";
    }
    $modded = true; // only use first mod
  }   
}

