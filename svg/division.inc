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

function includeDivision( DOMElement &$node, $layout ) {
  global $messages;

  $divisionType = $node->getAttribute('keyterm');
  $division = array ();
  $file = null;
  if ($layout->shape == 'flag') { // look in the flag sub-directory first
    $file ="divisions/flag/$divisionType.inc";
    if ( stream_resolve_include_path($file) === false ) $file = null;
  }
  if ($file == null) {
    $file = "divisions/$divisionType.inc";
    if ( stream_resolve_include_path($file) === false ) $file = null;
  }    
  $calcHeight = $layout->size->height;
  if ($file != null) { // calcHeight is available within the included file
    include ( $file );
  } else { // can't draw this ordinary
    $messages->addMessage('warning',"Cannot draw $divisionType, substituting per bend" . getLine($node) );
    include ( 'divisions/per-bend.inc');
  }
  return $division;
}


function makeDivision( DOMElement $node, ShieldLayout $layout, $counter = false ) {
  /*
   * @var messageStore
   */
  global $messages;

  $division = includeDivision($node, $layout);

  $height = $layout->size->height;

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
  $div4 = '';
  // Safety checks
  if (!isset($tinctures[1]) || !is_object($tinctures[1]->firstChild)) $tinctures[1] = createColourNode('gray');
  // set the main colours
  if ($tinctures[1]->firstChild->nodeName == 'counterchanged') {
    $div1 = '';
    $div2 = apply_tincture( $tinctures[1], $div2, $layout );
  } elseif ( $counter ) { // shuffle tinctures around for counterchanging, (depends how many colours in total)
      
    if ( isset($division['div4']) && isset($tinctures[4]) ) { // 4 colours, swap alternate pairs
        $div1 = apply_tincture ( $tinctures[3], $layout->rect_area()->svg_rect(), $layout, $layout->size );
        $div2 = apply_tincture( $tinctures[4], $div2, $layout );
        $div3 = apply_tincture( $tinctures[1], $division['div3'], $layout );
        $div4 = apply_tincture( $tinctures[2], $division['div4'], $layout );
    } elseif ( isset($division['div3']) && isset($tinctures[3]) ) { // 3 colours, move up 1
        $div1 = apply_tincture ( $tinctures[2], $layout->rect_area()->svg_rect(), $layout, $layout->size );
        $div2 = apply_tincture( $tinctures[3], $div2, $layout );
        $div3 = apply_tincture( $tinctures[1], $division['div3'], $layout );
    } else {  // 2 colours, just swap them.
        $div1 = apply_tincture ( $tinctures[2], $layout->rect_area()->svg_rect(), $layout, $layout->size );
        $div2 = apply_tincture( $tinctures[1], $div2, $layout );
    }
  } else { // Not counterchanged, colour as normal
    $div1 = apply_tincture ( $tinctures[1], $layout->rect_area()->margin(-100)->svg_rect(), $layout, $layout->size);
    $div2 = apply_tincture( $tinctures[2], $div2, $layout );
    if ( isset($division['div3']) && isset($tinctures[3]) ) {
      $div3 = apply_tincture( $tinctures[3], $division['div3'], $layout );
    }
    if ( isset($division['div4']) && isset($tinctures[4]) ) {
      $div4 = apply_tincture( $tinctures[4], $division['div4'], $layout );
    }
  }
  $retval = "$div1\n$div2\n$div3\n$div4\n";

  if ( array_key_exists('divx',$division) ) { // Add any extra embellishment
    $retval .= $division['divx'];
  }
  return '<g stroke-width="2" stroke="' . rgb('division-stroke') . '" >' . $retval . '</g>';
}
