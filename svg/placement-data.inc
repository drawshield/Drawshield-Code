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

$placementData = array (
    '*' => '>calcplace',
    'between-quadrate' => '///{@-20},{%30},{%16},{%16}:{@20},{%30},{%16},{%16}:{@-20},{%70}:{@20},{%70}', // Up to four items
    'between-chief' => 'x/125,700,200,600:875,700/250,450,160,160:750,450,160,160:500,950/250,450,160,160:500,700,700,300:750,450,160,160:250,950:750,950/250,450,160,160:500,700,700,300:750,450,160,160:500,200:250,950:750,950',
    'between' => 'x/125,500,200,800:875,500/250,200,300,280:750,200,300,280:500,1000/250,200,280,280:750,200,280,280:250,900:750,900/250,200,240,240:750,200,240,240:500,200:250,900:750,900',
    'between-facing-in' => 'x/200,600,250,1000,0,0,1:800,600,,,0,0,0/x/266,333,333,400,0,0,1:734,333,,,0,0,0:266,900,,,0,0,1:734,900,,,0,0,0/x/260,220,320,240,,,1:740,220,320,240,,,0:260,550,320,240,,,1:740,550,320,240,,,0:260,880,320,240,,,1:740,880,320,240,,,0',
    'between-facing-out' => 'x/200,600,300,1000,0,0,0:800,600,,,0,0,1/x/266,333,333,400,0,0,0:734,333,,,0,0,1:266,900,,,0,0,0:734,900,,,0,0,1/x/290,443,280,187:710,443,280,187,,,1:290,700,280,187,,,0:710,700,280,187,,,1:290,957,280,187,,,0:710,957,280,187,,,1',
    // 'between-facing-in-chief' => '',
    // 'between-facing-out-chief' => '',
    'inbend' => // need to rewrite as a helper function
                '500,500,400,800,45/
                333.33,333.33,180,180,45:666.67,666.67/
                {@-25},{%25},{%20},{%20},45:{@0},{%50}:{@25},{@25}/
                200,200,200,200,45:400,400:600,600:800,800/
                166.67,166.67,160,160,45:333.33,333.33:500,500:666.67,666.67:833.33,833.33/
                142.86,142.86,140,140,45:285.71,285.71:428.57,428.57:571.43,571.43:714.29,714.29:857.14,857.14'

);

function getBestMatchPlacement($target, $item) {
    global $messages, $placementData;
    $lowestPenalty = 9999;
    $bestEntry = '';
    $bestMissing = '';
    $bestExtra = '';
    $presentNotRequiredPenalty = 10;
    $requiredNotPresentPenalty = 20;
    $positionWeight = 2;
  
    $requiredParts = explode('-', $target);
    $numRequiredParts = count($requiredParts);
  
    foreach ($placementData as $entry => $value) {
      $penalty = 0;
      $currentRNPPenalty = $requiredNotPresentPenalty;
      $missing = '';
      $extra = '';
      $entryParts = preg_split('/(\.|-)/', $entry);
      $numEntryParts = count($entryParts);
  
      // check for required, not present
      for ($i = 0; $i < $numRequiredParts; $i++) {
        $found = false;
        for ($j = 0; $j < $numEntryParts; $j++) {
          if ($requiredParts[$i] == $entryParts[$j]) {
            $found = true;
            break;
          }
        }
        if (!$found) {
          $penalty += $currentRNPPenalty;
          $missing .= $requiredParts[$i] . ' ';
        }
        $currentRNPPenalty += $positionWeight;
      }
  
      // check for present, not required
      for ( $j = 0; $j < $numEntryParts; $j++) {
        $found = false;
        for ( $i = 0; $i < $numRequiredParts; $i++) {
          if ($requiredParts[$i] == $entryParts[$j]) {
            $found = true;
            break;
          }
        }
        if (!$found) {
          $penalty += $presentNotRequiredPenalty;
          $extra .= $entryParts[$j] . ' ';
        }
      }
      // ds_error_log("$entry scores $penalty");
      // compare to previous best fit
      if ( $penalty < $lowestPenalty ) {
        $lowestPenalty = $penalty;
        $bestValue = $value;
        $bestMissing = $missing;
        $bestExtra = $extra;
      }
      if ($penalty == 0 ) { // perfect match, skip out
        break;
      }
    }
    if ($bestMissing) $messages->addMessage('warning', $target . ' not arranged with ' . $bestMissing);
    // if ($bestExtra) $messages->addMessage('other', $base . ' is also ' . $bestExtra);
    if ($bestValue[0] == '>') {
        list($func,$arg) = explode('/', $bestValue . '//');
        $func = substr($func,1);
        $bestValue = $func($item, $arg);
    }
    return $bestValue;
}
