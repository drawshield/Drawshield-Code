<?php 

class frenchGrammar extends grammar
{
    private $previousTypes = [];

    /**
     * @return DOMElement
     */
    public function getShield($syncTo = true) // DONE
    {
        $node = null;
        // Try to find complete shield
        $this->matcher->syncOver(languageDB::SEMICOLON);
        $node = self::shield();

        // Report anything left that we don't understand
        if ($syncTo) $this->matcher->syncTo(array('NOT_A_CATEGORY'));
        return $node;
    }

    public function getRoll() {
        return null;
    }

    public function getCommand() {
        return null;
    }

    protected function simple() // DONE
    {
        $phrase = self::newPhrase();

        $node = null;
        if (($tinc = self::tincture()) != null) {
            $field = $this->schema->makeNode(blazonML::E_FIELD);
            $simple = $this->schema->makeNode(blazonML::E_PLAIN);
            $this->matcher->syncOver();
            $field->appendChild($tinc);
            $ordChgs = self::objects();
            $simple->appendChild($field);
            if ( $ordChgs ) {
                if ( $ordChgs->hasChildNodes() )
                    $simple->appendChild($ordChgs);
            }
            $node = $this->schema->makeIDNodeFromLexeme(blazonML::E_SHIELD,
                $phrase->create(languageDB::ONFIELDOF), array(blazonML::A_INDEX => '0'));
            $node->appendChild($simple);
        }
        if ($node == null)
            self::discardPhrase($phrase);
        return $node;
    }

    protected function shield() 
    {
        $node1 = self::simple();
        // Okay, we really couldn't find one...
        if ($node1 == null) {
            $node1 = $this->schema->makeIDNode(blazonML::E_SHIELD);
            $node1->appendChild($this->schema->makeNode(blazonML::E_MISSING));
            self::raiseError(self::ERROR_ORIGIN, 'Expected quartering or tincture');
        }
        return $node1;
    }

