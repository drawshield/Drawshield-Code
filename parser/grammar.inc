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

abstract class grammar
{
    const ERROR_ORIGIN = 'parser';
    const PRIORWORDS = 6;

    /**
     * @var BlazonML $schema
     */
    protected $schema;
    /**
     * @var languageDB $patternDB
     */
    protected $patternDB;
    /**
     * @var matcher $matcher
     */
    protected $matcher;
    protected $messages = array();

    protected $foundTinctures = [];
    protected $foundColours = [];

    protected $shieldNode;
    // protected $achievementNode = null;
    // protected $notesNode = null;
    protected $messagesNode = null;

    public function __construct(BlazonML $schema, $language = 'english')
    {
        switch ($language) {
            case 'english':
                include_once "english/lexicon.inc";
                break;
            default:
                echo 'Urgh! No language ' . $language;
                break;
        }
        $this->patternDB = new languageDB();
        $this->schema = $schema;
    }

    /**
     * @return DOMElement
     */

    abstract function getShield();
    abstract function getRoll();


    public function getAST($tokenList)
    {
        $this->matcher = new matcher($tokenList, $this->patternDB);

        $shield = $this->getRoll();
        if (is_null($shield)) $shield = $this->getShield();
        foreach ($this->matcher->getMatcherErrors() as $matchError) {
            $this->raiseError("blazon", $matchError);
        }
        $inputNode = $this->schema->makeNode(blazonML::E_INPUT);
        foreach ($this->matcher->listLexemes() as $lexText) {
              $tokenNode = $this->schema->makeNode(blazonML::E_TOKEN);
              $tokenNode->appendChild($this->schema->makeTextNode($lexText));
              $inputNode->appendChild($tokenNode);;
        }
        if ($shield == null)
            $shield = $this->schema->makeIDNode(blazonML::E_SHIELD, array(blazonML::A_INDEX => '0')); // TODO - add a field?
        return array($shield, $inputNode, $this->messagesNode);
    }

    // shorthand functions to make code read better

    protected function findLex($key)
    {
        return $this->matcher->matchLexeme($key);
    }

    protected function tokens()
    {
        return $this->matcher->getMatchedTokens();
    }

    protected function lineNo()
    {
        return $this->matcher->getLineNo();
    }

    protected function lookBack($category)
    {
        return $this->matcher->prevMatch($category);
    }

    protected function moreInput()
    {
        return $this->matcher->moreInput();
    }

    protected function newPhrase()
    {
        return new Phrase($this->matcher->getState());
    }

    protected function discardPhrase($phrase)
    {
        $this->matcher->restoreState($phrase->getState());
    }

    // Interface implementation
    public function save()
    {
        return ($this->matcher->save());
    }

    public function restore($state)
    {
        $this->matcher->restore($state);
    }

    public function discard($state)
    {
        $this->matcher->discard($state);
    }

    protected function raiseError($type, $message, $context = null)
    {
        if ($this->messagesNode == NULL)
            $this->messagesNode = $this->schema->makeNode(blazonML::E_MESSAGES);
        $lineNo = '';
        $tokens = '';
        // TODO temporary, remove this once all raiseError updated
        if (is_bool($context)) $context = null;
        if (!is_null($context)) {
            switch (get_class($context)) {
                case 'Lexeme':
                    $lineNo = " at " . $context->startPos . ':' . $context->endPos;
                    $tokens = $context->tokens;
                    break;
                case 'DomNode':
                    $tokens = $context->getAttribute(blazonML::A_TOKENS);
                    $lineNo = $context->getAttribute(blazonML::A_LINENUMBER);
                    break;
            }
        }
        $messageNode = $this->schema->makeNode(blazonML::E_MESSAGE, array(
            blazonML::A_CATEGORY => $type,
            blazonML::A_CONTEXT => $tokens,
            blazonML::A_LINERANGE => $lineNo,
        ));
        $messageText = $this->schema->createTextNode($message);
        $messageNode->appendChild($messageText);
        $this->messagesNode->appendChild($messageNode);
    }

    protected function semicolon($lookback = false)
    {

        $retval = false;
        if (self::findLex(languageDB::SEMICOLON)) {
            $retval = true;
        } elseif ($lookback) {
            if (self::lookBack(languageDB::SEMICOLON)) $retval = true;
        }
        return $retval;
    }

    protected function restoreAnd()
    {
        $this->matcher->putBack(languageDB::ANDD);
    }
}

