<?php 

class englishGrammar extends grammar {

  /**
   * @return DOMElement
   */
  public function getShield() {

    self::clearErrors(); // in case we are called multiple times
    $this->stateStore->flush();
    $this->phraseMatcher->getMatchedTokens(true);
    $count = 0;
    $node = null;
    $stackedInput = null; // temporary store to try out ignoring words
    while ( true ) {
      if ( $count++ > $this->tokenList->num_tokens ) {
        // echo "<p>loop error</p>\n";
        break;
      }
      // Try to find complete shield
      /**
       * @var DOMElement $node
       */
      self::drawn(); // allow shapes etc. to be given first, if required
      self::semicolon();
      self::comma();
      $node = self::shield();
      self::semicolon();
      if ( ($overall = self::overall(true)) != null )
        $node->appendChild($overall);
      do {
        $found = false;
        if (($mantle = self::mantling()) != null) {
          $found = true;
          $this->addAchievement($mantle);
        } elseif (($motto = self::motto()) != null) {
          $found = true;
          $this->addAchievement($motto);
        } elseif (($crest = self::crest()) != null) {
          $found = true;
          $this->addAchievement($crest);
        } elseif (($torse = self::torse()) != null) {
          $found = true;
          $this->addAchievement($torse);
        } elseif (($helmet = self::helmet()) != null) {
          $found = true;
          $this->addAchievement($helmet);
        } elseif (self::find(languageDB::ACHIEVEMENT)) {
          self::ignore(languageDB::WORD_OF);
          $this->addAchievement(null);
          $found = true;
        } elseif (self::drawn()) {
          $found = true;
        }
        if ($found) {
          self::ignore(languageDB::ANDD);
          self::ignore(languageDB::WITH);
        }
      } while ($found);
      // If last word (or only) is ignored, can't go further
      $lastEntry = $this->tokenList->num_tokens-1;
      if ( $lastEntry < 0 || $this->tokenList->words[$lastEntry]{0} == '[')
        break;
      // If input still left
      if ( $this->tokenList->cur_word < $this->tokenList->num_tokens ) {
        $node = null; // discard existing (destroy objects)
        // self::clearErrors(self::ERROR_ORIGIN);
        self::clearErrors(); // clear all errors, as we backtrack from the very start
        $this->phraseMatcher->getMatchedTokens(true);
        if ( $stackedInput == null ) { // This is our first time with this error
          // Try ignoring previous word
          $stackedInput = $this->phraseMatcher->furthestWord  - 1;
          $this->tokenList->ignoreWord($stackedInput);
          $this->tokenList->resetToken();
        } else { // 2nd time around, try ignoring current word
          $this->tokenList->restoreWord($stackedInput);
          $this->tokenList->ignoreWord($this->phraseMatcher->furthestWord);
          $this->tokenList->resetToken();
          $stackedInput = null;
        }
      }
      else {
        break; // All input consumed, success!
      }
    }
    return $node;
  }

  /**
   * @return DOMElement
   */
  protected function overall($meta = false) {
    /**
     * @var DOMElement $node
     */
    $node = null;
    if ( $meta )
      $lookFor = languageDB::METAOVERALL;
    else
      $lookFor = languageDB::OVERALL;

    while (self::moreInput() and self::find($lookFor) != null) {
      if ($node == null) $node = $this->schema->makeNode(blazonML::E_OVERALL, array ( 'keyterm' => 'overall' ), self::tokens(), self::lineNo());
      if (($over = self::objects()) == null) {
        self::raiseError(self::ERROR_ORIGIN, 'Nothing to go "overall"', true);
        return null;
      }
      $node->appendChild($over);
    }
    return $node;
  }

  protected function drawn() {

    $found = false;
    while ($option = self::find(LanguageDB::DRAWN_MODS)) {
      $found = true;
      $elementName = null;
      $value = strtolower($this->phraseMatcher->getValue());
      switch ($option) {
        case 'scheme':
          $elementName = blazonML::E_PALETTE;
          break;
        case 'style':
          $elementName = blazonML::E_EFFECT;
          break;
        case 'shape':
          $elementName = blazonML::E_SHAPE;          
          break;
        case 'aspect':
          // $value = calculateAR($value);
          $elementName = blazonML::E_ASPECT;
          break;
        default:
          break;
      }
      if (!is_null($elementName)) {
        $node = $this->schema->makeNode($elementName, array ( 
          blazonML::A_KEYTERM => $value ),
          self::tokens(),
          self::lineNo()
        );
        $this->addInstruction($node);
      }
      self::ignore(LanguageDB::ANDD);
    }
    return $found;
  }

  /**
   * @return DOMElement
   */
  protected function simple($endOnSemi = false) {
    $state = self::save();
    self::ignore(languageDB::ONFIELDOF);

    /**
     * @var DOMElement $node
     */
    $node = null;
    if ( ($tinc = self::tincture() ) != null ) {
      $field = $this->schema->makeNode(blazonML::E_FIELD);
      $simple = $this->schema->makeNode(blazonML::E_PLAIN);
      self::comma();
      $field->appendChild ( $tinc );
      $simple->appendChild ( $field );
      if (!($endOnSemi && self::semicolon())) {
        if ( ($ordChgs = self::objects())) {
          $simple->appendChild($ordChgs);
        }
      }
      $node = $this->schema->makeNode(blazonML::E_SHIELD, array ( blazonML::A_INDEX => '0' ));
      $node->appendChild($simple);

    }
    if ( $node == null )
      self::restore($state);
    else
      self::discard($state);
    return $node;
  }

  /**
   * @return DOMElement
   */
  protected function shield() {

    /**
     * @var DOMElement $node1 $node2
     */
    $checkOverAllParts = false;
    $node1 = null;
    //$node1 = builtin(); // TODO re-instate built in database
    if ( $node1 == null ) $node1 = self::quartered();
    if ( $node1 != null ) { // found a quartering, check for over all the parts
      $checkOverAllParts = true;
    }
    if ( $node1 == null ) $node1 = self::halved();
    if ( $node1 == null ) $node1 = self::simple();
    if ( $node1 == null ) {
      $node1 = $this->schema->makeNode(blazonML::E_SHIELD);
      $node1->appendChild($this->schema->makeNode(blazonML::E_MISSING));
      self::raiseError(self::ERROR_ORIGIN, 'Expected a shield description (replaced by blank)', true);
    }
    if ($checkOverAllParts && ($overall = self::overall(true)) != null) {
      $node1->appendChild($overall);
    } elseif ( self::moreInput() and !self::semicolon() and ($overall = self::overall()) != null )
      $node1->appendChild($overall);
    $retval = $node1;

    // Now do it all again, in case of "A impaled with B"
    if ( self::moreInput() and ($keyterm = self::find(languageDB::SPLITS)) != null ) {
      $node2 = null;
      // $node2 = builtin();
      if ( $node2 == null ) $node2 = self::quartered();
      if ( $node2 == null ) $node2 = self::halved();
      if ( $node2 == null ) $node2 = self::simple();
      if ( $node2 == null ) {
        $node2 = $this->schema->makeNode(blazonML::E_SHIELD);
        $node2->appendChild($this->schema->makeNode(blazonML::E_MISSING));
        self::raiseError(self::ERROR_ORIGIN, 'Expected a shield description (replaced by blank)', true);
      }
      if ( !self::semicolon() and ($overall = self::overall()) != null )
        $node2->appendChild($overall);
      // Got two nodes, now create a mother node
      $motherNode = $this->schema->makeNode(blazonML::E_SHIELD, array ( blazonML::A_INDEX => '0' ));
      $daughterNode = $this->schema->makeNode(blazonML::E_COMPLEX, array( 'keyterm' => $keyterm), self::tokens(), self::lineNo());
      $node1->setAttribute(blazonML::A_INDEX,'1');
      $node2->setAttribute(blazonML::A_INDEX,'2');
      $daughterNode->appendChild($node1);
      $daughterNode->appendChild($node2);
      $motherNode->appendChild($daughterNode);
      $retval = $motherNode;
    }
    if ( ($overall = self::overall()) != null )
      $retval->appendChild($overall);
    return $retval;
  }

