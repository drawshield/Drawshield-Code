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

class linkAdder {
  private $dom;
  private $xpath;
  private $linkDB;
  
  function __construct($xml) {
    if ( is_string($xml) ) {
      $this->dom = new DOMDocument;
      $this->dom->Loadxml($xml);  
    } elseif ( is_a($xml,'DOMDocument')) 
      $this->dom = $xml;
    else {
      echo "internal -> cannot figure out xml data type for format";
      return;
    }
    $this->xpath = new DOMXPath($this->dom);
    include "linkdb.inc";  
  }

  public function addLinks($format = 'dom') { 
    $nodeList = $this->xpath->query('//*[@keyterm]'); // every node with a keyterm attribute
      for ( $i = 0; $i < $nodeList->length; $i++ ) {
        $keyterm = $nodeList->item($i)->getAttribute('keyterm');
        if ( array_key_exists ( $keyterm, $this->linkDB ))
          $nodeList->item($i)->setAttribute('link',$this->linkDB[$keyterm]);
    }     
    return $this->dom;
  }
  
}
   
?>
