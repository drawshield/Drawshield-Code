<?php

//include "utilities.inc";
include "tokeniser.inc";
include "matcher.inc";
// abstract classes for the language data
include "lexicon.inc";
include_once "grammar.inc";
   
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
    $this->xmlMaker = new blazonML('1.0',"utf-8");
    switch ( $language ) {
      case 'english':
        include_once "english/grammar.inc";
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
    // $topNode = $this->xmlMaker->makeNode(blazonML::E_BLAZON, 
    //     array ( "blazonText" => $blazon,
    //             "blazonTokens" => (implode(' ', $this->tokenList->getTokens())),
    //             "creatorName" => $version['website'],
    //             "creatorVersion" => $version['release'],
    //             "timestamp" => time(),
    //           )
    //   );
    $topNode = $this->xmlMaker->createElementNS('http://drawshield.net/blazonML',blazonML::E_BLAZON);
    // $topNode->setAttribute("blazonText",$blazon);
    // $topNode->setAttribute("blazonTokens", (implode(' ', $this->tokenList->getTokens())));
    $topNode->setAttribute("creatorName", $version['website']);
    $topNode->setAttribute("creatorVersion", $version['release']);
    $topNode->setAttribute("timestamp", time());
    $inputNode = $this->xmlMaker->createElement(blazonML::E_INPUT);
    $inputNode->setAttribute("blazonText",$blazon);
    $tokens = $this->tokenList->getTokens();
    for($i = 0; $i < count($tokens); $i++) {
      $tokenNode = $this->xmlMaker->createElement(blazonML::E_TOKEN);
      $tokenNode->setAttribute(blazonML::A_INDEX,$i);
      $tokenNode->appendChild($this->xmlMaker->createTextNode($tokens[$i]));
      $inputNode->appendChild($tokenNode);
    }
    $topNode->appendChild($inputNode);
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