  protected function getPositions() {
    // TODO improve mapping of tokens to positions, how about having each
    // keyterm as a key to the actual tokens, the use array_has_key
    // instead of in_array and build tokens as we go along?
    $positions = array();
    while ( true ) {
      $specifiers = array();
      $locations = array();
      $state = self::save();

      self::ignore(languageDB::ANDD);
      if ( !self::find(languageDB::WORD_IN) ) {
        self::discard($state);
        break;
      }
      self::ignore(languageDB::WORD_THE);
      while ( $keyterm = self::find(languageDB::SPECIFIER) ) {
        $specifiers[] = $keyterm;
        self::comma();
        self::ignore(languageDB::ANDD);
      }
      while ( $keyterm = self::find(languageDB::LOCATION) ) {
        $locations[] = $keyterm;
        self::comma();
        self::ignore(languageDB::ANDD);
      }
      if ( $quantifier = self::find(languageDB::QUANTIFIER) ) {
        self::comma();
        self::ignore(languageDB::ANDD);
      }
      // Did we find anything?
      if ( count($locations) == 0 and count($specifiers) == 0 and $quantifier == null ) {
        self::restore($state);
        return null;
      }
      self::discard($state);
      self::restoreAnd();
      // Yes, now try to combine them into positions
      if ($quantifier == 'quarter' ) { // one or more specific quarters
        $foundPos = false;
        foreach ( $specifiers as $specifier ) {
          $newPosition = null;
          switch ( $specifier ) {
            case 'first': $newPosition = 'inQ1'; break;
            case 'second': $newPosition = 'inQ2'; break;
            case 'third': $newPosition = 'inQ3'; break;
            case 'fourth': $newPosition = 'inQ4'; break;
            case 'each': if (count($locations)==0) $newPosition = 'ineachQ'; break;
            case 'dexter': $newPosition = 'ineachQ13'; break;
            case 'sinister': $newPosition = 'ineachQ24'; break;
            default:
          }
          if ( $newPosition != null ) {
            $positions[] = $newPosition;
            $foundPos = true;
          }
        }
        if ( !$foundPos ) { // do we also have a chief or base location?
          if ( in_array ( 'chief', $locations ) )
            $positions[] = 'ineachQ12';
          elseif ( in_array ( 'base', $locations) )
            $positions[] = 'ineachQ34';
          else
            self::raiseError(self::ERROR_ORIGIN,'Cannot find quarter for position',true);
        }
      } elseif ( count($locations) > 0 ){ // no quantifier found (or it was "all"), but we have a location
        foreach ( $locations as $location ) {
          switch ( $location ) {
            //case 'flank': $positions[] = 'inflank'; break;
            case 'fesspoint': $positions[] = 'infesspoint'; break;
            case 'nombril': $positions[] = 'innombril'; break;
            case 'abyss': $positions[] = 'inabyss'; break;
            case 'honpoint': $positions[] = 'inhonpoint'; break;
            case 'side':
              if ( in_array ('dexter',$specifiers) ) $positions[] = 'indexside';
              if ( in_array ('sinister',$specifiers) ) $positions[] = 'insinside';
              if ( in_array ('upper',$specifiers) ) $positions[] = 'inupperhalf';
              if ( in_array ('lower',$specifiers) ) $positions[] = 'inlowerhalf';
              break;
            case 'flank':
            case 'chief':
            case 'base':
              $count = count($positions);
              if ( in_array ('dexter',$specifiers) ) $positions[] = 'in' . 'dex' . $location;
              if ( in_array ('sinister',$specifiers) ) $positions[] = 'in' . 'sin' . $location;
              if ( in_array ('middle',$specifiers) ) $positions[] = 'in' . 'mid' . $location;
              if ( $count == count($positions) ) // didn't find any specs
                $positions[] = 'in' . $location;
              break;
          }
        }
      } else { // no quantifier or location, need to have "each"
        if ( in_array( 'each', $specifiers) ) {
          if ( in_array( 'first', $specifiers ) )
            $positions[] = 'ineach1st';
          elseif ( in_array( 'second', $specifiers) )
            $positions[] = 'ineach2nd';
          else
            $positions[] = 'ineach';
        } else {
          self::raiseError(self::ERROR_ORIGIN,'Do not understand position',true);
        }
      }
    }
    if ( count($positions) > 0 ) {
      $posArray = array();
      foreach ( $positions as $position )
        $posArray[] = $this->schema->makeMod(languageDB::POSITION,null,$position,self::tokens(), self::lineNo());
      return $posArray;
    } else
      return null;
  }

  protected function getCrowns ( $foundTinc ) {
    $crownType = 'default'; // default crown
    $modifierType = '';
    $state = self::save();
    while ( $crown = self::find(languageDB::CROWN_MODS) ) {
      switch ( $crown ) {
        case 'crown': // just ignore (as in gorged with a naval crown
          break;
        case 'crowned':
        case 'gorged':
          $modifierType = $crown;
          break;
        default:
          $crownType = $crown;
          break;
      }
    }
    if ($modifierType == '') {
      self::restore($state);
      return null;
    }
    $modifier =  $this->schema->makeMod(languageDB::CROWN_MODS,$crownType,$modifierType,self::tokens(), self::lineNo());
    //$modifier = createModifierNode(languageDB::CROWN_MODS,$modifierType,$crownType);
    if ( $foundTinc and ($tinc = self::tincture() ))
      $modifier->appendChild($tinc);
    return $modifier;
  }

