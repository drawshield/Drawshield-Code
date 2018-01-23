<?php

$version = array (
  'release' => '2.6q',
  'date' => '22 Jan 2018',
  'name' => 'Version ',
  'website' => 'drawshield.net',
  'author' => 'Karl R. Wilcox',
  'email' => 'karlwilcox36@gmail.com',
);

// Set default values for arguments
$options = array(
  // These options are potentially set by URL arguments
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
  // These options can only be set here, or are set later by the program
  'logBlazon' => false,
  'notify' => false,
);