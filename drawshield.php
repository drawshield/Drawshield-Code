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

//
// Global Variables
//
$options = array();
include 'version.inc';
/**
 * @var DOMDocument $dom
 */
$dom = null;
/**
 * @var DOMXpath $xpath
 */
$xpath = null;
/**
 * @var messageStore $messages
 */
$messages = null;

//
// Argument processing
//
if ( isset($argc) and  $argc > 1 ) { // run in debug mode, probably
  $options['blazon'] = implode(' ', array_slice($argv,1));
  // $options['printable'] = true;
   $options['outputFormat'] = 'png';
}

// Process arguments
if (isset($_GET['blazon'])) $options['blazon'] = html_entity_decode(strip_tags(trim($_GET['blazon'])));
if (isset($_GET['saveformat'])) $options['saveFormat'] = strip_tags ($_GET['saveformat']);;
if (isset($_GET['outputformat'])) $options['outputFormat'] = strip_tags ($_GET['outputformat']);;
if (isset($_GET['asfile'])) $options['asFile'] = true;
if (isset($_GET['palette'])) $options['palette'] = strip_tags($_GET['palette']);
if (isset($_GET['stage'])) $options['stage'] = strip_tags($_GET['stage']);
if (isset($_GET['printable'])) $options['printable'] = true;
if (isset($_GET['effect'])) $options['effect'] = strip_tags($_GET['effect']);
if (isset($_GET['size'])) {
  $size = strip_tags ($_GET['size']);
  if ( $size < 100 ) $size = 100;
  $options['size'] = $size;
}
// Quick response for empty blazon
if ( $options['blazon'] == '' ) { // TODO "your shield here" message?
  header('Content-Type: text/xml; charset=utf-8');
  $output = '<?xml version="1.0" encoding="utf-8" ?><svg version="1.1" baseProfile="full" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" preserveAspectRatio="xMidYMid meet" height="' .
    ( $options['size'] * 1.2) . '" width="' .  $options['size'] . '" viewBox="0 0 1000 1200" >
    <g clip-path="url(#clipPath1)"><desc>argent</desc><g><title>Shield</title><g fill="#F0F0F0">
    <rect x="0" y="0" width="1000" height="1200" ><title>Field</title></rect></g></g></g>
    <defs><clipPath id="clipPath1" > <path d="M 0 0 L 0 800 A 800 400 0 0,0 500 1200 A 800 400 0 0,0 1000 800 L 1000 0 Z" /> </clipPath></defs>
    <text x="10" y="1160" font-size="30" >'
   . $version['name'] . ' ' . $version['release'] . '</text><text x="10" y="1190" font-size="30" >' . $version['website'] . '</text></svg>';
} else {
  // Otherwise log the blazon for research... (unless told not too)
  if ( $options['logBlazon']) error_log($options['blazon']);

  include "parser/parser.inc";
  $p = new parser('english');
  $dom = $p->parse($options['blazon'],'dom');
  $p = null; // destroy parser
  // Resolve references
  if ( $options['stage'] == 'parser') { echo $dom->saveXML(); exit; }
  include "analyser/references.inc";
  $references = new references($dom);
  $dom = $references->setReferences();
  $references = null; // destroy references
  if ( $options['stage'] == 'references') { echo $dom->saveXML(); exit; }
  // Add dictionary references
  include "analyser/addlinks.inc";
  $adder = new linkAdder($dom);
  $dom = $adder->addLinks();
  $adder = null; // destroy adder
  if ( $options['stage'] == 'links') { echo $dom->saveXML(); exit; }

  // Read in the drawing code  ( All formats start out as SVG )
  $xpath = new DOMXPath($dom);
  include "svg/draw.inc";
  $output = draw();
}

// Output content header
if ( $options['asFile'] ) {
  switch ($options['saveFormat']) {
    case 'svg':
     header("Content-type: application/force-download");
     header('Content-Disposition: inline; filename="shield.svg"');
     header("Content-Transfer-Encoding: text");
     header('Content-Disposition: attachment; filename="shield.svg"');
     header('Content-Type: image/svg+xml');
     echo $output;
     break;
    case 'jpg':
      $im = new Imagick();
      $im->readimageblob($output);
      $im->setimageformat('jpeg');
      $im->setimagecompressionquality(90);
      // $im->scaleimage(1000,1200);
      header("Content-type: application/force-download");
      header('Content-Disposition: inline; filename="shield.jpg"');
      header("Content-Transfer-Encoding: binary");
      header('Content-Disposition: attachment; filename="shield.jpg"');
      header('Content-Type: image/jpg');
      echo $im->getimageblob();
      break;
    case 'png':
    default:
     $im = new Imagick();
     $im->readimageblob($output);
     $im->setimageformat('png');
     // $im->scaleimage(1000,1200);
     header("Content-type: application/force-download");
     header('Content-Disposition: inline; filename="shield.png"');
     header("Content-Transfer-Encoding: binary");
     header('Content-Disposition: attachment; filename="shield.png"');
     header('Content-Type: image/png');
     echo $im->getimageblob();
     break;
   }
} else {
  switch ($options['outputFormat']) {
    case 'jpg':
      $im = new Imagick();
      $im->readimageblob($output);
      $im->setimageformat('jpeg');
      $im->setimagecompressionquality(90);
      // $im->scaleimage(1000,1200);
      header('Content-Type: image/jpg');
      echo $im->getimageblob();
      break;
    case 'png':
      $im = new Imagick();
      $im->readimageblob($output);
      $im->setimageformat('png');
      // $im->scaleimage(1000,1200);
      header('Content-Type: image/png');
       echo $im->getimageblob();
      break;
    default:
    case 'svg':
      header('Content-Type: text/xml; charset=utf-8');
      echo $output;
      break;
  }
}