  protected function simpleCharge( $assumeNum = false, $allowArr = false, $allowPos = false ) {
    $state = self::save();
    $mods = array();
    // Look for a number
    $chgNum = self::find(languageDB::QUANTITY);
    if ( $chgNum === null ) $chgNum = self::find(languageDB::NUMBER);
    if ( $chgNum === null ) {
      if ( $assumeNum )
        $chgNum = 1;
      else {
        self::restore($state);
        return null;
      }
    }
    $charge = $this->schema->makeNode(blazonML::E_CHARGE, array (blazonML::A_NUMBER => "$chgNum"),self::tokens(), self::lineNo());
    // Look for words indicating a multiple charge (e.g. "2 bundles of arrows")
    if ( $keyterm = self::find(languageDB::BUNDLE) ) {
      // Is the number made explicit?
      $num = self::find(languageDB::NUMBER);
      if (!$num) $num = ($keyterm == 'pair')?'2':'3';
      $mods[] = $this->schema->makeMod(languageDB::BUNDLE,$num,$keyterm,self::tokens(), self::lineNo());
    }
    // Look for prefixes, (e.g. "a demi-lion")
    while ( self::moreInput() and ($keyterm = self::find(languageDB::CHARGE_PREFIX)) )
      $mods[] = $this->schema->makeMod(languageDB::CHARGE_MODS,null,$keyterm,self::tokens(), self::lineNo());
    // Look for ordinaries that we might confuse with charges
    if ( self::find(languageDB::ORDINARY_NOT_CHARGE) ) {
      self::restore($state);
      return null;
    }
    // Now look for actual charges
    // TODO in herald knowledge replace special/same
    if( ($keyterm = self::find(languageDB::CHARGE_NOT_ORDINARY)) or (($keyterm = self::find(languageDB::ORDINARY_OR_CHARGE)) and $chgNum > 1) or ($keyterm = self::find(languageDB::CHARGE)) ) {
      // don't care if no flags
      list($keyterm, $flags) = explode('?', $keyterm . '?');
      $charge->setAttribute(blazonML::A_KEYTERM,$keyterm);
      $charge->setAttribute(blazonML::A_TOKENS, $charge->getAttribute(blazonML::A_TOKENS) . ' ' . self::tokens() );
      $charge->setAttribute(blazonML::A_LINENUMBER, self::lineNo() );
    } else { // Nothing found
      self::restore($state);
      return null;
    }
    // Set any flags
    $always_proper = strpos($flags,'P') !== false;
    // In some cases we want the next input token as a "value" (e.g. "the word Karl" matches "words? *")
    if ( $asterisk = $this->phraseMatcher->getValue() )
      $mods[] = $this->schema->makeMod('value',$asterisk,'value',self::tokens(), self::lineNo());
    if ($keyterm == 'special/escutcheon') {
      $foundTinc = true; // don't look for a colour, let the shield take it
      // $mods[] = self::simple(true);
      $mods[] = self::shield();
    }
    // Look for bundle again (e.g. "two arrow bundles"
    if ( $keyterm = self::find(languageDB::BUNDLE) )
    $mods[] = $this->schema->makeMod(languageDB::BUNDLE,null,$keyterm,self::tokens(), self::lineNo());
    // Now look for postfix stuff, including tinctures and arrangements
    $foundTinc = false;
    while ( self::moreInput() ) {
      if ( self::semicolon()) {
        self::lookAhead(-1);
        break;
      } 
      $state2 = self::save();
      self::comma();
      self::ignore(languageDB::ANDD);
      self::ignore(languageDB::WITH);
      self::ignore(languageDB::GROUP);
      if ( $keyterm = self::find(languageDB::CHARGE_MODS) )
        $mods[] = $this->schema->makeMod(languageDB::CHARGE_MODS,$this->phraseMatcher->getValue(),$keyterm,self::tokens(), self::lineNo());
      elseif ( $crowns = self::getCrowns($foundTinc) )
        $mods[] = $crowns;
      elseif ( $allowPos and $positions = self::getPositions() )
        $mods = array_merge( $mods, $positions );
      elseif ( $allowArr and $keyterm = self::find(languageDB::ARRANGEMENT) )
        $mods[] = $this->schema->makeMod(languageDB::ARRANGEMENT,null,$keyterm,self::tokens(), self::lineNo());
      elseif ( $num = self::find(languageDB::NUMBER) ) {
        // This might be rows, but must be >1 number 
        self::comma();
        self::ignore(languageDB::ANDD);
        $rows = array($num);
        $count = 1;
        while ( ($num = self::find(languageDB::NUMBER)) ) {
          $count += 1;
          $rows[] = $num;
          if ( self::semicolon()) break;
          self::comma();
          self::ignore(languageDB::ANDD);
        }
        if ( $count == 1 ) {
          self::restore($state2);
          break;
        } elseif ( $count > 1 ) {
          $mods[] = $this->schema->makeMod( 'rows', implode(',',$rows), 'arrange-by-rows', self::tokens(), self::lineNo() );
          self::discard($state2);
        }
      } elseif ( !$foundTinc && ($tinc = self::tincture()) ) {
        if ($always_proper && $tinc->firstChild->nodeName != 'proper') {
          self::raiseError(self::ERROR_ORIGIN,'Charge is always proper',true);
        } else {
          $mods[] = $tinc;
          $foundTinc = true;
        }
      } elseif ( $keyterm = self::find(languageDB::CHARGE_FEATURES) ) {
        $feature = $this->schema->makeMod(blazonML::V_FEATURE,null,$keyterm,self::tokens(), self::lineNo());
        if ( ($foundTinc || $always_proper) && ($tinc = self::tincture() ))
          $feature->appendChild($tinc);
        else {
          $feature->appendChild($this->schema->makeNode(blazonML::E_TINCTURE, array(blazonML::A_ORIGIN => 'pending',
              blazonML::A_INDEX => '1')));
        }
        $mods[] = $feature;
        // Look for non-standard adjusments
      } elseif ($adjustments = self::getAdjustments()) {
        $mods = array_merge($mods, $adjustments);
      } elseif ($keyterm = self::find(languageDB::FONT)) {
        $mods[] = $this->schema->makeMod(languageDB::FONT,$this->phraseMatcher->getValue(),$keyterm,self::tokens(), self::lineNo());
      } else {
        self::restore($state2);
        break;
      }
    }

    self::discard($state);
    if ( $always_proper ) {
      if (!$foundTinc) {
        $tinc = $this->schema->makeNode(blazonML::E_TINCTURE, array ( blazonML::A_ORIGIN => 'given',
          blazonML::A_INDEX => '1') );
        $tinc->appendChild($this->schema->makeNode(blazonML::E_PROPER));
        $mods[] = $tinc;
      }
    } elseif ( !$foundTinc ) {
      $mods[] = $this->schema->makeNode(blazonML::E_TINCTURE, array ( blazonML::A_ORIGIN => 'pending',
          blazonML::A_INDEX => '1') );
    }
    foreach ( $mods as $mod )
      $charge->appendChild($mod);
    return $charge;
  }

  protected function getAdjustments() {
    $adjustments = array();
    $foundMuch = false;
    $foundShown = false;
    while ((($keyterm = self::find(languageDB::SHOWN_MODS)) != null) || 
      ($foundShown && ($keyterm = self::find(languageDB::SHOWN_EXTRA)) != null)) {
      switch ($keyterm) {
        case 'than-normal':
          break; // just ignore this (syntactic sugar)
        case 'shown':
          $foundShown = true;
          break;
        case 'much':
        case 'slightly':
        case 'very-much':
          $foundMuch = $keyterm;
          break;
        default:
          if ($foundMuch) $keyterm = "$foundMuch-$keyterm";
          $adjustments[] = $this->schema->makeMod(languageDB::SHOWN_MODS,null,$keyterm,self::tokens(), self::lineNo());
          $foundMuch = false;
          $foundShown = true;
          break;
      }
      self::ignore(languageDB::ANDD);
    }
    if (count($adjustments))
      return $adjustments;
    return null;
  }

