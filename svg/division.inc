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
	 
function get_div_height ( DOMElement $node ) {
	// TODO rewrite this, possibly as full rewrite of divisions to match charges and ordinaries
	// it relies on exact knowledge of internal division name
  $subtype = $node->getAttribute('keyterm');
	if ( in_array($subtype,array('per-bend', 'per-bend-sinister', 'gyronny', 'quarterly', 'per-saltire', 'per-chevron')))
		return 1000;
	else
	  return 1200;
 }

function includeDivision( DOMElement &$node ) {
  global $messages, $options;

  $divisionType = $node->getAttribute('keyterm');
  $division = array ();
  $file = null;
  if ($options['shape'] == 'flag') { // look in the flag sub-directory first
    $file ="divisions/flag/$divisionType.inc";
    if ( stream_resolve_include_path($file) === false ) $file = null;
  }
  if ($file == null) {
    $file = "divisions/$divisionType.inc";
    if ( stream_resolve_include_path($file) === false ) $file = null;
  }    
  $calcHeight = 1200; // value to use for calculating drawing height
  if ($options['shape'] == 'flag') {
    $calcHeight = $options['flagHeight'];
  }
  if ($file != null) { // calcHeight is available within the included file
    include ( $file );
  } else { // can't draw this ordinary
    $messages->addMessage('warning',"Cannot draw $divisionType, substituting per bend" . getLine($node) );
    include ( 'divisions/per-bend.inc');
  }
  return $division;
}


function makeDivision( DOMElement $node, $counter = false ) {
  /*
   * @var messageStore
   */
  global $messages, $options;

  $division = includeDivision($node);

  $height = 1200;
  if ($options['shape'] == 'flag') {
    $height = $options['flagHeight'];
  }

  // Collect the required shapes
  $children = $node->childNodes;
  $secondDiv = null;
  /**
   * @var $tinctures DOMelement[]
   */
  $tinctures = array();
  /**
   * @var $child DOMElement
   */
  foreach ( $children as $child ) {
    if ( $child->nodeName == 'tincture' )
      $tinctures[$child->getAttribute('index')] = $child;
  }
  $div2 = $division['div2'];
  $div3 = '';
//  if ( array_key_exists('div3', $division)) {
//    if ( $counter) {
////      $messages->addMessage('blazon', 'Cannot counterchange 3 tinctures' . getLine($node) );
////      return '<g />';
//        $div3 = apply_tincture( $tinctures[1], $division['div3'] );
//    }
//  } else {
//    $div3 = '';
//  }
  // Safety checks
  if (!isset($tinctures[1]) || !is_object($tinctures[1]->firstChild)) $tinctures[1] = createColourNode('gray');
  // if (!isset($tinctures[2])) $tinctures[2] = createColourNode('argent');
  // set the main colours
  if ($tinctures[1]->firstChild->nodeName == 'counterchanged') {
    $div1 = '';
    $div2 = apply_tincture( $tinctures[1], $div2 );
  } elseif ( $counter ) {
    $div1 = apply_tincture ( $tinctures[2], '<rect x="0" y="0" width="1000" height="' . $height . '" />',
                              '1000,' . $height );
      if ( isset($division['div3']) && isset($tinctures[3]) ) {
          $div2 = apply_tincture( $tinctures[3], $div2 );
          $div3 = apply_tincture( $tinctures[1], $division['div3'] );
      } else {
          $div2 = apply_tincture( $tinctures[1], $div2 );
      }
  } else {
    $div1 = apply_tincture ( $tinctures[1], '<rect x="-100" y="-100" width="1200" height="' . ($height + 200) . '" />',
                              '1000,' . $height );
    $div2 = apply_tincture( $tinctures[2], $div2 );
    if ( isset($division['div3']) && isset($tinctures[3]) ) {
      $div3 = apply_tincture( $tinctures[3], $division['div3'] );
    }
  }
  $retval = "$div1\n$div2\n$div3\n";

  if ( array_key_exists('div4',$division) ) { // Add any extra embellishment
    $retval .= $division['div4'];
  }
  return '<g stroke-width="2" stroke="' . rgb('division-stroke') . '" >' . $retval . '</g>';
}
