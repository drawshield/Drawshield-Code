<?php

$version = array (
  'release' => '(development)',
  'date' => '6th October 2019',
  'website' => 'drawshield.net',
  'author' => 'Karl R. Wilcox',
  'email' => 'karlwilcox36@gmail.com',
);

if (file_exists('release.txt'))
  $version['release'] = @file_get_contents('release.txt');

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
  'effect' => 'shiny',
  'stage' => 'render',
  'shape' => 'default',
  'aspectRatio' => 0.5,
  // These options can only be set here, or are set later by the program
  'logBlazon' => false,
  'notify' => false,
  'useWebColours' => false,
  'useWarhammerColours' => false,
    'useTartanColours' => false,
);