  protected function charge($assumeNum = false, $allowArr = true, $allowPos = true) {
    $state = self::save();
    $mods = array();
    // if ( self::find(languageDB::ANDD) ) $assumeNum = true;
    while ( true ) {
      // Look for arrangements (e.g "in pale, three whatever...")
      if ( $keyterm = self::find(languageDB::ARRANGEMENT) ) {
        $mods[] = $this->schema->makeMod(languageDB::ARRANGEMENT,null,$keyterm,self::tokens(), self::lineNo());
        $allowArr = false;
        // Look for positions - may be more than one (e.g. "in chief and in base"
      } elseif ( $positions = self::getPositions() ) {
        $mods = array_merge( $mods, $positions );
        $allowPos = false;
      }
      else
        break;
    }

    // over x a y, not needed, under x a y not implemented
    self::ignore(languageDB::LAYERING);

    // if found, remember "on" for later
    $onMod = null;
    if ($keyterm = self::find(languageDB::WORD_ON)) {
      $onMod = $this->schema->makeMod(languageDB::CHARGE_WITH,null,
              'charged-with',self::tokens(), self::lineNo());
    }

    // Look for an actual charge
    if ( ($topCharge = self::simpleCharge( $assumeNum, $allowArr, $allowPos )) == null ) {
      self::restore($state);
      return null;
    }
    self::discard($state);
    if ( ($onMod != null) && $topCharge->getAttribute(blazonML::A_KEYTERM) == 'special/escutcheon') {
      $onMod = null; // don't need to say "on an escutcheon..." (but allow it anyway)
    }

    // We have a charge, look for other charges that may be "on" it 
    $currentCharge = $topCharge;
    do {
      $found = false;
      if ( ($onMod == null) && ($keyterm = self::find(languageDB::CHARGE_WITH))) {
        $onMod = $this->schema->makeMod(languageDB::CHARGE_WITH,
                      $this->phraseMatcher->getValue(),$keyterm,self::tokens(), 
                      self::lineNo());
      }

      if ($onMod == null) {
        break;
      }

      // There should be a charge (only) here
      if ( (($onCharge = self::simpleCharge( true, true, false )) == null) && (($onCharge = self::ordinary()) == null)) {
        $onCharge = $this->schema->makeNode(blazonML::E_MISSING);
        self::raiseError(self::ERROR_ORIGIN,"Expected something to go on charge", true);
        $onMod = null;
        break;
      } // else
      $found = true;
      $onMod->appendChild($onCharge);
      $currentCharge->appendChild($onMod);
      $currentCharge = $onCharge;

      $onMod = null;

      // if found, remember "on" for later
      // if ($keyterm = self::find(languageDB::WORD_ON)) {
      //   $onMod = $this->schema->makeMod(languageDB::CHARGE_WITH,null,
      //           'charged-with',self::tokens(), self::lineNo());
      // }

/*
      
      if ($topCharge == null ) { // already found charge, only look for charged with

        $topCharge = $charge;
        $currentCharge = $charge;
    
        $found = true;
        
  
      }


    // Is there anything else "on" this charge?
      if ( ($onMod != null) or ($keyterm = self::find(languageDB::CHARGE_WITH)) ) { 
        if ( $onMod == null) $onMod = $this->schema->makeMod(languageDB::CHARGE_WITH,$this->phraseMatcher->getValue(),$keyterm,self::tokens(), self::lineNo());
        if ( (($onCharge = self::simpleCharge( true, true, false )) == null) and (($onCharge = self::ordinary()) == null)) {
          $onCharge = $this->schema->makeNode(blazonML::E_MISSING);
          self::raiseError(self::ERROR_ORIGIN,"Expected something to go on charge", true);
        }
        $onMod->appendChild($onCharge);
      }
      // look for charge "on" charge (but can also be end of charge, with "on" belonging to next item)
      $state = self::save();
      if ( self::find(languageDB::WORD_ON) ) {
        if ( ($onCharge = self::simpleCharge( true, true, false )) == null ) {
          // The word we found belongs to something else
          self::restore($state);
        } else { // swap chargeA on chargeB to become chargeB charged with chargeA
          $onMod = $this->schema->makeMod(languageDB::CHARGE_WITH,null,'on',self::tokens(), self::lineNo());
          $onMod->appendChild($currentCharge);
          $onCharge->appendChild($onMod);
        } 
      }
      if ($onMod != null) {
        $currentCharge->appendChild($onMod);
        $currentCharge = $onCharge;
      } */
    } while ($found);
    // self::discard($state);
    // if ($topCharge == null) return null;
    $state = self::save();
    if ( !self::semicolon() ) {
      // Look for adjacent charges
      if ( $keyterm = self::find(languageDB::CHARGE_ADJACENT) ) {
        $adjMod = $this->schema->makeMod(languageDB::CHARGE_ADJACENT,null,$keyterm,self::tokens(), self::lineNo());
        if ( (($adjCharge = self::charge()) == null) and (($adjCharge = self::ordinary()) == null) ) {
          $adjCharge = $this->schema->makeNode(blazonML::E_MISSING);
          self::raiseError(self::ERROR_ORIGIN,"Expected charge or ordinary for $keyterm", true);
        }
        $adjMod->appendChild ( $adjCharge);
        // keep looking for more of the same 
        while ( !self::semicolon() and self::find(languageDB::ANDD) and (($adjCharge = self::charge()) != null ) ) {
          $adjMod->appendChild ( $adjCharge);
        }

        $mods[] = $adjMod;
        self::restoreAnd();
        /*   // If there is an explicit position, a following "between" is redundant (e.g. issue 173)
           $state = self::save();
           if ( $keyterm = self::find(languageDB::CHARGE_ADJACENT) ) {
             if ( $keyterm != 'between' ) // Backtrack, this belongs to the next item
               self::restore($state);
             // else just ignore the 'between'
           } */

        self::discard($state);
      } 
    } else {
      self::restore($state); // put the semi-colon back
    }
    // Add anything found earlier
    foreach ( $mods as $mod )
      $topCharge->appendChild($mod);
    return $topCharge;
  }

  protected function objects(){

    $node = $this->schema->makeNode(blazonML::E_OBJECTS);
    $assume_num = false;
    $found = false;

    $state = self::save();
    self::ignore(languageDB::WITH);

    while ( self::moreInput() ) {
      // We treat a counterchanged division a bit like an ordinary
      if ( $counterchangedDivision = self::division(true) ) {
        $node->appendChild($counterchangedDivision);
        $found = true;
      } elseif ($item = self::ordinary()  or  $item = self::charge($assume_num)) {
        $found = true;
        $assume_num = false; // reset if it was true
        $node->appendChild($item);
        self::comma();
        if ( self::find(languageDB::ANDD) ) $assume_num = true;
      } else {
        break;
      }
    }
    if ( !$found ) {
      $node = null;
      self::restore($state);
    } else {
      self::discard($state);
    }
      
    return $node;
  }
  // TODO put the comented code below into heralds knowledge
  /*         switch ( $num ) {
           case 1:
           case 2: $keyterm = 'cottice1'; break;
           case 3: $keyterm = 'cottice3'; break;
           case 4: $keyterm = 'cottice2'; break;
           case 6: $keyterm = 'cottice3'; break;
           default: $keyterm = 'cottice1';
             self::raiseError('parser',"strange number of cottices ($num)" ); break;
           } */

  protected function cotticing() {
    $state = self::save();
    $num = null;
    if ( self::find(languageDB::BETWEEN) ) {
      $num = self::find(languageDB::NUMBER);
      if ( !self::find(languageDB::COTTICE) ) {
        self::restore($state);
        return null;
      }
    }
    if ( !$num )  $num = self::find(languageDB::COTTICE_MODS);
    if ( !$num ) { // still not found anything
      self::restore($state);
      return null;
    } // got something!
    $mod = $this->schema->makeMod(languageDB::COTTICE_MODS,$num,'cotticing',self::tokens(), self::lineNo());
    // now look for a linetype or colour
    $found = true;
    while ( $found ) { // TODO can we use linetype function here?
      if ( $keyterm = self::find(languageDB::LINETYPE_PREFIX) ) {
        $mod2 = $this->schema->makeMod(languageDB::LINETYPE_PREFIX,null,$keyterm,self::tokens(), self::lineNo());
        if ( $keyterm = self::find(languageDB::LINETYPE) )
          $mod2->appendChild($this->schema->makeMod(blazonML::A_LINETYPE,$keyterm,self::tokens(), self::lineNo()));
        if ( $tinc = self::tincture( false ) )
          $mod2->appendChild($tinc);
        if ( !$mod2->hasChildNodes() ) {
          $mod2->appendChild($this->schema->makeNode(blazonML::E_MISSING));
          self::raiseError(self::ERROR_ORIGIN,"Expected line type or colour", true);
        }
        $mod->appendChild($mod2);
      } elseif ( $keyterm = self::find(languageDB::LINETYPE) )
        $mod->appendChild($this->schema->makeMod(blazonML::A_LINETYPE,$keyterm,self::tokens(), self::lineNo()));
      elseif ( $tinc = self::tincture( false ) )
        $mod->appendChild($tinc);
      else
        $found = false;
    }
    self::discard($state);
    return $mod;
  }

