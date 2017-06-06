<?php

include "utilities.inc";
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
    // set things up
    $this->tokenList = new tokeniser($blazon);
    $topNode = $this->xmlMaker->makeNode(blazonML::E_BLAZON, array ( "blazonText" => $blazon));
    $this->xmlMaker->appendChild($topNode);

    // Do the parsing
    $node = $this->grammar->readGrammar($this->tokenList);
    if ( $node != null )
      $topNode->appendChild($node);
    
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


