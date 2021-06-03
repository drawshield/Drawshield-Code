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

function includeDivision( DOMElement &$node, $layout, $tinctures ) {
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

  /**
   * @var $tinctures DOMelement[]
   */
  $tinctures = array();
  /**
   * @var $child DOMElement
   */
  foreach ( $node->childNodes as $child ) {
    if ( $child->nodeName == 'tincture' )
      $tinctures[$child->getAttribute('index') - 1] = $child;
  }

  // Safety checks
  if (!isset($tinctures[0]) || !is_object($tinctures[0]->firstChild))
    $tinctures[0] = createColourNode('gray');

  $division = includeDivision($node, $layout, $tinctures);

  $height = $layout->size->height;

  $in_divs = [''];
  for ( $i = 2; isset($division["div$i"]) && $i - 1 < count($tinctures); $i++ )
  {
    $in_divs[] = $division["div$i"];
  }

  if ( count($tinctures) > count($in_divs) )
    $tinctures = array_slice($tinctures, 0, count($in_divs));
  $ncols = count($tinctures);

  // set the main colours
  if ($tinctures[0]->firstChild->nodeName == 'counterchanged') {
    $out_divs = [
        apply_tincture( $tinctures[0], $in_divs[0], $layout )
    ];
  } else {
    if ( $counter ) { // shuffle tinctures around for counterchanging, (depends how many colours in total)
        if ( $ncols == 4 ) { // 4 colours, swap alternate pairs
            $tinctures = [
                $tinctures[2],
                $tinctures[3],
                $tinctures[0],
                $tinctures[1],
            ];
        } else { // move up 1
            $t0 = array_shift($tinctures);
            $tinctures[] = $t0;
        }
    }
    $out_divs = [
        apply_tincture ( $tinctures[0], $layout->rect_area()->margin(-100)->svg_rect(), $layout, $layout->size)
    ];
    for ( $i = 1; $i < $ncols; $i++ )
    {
        $out_divs[] = apply_tincture( $tinctures[$i], $in_divs[$i], $layout );
    }
  }
  $retval = implode("\n", $out_divs);

  if ( array_key_exists('divx',$division) ) { // Add any extra embellishment
    $retval .= $division['divx'];
  }
  return '<g stroke-width="2" stroke="' . rgb('division-stroke') . '" >' . $retval . '</g>';
}
