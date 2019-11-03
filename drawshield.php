<?php 

//
// Global Variables
//
$options = array();
include 'version.inc';
include 'parser/utilities.inc';
include "analyser/utilities.inc";
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

$spareRoom = str_repeat('*', 1024 * 1024);
$size = 500;

function calculateAR($ar) {
  if (strpos($ar, ':') > 0) {
    $arParts = explode(':', $ar);
    $numerator = intval($arParts[0]);
    $denominator = 0;
    if (count($arParts) > 1) {
      $denominator = intval($arParts[1]);
    }
    if ($denominator == 0) $denominator = 1;
    $ar = $numerator / $denominator;
  } else {
    $ar = floatval($ar);
  }
  if ($ar > 1.2) {
    $ar = 1.2;
  } elseif ($ar < 0.25) {
    $ar = 0.25;
  }
  return $ar;  
}

//
// Argument processing
//
if (isset($argc)) {
  if ( $argc > 1 ) { // run in debug mode, probably
    $options['blazon'] = implode(' ', array_slice($argv,1));
  } else {
    // $options['blazon'] = "vert mantling to the sinister gules and or to the dexter vert and sable";
    $options['blazon'] = "vert achievement  helmet gules";
    // $options['shape'] = "flag";
    $options['stage'] = 'parser';
  }
  // $options['printable'] = true;
   $options['outputFormat'] = 'svg';
}

// Process arguments
$ar = null;
$size = null;
// For backwards compatibility we support argument in GET, but prefer POST
if (isset($_SERVER["REQUEST_METHOD"]) && $_SERVER["REQUEST_METHOD"] == "POST") {
  if (isset($_FILES['blazonfile']) && ($_FILES['blazonfile']['name'] != "")) {
    $fileName = $_FILES['blazonfile']['name'];
    $fileSize = $_FILES['blazonfile']['size'];
    $fileTmpName  = $_FILES['blazonfile']['tmp_name'];
   // $fileType = $_FILES['blazonfile']['type']; // not currently used
    if (preg_match('/.txt$/i', $fileName) && $fileSize < 1000000) {
      $options['blazon'] = file_get_contents($fileTmpName);
    } 
  } else {    
      if (isset($_POST['blazon'])) $options['blazon'] = html_entity_decode(strip_tags(trim($_POST['blazon'])));
  }
  if (isset($_POST['outputformat'])) $options['outputFormat'] = strip_tags ($_POST['outputformat']);;
  if (isset($_POST['saveformat'])) $options['saveFormat'] = strip_tags ($_POST['saveformat']);;
  if (isset($_POST['asfile'])) $options['asFile'] = ($_POST['asfile'] == "1");
  if (isset($_POST['palette'])) $options['palette'] = strip_tags($_POST['palette']);
  if (isset($_POST['shape'])) $options['shape'] = strip_tags($_POST['shape']);
  if (isset($_POST['stage'])) $options['stage'] = strip_tags($_POST['stage']);
//  if (isset($_POST['printable'])) $options['printable'] = ($_POST['printable'] == "1");
  if (isset($_POST['effect'])) $options['effect'] = strip_tags($_POST['effect']);
  if (isset($_POST['size'])) $options['size']= strip_tags ($_POST['size']);
  if (isset($_POST['ar'])) $ar = strip_tags ($_POST['ar']);
} else { // for old API
  if (isset($_GET['blazon'])) $options['blazon'] = html_entity_decode(strip_tags(trim($_GET['blazon'])));
  if (isset($_GET['saveformat'])) $options['saveFormat'] = strip_tags ($_GET['saveformat']);;
  if (isset($_GET['outputformat'])) $options['outputFormat'] = strip_tags ($_GET['outputformat']);;
  if (isset($_GET['asfile'])) $options['asFile'] = ($_GET['asfile'] == "1");
  if (isset($_GET['palette'])) $options['palette'] = strip_tags($_GET['palette']);
  if (isset($_GET['shape'])) $options['shape'] = strip_tags($_GET['shape']);
  if (isset($_GET['stage'])) $options['stage'] = strip_tags($_GET['stage']);
  if (isset($_GET['raw'])) $options['raw'] = true;
  //  if (isset($_GET['printable'])) $options['printable'] = ($_GET['printable'] == "1");
  if (isset($_GET['effect'])) $options['effect'] = strip_tags($_GET['effect']);
  if (isset($_GET['size'])) $options['size'] = strip_tags ($_GET['size']);
  if (isset($_GET['ar'])) $ar = strip_tags ($_GET['ar']);
}


 register_shutdown_function(function()
    {
        global $options, $spareRoom;
        $spareRoom = null;
        if ((!is_null($err = error_get_last()))  
              && (!in_array($err['type'], array (E_NOTICE, E_WARNING))) // comment this line to get all
               )
        {
           error_log($err['message'] . " (" . $err['type'] . ") at " . $err['file'] . ":" . $err['line'] . ' - ' . $options['blazon']);
        }
    });