  protected function linetype() {
    $mod = null;
    // Look for modifiers that are followed a linetype (e.g. "the inner ones wavy")
    if ( $keyterm = self::find(languageDB::LINETYPE_PREFIX) ) {
      $mod = $this->schema->makeMod(languageDB::LINETYPE_PREFIX,null,$keyterm,self::tokens(), self::lineNo());
      if ( $keyterm = self::find(languageDB::LINETYPE) )
        $mod->appendChild($this->schema->makeMod(blazonML::A_LINETYPE,null,$keyterm,self::tokens(), self::lineNo()));
      else {
        $mod->appendChild($this->schema->makeNode(blazonML::E_MISSING));
        self::raiseError(self::ERROR_ORIGIN,"Expected line type", true);
      }
      // Look for simple linetype modifiers (e.g. "dancetty")
    } elseif ( $keyterm = self::find(languageDB::LINETYPE ))
      $mod = $this->schema->makeMod(blazonML::A_LINETYPE,null,$keyterm,self::tokens(), self::lineNo());
    if ( $mod )  { // also look for top/bottom modifiers
      if ( $keyterm = self::find(languageDB::LINETYPE_POSTFIX) )
        $mod->appendChild($this->schema->makeMod(languageDB::LINETYPE_POSTFIX,null,$keyterm,self::tokens(), self::lineNo()));
    }
    return $mod;
  }

  protected function ordinary() {
      $insideMod = null;
    $ord = $this->schema->makeNode(blazonML::E_ORDINARY);
    $state = self::save();
    $found = true;
    //if ( self::find(languageDB::WORD_IN) and ($keyterm = self::find(languageDB::LOCATION)) )   // TODO what was thie here for?
    //   $ord->appendChild($this->schema->makeMod(languageDB::ORDINARY_PREFIX,'in' . $keyterm,self::tokens()));

    if ($keyterm = self::find(languageDB::WORD_ON)) $onMod = $this->schema->makeMod(languageDB::WORD_ON,null,$keyterm,self::tokens(), self::lineNo());
    // if found, remember "inside" for later
    if ($keyterm = self::find(languageDB::BETWEEN)) $insideMod = $this->schema->makeMod(languageDB::BETWEEN,null,$keyterm,self::tokens(), self::lineNo());
    self::ignore(languageDB::LAYERING); // over x a y, not needed, under x a y not implementd
    // Look for a number (or assume one)
    $ordNum = self::find(languageDB::QUANTITY);
    if ( !$ordNum ) $ordNum = self::find(languageDB::NUMBER);
    if ( !$ordNum ) $ordNum = 1;
    // Look for various prefix modifiers (including positions, sometimes we get "in chief" or "in base"
    while ( $found ) {
      if ( $keyterm = self::find(languageDB::ORDINARY_PREFIX) ) {
        $ord->appendChild($this->schema->makeMod(languageDB::ORDINARY_PREFIX,$keyterm,self::tokens(), self::lineNo()));
      } else {
        $found = false;
      }
    }
    // Check that there is no confusion with charges that look a bit like ordinaries
    if ( self::find(languageDB::CHARGE_NOT_ORDINARY) ) {
      self::restore($state);
      return null;
    }
    // Now look for things that are definitely ordinaries
    $keyterm = self::find(languageDB::ORDINARY_NOT_CHARGE);
    // Now look for things that could be either but depend on the number given
    if ( ($keyterm == null) and $keyterm = self::find(languageDB::ORDINARY_OR_CHARGE) ) {
      $parts = explode('/',$keyterm);
      $type = $parts[1];
      if ($type == 'chevron') {  // TODO draw this (and 2 bends!)
        if ($ordNum > 3) {// must be a charge
          self::restore($state);
          return null;
        }
      } elseif ( $ordNum > 1 ) { // must be a charge, e.g. 3 frets
        self::restore($state);
        return null;
      }
      $keyterm = $type;
    } // check if found something earlier, or actually have a nice easy ordinary...
    if ( ($keyterm != null)  or ($keyterm = self::find(languageDB::ORDINARY)) ) {
      $ord->setAttribute(blazonML::A_KEYTERM,$keyterm);
      $ord->setAttribute(blazonML::A_NUMBER,"$ordNum");
      $ord->setAttribute(blazonML::A_TOKENS,self::tokens());
      $ord->setAttribute(blazonML::A_LINENUMBER,self::lineNo());
    } else { // no ordinary found, so backtrack.
      self::restore($state);
      return null;
    }
    // Bit of a fudge, we are always "within" a bordure
    if ($keyterm == "bordure") {$insideMod = null;}

// TODO check makeMod everywhere, does it need $this->phraseMatcher->getValue() as 2nd arg?
    $foundTinc = false;
    while ( self::moreInput() ) { // Look for prefixes again, just in case, and other common ordinary modifiers (e.g. "a bend sinister")
      self::comma();
      self::ignore(LanguageDB::ANDD);
      if ( $keyterm = self::find(languageDB::ORDINARY_MODS) or $keyterm = self::find(LanguageDB::ORDINARY_PREFIX) ) {
        $ord->appendChild($this->schema->makeMod(languageDB::ORDINARY_MODS,$this->phraseMatcher->getValue(),$keyterm,self::tokens(), self::lineNo()));
      } elseif ( ($keyterm = self::find(languageDB::ORIENTATION)) != null ) {
        $ord->appendChild($this->schema->makeMod(languageDB::ORIENTATION,null,$keyterm,self::tokens(), self::lineNo()));
        // Look for modifiers that are normally followed by a colour (e.g. "a bend compony vert")
      } elseif ( $keyterm = self::find(languageDB::COLOUR_MODS) ) {
        $comp = $this->schema->makeMod(languageDB::COLOUR_MODS,null,$keyterm,self::tokens(), self::lineNo());
        $tinc2 = self::tincture(true); // Must have a tincture
        $tinc2->setAttribute('index','2');
        $comp->appendChild($tinc2);
        $ord->appendChild($comp);
        // Look for cotticing (complicated, so moved to a separate function)
      } else if ( $mod = self::cotticing() )
        $ord->appendChild($mod);
      // similarly line types
      elseif ( $mod = self::linetype() ) {
        $ord->appendChild($mod);
        // Look for voided, (e.g. "a bend voided or")
      } elseif ( $keyterm = self::find(languageDB::VOIDED ) ) {
        $mod = $this->schema->makeMod(languageDB::VOIDED,null,$keyterm,self::tokens(), self::lineNo());
        // If ordinary already has tincture, look for a void tincture (e.g. "a bend or voided sable")
        if ( $foundTinc and ( $tinc = self::tincture( false ) )) $mod->appendChild($tinc);
        $ord->appendChild($mod);
      } elseif ( $tinc = self::tincture ( false ) ) {
        $ord->appendChild( $tinc );
        $foundTinc = true;
      } else break;
    }
    // Have we found a colour yet?
    if ( !$foundTinc ) { 
      $ord->appendChild($this->schema->makeNode(blazonML::E_TINCTURE,array(blazonML::A_INDEX => '1',blazonML::A_ORIGIN => 'pending')));
    }
    // Does this ordinary go "between" some charges?
    $keyterm = null;
    if ( ($insideMod != null) or ($keyterm = self::find(languageDB::BETWEEN))  ) {
      if ( !isset($insideMod) ) $insideMod = $this->schema->makeMod(languageDB::BETWEEN,null,$keyterm,self::tokens(), self::lineNo());
      if ( ($obj = self::charge(false,true,true)) or ($obj = self::ordinary()) ) {
        $insideMod->appendChild($obj);
      } else {
        $insideMod->appendChild($this->schema->makeNode(blazonML::E_MISSING));
        // Not necessarily an error - maybe need to rearrange previous charge?
        // self::raiseError(self::ERROR_ORIGIN,"Expected charge for within/inside", true);
      }
      // keep looking for more of the same
      while ( !self::semicolon() and self::find(languageDB::ANDD) and (($obj = self::charge()) != null ) ) {
        $insideMod->appendChild($obj);
      }
      self::restoreAnd();
      $ord->appendChild($insideMod);
    }
    // Are there charges (or other ordinaries) "on" this one?
    $keyterm = null;
    if ( isset($onMod) or ($keyterm = self::find(languageDB::CHARGE_WITH)) ) {
      self::comma();
      if ( !isset( $onMod )) $onMod = $this->schema->makeMod(languageDB::CHARGE_WITH,$this->phraseMatcher->getValue(),$keyterm,self::tokens(), self::lineNo());
      // in some cases, prefer ordinaries to charges
      if ( in_array($ord->getAttribute(blazonML::A_TYPE),array('chief','canton')) ) {
        $item = self::ordinary();
        if ( $item === null ) $item = self::charge(false);
      } else {// but usually we expect to find charges
        $item = self::charge(false,true,false);
        if ( $item === null ) $item = self::ordinary(); // Some arrangements are valid
      }
      if ( !$item ) {
        $item = $this->schema->makeNode(blazonML::E_MISSING);
        self::raiseError(self::ERROR_ORIGIN,"Expect something to go on ordinary", true);
      }
      $onMod->appendChild($item);
      $ord->appendChild($onMod);
    }
    self::discard($state);
    return $ord;
  }

