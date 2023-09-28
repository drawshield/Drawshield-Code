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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
 */

include 'utils/general.inc';  // general utilities, duh.
$timings = array('start' => microtime(true));

//////////////////////////////////////////////
// Stage 1a - Set default and fallback options
/////////////////////////////////////////////
$options = array();
$version = array();
include 'version.inc';  // sets default and fallback options

//////////////////////////////////////////////
// Stage 1b - command line arguments (if any)
/////////////////////////////////////////////
if (isset($argc)) {
    if ($argc > 1) { // run in debug mode, probably
        // Do we have any command line arguments? If so, incorporate into options array
        $myArgs = arguments($argv);
        foreach ($myArgs['options'] as $option => $value) {
            $value = urldecode($value);
            if ($option == 'webcols') $options['useWebColours'] = $value == 'yes';
            elseif ($option == 'tartancols') $options['useTartanColours'] = $value == 'yes';
            elseif ($option == 'whcols') $options['useWarhammerColours'] = $value == 'yes';
            else $options[$option] = $value;
        }
        $options['commandLine'] = true;
        if (!array_key_exists('blazon', $options))
            $options['blazon'] = implode(' ', $myArgs['arguments']);
    } else { // otherwise, run anything in the debug script
        // This NEVER HAPPENS ON THE SERVER (as $argc is never set)
        // so it is safe (but useless) to copy debug.inc to the server
        if (file_exists('debug.inc')) include 'debug.inc';
    }
}

//////////////////////////////////////////////
// Stage 1c -process any arguments from the superglobals? SERVER ONLY (probably)
/////////////////////////////////////////////
$ar = null;
$request = array_merge($_GET, $_POST);
$svgOutput = null;

// We do support sending the blazon in an uploaded file but this is not currently implemented
// on the site anywhere, we always use POST fields
if (isset($_FILES['blazonfile']) && ($_FILES['blazonfile']['name'] != "")) {
    $fileName = $_FILES['blazonfile']['name'];
    $fileSize = $_FILES['blazonfile']['size'];
    $fileTmpName = $_FILES['blazonfile']['tmp_name'];
    // $fileType = $_FILES['blazonfile']['type']; // not currently used
    if (preg_match('/.txt$/i', $fileName) && $fileSize < 1000000) {
        $options['blazon'] = file_get_contents($fileTmpName);
    }
} else {
    if (isset($request['blazon'])) $options['blazon'] = strip_tags($request['blazon']);
}

// Process superglobal settings into the options array
if (isset($request['outputformat'])) $options['outputFormat'] = strip_tags($request['outputformat']);
if (isset($request['saveformat'])) $options['saveFormat'] = strip_tags($request['saveformat']);
if (isset($request['asfile']) && $request['asfile'] != '0') $options['asFile'] = strip_tags($request['asfile']);
if (isset($request['palette'])) $options['palette'] = strip_tags($request['palette']);
if (isset($request['shape'])) {
    $options['shape'] = strip_tags($request['shape']);
    $options['shapeSet'] = true;
}
if (isset($request['stage'])) $options['stage'] = strip_tags($request['stage']);
if (isset($request['filename'])) $options['filename'] = strip_tags($request['filename']);
if (isset($request['effect'])) $options['effect'] = strip_tags($request['effect']);
if (isset($request['size'])) $options['size'] = strip_tags($request['size']);
if (isset($request['units'])) $options['units'] = strip_tags($request['units']);
if (isset($request['ar'])) $ar = strip_tags($request['ar']);
if (isset($request['webcols'])) $options['useWebColours'] = $request['webcols'] == 'yes';
if (isset($request['tartancols'])) $options['useTartanColours'] = $request['tartancols'] == 'yes';
if (isset($request['whcols'])) $options['useWarhammerColours'] = $request['whcols'] == 'yes';
if (isset($request['customPalette']) && is_array($request['customPalette'])) $options['customPalette'] = $request['customPalette'];
// We might be given the SVG to convert to another format, so do a basic sense check
if (isset($request['svg']) && strlen($request['svg']) > 20) $svgOutput = $request['svg'];


