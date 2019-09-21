<?php /* Copyright 2014 Karl R. Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

class references {
  /**
   * @var DOMDocument $dom
   */
  private $dom;
  /**
   * @var DOMXPath $xpath
   */
  private $xpath;
  
  function __construct($xml) {
      $this->dom = $xml;
      $this->xpath = new DOMXPath($this->dom);
  }

  protected function greyShield() {
      $simple = $this->dom->createElement(blazonML::E_PLAIN);
      $simple->setAttribute('ID',unique('N1-'));
      $field = $this->dom->createElement(blazonML::E_FIELD);
      $field->setAttribute('ID',unique('N1-'));
      $colour = createColourNode('cendree');
      $tincture = createTinctureNode();
      $tincture->appendChild($colour);
      $field->appendChild($tincture);
      $simple->appendChild($field);  
      return $simple; 
  }

  public function setReferences() {
    global $options;

    // Set IDREF first, to point to appropriate node
    $refList = array (
      array ( '//charge[@keyterm="special/same"]','ancestor::charge[1]|ancestor::ordinary[1]|preceding::charge[1]|preceding::ordinary[1]', true ), // two of the same | another ...
      array ( '//tincture[@origin="ofsame"]','ancestor::tincture[1]|preceding::tincture[1]', true ),// Of the same
      array ( '//tincture[@origin="oflast"]','ancestor::tincture[not(@origin="pending")][1]|preceding::tincture[not(@origin="pending")][1]', true ),// Of the last (in case this is different from of the same?)
//      array ( '//tincture[@origin="pending"]','preceding-sibling::modifier/tincture[1]|following::tincture[1]', false ),// Pending
      array ( '//tincture[@origin="offield"]','ancestor::simple/field/tincture', true ),// Of the field
      array ( '//tincture[@origin="offirst"]','preceding::tincture[colour][last()]', true ),// of the first
      array ( '//tincture[@origin="ofsecond"]','preceding::tincture[colour][last()-1]', true ),// of the second
      array ( '//tincture[@origin="ofthird"]','preceding::tincture[colour][last()-2]', true ),// of the third
      array ( '//tincture[@origin="offourth"]','preceding::tincture[colour][last()-3]', true ),// of the fourth
      );
    $messages = $this->xpath->query('//messages');
    if ( $messages->length == 0 ) {
      $messages = $this->dom->createElement('messages');
      $this->dom->firstChild->appendChild($messages);
    } else
      $messages = $messages->item(0);
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
          $messages->appendChild($this->dom->createElement('message',"Cannot find what \"$sourceTokens\" refers to"));
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
        $messages->appendChild($this->dom->createElement('message',"Cannot find number for \"as many\" (using \"2\")"));
        $sourceNode->setAttribute('number','2');
      }
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
        $messages->appendChild($this->dom->createElement('message',"No previous adjustments for likewise"));
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
            $messages->appendChild($this->dom->createElement('message',"Expected charge for within/inside"));
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
        $messages->appendChild($this->dom->createElement('message','No previous charge for "another"'));  
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
      if ( ($prev = $item->previousSibling) != null and $prev->nodeName == 'charge' and $prev->getAttribute('number') == '1' ) {
        $mod = getModifierNodeWithKeyterm($item,"insaltire");
        $mod->setAttribute("keyterm","bendsinwise");
        $mod->setAttribute('type','chargemod');
        $inbend = createModifierNode('chargemod', 'bendwise' );
        $prev->appendChild($inbend);
      }
    }

    // Fixup quarterings to the nearest known number
    $nodeList = $this->xpath->query("//complex[@keyterm='quartered']");
    foreach ($nodeList as $node) {
      //error_log("Testing quartering");
      // Work out the preferred number of quarterings (depends on what we can draw)
      //                   0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
      $newOrders = array ( 4,4,4,4,4,5,6,8,8,9,16,16,16,16,16,16,16,20,20,20,20,
        //                21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,
                          25,25,25,25,25,30,30,30,30,30,36,36,36,36,36,36,42,42,
                      //  39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
                          42,42,42,42,49,49,49,49,49,49,49,56,56,56,56,56,56,56,
                      //  57,58,59,60,61,62,63,64
                          64,64,64,64,64,64,64,64
                        );
      $newOrder = $newOrders[$node->getAttribute('order')];
      $node->setAttribute('order',"$newOrder");
      // get a list of the existing quartering IDs
      $existingIDs = array();
      $children = $node->childNodes;
      foreach ($children as $subShield) {
        $child = $subShield->firstChild;
        if ($child != null && $child->nodeName != blazonML::E_PENDING) {
          $thisID = $subShield->getAttribute('ID');
          if ($thisID != null) $existingIDs[] = $thisID;
          // error_log("Noting existing ID $thisID");
        }
      }
      // Idiot check no quarterings given at all
      if (count($existingIDs) == 0) {
        $simple = $this->greyShield();
        $firstChild = $node->firstChild; // the shield element
        $theID = unique('N1-');
        $firstChild->setAttribute('ID', $theID);
        $existingIDs[] = $theID;
        $pending = $firstChild->firstChild;
        $firstChild->replaceChild($simple,$pending);
      }
      // start counting through the available IDs
      $currentID = 0;
      // Add any additional quarterings that are needed, as pending
      // (There may already been some pending nodes from the parser)
      for($i = $children->length; $i < $newOrder; $i++ ) {
        //error_log("Setting quarter $i to pending");
        $shield = $this->dom->createElement(blazonML::E_SHIELD);
        $shield->setAttribute('index',$i + 1);
        $shield->setAttribute('ID',unique('P1-'));
        $shield->appendChild($this->dom->createElement(blazonML::E_PENDING));
        $node->appendChild($shield);
      }
      // Now go through the children, replacing any pending nodes with references
      // to existing subshields
      foreach ($children as $subShield) {
        $child = $subShield->firstChild;
        if ($child != null && $child->nodeName == blazonML::E_PENDING) {
          //error_log("setting reference to " . $existingIDs[$currentID]);
          $subShield->setAttribute('IDREF', $existingIDs[$currentID]);
          if (++$currentID >= count($existingIDs)) $currentID = 0; 
          $subShield->removeChild($child);
        }
      }
    }

    // If there are lots of quarterings, force a squarish shape
    $nodeList = $this->xpath->query("//shield");
    if ($options['shape'] == 'default' && $nodeList->length > 8) $options['shape'] = 'french';

    return $this->dom;
  }
}
   
