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

include 'charge.inc';
include 'division.inc';
include 'effects.inc';
include "messages.inc";
include "furs.inc";
include "infobox.inc";
include "inkscape.inc";
include "layout.inc";
include "linetypes.inc";
include "ordinary.inc";
include "placements.inc";
include "rgb.inc";
include "shield.inc";
include "svgutils.inc";
include "treatment.inc";
include "utilities.inc";


function region ( $action, $region = null ) {
  global $svg_region;
  static $stack = array();

  switch ( $action ) {
    case 'push':
      array_push( $stack, $svg_region );
      $svg_region = $region;
      break;
    case 'swap':
      $svg_region = $region;
      break;
    case 'pop':
      $svg_region = array_pop( $stack );
      break;
  }
}

function chief ( $action, $chief = null ) {
  global $svg_chief;
  static $stack = array();

  switch ( $action ) {
    case 'push':
      array_push( $stack, $svg_chief );
      $svg_chief = $chief;
      break;
    case 'swap':
      $svg_chief = $chief;
      break;
    case 'pop':
      $svg_chief = array_pop( $stack );
      break;
  }
}

function chiefPresent() {
  global $svg_chief;

  return ( $svg_chief == 'CH' );
}

function choose_fit( $choices ) {
  global $svg_region, $svg_chief, $messages;

  if ( !is_array($choices) ) return $choices;
  // Look for chief/no chief first
  if ( array_key_exists($svg_chief, $choices) ) return $choices[$svg_chief];
  // Look for region alone next
  if ( array_key_exists($svg_region, $choices) ) return $choices[$svg_region];
  // Last resort, look for the combination
  $combo = $svg_region . $svg_chief;
  if ( array_key_exists($combo, $choices) ) return $choices[$combo];
  // raise error
  $messages->addMessage('internal', 'Cannot find placement for region');
  return null;
}


function find_chief ( $ord_chgs ) {
  /**
   * @var DOMElement $child
   */
  foreach ( $ord_chgs->childNodes as $child ) {
    if ( $child->nodeName == 'ordinary' and $child->getAttribute('keyterm') == 'chief' ) return true;
  }
  return false;
}

