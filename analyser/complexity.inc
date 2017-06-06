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

class complexity {
  private $dom;
  private $xpath;
  
  function __construct($xml) {
    if ( is_string($xml) ) {
      $this->dom = new DOMDocument;
      $this->dom->Loadxml($xml);  
    } elseif ( is_a($xml,'DOMDocument')) 
      $this->dom = $xml;
    else
      echo "internal -> cannot figure out xml data type for format";
      $this->xpath = new DOMXPath($this->dom);
  }


  public function report( $format = 'text', $doAction = false ) { // TODO implement actions
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
    $counts = array();
    $counts['Plain Fields'] = $this->xpath->evaluate('count(//simple/field/tincture/colour)'); 
    $counts['Two Colour Fields']  = $this->xpath->evaluate('count(//simple/field/tincture/fur | //simple/tincture/treatment | //simple/tincture/semyde)'); 

    $counts['Ordinaries'] = $this->xpath->evaluate('count(//objects/ordinary)'); 
    $counts['Divisions'] = $this->xpath->evaluate('count(//division)');
    $counts['One Charge'] = $this->xpath->evaluate('count(//charge[@number=1])');
    $counts['Two Charges'] = $this->xpath->evaluate('count(//charge[@number=2])');
    $counts['Three Charges'] = $this->xpath->evaluate('count(//charge[@number=3])');
    $counts['Many Charges'] = $this->xpath->evaluate('count(//charge[@number>3])');
    $counts['Line Types'] = $this->xpath->evaluate('count(//ordinary/modifier[type="linetype"] | //division/modifier[type="linetype"])' );
    $this->score = $counts['Plain Fields'] +
             $counts['Two Colour Fields'] * 2 +
             $counts['Ordinaries'] +
             $counts['Divisions'] * 2 +
             $counts['One Charge'] +
             $counts['Two Charges'] * 2 +
             $counts['Three Charges'] * 3 +
             $counts['Many Charges'] * 3 +
             $counts['Line Types'];
    if ( $format = 'html' )
      $output .= "<p>Complexity Score: <strong>$this->score</strong></p>\n";
    else

      $output .= "Complexity Score: $score\n";
    $output .= $blockStart;
    foreach ( $counts as $key => $count )
      $output .= $lineStart . "$key: $count" . $lineEnd;
    $output .= $blockEnd;
    return $output;
  }
  
}
   
?>
