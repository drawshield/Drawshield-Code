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

// Global variables for parsing
$svg_region = 'PL';
$svg_chief = 'NC';

include 'getcharge.inc';
include 'division.inc';
include 'effects.inc';
include "messages.inc";
include "furs.inc";
include "infobox.inc";
include "svgtools.inc";
include "placecharge.inc";
include "linetypes.inc";
include "tinctures.inc";
include "ordinary.inc";
include "arrangements.inc";
include "quartering.inc";
include "flag.inc";
include "pathtools.inc";
include "treatment.inc";

function getLine($node) {
  $retval = '';
  if ($node !== null) {
    $lineNo = $node->getAttribute(BlazonML::A_LINENUMBER);
    if ($lineNo != null) {
      $retval = " near line $lineNo";
    }
  }
  return $retval;
}

function chiefPresent() {
  return false;
}


function choose_fit( $choices ) {
  global $svg_region, $svg_chief, $messages;
  $retval = null;
  $found = null;
  
  //  $messages->addMessage('internal', "Searching for $svg_region + $svg_chief");


  if ( !is_array($choices) ) {
      $retval = $choices;
      $found = "(fixed)";
  }
 // Look for chief/no chief first
  if ( $found == null && array_key_exists('NC', $choices) ) {
      $retval =  $choices[$svg_chief];
      $found = $svg_chief;
  }

  if ( $retval == null ) {
      $messages->addMessage('internal', 'Cannot find placement for region');     
  }
  
  // raise error
  return $retval;
}


function makeOrdChgs( $node ) {
  $retval = '';
  foreach ( $node->childNodes as $child ) {
    switch ( $child->nodeName ) {
      case 'ordinary':
        $retval .= makeOrdinary( $child );
        break;
      case 'missing':
      case 'charge':
        $retval .= makeCharge( $child );
        break;
    }
  }
  return $retval;
}

function draw() {
    global $dom;
    global $options;
    global $version;
    global $messages;

    $outline = 'm 0,0 h 1000 v ' . $options['flagHeight'] . 'h -1000 Z';

    $messages = new messageStore($dom);
    
    // if ($options['shape'] != 'default') {
    //   include "shapes.inc";
    //   $outline = getShape($options['shape']);
    // } 

     // Get all the charge data (once for each charge type)
    get_chg_data();
    $displayScale = $options['size'] / 1000;
    $displayWidth = $options['size'];
    $displayHeight = (int)(($options['flagHeight'] + 40) * $displayScale);
    $textY = ($options['flagHeight'] + 34);

    $header = '<?xml version="1.0" encoding="utf-8" ?>
    <svg version="1.1"
         baseProfile="full"
         xmlns="http://www.w3.org/2000/svg"
         xmlns:xlink="http://www.w3.org/1999/xlink" ' .
         'preserveAspectRatio="xMidYMid meet" height="' . $displayHeight . '" width="' . $displayWidth . '" ';
    $header .= 'viewBox="'. getEffect('viewbox',1000,$options['flagHeight'] + 40) . ' ">';
    $header .= '<defs>' . getEffect('defs') . '</defs>';
    $trailer =  "</svg>\n";

    $blazonNode = $dom->documentElement;
    $body = '';
    foreach ( $blazonNode->childNodes as $node ) {
        switch ( $node->nodeName ) {
            case 'shield':
              $body .= makeflag($node);
              break;
              // other elements for future expansion
        }
    }
    if (!array_key_exists('nomask', $options)) {
        $clipShield = add_def('mask', '<path fill="#FFFFFF" d="' . $outline . '" />' );
        // Add effects (if any) and Clip to shield shape
        if ( false && ( ($options['asFile'] && $options['saveFormat'] != 'svg') || (!$options['asFile'] && $options['outputFormat'] != 'svg')) ) {
            $bodyfill = add_def('pattern width="100%" height="100%" patternUnits="objectBoundingBox"', $body . getEffect('above') );
            $body = add_def(); // Note, we reset body here
            $body .= '<path fill="url(#' . $bodyfill . ')" ' . getEffect('filter') . ' d="' . $outline . '" />';
        } else  {
            $body .= add_def();
            $body = getEffect('below') . '<g ' . getEffect('filter') . '><g mask="url(#' . $clipShield . ')">' . $body . getEffect('above') . '</g></g>';
        }
    }
    // Add outline of shield shape, only if required
    if ( $options['palette'] == 'outline') {
            $body .=  '<path fill="none" stroke="' . rgb('division-stroke') . '" stroke-width="4" d="' . $outline . '" />';
    }
    $body .= '<text x="180" y="' . $textY  . '" font-size="20" >'
     . $version['name'] . ' ' . $version['release'] . '</text><text x="10" y="' . $textY . '" font-size="20" >' . $version['flagsite'] . '</text>';
    $infobox = new infobox();
    populateInfoBox($infobox);
    return $header . $body . $infobox->getInfobox(). $trailer;
}

