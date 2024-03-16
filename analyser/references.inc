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

class references {
  /**
   * @var DOMDocument $dom
   */
  private $dom;
  /**
   * @var DOMXPath $xpath
   */
  private $xpath;

  private $messages;


  private $unique;

  function __construct($xml) {
      $this->dom = $xml;
      $this->xpath = new DOMXPath($this->dom);
      $this->unique = new unique();
      $this->messages = $this->xpath->query('//messages');
      if ( $this->messages->length == 0 ) {
          $this->messages = $this->dom->createElement('messages');
          $this->dom->firstChild->appendChild($this->messages);
      } else
          $this->messages = $this->messages->item(0);
  }

  protected function message($text, $category = "blazon") {
      $messageItem = $this->dom->createElement('message',$text);
      $messageItem->setAttribute('category', $category);
      $this->messages->appendChild($messageItem);
  }

  protected function greyShield() {
      $simple = $this->dom->createElement(blazonML::E_PLAIN);
      $simple->setAttribute('ID',$this->unique->getRef("N"));
      $field = $this->dom->createElement(blazonML::E_FIELD);
      $field->setAttribute('ID',$this->unique->getRef("N"));
      $colour = createColourNode('cendree');
      $field->appendChild($colour);
      $simple->appendChild($field);  
      return $simple; 
  }