  protected function quartermarker() {
    $state = self::save();
    self::ignore(languageDB::WORD_THE);
    // Look for an obvious quartering
    $keyterm =self::find(languageDB::QUARTERS);
    self::ignore(languageDB::QUARTERMARK);
    if ($keyterm !== null) {
      self::discard($state);
      return $keyterm;
    }
    // Look for a number, but it MUST be followed by a quartermarker
    $keyterm = self::find(languageDB::NUMBER);
    if (self::find(languageDB::QUARTERMARK)) {
      self::discard($state);
      return $keyterm;
    }
    // Look for a letter, but it MUST be followed by a quartermarker
    $keyterm = self::find(languageDB::LETTERS);
    if (self::find(languageDB::QUARTERMARK)) {
      self::discard($state);
      return $keyterm;
    }
    self::restore($state);
    return null;
  }

  protected function quarteredMods() {
    if ( ($qmod = self::find(languageDB::QUARTERMODS)) != null ) {
       $quartered->appendChild( $this->schema->makeMod(languageDB::QUARTERMODS, null, $qmod, self::tokens(), self::lineNo()));
    }
  }

  protected function quartered() {
    /**
     * @var DOMElement[] $subshields
     */
    $subshields = array_fill(1,64,null);

    $state = self::save();
    $node = null;
    $numberStated = 0;
    $highest = 0;

    if ( ($marshall = self::find(languageDB::QUARTERED)) != null ) {
      $quartered = $this->schema->makeNode(blazonML::E_COMPLEX, array( 'keyterm' => blazonML::V_QUARTERED), self::tokens(), self::lineNo());
      if ( ($qmod = self::find(languageDB::QUARTERMODS)) != null ) {
         $quartered->appendChild( $this->schema->makeMod(languageDB::QUARTERMODS, null, $qmod, self::tokens(), self::lineNo()));
      }
      self::comma();
      if (self::find(languageDB::WORD_OF)) { // like "quarterly of 4"
        $numberStated = self::find(languageDB::NUMBER); // But ignore it and count the quarters we are given
        self::comma();
      }
      
      self::ignore(languageDB::SEMICOLON);
      $foundQuarter = false;
      do { // Look for quarters
        $found = false;
        $indices = array();
        while ( ($keyterm =self::quartermarker()) != null ) {
          $indices[] = $keyterm; // make a list of quarter numbers
          if ($keyterm > $highest) $highest = $keyterm;
          self::comma();
          self::ignore(languageDB::ANDD);
          $found = $foundQuarter = true;
        }
        if ( $found ) {
          $ID = null;
          self::ignore(languageDB::QUANTIFIER);
          $quarter = self::shield(); // Look for the description of this quarter
          if ( $quarter != null and count($indices) > 1 ) {
            $ID = $quarter->getAttribute('ID');
          }
          if ( $subshields[$indices[0]] != null )
            self::raiseError(self::ERROR_ORIGIN,'Duplicate quarter', true );
          else
            $subshields[$indices[0]] = $quarter; // Set the first quarter to this description
          for ( $i = 1; $i < count($indices); $i++ ) {
            $reference = $this->schema->makeNode(blazonML::E_SHIELD, array ( blazonML::A_IDREF => $ID)); // Set other quarters to point to this one
            if ( $subshields[$indices[$i]] != null )
              self::raiseError(self::ERROR_ORIGIN,'Duplicate quarter', true );
            else
              $subshields[$indices[$i]] = $reference;
          }
        } // Keep going as long as we find quarters
        if ( self::semicolon() ) {
          $found = false;
          // error_log("exiting quarterly loop");
        }
      } while ( $found );
      if ( $marshall == 'quarterly' and !$foundQuarter ) {
        self::restore($state); // backtrack, assume this is quarterly division
        return null;
      }
      $order = $highest;
      if ($numberStated > 0) {
        $order = $numberStated;
        if ($numberStated >= $highest) {
          $highest = $numberStated;
        } else {
          self::raiseError(self::ERROR_ORIGIN,"Quarter given higher than stated", true );
        }
      }
      for ( $i = 1; $i <= $highest; $i++ ) {
        if ( $subshields[$i] == null ) {
          $pendingShield = $this->schema->makeNode(blazonML::E_SHIELD);
          $pendingShield->setAttribute('index',$i);
          $pendingShield->appendChild($this->schema->makeNode(blazonML::E_PENDING));
          $subshields[$i] = $pendingShield;
          // self::raiseError(self::ERROR_ORIGIN,'Missing quarter (' . $i . ')', true);
        } else { 
          $subshields[$i]-> setAttribute(blazonML::A_INDEX,"$i");
        }
      }
      if ($marshall == 'quartered-per-saltire') { 
        self::raiseError(self::ERROR_ORIGIN,'Quarterly per saltire not supported, sorry', true );
        // $order = '4-saltire';
      }
      $quartered->setAttribute('order',$order);
      for ( $i = 1; $i <= $highest; $i++ )
        $quartered->appendChild($subshields[$i]);
      $node = $this->schema->makeNode(blazonML::E_SHIELD, array ( blazonML::A_INDEX => '0' ));
      $node->appendChild($quartered);
    }
    return $node;
  }

  protected function halved() {

    $node = null;
    if ( ($keyterm = self::find(languageDB::SPLITS)) != null ) {
      $half = $this->schema->makeNode(blazonML::E_COMPLEX, array( 'keyterm' => $keyterm), self::tokens(), self::lineNo());
      if ( ($shield1 = self::shield()) == null ) {
        self::raiseError(self::ERROR_ORIGIN, 'Expected 1st shield description (replaced by blank)', true);
        $shield1 = $this->schema->makeNode(blazonML::E_MISSING);
      }
      $shield1->setAttribute('index','1');
      if ( ! self::find(languageDB::WITH) ) {
        // uncomment to require "impaled A with B"
        //      self::raiseError('warning','expected "with"');
        //      return $shield1;
      }
      if ( ($shield2 = self::shield()) == null ) {
        self::raiseError(self::ERROR_ORIGIN, 'Expected 2nd shield description (replaced by blank)', true);
        $shield2 = $this->schema->makeNode(blazonML::E_MISSING);
      }
      $shield2->setAttribute('index','2');
      //$half = $$this->schema->makeNode($keyterm); // TODO is this needed (not tested yet)
      $half->appendChild($shield1);
      $half->appendChild($shield2);
      $node = $this->schema->makeNode(blazonML::E_SHIELD, array ( blazonML::A_INDEX => '0' ));
      $node->appendChild($half);
    }
    return $node;
  }

