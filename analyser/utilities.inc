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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */

function tidyOptions() {
    global $options;

    // Tidy up options
    if (!array_key_exists('shape',$options) || $options['shape'] == 'default') {
        $options['shape'] = 'heater';
    } elseif (in_array($options['shape'],array('circular','round'))) {
        // Synonyms for circle shape
        $options['shape'] = 'circle';
    } elseif ($options['shape'] == 'flag') {
        $options['flagHeight'] = (int)(round($options['aspectRatio'] * 1000));
    }
    if ($options['palette'] == 'default') $options['palette'] = 'drawshield';
}


function createModifierNode($type, $keyterm, $value = null ) {
  global $dom;

  $mod = $dom->createElement('modifier');
  $mod->setAttribute('type', $type);
  $mod->setAttribute('keyterm', $keyterm);
  $mod->setAttribute('ID',unique('S1-'));
  $mod->setIdAttribute('ID', true);
  if ( $value ) $mod->setAttribute('value', $value);
  return $mod;
}

function createChargeNode($keyterm, $number = 1) {
  global $dom;

  $charge = $dom->createElement('charge');
  $charge->setAttribute('keyterm', $keyterm);
  $charge->setAttribute('number', $number);
  $charge->setAttribute('ID',unique('S1-'));
  $charge->setIdAttribute('ID', true);
  return $charge;
}

function createTinctureNode($index = 1) {
  global $dom;

  $tincture = $dom->createElement('tincture');
  $tincture->setAttribute('index', $index);
  $tincture->setAttribute('origin', 'implied'); // or should it be 'internal'?
  $tincture->setAttribute('ID',unique('S1-'));
  $tincture->setIdAttribute('ID', true);
  return $tincture;
}

function createColourNode($colour, $index='1') {
  global $dom;

  $tinc = $dom->createElement('tincture');
  $tinc->setAttribute('ID',unique('S1-'));
  $tinc->setIdAttribute('ID', true);
  $col = $dom->createElement('colour');
  $col->setAttribute('keyterm',$colour);
  $col->setAttribute('ID',unique('S1-'));
  $col->setIdAttribute('ID', true);
  $tinc->setAttribute('index',$index);
  $tinc->appendChild($col);
  return $tinc;
}

function existModifierWithKeyterm(?DOMElement $node, $keyterm, $markUsed = true) {
// FIX sometimes we seem to call this with a DOMNodeList (from error_log on 18/7/14)
  if ( $node != null and get_class($node) <> 'DOMNodeList' and $node->hasChildNodes()) {
    /**
     * @var DOMElement $child
     */
    foreach ( $node->childNodes as $child ) {
      if ( $child->nodeName === 'modifier' and $child->getAttribute('keyterm') === $keyterm ) {
        if ( $markUsed ) setRenderInfo( $child, 'used' );
        return true;
      }
    }
  }
  return false;
}

function getModifierNodeByType(DOMElement $node, $type) { 
  if ( $node != null and get_class($node) <> 'DOMNodeList' and $node->hasChildNodes()) {
    foreach ( $node->childNodes as $child ) {
      if ( $child->nodeName === 'modifier' and $child->getAttribute('type') === $type ) {
        return $child;
      }
    }
  }
  return false;
}

function getModifierByType(DOMElement $node, $type, $markUsed = true) { // Also means don't get if already used
// FIX sometimes we seem to call this with a DOMNodeList (from error_log on 18/7/14)
  if ( $node != null and get_class($node) <> 'DOMNodeList' and $node->hasChildNodes()) {
    /**
     * @var DOMElement $child
     */
    foreach ( $node->childNodes as $child ) {
      if ( $child->nodeName === 'modifier' and $child->getAttribute('type') === $type ) {
        if ( $markUsed ) {
          if (getRenderInfo($child, 'used')) continue;
          /* else */ setRenderInfo( $child, 'used' );
        }
        return (string)$child->getAttribute('keyterm');
      }
    }
  }
  return false;
}


function getModifierValueByKeyterm(DOMElement $node, $name, $markUsed = false) {
// FIX sometimes we seem to call this with a DOMNodeList (from error_log on 18/7/14)
  if ( $node != null and get_class($node) <> 'DOMNodeList' and $node->hasChildNodes()) {
    /**
     * @var DOMElement $child
     */
    foreach ( $node->childNodes as $child ) {
      if ( $child->nodeName === 'modifier' and (($child->getAttribute('keyterm') === $name and getRenderInfo($child, 'used') != true ) ) ) {
        $retval = (string)$child->getAttribute('value');
        if ( $markUsed ) setRenderInfo( $child, 'used' );
        return $retval;
      }
    }
  }
  return null;
}

function has_feature(DOMElement $node, $name) {
  if ( $node == null or !$node->hasChildNodes()) return null;
  /**
   * @var DOMElement $child
   */
  foreach ( $node->childNodes as $child ) {
    if ( $child->nodeName === 'modifier' and $child->getAttribute('type') === 'feature' and $child->getAttribute('keyterm') === $name ) {
      return true;
    }
  }
  return false;
}

function getModifiersByTypeAsArray(DOMElement $node, $type, $returnNodes = false) {
  $retval = array();
  if ( $node == null ) return null;
  /**
   * @var DOMElement $child
   */
  foreach ( $node->childNodes as $child ) {
    if ( $child->nodeName == 'modifier' and $child->getAttribute('type') == $type ) {
      if ( $returnNodes ) {
        $retval[] = $child->cloneNode(true);
      } else {
        $val = $child->getAttribute('value');
        if ($val == null) $val = true; // Just say the modifier exists
        $retval[$child->getAttribute('keyterm')] = $val;
      }
    }
  }
  return $retval;
}

