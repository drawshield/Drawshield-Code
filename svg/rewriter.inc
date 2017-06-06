<?php

function rewrite() {
  /*
   *  @var $dom DOMElement
   */
  global $dom, $xpath;

 // $dom->firstChild->setAttribute('rewriter','true');


 // Bit of a fudge, look for 2 single charges, the second "in saltire", make them
  // look like they are both in saltire
  foreach ( $xpath->query("//charge[@number='1' and modifier/@param='insaltire']") as $item ) {
    if ( ($prev = $item->previousSibling) != null and $prev->nodeName == 'charge' and $prev->getAttribute('number') == '1' ) {
      $inbend = createModifierNode('arrangement', 'inbend' );
      $prev->appendChild($inbend);
    }
  }

  // A charge/within/ordinary becomes ordinary/between/charge
  foreach ( $xpath->query("//charge/modifier[@name='within']/ordinary") as $ordinary ) {
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
  }


  /*

  //
  foreach ( $xpath->query("//") as $item ) {
    ;
  }
*/
}

?>