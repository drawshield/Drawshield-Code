<?php  /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
 */

//
// Global Variables
//
$startTime = microtime(true);
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


//
// Argument processing
//

function arguments($args) {
    array_shift($args);
    $endofoptions = false;
    $ret = array(
        // 'commands' => array(),
        'options' => array(),
        'flags' => array(),
        'arguments' => array(),
    );
    while ($arg = array_shift($args)) {
        // if we have reached end of options,
        //we cast all remaining argvs as arguments
        if ($endofoptions) {
            $ret['arguments'][] = $arg;
            continue;
        }
        // Is it a command? (prefixed with --)
        if (substr($arg, 0, 2) === '--') {
            // is it the end of options flag?
            if (!isset($arg[3])) {
                $endofoptions = true;; // end of options;
                continue;
            }
            $value = "";
            $com = substr($arg, 2);
            // is it the syntax '--option=argument'?
            if (strpos($com, '='))
                list($com, $value) = explode("=", $com, 2);
            // is the option not followed by another option but by arguments
            elseif(strpos($args[0], '-') !== 0) {
                while (strpos($args[0], '-') !== 0)
                    $value .= array_shift($args) . ' ';
                $value = rtrim($value, ' ');
            }
            $ret['options'][$com] = !empty($value) ? $value : true;
            continue;
        }
        // Is it a flag or a serial of flags? (prefixed with -)
        if (substr($arg, 0, 1) === '-') {
            for ($i = 1; isset($arg[$i]); $i++)
                $ret['flags'][] = $arg[$i];
            continue;
        }
        // finally, it is not option, nor flag, nor argument
        //$ret['commands'][] = $arg;
        $ret['arguments'][] = $arg;
        continue;
    }
    //if (!count($ret['options']) && !count($ret['flags'])) {
      //  $ret['arguments'] = array_merge($ret['commands'], $ret['arguments']);
      //  $ret['commands'] = array();
    //}
    return $ret;
}

if (isset($argc)) {
  if ( $argc > 1 ) { // run in debug mode, probably
      $myArgs = arguments($argv);
      foreach($myArgs['options'] as $option => $value) {
        $options[$option] = $value;
      }
    $options['blazon'] = implode(' ', $myArgs['arguments']);
  } else {
  if (file_exists('debug.inc')) include 'debug.inc';
  }
}

// Process arguments
$ar = null;
$request = array_merge($_GET, $_POST);

// For backwards compatibility we support argument in GET, but prefer POST
if (isset($_FILES['blazonfile']) && ($_FILES['blazonfile']['name'] != "")) {
    $fileName = $_FILES['blazonfile']['name'];
    $fileSize = $_FILES['blazonfile']['size'];
    $fileTmpName  = $_FILES['blazonfile']['tmp_name'];
    // $fileType = $_FILES['blazonfile']['type']; // not currently used
    if (preg_match('/.txt$/i', $fileName) && $fileSize < 1000000) {
        $options['blazon'] = file_get_contents($fileTmpName);
    }
} else {
    if (isset($request['blazon'])) $options['blazon'] = strip_tags($request['blazon']);
}

if (isset($request['outputformat'])) $options['outputFormat'] = strip_tags ($request['outputformat']);
if (isset($request['saveformat'])) $options['saveFormat'] = strip_tags ($request['saveformat']);
if (isset($request['asfile']) && $request['asfile'] != '0') $options['asFile'] = strip_tags($request['asfile']);
if (isset($request['palette'])) $options['palette'] = strip_tags($request['palette']);
if (isset($request['shape'])) {
    $options['shape'] = strip_tags($request['shape']);
    $options['shapeSet'] = true;
}
if (isset($request['stage'])) $options['stage'] = strip_tags($request['stage']);
if (isset($request['filename'])) $options['filename'] = strip_tags($request['filename']);
//  if (isset($request['printable'])) $options['printable'] = ($request['printable'] == "1");
if (isset($request['effect'])) $options['effect'] = strip_tags($request['effect']);
if (isset($request['size'])) $options['size']= strip_tags ($request['size']);
if (isset($request['units'])) $options['units']= strip_tags ($request['units']);
if (isset($request['ar'])) $ar = strip_tags ($request['ar']);
if (isset($request['webcols'])) $options['useWebColours'] = $request['webcols'] == 'yes';
if (isset($request['tartancols'])) $options['useTartanColours'] = $request['tartancols'] == 'yes';
if (isset($request['whcols'])) $options['useWarhammerColours'] = $request['whcols'] == 'yes';
if (isset($request['customPalette']) && is_array($request['customPalette'])) $options['customPalette'] = $request['customPalette'];

