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

  public function getMessageArray() {
      return $this->messages;
  }

  public function insertMessages () {
    global $xpath;

    $messageParent = ($xpath->query('//messages'))->item(0);
    if ($messageParent == null) return; // or should we create it?
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

  public function getCredits($format = 'html') {
      $retval = '';
      foreach($this->messages['licence'] as $message) {
         $retval .= "<p>" . $message . "</p>\n";
      }
      return $retval;
  }
}