function makeOrdChgs( $node ) {
  $retval = '';
  foreach ( $node->childNodes as $child ) {
    switch ( $child->nodeName ) {
      case 'ordinary':
        $retval .= makeOrdinary( $child );
        break;
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


  $infobox = new infobox();
  $messages = new messageStore($dom);

  // Apply heraldic knowledge!
  // rewrite();

  // Get all the charge data (once for each charge type)
  get_chg_data();

  $header = '<?xml version="1.0" encoding="utf-8" ?>
  <svg version="1.1"
       baseProfile="full"
       xmlns="http://www.w3.org/2000/svg"
       xmlns:xlink="http://www.w3.org/1999/xlink" ' .
       'preserveAspectRatio="xMidYMid meet" height="' . (int)($options['size'] * 1.2) . '" width="' . $options['size'];
  $header .= '" viewBox="'. getEffect('viewbox') . ' ">';
  $header .= '<defs>' . getEffect('defs') . '</defs>';
  $trailer =  "</svg>\n";

  $blazonNode = $dom->documentElement;
  $body = '';
  foreach ( $blazonNode->childNodes as $node ) {
    switch ( $node->nodeName ) {
    case 'shield':
      $body .= makeshield($node,'10:12');
      break;
      // other elements for future expansion
    }
  }
  $clipShield = add_def('mask', '<path fill="#FFFFFF" d="m 0,0 0,800 c -6.0479349,41.93063 1.2341419,84.33503 21.489586,125.13639 20.255443,40.80137 53.477613,79.98631 98.039434,115.63581 44.56181,35.6494 100.44867,67.7517 164.92346,94.7345 64.47479,26.9828 137.51639,48.8374 215.54752,64.4933 78.03113,-15.6559 151.07273,-37.5105 215.54752,-64.4933 64.47479,-26.9828 120.36165,-59.0851 164.92346,-94.7345 44.56182,-35.6495 77.78399,-74.83444 98.03943,-115.63581 C 998.76586,884.33503 1006.0479,841.93063 1000,800 l 0,-800 z" />' );
  // Add effects (if any) and Clip to shield shape
  if ( ($options['asFile'] and $options['saveFormat'] != 'svg') or (!$options['asFile'] and $options['outputFormat'] != 'svg')) {
    $bodyfill = add_def('pattern width="100%" height="100%" patternUnits="objectBoundingBox"', $body . getEffect('above') );
    $body = add_def(); // Note, we reset body here
    $body .= '<path fill="url(#' . $bodyfill . ')" ' . getEffect('filter') . ' d="m 0,0 0,800 c -6.0479349,41.93063 1.2341419,84.33503 21.489586,125.13639 20.255443,40.80137 53.477613,79.98631 98.039434,115.63581 44.56181,35.6494 100.44867,67.7517 164.92346,94.7345 64.47479,26.9828 137.51639,48.8374 215.54752,64.4933 78.03113,-15.6559 151.07273,-37.5105 215.54752,-64.4933 64.47479,-26.9828 120.36165,-59.0851 164.92346,-94.7345 44.56182,-35.6495 77.78399,-74.83444 98.03943,-115.63581 C 998.76586,884.33503 1006.0479,841.93063 1000,800 l 0,-800 z" />';

    /* $body .= add_def();
    $body = getEffect('below') . '<g ' . getEffect('filter') . '><g mask="url(#' . $clipShield . ')">' . $body . getEffect('above') . '</g></g>';
    */
  } else  {
    $body .= add_def();
    $body = getEffect('below') . '<g ' . getEffect('filter') . '><g mask="url(#' . $clipShield . ')">' . $body . getEffect('above') . '</g></g>';
  }
  // Add outline of shield shape, only if required
  if ( $options['palette'] == 'nocolour')
    $body .=  '<path fill="none" stroke="' . rgb('division-stroke') . '" stroke-width="4" d="m 0,0 0,800 c -6.0479349,41.93063 1.2341419,84.33503 21.489586,125.13639 20.255443,40.80137 53.477613,79.98631 98.039434,115.63581 44.56181,35.6494 100.44867,67.7517 164.92346,94.7345 64.47479,26.9828 137.51639,48.8374 215.54752,64.4933 78.03113,-15.6559 151.07273,-37.5105 215.54752,-64.4933 64.47479,-26.9828 120.36165,-59.0851 164.92346,-94.7345 44.56182,-35.6495 77.78399,-74.83444 98.03943,-115.63581 C 998.76586,884.33503 1006.0479,841.93063 1000,800 l 0,-800 z" />';
  // Rest depends on output format
  if ( $options['asFile'] or $options['printable'] ) { // minimal decoration, no infobox
      $body .= '<text id="test1" x="10" y="1160" font-size="30" >' . $version['name'] . ' ' . $version['release'] . '</text><text x="10" y="1190" font-size="30" >' . $version['website'] . '</text>';
  } else { // in line version, all the bells and whistles
    $body .= '<text id="test1" x="10" y="1160" font-size="30" >' . $version['name'] . ' ' . $version['release'] . '</text><text x="10" y="1190" font-size="30" >' . $version['website'] . '</text>';
    if ( $options['outputFormat'] == 'svg') {
      $body .= '<g onclick="document.getElementById(\'infobox\').setAttribute(\'visibility\',\'visible\');document.getElementById(\'text1\').setAttribute(\'visibility\',\'visible\');" >';
      populateInfoBox($infobox); // Side effect, sets 'notify' option
      if ($options['notify']) $body .= '<text fill="red" text-anchor="end" x="990" y="1160" font-size="30">Error reported</text>';
      $body .= '<text fill="black" text-anchor="end" x="990" y="1190" font-size="30">Click for information</text>';
      $body .= '</g>';
      $body .= $infobox->getInfobox();
    }
  }
  return $header . $body . $trailer;
}