    protected function simpleCharge($assumeNum = false, $allowArr = false, $allowPos = false) // DONE
    {
        $phrase = self::newPhrase();
        $mods = array();
        // Look for a number
        $chgNum = $phrase->add(self::findLex(languageDB::NOMBRE));
        if ($chgNum === null) {
            if ($assumeNum)
                $chgNum = $phrase->add(new Lexeme( languageDB::NUMBER, '1','','',''));
            else {
                self::discardPhrase($phrase);
                return null;
            }
        }
        // Now look for actual charges
        $chargeLex = $phrase->add(self::findLex(languageDB::CHARGE));
        if (is_null($chargeLex)) {
            // nothing to see here, move along
            self::discardPhrase($phrase);
            return null;
        }
        // We have a charge keyterm in $chargeLex, but $phrase contains any previous tokens.
        // We need to extract flags from the end of the charge keyterm and combine all the tokens
        // don't care if no flags
        list($keyterm, $flags) = explode('?', $chargeLex->keyterm . '?');
        $chargeNode = $this->schema->makeNodeFromLexeme( blazonML::E_CHARGE, $phrase->create($keyterm) );
        $chargeNode->setAttribute('number', $chgNum->keyterm);
        // $chargeNode->setAttribute(blazonML::A_NUMBER, "$chgNum");
        // Set any flags
        $always_proper = strpos($flags, 'P') !== false;
        $optional_tincture = strpos($flags, 'O') !== false;

        // Look for bundle again (e.g. "two arrow bundles"
        // Now look for postfix stuff, including tinctures and arrangements
        $foundTinc = intval($chgNum->keyterm);
        if ($foundTinc == 0) $foundTinc = 1;
        $tincCount = 1;
        $gotMainTincture = false;
        while (self::moreInput()) {
            if (self::semicolon()) {
                break;
            }
            $phrase4 = self::newPhrase();
            if ($foundTinc > 0 && ($tinc = self::tincture())) {
                $gotMainTincture = true;
                if ($always_proper && is_object($tinc->firstChild) && $tinc->firstChild->nodeName != 'proper') {
                    self::raiseError(self::ERROR_WARNING, "Charge $keyterm is always proper", $phrase);
                } else { 
                    $tinc->setAttribute(blazonML::A_INDEX, $tincCount++);
                    $mods[] = $tinc;
                    $foundTinc -= 1;
                    $this->matcher->syncOver(languageDB::ANDD);
                }
            } else {
                break;
            }
        }

        if ($tincCount == 1) { // i.e. none found
            if ( $always_proper ||  $optional_tincture) {
                $tinc = $this->schema->makeNode(blazonML::E_TINCTURE, array(blazonML::A_ORIGIN => 'given',
                    blazonML::A_INDEX => '1'));
                $tinc->appendChild($this->schema->makeNode(blazonML::E_PROPER));
                $mods[] = $tinc;
            } else {
                $mods[] = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_ORIGIN => 'pending',
                    blazonML::A_INDEX => '1'));
            }
        }
        return $chargeNode;
    }

    protected function objects()
    {
        $node = $this->schema->makeNode(blazonML::E_OBJECTS);
        $assume_num = false;
        $found = false;

        $phrase = self::newPhrase();

        while (self::moreInput()) {
            if (self::findLex(languageDB::MOT_ET)) $assume_num = true;
            if ($item = self::ordinary()) {
                $found = true;
                $node->appendChild($item);
                if (self::semicolon()) break;
            } elseif ( $item = self::simpleCharge($assume_num)) {
                $found = true;
                $assume_num = false; // reset if it was true
                $node->appendChild($item);
            } else {
                break;
            }
        }
        if (!$found) {
            $node = null;
            self::discardPhrase($phrase);
        }
        return $node;
    }

    protected function linetype() // DONE
    {
        $mod = null;
        // Look for simple linetype modifiers (e.g. "dancetty")
        if ($lexeme = self::findLex(languageDB::LIGNE)) {
            $mod = $this->schema->makeModFromLexeme($lexeme, blazonML::A_LINETYPE);
        }
        return $mod;
    }

    protected function ordinary() // DONE
    {
        $phrase = self::newPhrase();
        $ord = null;

        // Look for a number (or assume one)
        $lexeme = self::findLex(languageDB::QUANTITE);
        if ($lexeme == null) {
            $lexeme = self::findLex(languageDB::NOMBRE);
        }
        if (!$lexeme) {
            $ordNum = 1;
        } else {
            $ordNum = $lexeme->keyterm;
        }
        // Now look for things that are definitely ordinaries
        $lexeme = self::findLex(languageDB::ORDINAIRE);
        if ($lexeme != null) {
           $ord = $this->schema->makeNodeFromLexeme(blazonML::E_ORDINARY, $lexeme, array ( blazonML::A_NUMBER => "$ordNum"));
        } else { // no ordinary found, so backtrack.
            self::discardPhrase($phrase);
            return null;
        }

        $foundTinc = false;
        $tincIndex = 1;
        while (self::moreInput()) { // Look for prefixes again, just in case, and other common ordinary modifiers (e.g. "a bend sinister")
            // line types
            if ($mod = self::linetype()) {
                $ord->appendChild($mod);
            } elseif ($tinc = self::tincture(false)) {
                $tinc->setAttribute(blazonML::A_INDEX,"$tincIndex");
                $tincIndex++;
                $ord->appendChild($tinc);
                $foundTinc = true;
            } else break;
        }
        // Have we found a colour yet?
        if (!$foundTinc) {
            $ord->appendChild($this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1', blazonML::A_ORIGIN => 'pending')));
        }
        return $ord;
    }

    protected function fur() 
    {
        $phrase = self::newPhrase();
        $phrase->add(self::findLex(languageDB::MOT_DE));
        $fur = $phrase->add(self::findLex(languageDB::FOURRURE));
        if ($fur == null) {
            self::discardPhrase($phrase);
            return null;
        }
        return $this->schema->makeNodeFromLexeme(blazonML::E_FUR,
            $phrase->create());

    }

    protected function colour($allowProper = true)
    {
        global $options;

        $phrase = self::newPhrase();
        $phrase->add(self::findLex(languageDB::MOT_DE));
        // proper is also a valid colour
        if ($phrase->add(self::findLex(languageDB::NATUREL))) {
            if ($allowProper) {
                return $this->schema->makeNodeFromLexeme(blazonML::E_PROPER, $phrase->create() );
            } else {
                self::raiseError(self::ERROR_ORIGIN, "Proper not valid, specify colour", true);
                return null;
            }
        }
        $colour = $phrase->add(self::findLex(languageDB::COULEUR));
        // FFS! Have we found anything yet? If not, give up
        if ($colour == null) {
            self::discardPhrase($phrase);
            return null;
        }
        return $this->schema->makeNodeFromLexeme(blazonML::E_COLOUR, $phrase->create() );
    }

  protected function number() { // DONE
      if (($num = self::findLex(languageDB::NOMBRE)) == null)
          return null;
      if ($num->keyterm == 'bignum')
          $number = intval($num->tokens);
      else
          $number = $num->keyterm;
      return $number;
    }

  protected function treatment() { // DONE
        $phrase = self::newPhrase();
    $treatment = $phrase->add(self::findLex(languageDB::TRAITEMENT));
    if ( $treatment == null ) {
        self::discardPhrase($phrase);
      return null;
    }
    $node = $this->schema->makeNodeFromLexeme(blazonML::E_TREATMENT, $phrase->create($treatment->keyterm));
    return $node;
  }

  // Look for a tincture, return a pending tincture if not found
  protected function tincture($required=false, $counterchange = "allow") { // DONE

    $found = true;
    $colourOrFur = null;
    $phrase = self::newPhrase();
    $node = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
        blazonML::A_ORIGIN => 'given'));

      //  Look for a reference to proper
    if ( $lexeme = self::findLex(languageDB::NATUREL) ) {
        $proper = $this->schema->makeIDNodeFromLexeme(blazonML::E_PROPER, $lexeme);
        $node->appendChild($proper);
    } elseif ( ($colourOrFur = self::colour(false))
        || ($colourOrFur = self::fur())) {
      // This could be [colour] [treatment] [colour],
      // unless it is followed by a semi colon
        if ( ($treat = self::treatment()) ) {
          $keyterm = $treat->getAttribute('keyterm');
          $tinc1 = $this->schema->makeIDNode(blazonML::E_TINCTURE, array ( blazonML::A_INDEX => '1'));
          $tinc1->appendChild($colourOrFur);
          $colourOrFur = null; // used
          $treat->appendChild($tinc1);
          $this->matcher->syncOver(languageDB::MOT_ET);
          $tinc2 = self::tincture(false);
          if (is_null($tinc2)) {
              $tinc2 = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
                  blazonML::A_ORIGIN => 'missing'));
          }
          if ($tinc2 != null) {
              $tinc2->setAttribute(blazonML::A_INDEX, '2');
              $treat->appendChild($tinc2);
          }
          $node->appendChild($treat);
          // Could also be [colour] [semyde] [charge]
        } elseif ( $lexeme = self::findLex(languageDB::SEME) ) {
          $semyde = $this->schema->makeNodeFromLexeme(blazonML::E_SEMYDE, $lexeme);
          $tinc1 = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
              blazonML::A_ORIGIN => 'given'));
          $tinc1->appendChild($colourOrFur);
          $colourOrFur = null; // used
          $semyde->appendChild($tinc1);
          if ( ($charge = self::simpleCharge( true ))) { // Do not worry if no number given
            $semyde->appendChild($charge);
          } else {
            $semyde->appendChild($this->schema->makeNode(blazonML::E_MISSING));
          }
          // we also allow a second charge, which MUST be preceeded by AND
            $phrase2 = self::newPhrase();
            if (!self::semicolon() && self::findLex(languageDB::MOT_ET)) {
                if ($charge2 = self::simpleCharge(true)) {
                    $semyde->appendChild($charge2);
                } else {
                    self::discardPhrase($phrase2);
                }
            }
          $node->appendChild($semyde);
      }
      // Look for [treatment] [colour] [colour]
    } elseif ( ($treat = self::treatment()) ) {
        $this->matcher->syncOver();
      $keyterm = $treat->getAttribute('keyterm');
      $treatTinc1 = self::tincture(false);
      if (is_null($treatTinc1)) {
          $treatTinc1 = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
              blazonML::A_ORIGIN => 'missing'));
      }
      $treat->appendChild($treatTinc1);
      $tinc2 = self::tincture(false); // grady might not need second tincture
        if (is_null($tinc2)) {
            $tinc2 = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
                blazonML::A_ORIGIN => 'missing'));
        }
      if ($tinc2 != null) {
          $tinc2->setAttribute('index', 2);
          $treat->appendChild($tinc2);
      }
      $node->appendChild($treat);
      /*
       * Uncomment after implement division
       */
      // Look for counterchange
      // Look for division
    // } elseif ( ($div1 = self::division($counterchange)) ) {
    //   $node->appendChild($div1);
      // Look for a colour or a fur
    } else { // Didn't find anything
      self::discardPhrase($phrase);
      $found = false;
    }
    if ( $colourOrFur ) {
        $node->appendChild($colourOrFur); // we have a colour or fur on its own
        $this->foundColours[] = $node->cloneNode(true);
    }
    if ( $node == null && $required ) {
        $node = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
            blazonML::A_ORIGIN => 'pending'));
    } 
    return $node;
  }
}

