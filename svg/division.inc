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
	 
function get_div_height ( DOMElement $node ) {
	// TODO rewrite this, possibly as full rewrite of divisions to match charges and ordinaries
	// it relies on exact knowledge of internal division name
  $subtype = $node->getAttribute('keyterm');
	if ( in_array($subtype,array('per-bend', 'per-bend-sinister', 'gyronny', 'quarterly', 'per-saltire', 'per-chevron')))
		return 1000;
	else
	  return 1200;
 }

function includeDivision( DOMElement &$node, $layout, &$tinctures ) {
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
  $counterchanged =   $tinctures[0]->hasChildNodes() && ($tinctures[0]->firstChild->nodeName == 'counterchanged');

  // Safety checks
  if (!isset($tinctures[0]) || !is_object($tinctures[0]->firstChild))
    $tinctures[0] = createColourNode('gray');

  $division = includeDivision($node, $layout, $tinctures);

  $height = $layout->size->height;

  $in_divs = [''];
  for ( $i = 2; isset($division["div$i"]) && ($counterchanged || $i - 1 < count($tinctures)) ; $i++ )
  {
    $in_divs[] = $division["div$i"];
  }

  if ( count($tinctures) > count($in_divs) )
    $tinctures = array_slice($tinctures, 0, count($in_divs));
  $ncols = count($tinctures);

  // set the main colours
  if ($counterchanged) {
    $out_divs = [
        apply_tincture( $tinctures[0], $in_divs[1], $layout )
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