  protected function fur() {

    $state = self::save();
    self::ignore(languageDB::WORD_OF);
    $keyterm = self::find(languageDB::FUR);
    if ( $keyterm == null ) {
      self::restore($state);
      return null;
    }
    self::discard($state);
    return $this->schema->makeNode(blazonML::E_FUR, array ( blazonML::A_KEYTERM => $keyterm ), self::tokens(), self::lineNo());
  }

  protected function colour() {

    $state = self::save();
    self::ignore(languageDB::WORD_OF);
    $keyterm = self::find(languageDB::COLOUR);
    if ( $keyterm == null ) {
      self::restore($state);
      return null;
    }
    self::discard($state);
    return $this->schema->makeNode(blazonML::E_COLOUR, array ( blazonML::A_KEYTERM => $keyterm ), self::tokens(), self::lineNo());
  }

  protected function treatment() {
    $state = self::save();
    $keyterm = self::find(languageDB::TREATMENT);
    if ( $keyterm == null ) {
      self::restore($state);
      return null;
    }
    self::ignore(languageDB::WORD_OF);
    if ($ofNum = self::find(languageDB::NUMBER))
      self::ignore(languageDB::POINTS);
    if ( $ofNum == null || $ofNum < 5 )
      $ofNum = 5;
    elseif ( $ofNum > 10 )
      $ofNum = 10;
    if ( $keyterm == 'mulletty')
      $keyterm .= '-' . $ofNum;
    if ($keyterm == 'checky' && $ofNum == 9) {
      $keyterm = 'checky9';
    }
    $node = $this->schema->makeNode(blazonML::E_TREATMENT, array ( blazonML::A_KEYTERM => $keyterm ), self::tokens(), self::lineNo());
    if (substr($keyterm,0,5) == 'grady') {
      if (self::find(languageDB::INVERSE)) {
        $node->appendChild($this->schema->makeMod(languageDB::INVERSE,null,languageDB::INVERSE, self::tokens(), self::lineNo()));
      }
    }
    self::discard($state);
    return $node;
  }

