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

abstract class grammar  {
  const ERROR_ORIGIN = 'parser';
  const PRIORWORDS = 6;

  /**
   * @var tokeniser $tokenList
   */
  protected $tokenList;
  /**
   * @var BlazonML $schema
   */
  protected $schema;
  /**
   * @var languageDB $patternDB
   */
  protected $patternDB;
  /**
   * @var matcher $phraseMatcher
   */
  protected $phraseMatcher;
  protected $messages = array();
  /**
   * @var persistentStore $stateStore
   */
  protected $stateStore;

  protected $shieldNode;
  protected $achievementNode = null;
  protected $notesNode = null;
  protected $messagesNode = null;
  
  protected function addInstruction($child) {
    if ($this->notesNode == null)
          $this->notesNode = $this->schema->makeNode(blazonML::E_INSTRUCTIONS);
    $this->notesNode->appendChild($child);
  }
 

  protected function addAchievement($child) {
    if ($this->achievementNode == null) {
      $this->achievementNode = $this->schema->makeNode(blazonML::E_ACHIEVEMENT);
    }
    $this->achievementNode->appendChild($child);
  }

  public function __construct( $language = 'english', BlazonML $schema) {
    switch ( $language ) {
      case 'english':
        include "english/lexicon.inc";
        break;
      default:
        echo 'Urgh! No language ' . $language;
        break;
    }
    $this->patternDB = new languageDB();
    $this->stateStore = new persistentStore();
    $this->schema = $schema;
  }

  /**
   * @return DOMElement
   */

  abstract function getShield();

  public function readGrammar ( tokeniser $tokenList ) {
    $this->phraseMatcher = new matcher($tokenList, $this->patternDB);
    $this->tokenList = $tokenList;

    $shield = $this->getShield();
    if ($shield == null)
      $shield = $this->schema->makeNode(blazonML::E_SHIELD, array ( blazonML::A_INDEX => '0' )); // TODO - add a field?
    $this->checkBadWords();
    return array ($shield, $this->achievementNode, $this->notesNode, $this->messagesNode);
  }

  // shorthand functions to make code read better
  protected function ignore ( $key ) { $this->phraseMatcher->searchMatch($key); }
  protected function find ( $key ) { return $this->phraseMatcher->searchMatch($key); }
  protected function tokens () { return $this->phraseMatcher->getMatchedTokens(); }
  protected function lineNo () { return $this->phraseMatcher->getLineNo(); }
  protected function lookAhead ( $offset ) { $this->phraseMatcher->lookahead( $offset ); }
  protected function moreInput() { return $this->tokenList->moreInput(); }
    
  // Interface implementation
  public function save() {
    return ( $this->stateStore->store($this->tokenList->cur_word) );
  }

  public function atWord() {
    return ( $this->tokenList->cur_word );
  }
  
  public function restore($state) {
    global $trace;
    
    $data = $this->stateStore->restore($state);
    if ( $trace ) echo "<p>Restore to $data</p>\n";
    if ($data !== null ) {
      $this->tokenList->cur_word = $data;
    } else {
      self::raiseError ( 'internal', 'No state to restore' );
    }
  }
  
  public function discard($state) {
    $this->stateStore->discard($state);
  }
  
  protected function raiseError ( $type, $message, $getOffset = false ) {
    if ($this->messagesNode == NULL) 
      $this->messagesNode = $this->schema->makeNode(blazonML::E_MESSAGES);
    $context = '';
    $lineNo = '';
    if ( $getOffset === true ) {
      $tokens = $this->tokenList->getTokens();
      $lineNos = $this->tokenList->getLineNos();
      $offset = $this->phraseMatcher->getTokenOffset();
      if ($offset >= count($tokens)) $offset = count($tokens) - 1;
      $start = $offset - self::PRIORWORDS;
         $start = $start < 0 ? 0 : $start; 
         for ( $i = $start; $i < $offset; $i++ ) 
           $context .= $tokens[$i] . ' ';
         $lineNo = " near line " . $lineNos[$offset];
    } elseif ( $getOffset !== false )
      $context = $getOffset;
    $messageNode = $this->schema->makeNode(blazonML::E_MESSAGE, array ( 
      blazonML::A_CATEGORY => 'blazon',
      blazonML::A_CONTEXT => $context,
      blazonML::A_LINERANGE => $lineNo,
    ));
    $messageText = $this->schema->createTextNode($message);
    $messageNode->appendChild($messageText);
    $this->messagesNode->appendChild($messageNode);
  }
  
  protected function clearErrors( $type = null ) {
    if (is_null($this->messagesNode) || !$this->messagesNode->hasChildNodes()) return;
    for ($i = 0; $i < $this->messagesNode->childNodes->length; $i++ ) {
      $message = $this->messagesNode->childNodes->item($i);
      if ($type == null || $message->getAttribute(blazonML::A_CATEGORY) == $type ) 
        $this->messagesNode->removeChild($message);
    }
  }

  public function checkBadWords () {
    // Was there any unknown input?
    if ( $this->tokenList->badWords ) {
      $allTokens = $this->tokenList->getTokens();
      for ($i = 0; $i < count($allTokens); $i++ ) {
        if ($allTokens[$i][0] == '[') break;
      }
      $start = max(1,$i - 5);
      self::raiseError('blazon',"Don't understand:", '...' . implode(' ', array_slice($allTokens, $start, 10)) . '...');
    }
  }
      
  protected function semicolon($lookback = true) {
  
    $retval = false;
    if ( self::find(languageDB::SEMICOLON) ) {
      $retval = true;
    } elseif ( $lookback ) {
      $state = self::save();
      if ( self::lookAhead(-1) && self::find(languageDB::SEMICOLON) != null ) $retval = true;
      self::restore($state);
    } 
    return $retval;
  }
  
  
  protected function comma() {
    
    // Consume a comma (or similar) character if present, and return true
    // But also return true if the *previous* character was a comma
    if ( self::find(languageDB::COMMA) ) return true;
    $state = self::save();
    if ( self::lookAhead(-1) && self::find(languageDB::COMMA) ) { self::discard($state); return true; }
    self::restore($state); 
    return false;
  }
  
  protected function restoreAnd() {
    // If the last word was "and", put it back again
    $state = self::save();
    self::lookAhead(-1);
    if ($keyterm = self::find(languageDB::ANDD)) {
      self::lookAhead(-1);
      self::discard($state);
    } else
      self::restore($state);
  }
}

