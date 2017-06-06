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

class obsolete {
  private $dom;
  private $xpath;
  private $obsList = array (
    // xpath,   comment,   replacement action
    array ( '//charge[@keyterm="geometric/gastell"]', 'Gastell: use of roundel is preferred', 'keyterm=geometric/roundel' ),
    array ( '//division[@keyterm="enty"]', 'Enty: per chevron is preferred', 'keyterm=per-chevron' ),
    array ( '//colour[@keyterm="amethyst"]', 'Amethyst: purpure is preferred', 'keyterm=purpure' ),
    array ( '//colour[@keyterm="diamond"]', 'Diamond: sable is preferred', 'keyterm=sable' ),
    array ( '//colour[@keyterm="dragons head"]', 'Dragon Head: tenne is preferred', 'keyterm=tenne' ),
    array ( '//colour[@keyterm="dragons tail"]', 'Dragon Tail: sanguine is preferred', 'keyterm=sanguine' ),
    array ( '//colour[@keyterm="emerald"]', 'Emerald: vert is preferred', 'keyterm=vert' ),
    array ( '//colour[@keyterm="venus"]', 'Venus: vert is preferred', 'keyterm=vert' ),
    array ( '//colour[@keyterm="jacinth"]', 'Jacinth: tenne is preferred', 'keyterm=tenne' ),
    array ( '//colour[@keyterm="jupiter"]', 'Jupiter: azure is preferred', 'keyterm=azure' ),
    array ( '//colour[@keyterm="luna"]', 'Luna: argent is preferred', 'keyterm=argent' ),
    array ( '//colour[@keyterm="mars"]', 'Mars: gules is preferred', 'keyterm=gules' ),
    array ( '//colour[@keyterm="mercury"]', 'Mercury: purpure is preferred', 'keyterm=purpure' ),
    array ( '//colour[@keyterm="pearl"]', 'Pearl: argent is preferred', 'keyterm=argent' ),
    array ( '//colour[@keyterm="ruby"]', 'Ruby: gules is preferred', 'keyterm=gules' ),
    array ( '//colour[@keyterm="sapphire"]', 'Sapphire: azure is preferred', 'keyterm=azure' ),
    array ( '//colour[@keyterm="sardonyx"]', 'Sardonyx: sanguine is preferred', 'keyterm=sanguine' ),
    array ( '//colour[@keyterm="sol"]', 'Sol: or is preferred', 'keyterm=or' ),
    array ( '//colour[@keyterm="topaz"]', 'Topaz: or is preferred', 'keyterm=or' ),
  );
  
  function __construct($xml) {
    if ( is_string($xml) ) {
      $this->dom = new DOMDocument;
      // We don't want to bother with white spaces
      $this->dom->preserveWhiteSpace = false;
      $this->dom->Loadxml($xml);  
    } elseif ( is_a($xml,'DOMDocument')) 
      $this->dom = $xml;
    else
      echo "internal -> cannot figure out xml data type for format";
      $this->xpath = new DOMXPath($this->dom);
  }


  public function report( $format = 'text', $doAction = false ) { // TODO implement actions and HTML
    switch ( $format ) {
    case 'html':
      $blockStart = '<ul>' . PHP_EOL;
      $blockEnd = '</ul>' . PHP_EOL;
      $lineStart = '<li>';
      $lineEnd = '</li>' . PHP_EOL;
      break;
    case 'text':
    default:
      $blockStart = '';
      $blockEnd = '';
      $lineStart = '';
      $lineEnd = PHP_EOL;
    }
    $output = '';
    foreach ( $this->obsList as $obs ) {
      $nodeList = $this->xpath->query($obs[0]);
       if ($node = $nodeList->item(0))

        {
        $output .= $lineStart . $node->nodeName . ' (' . $node->getAttribute('ID') . ') ' . $obs[1] . $lineEnd;
      }
    }
    return $output;
  }
}
   
?>
