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


function unique($prefix = 'item') {
  static $uni_num = 0;

  $uni_num += 1;
  return "$prefix$uni_num";
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

function existModifierWithKeyterm(DOMElement $node, $keyterm, $markUsed = true) {
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

function getModifierByType(DOMElement $node, $type, $markUsed = true) { // Also means don't get if already used
// FIX sometimes we seem to call this with a DOMNodeList (from error_log on 18/7/14)
  if ( $node != null and get_class($node) <> 'DOMNodeList' and $node->hasChildNodes()) {
    /**
     * @var DOMElement $child
     */
    foreach ( $node->childNodes as $child ) {
      if ( $child->nodeName === 'modifier' and $child->getAttribute('type') === $type ) {
        if ( $markUsed ) setRenderInfo( $child, 'used' );
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
        $retval[] = $child;
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
      if ( $markUsed ) $child->setAttribute('value','used');
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

function get_rgb(DOMElement $tinc_node) {
  global $messages;
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
  global $xpath, $options;

  // TODO, check for loops by comparing nodes?
  $counter = 0;
  while ( ($idRef = $node->getAttribute('IDREF')) != null ) {
    if (++$counter > 6 ) {
      error_log('Loop in IDREF - ' . $options['blazon']);
      break;
    }
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
  $node->setAttribute('renderinfo', $node->getAttribute('renderinfo') . '&' . $param . '=' . ($value) ? urlencode($value) : '1');
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
  do {
    $node = $node->parentNode;
  } while ($node->nodeName != blazonML::E_PLAIN);
  return getTinctureNode($node->firstChild);
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