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

// Process arguments
if (isset($_GET['blazon'])) $options['blazon'] = html_entity_decode(strip_tags(trim($_GET['blazon'])));
if (isset($_GET['outputformat'])) $options['outputFormat'] = strip_tags ($_GET['outputformat']); else $options['outputFormat'] = 'png';
if (isset($_GET['palette'])) $options['palette'] = strip_tags($_GET['palette']);
if (isset($_GET['effect'])) $options['effect'] = strip_tags($_GET['effect']);
if (isset($_GET['size'])) {
  $size = strip_tags ($_GET['size']);
  if ( $size < 100 ) $size = 100;
  $options['size'] = $size;
}

  include "parser/parser.inc";
  $p = new parser('english');
  $dom = $p->parse($options['blazon'],'dom');
  $p = null; // destroy parser
  // Resolve references
  include "analyser/references.inc";
  $references = new references($dom);
  $dom = $references->setReferences();
  $references = null; // destroy references

  // Read in the drawing code  ( All formats start out as SVG )
  $xpath = new DOMXPath($dom);
  include "svg/draw.inc";
  $output = draw();
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
      // $im->paintTransparentImage('white',0.0,1000.0);
      // $im->scaleimage(1000,1200);
      header('Content-Type: image/png');
       echo $im->getimageblob();
      break;
    default:
      break;
}

