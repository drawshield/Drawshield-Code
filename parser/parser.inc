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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */

//include "utilities.inc";
require_once "tokeniser.inc";
require_once "matcher.inc";
// abstract classes for the language data
require_once "lexicon.inc";
require_once "grammar.inc";
   
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

  public function __construct( $language = 'english' ) {
    $this->xmlMaker = new blazonML('1.0',"utf-8");
    switch ( $language ) {
      case 'english':
        include_once "english/grammar.inc";
        $this->grammar = new englishGrammar( $this->xmlMaker, $language);
        break;
      default:
        echo 'Wah! No language found in parser';
        break;
    }
  }
  
  public function parse($blazon, $format = 'xml' ) {
    global $version, $options;
    // set things up
    $tokeniser = new Tokeniser();
    $tokenList = $tokeniser->tokenise($blazon);
    if ($options["stage"] == "tokens") {
        echo $blazon . "\n";
        echo "         11111111112222222222333333333344444444445555555555666666666677777777778888888888\n";
        echo "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n";
        foreach($tokenList as $token) {
            echo $token . "\n";
        }
        exit(0);
    }
    $topNode = $this->xmlMaker->createElementNS('http://drawshield.net/blazonML',blazonML::E_BLAZON);
    $topNode->setAttribute("creatorName", $version['website']);
    $topNode->setAttribute("creatorVersion", $version['release']);
    $topNode->setAttribute("timestamp", time());
    /* $inputNode = $this->xmlMaker->createElement(blazonML::E_INPUT);
    $inputNode->setAttribute("blazonText",$blazon);
     for($i = 0; $i < count($tokenList); $i++) {
      $tokenNode = $this->xmlMaker->createElement(blazonML::E_TOKEN);
      $tokenNode->setAttribute(blazonML::A_INDEX,$i);
      $tokenNode->appendChild($this->xmlMaker->createTextNode($tokenList[$i]->text));
      $inputNode->appendChild($tokenNode);
    }
    $topNode->appendChild($inputNode); */
    $this->xmlMaker->appendChild($topNode);

    // Do the parsing
    $nodes = $this->grammar->getAST($tokenList);
    foreach ($nodes as $node) {
        if ($node != NULL) {
            if ($node->nodeName == blazonML::E_INPUT) {
                $node->setAttribute("blazonText", $blazon);
            }
            $topNode->appendChild($node);
        }
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


