<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see https://www.gnu.org/licenses/. */

$version = array (
  'release' => '(development)',
  'date' => '(unkown)',
  'website' => 'drawshield.net',
  'author' => 'Karl R. Wilcox',
  'email' => 'contact@drawshield.net',
);

if (file_exists('release.txt'))
  $version['release'] = @(trim(file_get_contents('release.txt')));
if (file_exists('date.txt'))
  $version['date'] = @(trim(file_get_contents('date.txt')));

// Set default values for arguments
$options = array(
  // These options are potentially set by URL arguments
  'blazon' => '',
  'palette' => 'drawshield',
  'printable' => false,
  'filename' => 'shield',
  'size' => 500,
  'saveFormat' => 'png',
  'outputFormat' => 'svg',
  'asFile' => false,
  'effect' => 'none',
  'stage' => 'render',
  'shape' => 'default',
    'printSize' => 450, // 5 inches at 90ppi
    'units' => 'px',
  'aspectRatio' => 0.5,
  // These options can only be set here, or are set later by the program
  'logBlazon' => false,
  'notify' => false,
  'useWebColours' => false,
  'useWarhammerColours' => false,
    'useTartanColours' => false,
);