if (is_null($svgOutput)) { // not given SVG, so generate it from the blazon

    //////////////////////////////////////////////
    // Stage 2a - blazon preparation
    /////////////////////////////////////////////

    $dom = null;
    // If blazon is null assume 'argent', but skip parsing as we already know the outcome
    if ($options['blazon'] == '') {
        $dom = new DOMDocument('1.0');
        $dom->loadXML($version['dummyAST']);
    } else {
        $options['blazon'] = preg_replace("/&#?[a-z0-9]{2,8};/i", "", $options['blazon']); // strip all entities.
        $options['blazon'] = preg_replace("/\\x[0-9-a-f]{2}/i", "", $options['blazon']); // strip all entities.
        // log the blazon for research... (unless told not too)
        if ($options['logBlazon']) error_log($options['blazon']);
    }


    include 'parser/utilities.inc';
    include "analyser/utilities.inc";
    if (is_null($dom)) {
        //////////////////////////////////////////////
        // Stage 2b - Parsing into XML AST
        /////////////////////////////////////////////
        include "parser/parser.inc";

        $p = new parser('english');
        $dom = $p->parse($options['blazon'], 'dom');
        $p = null; // destroy parser to save memory
        $timings['parser'] = microtime(true);

        //////////////////////////////////////////////
        // Stage 2c - Filtering out stuff we don't like (code not in repo)
        /////////////////////////////////////////////
        if (file_exists("/opt/bitnami/apache/etc/filter.inc")) {
            include "/opt/bitnami/apache/etc/filter.inc";
            $filter = new filter($dom);
            $dom = $filter->runFilter();
        }

        //////////////////////////////////////////////
        // Stage 2d - Additional Options may have been in blazon, tidy them up
        /////////////////////////////////////////////
        if ($options['asFile']) {
            $options['printSize'] = $options['size'];
            $options['size'] = 1000;
        }
        // Minimum sensible size
        if ($options['size'] < 100) $options['size'] = 100;
        if ($ar != null) {
            $options['aspectRatio'] = calculateAR($ar);
        }
        tidyOptions();;


        //////////////////////////////////////////////
        // Stage 2e - resolve cross references & other fixups
        /////////////////////////////////////////////
        include "analyser/references.inc";
        $references = new references($dom);
        $dom = $references->setReferences();
        $references = null; // destroy references to save memory
        $timings['fixups'] = microtime(true);
    }


    //////////////////////////////////////////////
    // Stage 3a - preparation for drawing
    /////////////////////////////////////////////
    $xpath = new DOMXPath($dom);
    include 'utils/messages.inc';
    $messages = new messageStore($dom);
    include "svg/draw.inc";
    set_error_handler("report_errors_svg");


    //////////////////////////////////////////////
    // Stage 3b - create SVG version (all formats start as SVG)
    /////////////////////////////////////////////
    $svgOutput = draw();
}

// At this point we should have some valid SVG in $svgOutput, now what?
$target = $options['outputFormat'];
if ($target == 'json') {
    $target = 'png';
} elseif ($options['asFile'] == 'printable') {
    $target = 'svg';
} elseif ($options['asFile'] || $options['commandLine']) {
    $target = $options['saveFormat'];
}
// (Sorry about the above mess, needed for historic reasons and to preserve API arguments)


//////////////////////////////////////////////
// Stage 3c - Change image format if required
/////////////////////////////////////////////

if ($target == 'svg') {
    $targetImage = $svgOutput;
} else {
    include 'utils/render.inc';
    $targetImage = convertImageFormat($svgOutput, $target);
    $timings['conversion'] = microtime(true);
}

