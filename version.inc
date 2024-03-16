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

$version = array (
  'release' => "R.1335",
  'date' => "10th October 2023",
  'website' => 'drawshield.net',
  'author' => 'Karl R. Wilcox',
  'email' => 'contact@drawshield.net',
  'logBlazon' => false,
  'dummyAST' => '<blazon><shield keyterm="onfieldof" tokens="" linenumber="-" ID="N1-1"><simple><field><tincture index="1" origin="given" ID="N1-0"><colour keyterm="argent" tokens="argent" linenumber="1:1-1:6"></colour></tincture></field></simple></shield><input blazonText="argent"><token>1:1 argent=colour:argent   </token></input></blazon>',
);



if (file_exists('release.txt'))
  $version['release'] = @(trim(file_get_contents('release.txt')));
if (file_exists('date.txt'))
  $version['date'] = @(trim(file_get_contents('date.txt')));

// Set default values for arguments
$options = array(
  // These options are potentially set by URL arguments
  'blazon' => '',
  'logged' => false,
  'palette' => 'drawshield',
  'filename' => 'shield',
  'size' => 500,
  'background' => 'transparent',
  'margin' => 0,
  'saveFormat' => 'png',
  'outputFormat' => 'svg',
  'asFile' => false,
  'effect' => 'none',
  'requestSource' => $_SERVER['REQUEST_METHOD'] ?? "???",
  'shape' => 'heater',
  'printSize' => 450, // 5 inches at 90ppi
  'units' => 'px',
  'aspectRatio' => 0.5,
  'useWebColours' => false,
  'useWarhammerColours' => false,
  'useTartanColours' => false,
);
