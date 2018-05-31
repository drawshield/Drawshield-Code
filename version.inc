<?php

$version = array (
  'release' => '2.6r',
  'date' => '28 May 2018',
  'name' => 'Version ',
  'website' => 'drawshield.net',
  'flagsite' => 'drawflag.net',
  'author' => 'Karl R. Wilcox',
  'email' => 'karlwilcox36@gmail.com',
);

// Set default values for arguments
$options = array(
  // These options are potentially set by URL arguments
  'drawType' => 'shield',
  'blazon' => '',
  'palette' => 'drawshield',
  'printable' => false,
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
);