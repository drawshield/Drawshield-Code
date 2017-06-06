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
  global $messages;

  $divisionType = $node->getAttribute('keyterm');
  $division = array ();
  $file = "divisions/$divisionType.inc";
  // WARNINGS Accepted
  if ( stream_resolve_include_path($file) !== false )
    include ( $file );
  else { // can't draw this division
    $messages->addMessage('warning','Cannot draw $type, substituting per-bend' );
    include ( 'divisions/per-bend.inc');
  }
  return $division;
}


function makeDivision( DOMElement $node, $counter = false ) {
  /*
   * @var messageStore
   */
  global $messages;

  $division = includeDivision($node);

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
    elseif ( $child->nodeName == 'division' )
      $secondDiv = $child;
  }
  $div2 = $division['div2'];
  if ( array_key_exists('div3', $division)) {
    if ( $counter) {
      $messages->addMessage('blazon', 'Cannot counterchange 3 tinctures' );
      return '<g />';
    }
    $div3 = $division['div3'];
  } else
    $div3 = '';
  // set the main colours
  if ( $tinctures[1]->getAttribute('origin') == 'implied' ) { // This is a 2nd division
    $div1 = '';
    $tinctures[1]->setAttribute('origin','given');
    $div2 = apply_tincture( $tinctures[1], $div2 );
  } else {
    if ( $counter ) {
      $div1 = apply_tincture ( $tinctures[2], '<rect x="0" y="0" width="1000" height="1200" />',
                                '1000,1200' );
      $div2 = apply_tincture( $tinctures[1], $div2 );
    } else {
      $div1 = apply_tincture ( $tinctures[1], '<rect x="-100" y="-100" width="1200" height="1400" />',
                                '1000,1200' );
      $div2 = apply_tincture( $tinctures[2], $div2 );
      if ( $div3 != '' and isset($tinctures[3]) ) $div3 = apply_tincture( $tinctures[3], $div3 );
    }
  }
  $retval = "$div1\n$div2\n$div3\n";
  if ( !$counter and $secondDiv ) {
    $retval .= makeDivision( $secondDiv );
  }
  if ( array_key_exists('div4',$division) ) { // Add any extra embellishment
    $retval .= $division['div4'];
  }
  return '<g stroke-width="2" stroke="' . rgb('division-stroke') . '" >' . $retval . '</g>';
}