$options['blazon'] = preg_replace("/&#?[a-z0-9]{2,8};/i","",$options['blazon']); // strip all entities.
$options['blazon'] = preg_replace("/\\x[0-9-a-f]{2}/i","",$options['blazon']); // strip all entities.

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
      $dom->formatOutput = true;
      echo $dom->saveXML();
      echo  "Execution time: " . microtime(true) - $startTime;
      exit;
  }
  // filter blazon (if present)
  if (file_exists("/var/www/etc/filter.inc")) {
      include "/var/www/etc/filter.inc";
      $filter = new filter($dom);
      $dom = $filter->runFilter();
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
      $dom->formatOutput = true;
      echo $dom->saveXML(); 
      exit; 
  }
}
// Make the blazonML searchable
$xpath = new DOMXPath($dom);

/*
 * General options tidy-up
 */
if ($options['asFile']) {
    $options['printSize'] = $options['size'];
    $options['size'] = 1000;
}
// Minimum sensible size
if ( $options['size'] < 100 ) $options['size'] = 100;
if ($ar != null) {
    $options['aspectRatio'] = calculateAR($ar);
}
tidyOptions();;
  // Read in the drawing code  ( All formats start out as SVG )


include "svg/draw.inc";

function report_errors_svg($errno, $errstr, $errfile, $errline)
{
    global $messages;
    if ( $messages )
    {
        $dirname = __dir__;
        if ( substr($errfile, 0, strlen($dirname)) == $dirname )
            $errfile = substr($errfile, strlen($dirname));


        switch ($errno)
        {
            case E_ERROR:
            case E_PARSE:
            case E_CORE_ERROR:
            case E_COMPILE_ERROR:
            case E_USER_ERROR:
            case E_RECOVERABLE_ERROR:
                $err_type = "error";
                break;
            case E_COMPILE_WARNING:
            case E_WARNING:
            case E_USER_WARNING:
                $err_type = "warning";
                break;
            default:
                $err_type = "notice";
                break;
        }

        $messages->addMessage("$err_type", "$errfile:$errline : $errstr\n");
    }
    return false;
}

// if (array_key_exists('HTTP_REFERER',$_SERVER) && strpos($_SERVER["HTTP_REFERER"], "demopage.php") !== false )
    set_error_handler("report_errors_svg");

$output = draw();