function getModifierNodeWithKeyterm(DOMElement $node, $keyterm, $markUsed = true) {
  if ( $node == null ) return null;
  /**
   * @var DOMElement $child
   */
  foreach ( $node->childNodes as $child ) {
    if ( $child->nodeName == 'modifier' and $child->getAttribute('keyterm') == $keyterm ) {
      if ( $markUsed ) setRenderInfo($child, 'used');
      return $child;
    }
  }
  return null;
}


function getChildOfType(DOMElement $node, $nodeName) {
  $retval = null;
  /**
   * @var DOMElement $child
   */
  foreach ( $node->childNodes as $child ) {
    if ( $child->nodeName == $nodeName ) {
      $retval = $child;
      break;
    }
  }
  return $retval;
}

function get_rgb($tinc_node) {
  global $messages;
    if (!$tinc_node) return ('#555555'); // but probably an error
	$col = $tinc_node->firstChild;
	if ( !$col or $col->nodeName != 'colour' ) {
      $messages->addMessage('warning','plain colour expected');
	  return ( '#555555' );
	} // So now we have a colour node
	return rgb($col->getAttribute('keyterm'));
}

function getTinctureNode(DOMElement $node,$index=null) {
  /**
   * @var DOMElement $child
   */
  foreach ( $node->childNodes as $child ) {
    if ( $child->nodeName == 'tincture' ) {
		  if ( !$index ) return resolveReference($child);
			if ( $child->getAttribute('index') == $index ) {
			  return resolveReference( $child );
			}
    }
  }
  return null; // TODO should this return missing node?
}

function resolveReference(DOMElement $node) {
  global $xpath;
  $visitedNodes = [];

  while ( $node != null && ($idRef = $node->getAttribute('IDREF')) != null ) {
    $currentNodeID = $node->getAttribute('ID');
    if (in_array($currentNodeID, $visitedNodes)) {
      // Not an coding error, probably a missing tincture in the blazon
      break;
    }
    $visitedNodes[] = $currentNodeID;
    $node = $xpath->query('//*[@ID="' . $idRef . '"]')->item(0);
  }
  return $node;
}

function getNumberOfCharges(DOMElement $node) { // Find total number of charges, including if it is "between" some others
  $number = $node->getAttribute('number');
  if ( $node->parentNode->getAttribute('keyterm') == 'on') {
    /**
     * @var DOMElement $between
     */
    if ( $between = getModifierNodeWithKeyterm($node,'between')) {
      $number += (integer)$between->firstChild->getAttribute('number');
    }
  }
  return $number;
}

function setRenderInfo(DOMElement $node, $param, $value = null ) {
  // TODO what if it already exists?
  $curValue = $node->getAttribute('renderinfo');
  if ($curValue != "") $curValue .= '&';
  $newValue = $curValue . $param . '=' . (($value) ? urlencode($value) : '1');
  $node->setAttribute('renderinfo',  $newValue);
}

function getRenderInfo(DOMElement $node, $match ) {
  if (! ($info = $node->getAttribute('renderinfo'))) return null;
  foreach ( explode('&', $info) as $item ) {
    list ( $param, $value ) = explode('=',$item );
    if ( $param == $match ) {
      return $value;
    }
  }
  return null;
}

function getFieldTincture(DOMElement $node) {
    $loopDetect = 0;
    while ($loopDetect++ < 50 && !is_null($node) && is_object($node) && $node->nodeName != blazonML::E_BLAZON) {
        if ($node->nodeName == blazonML::E_PLAIN) {
            return getTinctureNode($node->firstChild);
        }
    }
    return null;
//  do {
//    if (is_null($node) || !is_object($node) )
//      return null;
//    $node = $node->parentNode;
//  } while (is_object($node) && $node->nodeName != blazonML::E_PLAIN);
//  return getTinctureNode($node->firstChild);
}

function words2digits ($words) {
  $additions = array ( 'a' => 1, 'one' => 1, 'two' => 2, 'three' => 3, 'four' => 4, 'five' => 5,
      'six' => 6, 'seven' => 7, 'eight' => 8, 'nine' => 9, 'zero' => 0,
      'ten' => 10, 'eleven' => 11, 'twelve' => 12, 'thirteen' => 13,
      'fourteen' => 14, 'fifteen' => 15, 'sixteen' => 16, 'seventeen' => 17,
      'eighteen' => 18, 'nineteen' => 19, 'twenty' => 20, 'thirty' => 30,
      'forty' => 40, 'fifty' => 50, 'sixty' => 60, 'seventy' => 70, 'eighty' => 80,
      'ninety' => 90 );
  $multipliers = array ( 'hundred' => 100, 'thousand' => 1000 );

  $found = false;
  $value = 0;
  $previous = 0;
  foreach ( explode(' ',$words) as $word ) {
    $word = strtolower($word);
    if (array_key_exists($word,$additions)) {
      $found = true;
      $previous = $additions[$word];
      $value += $previous;
    } elseif (array_key_exists($word,$multipliers)) {
      $found = true;
      $value -= $previous; // It is a multiplier, not an addition
      if ($previous == 0)
        $value += $multipliers[$word];
      else
        $value += $previous * $multipliers[$word];
    }
    // no error if nothing found
  }
  return $found ? sprintf("%d",$value) : false;
}