  public function setReferences() {
    global $options;

    // Set IDREF first, to point to appropriate node
    $refList = array (
      array ( '//charge[@keyterm="special/same"]','ancestor::charge[1]|ancestor::ordinary[1]|preceding::charge[1]|preceding::ordinary[1]', true ), // two of the same | another ...
      // array ( '//tincture[@origin="ofsame"]','ancestor::tincture[1]|preceding::tincture[1]', true ),// Of the same
      // array ( '//tincture[@origin="oflast"]','ancestor::tincture[not(@origin="pending")][1]|preceding::tincture[not(@origin="pending")][1]', true ),// Of the last (in case this is different from of the same?)
//      array ( '//tincture[@origin="pending"]','preceding-sibling::modifier/tincture[1]|following::tincture[1]', false ),// Pending
      array ( '//tincture[@origin="offield"]','ancestor::simple/field/tincture', true ),// Of the field
      // array ( '//tincture[@origin="offirst"]','(//tincture[colour])[1]', true ),// of the first
      // array ( '//tincture[@origin="ofsecond"]','(//tincture[colour])[2]', true ),// of the second
      // array ( '//tincture[@origin="ofthird"]','(//tincture[colour])[3]', true ),// of the third
      // array ( '//tincture[@origin="offourth"]','(//tincture[colour])[4]', true ),// of the fourth
      );
    foreach ( $refList as $ref ) {
      /**
       * @var DOMNodeList $nodeList
       */
      $nodeList = $this->xpath->query($ref[0]);
      /**
       * @var DOMElement $sourceNode
       */
      foreach ($nodeList as $sourceNode ) {
        /**
         * @var DOMNodeList $refNodes
         */
        $refNodes = $this->xpath->query($ref[1],$sourceNode);
        if ( $refNodes->length ) {
          /**
           * @var DOMElement $target
           */
          $target = $refNodes->item(0);
          $targetID = $target->getAttribute('ID');
          $sourceNode->setAttribute('IDREF',$targetID);
        } else { // Error reporting
          $sourceTokens = $sourceNode->getAttribute('tokens');
          $this->message("Cannot find what \"$sourceTokens\" refers to");
        }
      }
    }
    // Resolve "as many" (Need to set attribute, can't just point to node)
    $nodeList = $this->xpath->query('//charge[@number="asmany"]');
    foreach ($nodeList as $sourceNode ) {
      $refNodes = $this->xpath->query('preceding::charge[@number>"1"]|preceding::ordinary[@number>"1"]',$sourceNode);
      if ( $refNodes->length ) {
        /**
         * @var DOMElement $target
         */
        $target = $refNodes->item(0);
        $sourceNode->setAttribute('number',$target->getAttribute('number'));
      } else {
        $this->message("Cannot find number for \"as many\" (using \"2\")");
        $sourceNode->setAttribute('number','2');
      }
    }

    // Make sure that the boss is on top(!)
      $nodeList = $this->xpath->query('//ordinary[@keyterm="boss"]');
      foreach ($nodeList as $sourceNode ) {
          $parent = $sourceNode->parentNode;
          $parent->appendChild($sourceNode);
      }

    // Resolve tinctures that remain as "pending" (i.e. not given but same as previous - if there was one)
    $nodeList = $this->xpath->query('//tincture[@origin="pending"]');
    foreach ($nodeList as $sourceNode ) {
      // Nodes are returned in DOCUMENT order, first the preceeding (if there is one)
      // then the following, so we chose the END of the list as we want to look forward first
      $refNodes = $this->xpath->query('following::tincture[1]|preceding::tincture[1]',$sourceNode);
      if ( $refNodes->length ) {
        /**
         * @var DOMElement $target
         */
        $target = $refNodes->item($refNodes->length - 1);
        $targetID = $target->getAttribute('ID');
        $sourceNode->setAttribute('IDREF',$targetID);
      }
       // else leave as pending, to be found later (in svgutils)
    }

    // Resolve likewise charge adjustments
    $nodeList = $this->xpath->query('//charge/modifier[@keyterm="likewise"]');
    foreach ($nodeList as $sourceNode ) {
      $refNodes = $this->xpath->query('preceding::modifier[@type="shown"]',$sourceNode);
      if ( $refNodes->length ) {
        /**
         * @var DOMElement $target
         */
        foreach ($refNodes as $refNode) {
          $sourceNode->parentNode->appendChild($refNode->cloneNode());
        }
      }  else { // else leave as likewise, it will be ignored
        $this->message("No previous adjustments for likewise");
      }
    }

    // Rearrange "charge within ordinary" as "within ordinary charge"
    $nodeList = $this->xpath->query('//modifier[@keyterm="within" and parent::ordinary and child::missing]');
    foreach ($nodeList as $modifierNode) {
      $ordinaryNode = $modifierNode->parentNode;
      $prevCharges = $this->xpath->query('preceding-sibling::charge',$ordinaryNode);
      if ($prevCharges->length) {
        $adjacentCharge = $prevCharges->item($prevCharges->length - 1);
        $modifierNode->replaceChild($adjacentCharge,$modifierNode->firstChild); 
      } else {
            $this->message("Expected charge for within/inside");
      }
    }

    // A single label is an ordinary (unless it is "for difference")
    // $nodeList = $this->xpath->query('//charge[@keyterm="label/label" and @number="1" and not(modifier/@keyterm="difference")]');
    // foreach ($nodeList as $modifierNode) {
    //   $ord = $this->dom->createElement(blazonML::E_ORDINARY);
    //   $ord->setAttribute('ID',$modifierNode->getAttribute('ID'));
    //   $ord->setAttribute('keyterm','label');
    //   $ord->setAttribute('number','1');
    //   foreach ($modifierNode->childNodes as $child) {
    //     $ord->appendChild($child->cloneNode(true));
    //   }
    //   $parent = $modifierNode->parentNode;
    //   $parent->replaceChild($ord,$modifierNode);
    // }

    // counter passant is passant reversed
    $nodeList = $this->xpath->query('//charge/modifier[@keyterm="counterpassant"]');
    foreach ($nodeList as $modifierNode) {
      $modifierNode->setAttribute('keyterm','passant');
      $new_mod = createModifierNode('chargemod','reversed');
      $modifierNode->parentNode->appendChild($new_mod);
    }

    // make armed, clawed and horned synonyms, unless both are given
    $nodeList = $this->xpath->query('//modifier[@keyterm="horned" or @keyterm="clawed"]');
    $addArmed = true;
    foreach ( $nodeList as $item ) {
      // look for an existing armed modifier
      $parent = $item->parentNode;
      foreach($parent->childNodes as $child) {
        if ($child->nodeName == 'modifier' && $child->getAttribute('keyterm') == 'armed') {
          $addArmed = false;
          break;
        }
      }
      if ($addArmed) {
        $armed = $item->cloneNode(true);
        $armed->setAttribute('keyterm','armed');
        $parent->appendChild($armed);
      }
    }
    $nodeList = $this->xpath->query('//modifier[@keyterm="armed"]');
    $addHorned = true;
    foreach ( $nodeList as $item ) {
      // look for an existing armed modifier
      $parent = $item->parentNode;
      foreach($parent->childNodes as $child) {
        if ($child->nodeName == 'modifier' && $child->getAttribute('keyterm') == 'horned') {
          $addHorned = false;
          break;
        }
      }
      if ($addHorned) {
        $armed = $item->cloneNode(true);
        $armed->setAttribute('keyterm','horned');
        $parent->appendChild($armed);
        $armed = $item->cloneNode(true);
        $armed->setAttribute('keyterm','clawed');
        $parent->appendChild($armed);
      }
    }


    // Replace missing shields with plain grey
    $nodeList = $this->xpath->query('//missing[parent::shield]');
    foreach ( $nodeList as $item ) {
      $parent = $item->parentNode;
      $parent->replaceChild($this->greyShield(),$item);
    }

    // replace "another" with a copy of the most recent charge
    $nodeList = $this->xpath->query('//charge[@keyterm="special/same"]');
    foreach ( $nodeList as $item ) {
      $prevCharges = $this->xpath->query('preceding-sibling::charge',$item);
      if ($prevCharges->length) {
        $prevCharge = $prevCharges->item($prevCharges->length - 1);
        $item->setAttribute('keyterm', $prevCharge->getAttribute('keyterm'));
      } else {
        $this->message('No previous charge for "another"');
        $item->setAttribute('keyterm', 'characters/question');
      }
    }

    // Bit of a fudge, look for 2 single charges, the second "in saltire", make them
    // look like they are both in saltire
    $nodeList = $this->xpath->query("//charge[@number='1' and modifier/@keyterm='insaltire']");
    foreach ( $nodeList as $item ) {
      if ( ($prev = $item->previousSibling) != null and $prev->nodeName == 'charge' and $prev->getAttribute('number') == '1' ) {
        $mod = getModifierNodeWithKeyterm($item,"insaltire");
        $mod->setAttribute("keyterm","bendsinwise");
        $mod->setAttribute('type','chargemod');
        $inbend = createModifierNode('chargemod', 'bendwise' );
        $prev->appendChild($inbend);
      }
    }

    // If "grady" does not give a direction, use the shape of the ordinary
    $nodeList = $this->xpath->query("//treatment[@keyterm='grady']");
    foreach ( $nodeList as $item ) {
      $object = $item->parentNode->parentNode;
      if ($object->nodeName == 'ordinary') {
        $ordinary = $object->getAttribute("keyterm");
        switch ($ordinary) {
          case 'fess':
          case 'chief':
          case 'base':
          case 'bar':
            $item->setAttribute("keyterm","grady-in-fess");
            break;
          case 'pale':
          case 'pall':
          case 'tierce':
          case 'palet':
          case 'pile':
            $item->setAttribute("keyterm","grady-in-pale");
            break;
          case 'bend':
          case 'bendlet':
            $item->setAttribute("keyterm","grady-in-bend");
            break;
          default:
            break;
        }
      }
    }

    // Fixup quarterings to the nearest known number
    $nodeList = $this->xpath->query("//complex");
    foreach ($nodeList as $node) {
        $keyterm = $node->getAttribute("keyterm");
        if ( $keyterm == "dimidiated" || $keyterm == "impaled" )
            continue;

        $newOrder = (int)$node->getAttribute('order');

        if ( $keyterm == "quartered" )
        {
            // Work out the preferred number of quarterings (depends on what we can draw)
            //                   0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
            $newOrders = array ( 4,4,4,4,4,5,6,8,8,9,12,12,12,16,16,16,16,20,20,20,20,
                //                21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,
                                25,25,25,25,25,30,30,30,30,30,36,36,36,36,36,36,42,42,
                            //  39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
                                42,42,42,42,49,49,49,49,49,49,49,56,56,56,56,56,56,56,
                            //  57,58,59,60,61,62,63,64
                                64,64,64,64,64,64,64,64
                                );
            $newOrder = $newOrders[$newOrder];
            $node->setAttribute('order',"$newOrder");
        }

        // get a list of the existing quartering IDs
        $existingIDs = array();
        $existingNodes = array();
        $children = $node->childNodes;
        foreach ($children as $subShield) {
            $child = $subShield->firstChild;
            if ($child != null && $child->nodeName != blazonML::E_PENDING) {
                $thisID = $subShield->getAttribute('ID');
                if ($thisID != null) {
                    $existingIDs[] = $thisID;
                    $existingNodes[$thisID] = $subShield;
                }
            }
        }
        // Idiot check no quarterings given at all
        if (count($existingIDs) == 0) {
            $simple = $this->greyShield();
            $firstChild = $node->firstChild; // the shield element
            if ($firstChild != null) {
            $theID = $this->unique->getRef("N");
            $firstChild->setAttribute('ID', $theID);
            $existingIDs[] = $theID;
            $pending = $firstChild->firstChild;
            $firstChild->replaceChild($simple,$pending);
            }
        }
        // start counting through the available IDs
        $currentID = 0;
        // Add any additional quarterings that are needed, as pending
        // (There may already been some pending nodes from the parser)
        for($i = $children->length; $i < $newOrder; $i++ ) {
            $shield = $this->dom->createElement(blazonML::E_SHIELD);
            $shield->setAttribute('index',$i + 1);
            $shield->setAttribute('ID',$this->unique->getRef("P"));
            $shield->appendChild($this->dom->createElement(blazonML::E_PENDING));
            $node->appendChild($shield);
        }
        // Now go through the children, replacing any pending nodes with copies of
        // existing subshields
        $missingReported = false;
        for ($j = 0; $j < $children->count(); $j++) {
            $subShield = $children->item($j);
        // foreach ($children as $subShield) {
            $child = $subShield->firstChild;
            if ($child != null && $child->nodeName == blazonML::E_PENDING && isset($existingIDs[$currentID])) {
                $refNode = $existingIDs[$currentID];
                if (is_null($refNode)) {
                    ds_error_log('Null ref node in subshield', true);
                    continue;
                }
                if (!array_key_exists($refNode, $existingNodes)) {
                    if (!$missingReported) {
                        $this->message( 'No quarter description found');
                        $missingReported = true;
                    }
                    continue;
                }
                $newNode = $existingNodes[$refNode]->cloneNode(true);
                $newNode->setAttribute('ID',$this->unique->getRef("clone"));
                $newNode->setAttribute('index',$j + 1);
                $node->replaceChild( $newNode, $subShield);
                if (++$currentID >= count($existingIDs))
                    $currentID = 0;
            }
        }
    }

    while ( true )
    {
        // Doing this instead of foreach to ensure all occurences are cloned
        $list = $this->xpath->query('//modifier[@type="holding" and @keyterm="in-hand-both"][1]');
        if ( count($list) == 0 )
            break;
        $node = $list[0];

        $node->setAttribute("keyterm", "in-hand-left");
        $clone = $node->cloneNode(true);
        $clone->setAttribute("keyterm", "in-hand-right");
        $node->parentNode->appendChild($clone);
    }

    return $this->dom;
  }
}
   
