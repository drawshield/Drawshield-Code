<?php

//include "utilities.inc";
include "tokeniser.inc";
include "matcher.inc";
// abstract classes for the language data
include "lexicon.inc";
include "grammar.inc";
   
class parser {
  protected $lexer;
  /**
   * @var blazonML $xmlMaker
   */
  protected $xmlMaker;
  /**
   * @var englishGrammar $grammar
   */
  protected $grammar;
  protected $tokenList;

  public function __construct( $language = 'english' ) {
    $this->xmlMaker = new blazonML('1.0');
    switch ( $language ) {
      case 'english':
        include "english/grammar.inc";
        $this->grammar = new englishGrammar($language, $this->xmlMaker);
        break;
      default:
        echo 'Wah! No language found in parser';
        break;
    }
  }
  
  public function parse($blazon, $format = 'xml' ) {
    global $version;
    // set things up
    $this->tokenList = new tokeniser($blazon);
    $topNode = $this->xmlMaker->makeNode(blazonML::E_BLAZON, 
        array ( "blazonText" => $blazon,
                "blazonTokens" => (implode(' ', $this->tokenList->getTokens())),
                "creatorName" => $version['website'],
                "creatorVersion" => $version['release'],
                "timestamp" => time(),
              )
      );
    $this->xmlMaker->appendChild($topNode);

    // Do the parsing
    $nodes = $this->grammar->readGrammar($this->tokenList);
    foreach ($nodes as $node) {
      if ($node != NULL) $topNode->appendChild($node);
    }
    
    // Return the result
    switch ( $format ) {
      case 'dom':
        return $this->xmlMaker;
        break;
      case 'xml':
      default:
        return $this->xmlMaker->saveXML();
        break;
      }
    }
}


