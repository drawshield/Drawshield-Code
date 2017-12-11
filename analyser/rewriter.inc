<?php

function rewrite() {
  /*
   *  @var $dom DOMElement
   */
  global $dom, $xpath;

 // $dom->firstChild->setAttribute('rewriter','true');


 // Bit of a fudge, look for 2 single charges, the second "in saltire", make them
  // look like they are both in saltire
  foreach ( $xpath->query("//charge[@number='1' and modifier/@keyterm='insaltire']") as $item ) {
    if ( ($prev = $item->previousSibling) != null and $prev->nodeName == 'charge' and $prev->getAttribute('number') == '1' ) {
      $mod = getModifierNodeWithKeyterm($item,"insaltire");
      $mod->setAttribute("keyterm","bendsinwise");
      $mod->setAttribute('type','chargemod');
      $inbend = createModifierNode('chargemod', 'bendwise' );
      $prev->appendChild($inbend);
    }
  }
  foreach ( $xpath->query("//missing[parent::shield]") as $item ) {
    $simple = $dom->createElement(blazonML::E_PLAIN);
    $simple->setAttribute('ID',unique('N1-'));
    $field = $dom->createElement(blazonML::E_FIELD);
    $field->setAttribute('ID',unique('N1-'));
    $colour = createColourNode('cendree');
    $tincture = createTinctureNode();
    $tincture->appendChild($colour);
    $field->appendChild($tincture);
    $simple->appendChild($field);
    $parent = $item->parentNode;
    $parent->replaceChild($simple,$item);
  }
  // A charge/within/ordinary becomes ordinary/between/charge
 /* foreach ( $xpath->query("//charge/modifier[@name='within']/ordinary") as $ordinary ) {
    $charge = $ordinary->parentNode->parentNode;
    $parent = $charge->parentNode;
    $new_mod = createModifierNode('between');
    $new_charge = deep_copy($charge);
    foreach ( $new_charge->childNodes as $child )
      if ( $child->nodeName == 'modifier' and $child->getAttribute('name') == 'within' )
        $new_charge->removeChild($child);
    $new_mod->appendChild($new_charge);
    $ordinary->appendChild($new_mod);
    $parent->replaceChild($ordinary,$charge);
  } */

// counter passant is passant reversed
  foreach ( $xpath->query("//charge/modifier[@keyterm='counterpassant']") as $mod1 ) {
    $mod1->setAttribute('keyterm','passant');
    $new_mod = createModifierNode('chargemod','reversed');
    $mod1->parentNode->appendChild($new_mod);
  }

  /*

  //
  foreach ( $xpath->query("//") as $item ) {
    ;
  }
*/
}