// Output content header
if ($options['asFile'] == '1') {
    $name = $options['filename'];
    if ($name == '') $name = 'shield';
    $pageWidth = $pageHeight = false;
    if ($options['units'] == 'in') {
        $options['printSize'] *= 90;
    } elseif ($options['units'] == 'cm') {
        $options['printSize'] *= 35;
    }
    // $proportion = ($options['shape'] == 'flag') ? $options['aspectRatio'] : 1.2;
    $proportion = 1.2;
    switch ($options['saveFormat']) {
        case 'svg':
            if (substr($name, -4) != '.svg') $name .= '.svg';
            header("Content-type: application/force-download");
            header('Content-Disposition: inline; filename="' . $name);
            header("Content-Transfer-Encoding: text");
            header('Content-Disposition: attachment; filename="' . $name);
            header('Content-Type: image/svg+xml');
            echo $output;
            break;
        case 'pdfLtr':
            $pageWidth = 765;
            $pageHeight = 990;
        // flowthrough
        case 'pdfA4':
            if (!$pageWidth) $pageWidth = 744;
            if (!$pageHeight) $pageHeight = 1051;
            $im = new Imagick();
            $im->readimageblob($output);
            $im->setimageformat('pdf');
            $margin = 40; // bit less than 1/2"
            $maxWidth = $pageWidth - $margin - $margin;
            $imageWidth = $options['printSize'];
            if ($imageWidth > $maxWidth) $imageWidth = $maxWidth;
            $imageHeight = $imageWidth * $proportion;
            $im->scaleImage($imageWidth, $imageHeight);
            $fromBottom = $pageHeight - $margin - $margin - $imageHeight;
            $fromSide = $margin + (($pageWidth - $margin - $margin - $imageWidth) / 2);
            $im->setImagePage($pageWidth, $pageHeight, $fromSide * 0.9, $fromBottom * 0.9);
            if (substr($name, -4) != '.pdf') $name .= '.pdf';
            header("Content-type: application/force-download");
            header('Content-Disposition: inline; filename="' . $name);
            header("Content-Transfer-Encoding: 8bit");
            header('Content-Disposition: attachment; filename="' . $name);
            header('Content-Type: application/pdf');
            echo $im->getimageblob();
            break;
        case 'jpg':
            $dir = sys_get_temp_dir();
            $base = tempnam($dir, 'shield');
            rename($base, $base . '.svg');
            file_put_contents($base . '.svg', $output);
            $width = $options['printSize'];
            $height = $width * $proportion;
            $result = shell_exec("java -jar /var/www/etc/batik/batik-rasterizer-1.14.jar $base.svg -d $base.jpg -q 0.9 -m image/jpeg -w $width -h $height");
            unlink($base . '.svg');
            if (substr($name, -4) != '.jpg') $name .= '.jpg';
            header("Content-type: application/force-download");
            header('Content-Disposition: inline; filename="' . $name);
            header("Content-Transfer-Encoding: binary");
            header('Content-Disposition: attachment; filename="' . $name);
            header('Content-Type: image/jpg');
            echo file_get_contents($base . '.jpg');
            unlink($base . '.jpg');
            break;
/*            $im = new Imagick();
            $im->readimageblob($output);
            $im->setimageformat('jpeg');
            $im->setimagecompressionquality(90);
            $imageWidth = $options['printSize'];
            $imageHeight = $imageWidth * $proportion;
            $im->scaleimage($imageWidth, $imageHeight);
            if (substr($name, -4) != '.jpg') $name .= '.jpg';
            header("Content-type: application/force-download");
            header('Content-Disposition: inline; filename="' . $name);
            header("Content-Transfer-Encoding: binary");
            header('Content-Disposition: attachment; filename="' . $name);
            header('Content-Type: image/jpg');
            echo $im->getimageblob();
            break; */
        case 'png-old':
            $im = new Imagick();
            $im->setBackgroundColor(new ImagickPixel('transparent'));
            $im->readimageblob($output);
            $im->setimageformat('png32');
            $imageWidth = $options['printSize'];
            $imageHeight = $imageWidth * $proportion;
            $im->scaleimage($imageWidth, $imageHeight);
            if (substr($name, -4) != '.png') $name .= '.png';
            header("Content-type: application/force-download");
            header('Content-Disposition: inline; filename="' . $name);
            header("Content-Transfer-Encoding: binary");
            header('Content-Disposition: attachment; filename="' . $name);
            header('Content-Type: image/png');
            echo $im->getimageblob();
            break;
        case 'png':
        default:
            $dir = sys_get_temp_dir();
            $base = tempnam($dir, 'shield');
            rename($base, $base . '.svg');
            file_put_contents($base . '.svg', $output);
            $width = $options['printSize'];
            $height = $width * $proportion;
            $result = shell_exec("java -jar /var/www/etc/batik/batik-rasterizer-1.14.jar $base.svg -d $base.png -w $width -h $height");
            unlink($base . '.svg');
        if (substr($name, -4) != '.png') $name .= '.png';
        header("Content-type: application/force-download");
            header('Content-Disposition: inline; filename="' . $name);
            header("Content-Transfer-Encoding: binary");
            header('Content-Disposition: attachment; filename="' . $name);
            header('Content-Type: image/png');
            echo file_get_contents($base . '.png');
            unlink($base . '.png');
            break;

    }
} else {
    switch ($options['outputFormat']) {
        case 'jpg':
            $dir = sys_get_temp_dir();
            $base = tempnam($dir, 'shield');
            rename($base, $base . '.svg');
            file_put_contents($base . '.svg', $output);
            $result = shell_exec("java -jar /var/www/etc/batik/batik-rasterizer-1.14.jar $base.svg -d $base.jpg -q 0.9 -m image/jpeg");
            unlink($base . '.svg');
            header('Content-Type: image/jpg');
            echo file_get_contents($base . '.jpg');
            unlink($base . '.jpg');
/*            $im = new Imagick();
            $im->readimageblob($output);
            $im->setimageformat('jpeg');
            $im->setimagecompressionquality(90);
            // $im->scaleimage(1000,1200);
            header('Content-Type: image/jpg');
            echo $im->getimageblob(); */
            break;
        case 'json':
            $newDom = new DOMDocument();
            $newDom->loadXML($output);
            $dir = sys_get_temp_dir();
            $base = tempnam($dir, 'shield');
            rename($base, $base . '.svg');
            file_put_contents($base . '.svg', $output);
            $result = shell_exec("java -jar /var/www/etc/batik/batik-rasterizer-1.14.jar $base.svg -d $base.png");
            unlink($base . '.svg');
            header('Content-Type: image/png');
            $json = [];
            $json['image'] = base64_encode(file_get_contents($base . '.png'));
            unlink($base . '.png');
            $json['options'] = $options;
            $allMessages = $newDom->getElementsByTagNameNS('http://drawshield.net/blazonML', 'message');
            $messageArray = [];
            foreach ($allMessages as $node) {
                $thisMessage = [];
                for ($i = 0; $i < $node->attributes->length; $i++) {
                    $thisMessage[$node->attributes->item($i)->nodeName] = $node->attributes->item($i)->nodeValue;
                }
                $thisMessage['content'] = $node->nodeValue;
                $messageArray[] = $thisMessage;
            }
            $json['messages'] = $messageArray;
            $json['tree'] = $dom->saveXML();
            $baggage = $dom->getElementsByTagNameNS('http://drawshield.net/blazonML', 'input')->item(0);
            $baggage->parentNode->removeChild($baggage);
            $baggage = $dom->getElementsByTagNameNS('http://drawshield.net/blazonML', 'messages')->item(0);
            $baggage->parentNode->removeChild($baggage);
            $minTree = $dom->saveXML();
            $minTree = preg_replace('/blazonML:/', '', $minTree);
            $minTree = preg_replace('/<\?xml.*\?>\n/', '', $minTree);
            $minTree = preg_replace('/<\/?blazon.*>\n/', '', $minTree);
            $minTree = preg_replace('/[<>"]/', '', $minTree);
            $json['mintree'] = $minTree;
            header('Content-Type: application/json');
            echo json_encode($json);
            break;
        case 'png-old':
            $im = new Imagick();
            $im->setBackgroundColor(new ImagickPixel('transparent'));
            $im->readimageblob($output);
            $im->setimageformat('png32');
            // $im->scaleimage(1000,1200);
            header('Content-Type: image/png');
            echo $im->getimageblob();
            break;
        case 'png':
            $dir = sys_get_temp_dir();
            $base = tempnam($dir, 'shield');
            rename($base, $base . '.svg');
            file_put_contents($base . '.svg', $output);
            $result = shell_exec("java -jar /var/www/etc/batik/batik-rasterizer-1.14.jar $base.svg -d $base.png");
            unlink($base . '.svg');
            header('Content-Type: image/png');
            echo file_get_contents($base . '.png');
            unlink($base . '.png');
            break;
        default:
        case 'svg':
            if ($options['asFile'] == 'printable') {
                $xpath = new DOMXPath($dom); // re-build xpath with new messages
                header('Content-Type: text/html; charset=utf-8');
                echo "<!doctype html>\n\n<html lang=\"en\">\n<head>\n<title>Shield</title>\n";
                echo "<style>\nsvg { margin-left:auto; margin-right:auto; display:block;}</style>\n</head>\n<body>\n";
                echo "<div>\n$output</div>\n";
                echo "<h2>Blazon</h2>\n";
                echo "<p class=\"blazon\">${options['blazon']}</p>\n";
                echo "<h2>Image Credits</h2>\n";
                echo "<p>This work is licensed under a <em>Creative Commons Attribution-ShareAlike 4.0 International License</em>.";
                echo " It is a derivative work based on the following source images:</p>";
                echo $messages->getCredits();
                echo "</body>\n</html>\n";
            } else {
                header('Content-Type: text/xml; charset=utf-8');
                echo $output;
            }
            break;
    }
}


