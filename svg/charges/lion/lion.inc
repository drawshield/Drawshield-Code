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

// lion
// Make some assumptions
// TODO these should be null, and checked if set and used, error otherwise
$stance = 'rampant';
$looking = 'normal';
$tail = 'normal';
$charge['file'] = 'inkscape/lion-courant.svg';

$mods = getModifiersByTypeAsArray($node,'chargemod'); // Get array of modifiers
// Override assumptions if found
foreach ( $mods as $key => $value ) {
  switch ($key) {
    case 'rampant':
    case 'salient':
    case 'passant':
    case 'statant':
    case 'sejant':
    case 'sejant-erect':
    case 'couchant':
    case 'courant':
    case 'dormant':
    case 'coward':
    case 'cadent':
    case 'head-only':
      $stance = $key;
      existModifierWithKeyterm($node,$key,true); // Mark as used
      break;
    case 'nowedtail':
      $tail = 'nowed';
      existModifierWithKeyterm($node,$key,true); // Mark as used
      break;
    case 'forktail':
      $tail = 'fourche';
      existModifierWithKeyterm($node,$key,true); // Mark as used
      break;
    case 'forktailsaltire':
      $tail = 'saltire';
      existModifierWithKeyterm($node,$key,true); // Mark as used
      break;
    case 'forktailsaltirereverse':
      $tail = 'saltire-reverse';
      existModifierWithKeyterm($node,$key,true); // Mark as used
      break;
    case 'guardant':
    case 'reguardant':
      $looking = $key;
      existModifierWithKeyterm($node,$key,true); // Mark as used
      break;
  }
}

if ( existModifierWithKeyterm($node,'head-only'))
  $stance = 'head-only';

$filename = "inkscape/lion-$stance-$looking-$tail.svg";
if ( file_exists($folder . $filename) )
  $charge['file'] = $filename;
else {
  $filename = "inkscape/lion-$stance-$looking.svg";
  if ( file_exists($folder . $filename) )
    $charge['file'] = $filename;
  else {
    $filename = "inkscape/lion-$stance.svg";
    if ( file_exists($folder . $filename) )
      $charge['file'] = $filename;
    else {
      $messages->addMessage ( 'warning', 'Some lion modifiers not drawn' );
    }
  }
}
