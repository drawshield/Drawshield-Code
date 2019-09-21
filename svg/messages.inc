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

class messageStore {
  private $messages;
  private $dom;

  public function __construct ($dom) {
    $this->dom = $dom;
    // global $xpath;
    $this->messages = array();

    // foreach ( $xpath->query("//messages/message") as $item )
    //   self::addMessage('blazon',$item->textContent);
  }

  public function addMessage( $category, $message ) {

  if (  !array_key_exists($category, $this->messages)) $this->messages[$category] = array();
	foreach ( $this->messages[$category] as $item )
	  if ( strcmp ( $item, $message ) === 0)
		  return; // ignore duplicate messages

  $this->messages[$category][] = $message;
  }

  // public function getMessages( $category ) {
  //   if ( array_key_exists($category,$this->messages)) {
  //     return $this->messages[$category];
  //   } else {
  //     return array();
  //   }
  // }

  public function insertMessages () {
    global $xpath;

    $messageParent = ($xpath->query('//messages'))->item(0);
    foreach ($this->messages as $category => $messageSet) {
      foreach ($messageSet as $message) {
        $messageNode = $this->dom->createElement(blazonML::E_MESSAGE);
        $messageNode->setAttribute('category', $category);
        $messageText = $this->dom->createTextNode($message);
        $messageNode->appendChild($messageText);
        $messageParent->appendChild($messageNode);
      }
    }
  }
}