  protected function division( $onlyCounterchanged = false ) { 
    $need3Tinc = array ( 'per-pall', 'per-pall-arrondi', 'tierced-in-pale',
    'tierced-in-fess', 'tierced-in-bend', 'tierced-in-bend', 'tierced-in-chevron' );
    $opt3Tinc = array ( 'per-pile', );

    $state = self::save();
    $node = null;
    if ( ($keyterm = self::find( languageDB::DIVISION )) != null ) {
      $node = $this->schema->makeNode(blazonML::E_DIVISION,array(blazonML::A_KEYTERM => $keyterm), self::tokens(), self::lineNo());
      self::comma();
      // generic modifiers, including tinctures
      $maxTinc = 2;
      $minTinc = 2;
      if (in_array( $keyterm, $need3Tinc)) {
        $maxTinc = 3;
        $minTinc = 3;  
      } elseif (in_array($keyterm, $opt3Tinc)) {
        $maxTinc = 3;
      }
      $tincCount = 0;
      // if ( $depth == 2 ) { // The tincture is implied by the previous division
      //   $found = true;
      //   while ($found) {
      //     if ($keyterm = self::find(languageDB::LINETYPE)) {
      //       $node->appendChild( $this->schema->makeMod(blazonML::A_LINETYPE, null, $keyterm, self::tokens(), self::lineNo()));
      //     } elseif ($keyterm = self::find(languageDB::ORIENTATION)) {
      //       $node->appendChild($this->schema->makeMod(languageDB::ORIENTATION,null,$keyterm, self::tokens(), self::lineNo()));
      //     } else {
      //       $found = false;
      //     }
      //   }    
      //   if ( !self::find(languageDB::COUNTERCHANGE) ) {
      //     self::raiseError(self::ERROR_ORIGIN,"Second division should be counterchanged?");
      //   } else {
      //     $maxTinc = 1; // We have all the tinctures that we need
      //     $minTinc = 1;
      //     $tincCount = 1;
      //     $secondTinc = $this->schema->makeNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
      //         blazonML::A_ORIGIN => 'implied' ));
      //     $secondTinc->appendChild($this->schema->makeNode(blazonML::E_COUNTERCHANGED, null, self::tokens(), self::lineNo()));
      //     $node->appendChild($secondTinc);
      //   }
      // } else {
      //   $tincCount = 0;
      // }
      $found = true;
      $foundCounterchange = false;
      while ( $found ) {
        if (self::semicolon()) break;
        self::comma();
        self::ignore(languageDB::ANDD);
        if ($keyterm = self::find(languageDB::LINETYPE)) {
          $node->appendChild( $this->schema->makeMod(blazonML::A_LINETYPE, null, $keyterm, self::tokens(), self::lineNo()));
        } elseif ( $keyterm = self::find(languageDB::DIVISION_MODS)) {
          $node->appendChild($this->schema->makeMod(languageDB::DIVISION_MODS, $this->phraseMatcher->getValue(), $keyterm, self::tokens(), self::lineNo()));
        } elseif ($keyterm = self::find(languageDB::ORIENTATION)) {
          $node->appendChild($this->schema->makeMod(languageDB::ORIENTATION,null,$keyterm, self::tokens(), self::lineNo()));
        } elseif ( ($tincCount < $maxTinc) && ($tinc = self::tincture(false)) ) {
          $tincCount++;
          $tinc->setAttribute(blazonML::A_INDEX,"$tincCount");
          $node->appendChild($tinc);
          if ($tinc->firstChild->nodeName == blazonML::E_COUNTERCHANGED) {
            $foundCounterchange = true;
            $tincCount = 2;
            break;
          }
        } else
          $found = false;
      }
      if ($onlyCounterchanged && !$foundCounterchange) {
        self::restore($state);
        return null;
      } 
      // Did we find all the tinctures that we need?
      while ( $tincCount < $minTinc ) {
        $node->appendChild($this->schema->makeNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => ++$tincCount,
            blazonML::A_ORIGIN => 'missing' )));
        self::raiseError(self::ERROR_ORIGIN,"Missing tincture(s) on division", true);
      }
      // if ($tincCount == $maxTinc) {
        // self::raiseError(self::ERROR_ORIGIN,"Too many tinctures on division", true);
      // }
    }
    self::discard($state);
    return $node;
  }

  protected function crest() {
    if (($type = self::find(languageDB::CREST)) == null) {
      return;
    }
    $crestNode = $this->schema->makeNode(blazonML::E_CREST, array (
      blazonML::A_KEYTERM => $type),
      self::tokens(),
      self::lineno());
    $charge = self::charge(true,false,false); // assume number, don't allow positions or arrangements
    if ($charge == null) {
      self::raiseError(self::ERROR_ORIGIN,"Expected charge for the crest", true);
      return null;
    } else {
      $crestNode->appendChild($charge);
    }
    return $crestNode;
  }

  protected function mantling() {
    if (($type = self::find(languageDB::MANTLING)) == null) {
      return;
    }
    $mantlingNode = $this->schema->makeNode(blazonML::E_MANTLING, array (
      blazonML::A_KEYTERM => $type),
      self::tokens(),
      self::lineno());
    $locNode = null;
    while (true) {
      if (($loc = self::find(languageDB::ACHIEVEMENT_LOC)) != null) {
        $locNode = $this->schema->makeNode(blazonML::E_LOCATION, array (
            blazonML::A_KEYTERM => $loc 
          ),
          self::tokens(),
          self::lineno()
        );
      }
      $tinctures = [];
      $colCount = 0;
      while (($tincNode = self::tincture()) != null) {
        $tinctures[] = $tincNode;
        self::ignore(languageDB::ANDD);
      }
      if (count($tinctures)) { // found some colours
        if ($locNode == null) { // use a default location
          $locNode = $this->schema->makeNode(blazonML::E_LOCATION, array (
              blazonML::A_KEYTERM => 'default'));
        }
        foreach($tinctures as $tincture) 
          $locNode->appendChild($tincture);
        $mantlingNode->appendChild($locNode);
      } else {
        break;
      }
      self::ignore(languageDB::ANDD);
    }

    return $mantlingNode;
  }

  protected function torse() {
    if (($type = self::find(languageDB::TORSE)) == null) {
      return '';
    }
    $torseNode = $this->schema->makeNode(blazonML::E_TORSE, array (
      blazonML::A_KEYTERM => $type),
      self::tokens(),
      self::lineno());
    $locNode = null;
    while (true) {
      if (($loc = self::find(languageDB::ACHIEVEMENT_LOC)) != null) {
        $locNode = $this->schema->makeNode(blazonML::E_LOCATION, array (
            blazonML::A_KEYTERM => $loc 
          ),
          self::tokens(),
          self::lineno()
        );
      }
      $tinctures = [];
      $colCount = 0;
      while (($tincNode = self::tincture()) != null) {
        $tinctures[] = $tincNode;
        self::ignore(languageDB::ANDD);
      }
      if (count($tinctures)) { // found some colours
        if ($locNode == null) { // use a default location
          $locNode = $this->schema->makeNode(blazonML::E_LOCATION, array (
              blazonML::A_KEYTERM => 'default'));
        }
        foreach($tinctures as $tincture) 
          $locNode->appendChild($tincture);
        $torseNode->appendChild($locNode);
      } else {
        break;
      }
      self::ignore(languageDB::ANDD);
    }

    return $torseNode;
  }

  protected function motto() {
    if (!self::find(languageDB::MOTTO)) {
      return null;
    }
    $text = $this->phraseMatcher->getValue();
    $mottoNode = $this->schema->makeNode(blazonML::E_MOTTO, array (
      blazonML::A_KEYTERM => 'plain',  // expansion to other styles? 
      blazonML::A_TEXT => $text
      ),
      self::tokens(),
      self::lineno()
    );
    return $mottoNode;
  }

  protected function helmet() {
    if (($helmType = self::find(languageDB::HELMET)) == null) {
      return null;
    }
    $helmetNode = $this->schema->makeNode(blazonML::E_HELMET, array (
      blazonML::A_KEYTERM => $helmType,  // expansion to other styles? 
      ),
      self::tokens(),
      self::lineno()
    );
    $locNode = null;
    if (($loc = self::find(languageDB::ACHIEVEMENT_LOC)) != null) {
      $locNode = $this->schema->makeNode(blazonML::E_LOCATION, array (
          blazonML::A_KEYTERM => $loc 
        ),
        self::tokens(),
        self::lineno()
      );
    }
    if (($tincNode = self::tincture()) != null) {
      if ($locNode == null) { // use a default location
        $locNode = $this->schema->makeNode(blazonML::E_LOCATION, array (
            blazonML::A_KEYTERM => 'default'));
      }
      $locNode->appendChild($tincNode);
      $helmetNode->appendChild( $locNode );
    }

    return $helmetNode;
  }

  // Look for a tincture, return a pending tincture if not found
  protected function tincture($required=false) {

    $found = true;
    $colourOrFur = null;
    $state = self::save();
    $node = $this->schema->makeNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
        blazonML::A_ORIGIN => 'given'));
    // zeroth, discard grouping words TODO should this be part of charge & ordinary?
    self::ignore(languageDB::GROUP);

    if ( ($colourOrFur = self::colour()) or ($colourOrFur = self::fur()) ) {
      // This could be [colour] [treatment] [colour],
      // unless it is followed by a comma
      self::comma();
      if ( self::moreInput() /* and !self::semicolon() */ ) {
        if ( ($treat = self::treatment()) ) {
          $keyterm = $treat->getAttribute('keyterm');
          $grady = (substr($keyterm,0,5) == 'grady');
          $tinc1 = $this->schema->makeNode(blazonML::E_TINCTURE, array ( blazonML::A_INDEX => '1'));
          $tinc1->appendChild($colourOrFur);
          $colourOrFur = null; // used
          $treat->appendChild($tinc1);
          self::ignore(languageDB::ANDD);
          // Some treatments only require one tincture
          if ( !in_array( $keyterm, array ( 'bezanty', 'hurty', 'platy' ))) {
            $tinc2 = self::tincture(!$grady);
            if ($tinc2 != null) {
              $tinc2->setAttribute(blazonML::A_INDEX, '2');
              $treat->appendChild($tinc2);
            }
          }
          $node->appendChild($treat);
          // Could also be [colour] [semyde] [charge]
        } elseif ( self::find(languageDB::SEMY) ) {
          $semyde = $this->schema->makeNode(blazonML::E_SEMYDE,null,self::tokens(), self::lineNo());
          $tinc1 = $this->schema->makeNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
              blazonML::A_ORIGIN => 'given'));
          $tinc1->appendChild($colourOrFur);
          $colourOrFur = null; // used
          $semyde->appendChild($tinc1);
          if ( ($charge = self::simpleCharge( true ))) { // Do not worry if no number given
            $semyde->appendChild($charge);
          } else {
            $semyde->appendChild($this->schema->makeNode(blazonML::E_MISSING));
          }
          $node->appendChild($semyde);
        }
      }
      // Look for [treatment] [colour] [colour]
    } elseif ( ($treat = self::treatment()) ) {
      self::comma();
      $treat->appendChild(self::tincture(true));
      $keyterm = $treat->getAttribute('keyterm');
      $grady = (substr($keyterm,0,5) == 'grady');
      self::comma();
      self::ignore(languageDB::ANDD);
      $tinc2 = self::tincture(!$grady); // grady might not need second tincture
      if ($tinc2 != null) {
        $tinc2->setAttribute('index',2);
        $treat->appendChild($tinc2);
      }
      $node->appendChild($treat);
      // Look for counterchange
    } elseif ( self::find(languageDB::COUNTERCHANGE) ) {
      $node->appendChild($this->schema->makeNode(blazonML::E_COUNTERCHANGED, null, self::tokens(), self::lineNo()));
      // Look for division
    } elseif ( ($div1 = self::division()) ) {
      // Look for a second (normally counterchanged) division
      // if ( ( $div2 = self::division(true) ) ) $div1->appendChild($div2);
      $node->appendChild($div1);
      // Look for a back reference
    } elseif ( ($keyterm = self::find(languageDB::BACKREF)) ) {
      $node->setAttribute( blazonML::A_ORIGIN, $keyterm);
      $node->setAttribute( blazonML::A_TOKENS, self::tokens());
      $node->setAttribute( blazonML::A_LINENUMBER, self::lineNo());
      // Or a reference to proper
    } elseif ( self::find(languageDB::PROPER) ) {
      $node->appendChild( $this->schema->makeNode(blazonML::E_PROPER,null,self::tokens(), self::lineNo()));
      // Look for a colour or a fur
    } else { // Didn't find anything
      self::restore($state);
      $found = false;
    }
    self::discard($state);
    if ( $colourOrFur ) $node->appendChild($colourOrFur); // we have a colour or fur on its own

    if ( !$found ) {
      if ( $required ) {
        $node = $this->schema->makeNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
            blazonML::A_ORIGIN => 'pending'));
      } else {
        $node = null;
      }
    }
    return $node;
  }
}