// Quick response for empty blazon
if ( $options['blazon'] == '' ) {
  $dom = new DOMDocument('1.0');
  $dom->loadXML('<blazon ID="N1-0" blazonText="argent" blazonTokens="argent" creatorName="drawshield.net" ><shield ID="N1-6"><simple ID="N1-4"><field ID="N1-3"><tincture ID="N1-1" index="1" origin="given"><colour ID="N1-2" keyterm="argent" tokens="argent" linenumber="1" link="https://drawshield.net/reference/parker/a/argent.html"></colour></tincture></field></simple></shield><messages></messages></blazon>');
} else {
  // log the blazon for research... (unless told not too)
  if ( $options['logBlazon']) error_log($options['blazon']);
  include "parser/parser.inc";
  $p = new parser('english');
  $dom = $p->parse($options['blazon'],'dom');
  $p = null; // destroy parser
  if ( $options['stage'] == 'parser') { 
      $note = $dom->createComment("Debug information - parser stage.\n(Did you do SHIFT + 'Save as File' by accident?)");
      $dom->insertBefore($note,$dom->firstChild);
      header('Content-Type: text/xml; charset=utf-8');
      $dom->outputFormat = true;
      echo $dom->saveXML(); 
      exit; 
  }

  // Resolve references
  include "analyser/references.inc";
  $references = new references($dom);
  $dom = $references->setReferences();
  $references = null; // destroy references
  if ( $options['stage'] == 'references') { 
      $note = $dom->createComment("Debug information - references stage.\n(Did you do SHIFT + 'Save as File' by accident?)");
      $dom->insertBefore($note,$dom->firstChild);
      header('Content-Type: text/xml; charset=utf-8');
      $dom->outputFormat = true;
      echo $dom->saveXML(); 
      exit; 
  }
  // Add dictionary references
  include "analyser/addlinks.inc";
  $adder = new linkAdder($dom);
  $dom = $adder->addLinks();
  $adder = null; // destroy adder
  if ( $options['stage'] == 'links') { 
      $note = $dom->createComment("Debug information - links stage.\n(Did you do SHIFT + 'Save as File' by accident?)");
      $dom->insertBefore($note,$dom->firstChild);
      header('Content-Type: text/xml; charset=utf-8');
      $dom->outputFormat = true;
      echo $dom->saveXML(); 
      exit; 
  }
}
// Make the blazonML searchable
$xpath = new DOMXPath($dom);

/*
 * Update any optiona that were set in the blazon itself
 */
$blazonOptions = $xpath->query('//instructions/child::*');
if (!is_null($blazonOptions)) {
  for ($i = 0; $i < $blazonOptions->length; $i++) {
    $blazonOption = $blazonOptions->item($i);
    switch ($blazonOption->nodeName) {
      case blazonML::E_SHAPE:
        $options['shape'] = $blazonOption->getAttribute('keyterm');
        break;
      case blazonML::E_PALETTE:
        $options['palette'] = $blazonOption->getAttribute('keyterm');
        break;
      case blazonML::E_EFFECT:
        $options['effect'] = $blazonOption->getAttribute('keyterm');
        break;
      case blazonML::E_ASPECT:
        $ar = $blazonOption->getAttribute('keyterm');
        break;
    }
  }
}

/*
 * General options tidy-up
 */
// Minimum sensible size
if ( $options['size'] < 100 ) $options['size'] = 100;
// Synonyms for circle shape
if (in_array($options['shape'],array('circular','round'))) $options['shape'] = 'circle';
// Calculate actual flagHeight
if ($options['shape'] == 'flag') {
  if ($ar != null) $options['aspectRatio'] = calculateAR($ar);
  $options['flagHeight'] = (int)(round($options['aspectRatio'] * 1000));
}

  // Read in the drawing code  ( All formats start out as SVG )

  //include "analyser/rewriter.inc";
  // some fudges / heraldic knowledge
  // rewrite();
  // if ( $options['stage'] == 'rewrite') { 
  //     $note = $dom->createComment("Debug information - rewrite stage.\n(Did you do SHIFT + 'Save as File' by accident?)");
  //     $dom->insertBefore($note,$dom->firstChild);
  //     header('Content-Type: text/xml; charset=utf-8');
  //     echo $dom->saveXML(); 
  //     exit; 
  // }
  include "svg/draw.inc";
$output = draw();


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
    case 'pdfA4':
    $im = new Imagick();
    $im->readimageblob($output);
    $im->setimageformat('pdf');
    header("Content-type: application/force-download");
    header('Content-Disposition: inline; filename="shield.pdf"');
    header("Content-Transfer-Encoding: 8bit");
    header('Content-Disposition: attachment; filename="shield.pdf"');
    header('Content-Type: application/pdf');
    echo $im->getimagesblob();
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
     $im->setBackgroundColor(new ImagickPixel('transparent'));
     $im->readimageblob($output);
     $im->setimageformat('png32');
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
      $im->setBackgroundColor(new ImagickPixel('transparent'));
      $im->readimageblob($output);
      $im->setimageformat('png32');
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