//////////////////////////////////////////////
// Stage 4 - output image as per option request
/////////////////////////////////////////////
if ($options['asFile'] == 'printable') {
    //////////////////////////////////////////////
    // Stage 4 option 1 - a printable HTML page
    /////////////////////////////////////////////
    $xpath = new DOMXPath($dom); // re-build xpath with new messages
    header('Content-Type: text/html; charset=utf-8');
    echo "<!doctype html>\n\n<html lang=\"en\">\n<head>\n<title>Shield</title>\n";
    echo "<style>\nsvg { margin-left:auto; margin-right:auto; display:block;}</style>\n</head>\n<body>\n";
    echo "<div>\n$targetImage</div>\n";
    echo "<h2>Blazon</h2>\n";
    echo "<p class=\"blazon\">${options['blazon']}</p>\n";
    echo "<h2>Image Credits</h2>\n";
    echo "<p>This work is licensed under a <em>Creative Commons Attribution-ShareAlike 4.0 International License</em>.";
    echo " It is a derivative work based on the following source images:</p>";
    echo $messages->getCredits();
    echo "</body>\n</html>\n";
    //////////////////////////////////////////////
    // Stage 4 option 2 - An image file downloaded over HTTP
    /////////////////////////////////////////////
} elseif ($options['asFile']) {
    $name = $options['filename'];
    if ($name == '') $name = 'shield';
    switch ($options['saveFormat']) {
        case 'svg':
            if (substr($name, -4) != '.svg') $name .= '.svg';
            header("Content-type: application/force-download");
            header('Content-Disposition: inline; filename="' . $name);
            header("Content-Transfer-Encoding: text");
            header('Content-Disposition: attachment; filename="' . $name);
            header('Content-Type: image/svg+xml');
            break;
        case 'pdfLtr':
        case 'pdfA4':
            if (substr($name, -4) != '.pdf') $name .= '.pdf';
            header("Content-type: application/force-download");
            header('Content-Disposition: inline; filename="' . $name);
            header("Content-Transfer-Encoding: 8bit");
            header('Content-Disposition: attachment; filename="' . $name);
            header('Content-Type: application/pdf');
            break;
        case 'jpg':
        case 'jpeg':
            if (substr($name, -4) != '.jpg') $name .= '.jpg';
            header("Content-type: application/force-download");
            header('Content-Disposition: inline; filename="' . $name);
            header("Content-Transfer-Encoding: binary");
            header('Content-Disposition: attachment; filename="' . $name);
            header('Content-Type: image/jpg');
            break;
        case 'png':
            if (substr($name, -4) != '.png') $name .= '.png';
            header("Content-type: application/force-download");
            header('Content-Disposition: inline; filename="' . $name);
            header("Content-Transfer-Encoding: binary");
            header('Content-Disposition: attachment; filename="' . $name);
            header('Content-Type: image/png');
            break;
        case 'webp':
            if (substr($name, -4) != '.webp') $name .= '.webp';
            header("Content-type: application/force-download");
            header('Content-Disposition: inline; filename="' . $name);
            header("Content-Transfer-Encoding: binary");
            header('Content-Disposition: attachment; filename="' . $name);
            header('Content-Type: image/webp');
            break;
    }
    echo $targetImage;
    //////////////////////////////////////////////
    // Stage 4 option 3 - file saved locally in file system
    /////////////////////////////////////////////
} elseif ($options['commandLine']) {
    // This only occurs if the file is run locally, and we don't specify "asFile" (i.e. HTTP download). We write the file locally.
    $name = $options['filename'];
    if ($name == '') $name = 'shield';
    $fileContent = '';
    switch ($options['saveFormat']) {
        case 'svg':
            if (substr($name, -4) != '.svg') $name .= '.svg';
            break;
        case 'pdfLtr':
        case 'pdfA4':
            if (substr($name, -4) != '.pdf') $name .= '.pdf';
            break;
        case 'jpg':
            if (substr($name, -4) != '.jpg') $name .= '.jpg';
            break;
        case 'png':
            if (substr($name, -4) != '.png') $name .= '.png';
        default:
            break;
    }
    file_put_contents($name, $targetImage);
    //////////////////////////////////////////////
    // Stage 4 option 4 - image returned over HTTP
    /////////////////////////////////////////////
} else {
    switch ($options['outputFormat']) {
        case 'jpg':
            header('Content-Type: image/jpg');
            break;
        case 'json': // A PNG image + other data all wrapped up in JSON
            $newDom = new DOMDocument();
            $newDom->loadXML($svgOutput);
            $json = [];
            $json['image'] = base64_encode($targetImage);
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
            $json['timings'] = showTimings();
            header('Content-Type: application/json');
            $targetImage = json_encode($json);
            break;
        case 'webp':
            header('Content-Type: image/webp');
            break;
        case 'png':
            header('Content-Type: image/png');
            break;
        default:
        case 'svg':
            header('Content-Type: text/xml; charset=utf-8');
            break;
    }
    echo $targetImage;
}


