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

class englishGrammar extends grammar
{
    private $previousTypes = [];

    /**
     * @return DOMElement
     */
    public function getShield($syncTo = true) // DONE
    {

        $complex = null;
        $node = null;
        $shields = [];
        $achievementList = [];
        // Try to find complete shield
        $this->matcher->syncOver();
        self::findLex(languageDB::BOT_COMMAND);
        while (true) { // This is the loop for >1 shield per achievement
            $this->matcher->syncOver();
            $preInstructions = self::drawn(); // allow shapes etc. to be given first, if required
            $this->matcher->syncOver(languageDB::SEMICOLON);
            $node = self::shield($syncTo);
            if ($preInstructions)
                $node->appendChild($preInstructions);
            self::semicolon();
            if (($overall = self::overall(true)) != null)
                $node->appendChild($overall);
            // Look for things that _can_ be part of the shield
            if (($shieldParts = $this->achievement(false)) != null) {
                $node->appendChild($shieldParts);
            }
            // if not already found drawing instructions look again
            if (!$preInstructions && ($instructions = self::drawn()) != null) {
                $node->appendChild($instructions);
            }
            $shields[] = $node;
            // Found one shield, maybe with instructions and stuff, look for another
            $alongside = self::findLex(languageDB::ALONGSIDE);
            if (is_null($alongside)) {
                break;
            }
            $complex = $this->schema->makeNodeFromLexeme(§blazonML::E_COMPLEX, $alongside);
        }
        // Found all the shields, now look for achievements (if any)
        while (($achievement = $this->achievement(true)) != null) {
            $achievementList[] = $achievement;
        }
        // and any instructions that come after the final achievements
        $instructions = self::drawn();

        // and finally look for a caption
        $caption = null;
        if (self::findLex(languageDB::CAPTION)) {
            $caption = self::findLex(languageDB::VALUE);
        }

        // Report anything left that we don't understand
        if ($syncTo) $this->matcher->syncTo(array('NOT_A_CATEGORY'));

        if (is_null($complex)) {
            foreach ($achievementList as $achievement ) {
                if (!is_null($achievement) && $achievement->hasChildNodes()) {
                    $node->appendChild($achievement);
                }
            }
            if (!is_null($instructions) && $instructions->hasChildNodes()) {
                $node->appendChild($instructions);
            }
            if (!is_null($caption)) {
                $node->setAttribute(blazonML::A_CAPTION, $caption->value);
            }
            return $node;
        } else {
            $index = 1;
            foreach($shields as $shield) {
                $shield->setAttribute(blazonML::A_INDEX, $index++);
                $complex->appendChild($shield);
            }
            foreach ($achievementList as $achievement ) {
                if (!is_null($achievement) && $achievement->hasChildNodes()) {
                    $complex->appendChild($achievement);
                }
            }
            if (!is_null($instructions) && $instructions->hasChildNodes()) {
                $complex->appendChild($instructions);
            }
            if (!is_null($caption)) {
                $node->setAttribute(blazonML::A_CAPTION, $caption->value);
            }
            return $complex;
        }
    }

    protected function achievement($allowAll = false) // DONE
    {
        // switch between only things on a shield and everything
        $crests = [];
        $helmets = [];
        $torses = [];
        $crestHeight = 'no-crest';
        $achievement = $this->schema->makeNode(blazonML::E_ACHIEVEMENT);
        $thisHelmet = null;
        $thisCrest = null;
        $thisTorse = null;

        // All the following can only occur once per achievement (but can cause a new "outer" achievement to be created)
        $singleItems = [];

        do { // Order is important here - look for charge LAST so as not to confuse with helmet or torse
            $found = false;
            if (!in_array('mantling', $singleItems) && ($mantle = self::mantling()) != null) {
                $found = true;
                $singleItems[] = 'mantling';
                $achievement->appendChild($mantle);
            } elseif (!in_array('supports', $singleItems) && ($support = self::supporters()) != null) {
                $found = true;
                $achievement->appendChild($support);
                $singleItems[] = 'supports';
            } elseif (($motto = self::motto()) != null) {
                $found = true;
                $achievement->appendChild($motto);
            } elseif (!in_array('ornament', $singleItems) && (($ornament = self::ornaments()) != null)) {
                $found = true;
                // this might just be a marker for helmets etc. or it might be a group
                if (is_object($ornament)) {
                    $achievement->appendChild($ornament);
                    $singleItems[] = 'ornament';
                }
            } elseif (($crest = self::findLex(languageDB::CREST)) != null) {
                $found = true;
                if ($crest->keyterm == 'no-crest') { // this helmet has no crest, move on
                    $thisCrest = 'no-crest';
                } else { // if ($crest == 'crest')
                    if (!is_null($thisCrest)) { // already go a crest, so move on
                        $crests[] = $thisCrest;
                        $torses[] = $thisTorse; $thisTorse = null;
                        $helmets[] = $thisHelmet; $thisHelmet = null;
                    }
                    $thisCrest = $this->schema->makeNodeFromLexeme(blazonML::E_CREST, $crest,
                        array(blazonML::A_KEYTERM => 'group'));
                }
            } elseif (!in_array('compartment', $singleItems) && ($compartment = self::compartment()) != null) {
                $found = true;
                $achievement->appendChild($compartment);
                $singleItems[] = 'compartment';
            } elseif (($fringe = self::fringe()) != null) {
                $found = true;
                $achievement->appendChild($fringe);
            } elseif (($torse = self::torse()) != null) {
                $found = true;
                if (!is_null($thisTorse)) {
                    self::raiseError(self::ERROR_ORIGIN, "Only one torse per crest", $torse);
                } else {
                   $thisTorse = $torse;
                }
            } elseif (($helmet = self::helmet()) != null) {
                $found = true;
                if (!is_null($thisHelmet)) { // already got a helmet, so move on
                    $crests[] = $thisCrest; $thisCrest = null;
                    $torses[] = $thisTorse; $thisTorse = null;
                    $helmets[] = $thisHelmet;
                }
                $thisHelmet = $helmet;
            } elseif ($lexeme = self::findLex(languageDB::SHIELD)) {
                if (($adjust = $this->getAdjustments()) != null) {
                    $shieldNode = $this->schema->makeIDNodeFromLexeme(blazonML::E_SHIELD, $lexeme);
                    foreach ($adjust as $adj) {
                        $shieldNode->appendChild($adj);
                    }
                    $achievement->appendChild($shieldNode);
                }
            } elseif (($charge = self::charge(true,true,false)) != null) {
                $found = true;
                if (is_null($thisCrest)) {
                    $thisCrest = $this->schema->makeNode(blazonML::E_CREST,
                        array(blazonML::A_KEYTERM => 'group'));
                }
                $thisCrest->appendChild($charge);
            } elseif (!in_array('achievement', $singleItems) && self::findLex(languageDB::ACHIEVEMENT)) {
                    // this is OK, it is just a marker between the shield and the achievements
                    $singleItems[] = 'achievement';
                    $found = true;
            }
            if ($found) {
                $this->matcher->syncOver(array(languageDB::ANDD, languageDB::WITH));
            }
        } while ($found);
        // Finish off any pending crest bits
        if (!is_null($thisHelmet) || !is_null($thisCrest) || !is_null($thisTorse)) {
            $crests[] = $thisCrest;
            $helmets[] = $thisHelmet;
            $torses[] = $thisTorse;
        }
        // Now sort all the crests, helmets and torses into groups (up to 3)
        $numCrests = max(count($crests), count($helmets), count($torses));
        if ($numCrests) $crestHeight = 'short-crest'; // unless we find out more later
        for ($i = 0; $i < $numCrests; $i++) {
            // replace explicit no crest with null
            if (isset($crests[$i]) && $crests[$i] == 'no-crest') $crests[$i] = null;
            if (isset($crests[$i]) && is_object($crests[$i]) && !$crests[$i]->hasChildNodes()) $crests[$i] = null;
            if (isset($crests[$i]) && isset($helmets[$i])) {
                $crestHeight = 'tall-crest';
            }
            if (!isset($crests[$i])) {
                $crests[$i] = $this->schema->makeNode(blazonML::E_CREST,
                    array(blazonML::A_KEYTERM => 'group'));
            }
            if (isset($helmets[$i])) {
                $crests[$i]->appendChild($helmets[$i]);
            }
            if (isset($torses[$i])) {
                $crests[$i]->appendChild($torses[$i]);
            }
            if ($crests[$i]->hasChildNodes()) $achievement->appendChild($crests[$i]);
        }
        if ($achievement->hasChildNodes()) {
            $achievement->setAttribute(blazonML::A_KEYTERM, $crestHeight);
            return $achievement;
        } // else
        return null;
    }

    protected function overall($meta = false) // DONE
    {
        $node = null;
        if ($meta)
            $lookFor = languageDB::METAOVERALL;
        else
            $lookFor = languageDB::OVERALL;

        while (!is_null($overall = self::findLex($lookFor))) {
            if ($node == null) $node = $this->schema->makeNodeFromLexeme(blazonML::E_OVERALL, $overall, array('keyterm' => 'overall'));
            $this->matcher->syncOver();
            if (($over = self::objects()) == null) {
                self::raiseError(self::ERROR_ORIGIN, 'Nothing to go "overall"', true);
                return null;
            }
            $node->appendChild($over);
        }
        return $node;
    }

    // To add more debug options add them to lexicon.inc under drawn mods
    // then add the keyword under case 'grid':
    protected function drawn() // DONE
    {
        $found = true;
        $debugOptions = [];
        $instructions = $this->schema->makeNode(blazonML::E_INSTRUCTIONS);
        while ($found) {
            $phrase = self::newPhrase();
            $found = false;
            $phrase->add(self::findLex(languageDB::DRAWN));
            $phrase->add(self::findLex(languageDB::USING));
            $phrase->add(self::findLex(languageDB::WORD_IN));
            $phrase->add(self::findLex(languageDB::WORD_ON));
            $phrase->add(self::findLex(languageDB::SIGNIFIER));
            if ($option = $phrase->add(self::findLex(LanguageDB::DRAWN_MODS))) {
                // $phrase->add(self::findLex(languageDB::USING));
                $elementName = null;
                $value = strtolower($option->value ?? "");
                switch ($option->keyterm) {
                    case 'colours':
                        $elementName = blazonML::E_COLOURSET;
                        break;
                    case 'palette':
                        $elementName = blazonML::E_PALETTE;
                        break;
                    case 'style':
                        $elementName = blazonML::E_EFFECT;
                        break;
                    case 'shape':
                        $elementName = blazonML::E_SHAPE;
                        break;
                    case 'grid':
                        $debugOptions[] = $option;
                        break;
                    case 'aspect':
                        // only need the ratio
                        continue 2;
                    case 'ratio':
                        $elementName = blazonML::E_ASPECT;
                        $value = $option->tokens;
                        break;
                }
                if (!is_null($elementName)) {
                    $node = $this->schema->makeNodeFromLexeme($elementName, $phrase->create($value));
                    $instructions->appendChild($node);
                }
                $found = true;
                $this->matcher->syncOver(languageDB::ANDD);
            } elseif (($trad = $phrase->add(self::findLex(languageDB::TRADITION_PREFIX))) != null) {
                if (is_null($phrase->add(self::findLex(languageDB::TRADITION_DESCRIPTION)))) {
                    self::discardPhrase($phrase);
                    return null;
                }
                $phrase->add(self::findLex(languageDB::WORD_OF));
                $node = $this->schema->makeNodeFromLexeme(blazonML::E_SHAPE, $phrase->create($trad->keyterm ));
                $instructions->appendChild($node);
                $this->matcher->syncOver(languageDB::ANDD);
                $found = true;
            } else {
               self::discardPhrase($phrase);
            }
        }
        if (count($debugOptions)) {
            $node = $this->schema->makeNode(blazonML::E_DEBUG, array(
                blazonML::A_KEYTERM => implode(',', $debugOptions)) );
            $instructions->appendChild($node);
        }
        if ($instructions->hasChildNodes()) {
            return $instructions;
        } // else
        return null;
    }

    private function chief_on($chief_ordinary)
    {
        $xpath = new DOMXPath($chief_ordinary->ownerDocument);
        $on_list = $xpath->query("modifier[@type='wordon']", $chief_ordinary);
        if ( count($on_list) != 0 )
            return $on_list[0];

        $mod = $this->schema->makeMod(languageDB::WORD_ON, null, "on", null, null);
        $chief_ordinary->appendChild($mod);
        return $mod;
    }

    protected function adjust_chief($simple, $ordChgs)
    {
        foreach ( $ordChgs->childNodes as $oc )
        {
            if ( $oc->nodeName == blazonML::E_ORDINARY && $oc->getAttribute(blazonML::A_KEYTERM) == 'chief' )
            {
                $chief = $this->schema->makeNode("chief");
                $ordChgs->removeChild($oc);
                $chief->appendChild($oc);
                $simple->appendChild($chief);
                $on = null;
                for ( $i = 0; $i < count($ordChgs->childNodes); )
                {
                    $child = $ordChgs->childNodes[$i];
                    $pos = getModifierByType($child, 'position', false);
                    $arr = getModifierByType($child, 'arrangement', false);
                    if ( $pos == "inchief" || $arr == "inchiefthrough" )
                    {
                        $ordChgs->removeChild($child);
                        if ( $on == null )
                            $on = $this->chief_on($oc);
                        $on->appendChild($child);
                    }
                    else
                    {
                        $i++;
                    }
                }

                return $chief;
            }
        }

        return null;
    }

    protected function simple() // DONE
    {
        $phrase = self::newPhrase();
        $phrase->add(self::findLex(array(languageDB::ONFIELDOF, languageDB::WITH)));

        $node = null;
        $chief = null;
        if (($tinc = self::tincture()) != null) {
            $field = $this->schema->makeNode(blazonML::E_FIELD);
            $simple = $this->schema->makeNode(blazonML::E_PLAIN);
            $this->matcher->syncOver();
            $field->appendChild($tinc);
            $ordChgs = self::objects();
            $simple->appendChild($field);
            if ( $ordChgs ) {
                 $chief = $this->adjust_chief($simple, $ordChgs);
                if ( $ordChgs->hasChildNodes() )
                    $simple->appendChild($ordChgs);
            }
            $node = $this->schema->makeIDNodeFromLexeme(blazonML::E_SHIELD,
                $phrase->create(languageDB::ONFIELDOF), array(blazonML::A_INDEX => '0'));
            $node->appendChild($simple);
            if ($ordChgs && !self::semicolon(true) && (($adjust = $this->getAdjustments()) != null)) {
                foreach ($adjust as $adj) {
                    $node->appendChild($adj);
                }
            }
        }
        if ($node == null)
            self::discardPhrase($phrase);
        return $node;
    }

    protected function shield($syncTo = true) // DONE
    {
        $this->matcher->syncOver();
        $checkOverAllParts = false;
        $node1 = null;
        // This loop discards things until we find a dang shield!
        while (self::moreInput()) {
            $node1 = null;
            if ($node1 == null) $node1 = self::quartered();
            if ($node1 != null) { // found a quartering, check for over all the parts
                $checkOverAllParts = true;
            }
            if ($node1 == null) $node1 = self::halved();
            if ($node1 == null) $node1 = self::simple();
            if (!is_null($node1)) break; // found something!
            // else, no we didn't. Chuck stuff away until we get something useful
            if ($syncTo) {
                $this->matcher->syncTo(array(
                    languageDB::COLOUR, languageDB::QUARTERED, languageDB::DIVISION,
                    languageDB::FUR, languageDB::TREATMENT
                ));
            }
        }

        // Okay, we really couldn't find one...
        if ($node1 == null) {
            $node1 = $this->schema->makeIDNode(blazonML::E_SHIELD);
            $node1->appendChild($this->schema->makeNode(blazonML::E_MISSING));
            self::raiseError(self::ERROR_ORIGIN, 'Expected quartering or tincture');
            if (!$syncTo) {
                return $node1;
            }
        }
        $this->matcher->syncOver();
        if ($checkOverAllParts && ($overall = self::overall(true)) != null) {
            $node1->appendChild($overall);
        } elseif (!self::semicolon() && ($overall = self::overall()) != null) {
            $node1->appendChild($overall);
        }
        $retval = $node1;

        // Now do it all again, in case of "A impaled with B"
        $this->matcher->syncOver();
        if (($split = self::findLex(languageDB::SPLITS)) != null) {
            // As above, this loop is fairly aggressive about finding a field
            while (self::moreInput()) {
                $node2 = null;
                if ($node2 == null) $node2 = self::quartered();
                if ($node2 == null) $node2 = self::halved();
                if ($node2 == null) $node2 = self::simple();
                if (!is_null($node2)) break; // found something!
                // else, no we didn't. Chuck stuff away until we get something useful
                $this->matcher->syncTo(array(
                    languageDB::COLOUR, languageDB::QUARTERED, languageDB::DIVISION,
                    languageDB::FUR, languageDB::TREATMENT
                ));
            }
            // Okay, we really couldn't find one...
            if ($node2 == null) {
                $node2 = $this->schema->makeIDNode(blazonML::E_SHIELD);
                $node2->appendChild($this->schema->makeNode(blazonML::E_MISSING));
                self::raiseError(self::ERROR_ORIGIN, 'Expected quartering or tincture');
            }
            $this->matcher->syncOver();
            if ($checkOverAllParts && ($overall = self::overall(true)) != null) {
                $node2->appendChild($overall);
            } elseif (!self::semicolon() && ($overall = self::overall()) != null) {
                $node2->appendChild($overall);
            }
            // Got two nodes, now create a mother node
            $motherNode = $this->schema->makeIDNode(blazonML::E_SHIELD, array(blazonML::A_INDEX => '0'));
            $daughterNode = $this->schema->makeNodeFromLexeme(blazonML::E_COMPLEX, $split);
            $node1->setAttribute(blazonML::A_INDEX, '1');
            $node2->setAttribute(blazonML::A_INDEX, '2');
            $daughterNode->appendChild($node1);
            $daughterNode->appendChild($node2);
            $motherNode->appendChild($daughterNode);
            $retval = $motherNode;
        }
        $this->matcher->syncOver();
        if (($overall = self::overall()) != null)
            $retval->appendChild($overall);
        return $retval;
    }

    protected function getPositions() // DONE
    {
        // $positions = array();
        $phrases = [];
        while (true) {
            $specifiers = array();
            $locations = array();
            $quantifier = null;
            $phrase = self::newPhrase();

            $this->matcher->syncOver(languageDB::ANDD);
            if (!$phrase->add(self::findLex(languageDB::WORD_IN))) {
                break;
            }
            $phrase->add(self::findLex(languageDB::WORD_THE));
            while ($lexeme = $phrase->add(self::findLex(languageDB::SPECIFIER))) {
                $specifiers[] = $lexeme->keyterm;
                $this->matcher->syncOver(languageDB::ANDD);
            }
            while ($keyterm = $phrase->add(self::findLex(languageDB::LOCATION))) {
                $locations[] = $keyterm;
                $this->matcher->syncOver(languageDB::ANDD);
            }
            if ($quantifier = $phrase->add(self::findLex(languageDB::QUANTIFIER))) {
                $this->matcher->syncOver(languageDB::ANDD);
            }
            // Did we find anything?
            if (count($locations) == 0 && count($specifiers) == 0 && $quantifier == null) {
                self::discardPhrase($phrase);
                return null;
            }
            self::restoreAnd();
            // Yes, now try to combine them into positions
            if (!is_null($quantifier) && $quantifier->keyterm == 'quarter') { // one or more specific quarters
                $foundPos = false;
                foreach ($specifiers as $specifier) {
                    $newPosition = null;
                    switch ($specifier) {
                        case 'first':
                            $newPosition = 'inQ1';
                            break;
                        case 'second':
                            $newPosition = 'inQ2';
                            break;
                        case 'third':
                            $newPosition = 'inQ3';
                            break;
                        case 'fourth':
                            $newPosition = 'inQ4';
                            break;
                        case 'each':
                            if (count($locations) == 0) $newPosition = 'ineachQ';
                            break;
                        case 'dexter':
                            $newPosition = 'ineachQ13';
                            break;
                        case 'sinister':
                            $newPosition = 'ineachQ24';
                            break;
                        default:
                    }
                    if ($newPosition != null) {
                       //  $positions[] = $newPosition;
                        $phrases[] = $phrase->create($newPosition);
                        $foundPos = true;
                    }
                }
                if (!$foundPos) { // do we also have a chief or base location?
                    if (in_array('chief', $locations)) {
                        // $positions[] = 'ineachQ12';
                        $phrases[] = $phrase->create('ineachQ12');
                    } elseif (in_array('base', $locations)) {
                        // $positions[] = 'ineachQ34';
                        $phrases[] = $phrase->create('ineachQ34');
                    } else {
                        self::raiseError(self::ERROR_ORIGIN, 'Cannot find quarter for position', $phrase->create(languageDB::LOCATION));
                        $this->matcher->syncOver();
                    }
                }
            } elseif (count($locations) > 0) { // no quantifier found (or it was "all"), but we have a location
                foreach ($locations as $location) {
                    switch ($location->keyterm) {
                        //case 'flank': $positions[] = 'inflank'; break;
                        case 'fesspoint':
                            $phrases[] = $phrase->create('infesspoint');
                            break;
                        case 'nombril':
                            $phrases[] = $phrase->create('innombril');
                            break;
                        case 'abyss':
                            $phrases[] = $phrase->create('inabyss');
                            break;
                        case 'honpoint':
                            $phrases[] = $phrase->create('inhonpoint');
                            break;
                        case 'side':
                            /* if (in_array('dexter', $specifiers)) $positions[] = 'indexside';
                            if (in_array('sinister', $specifiers)) $positions[] = 'insinside';
                            if (in_array('upper', $specifiers)) $positions[] = 'inupperhalf';
                            if (in_array('lower', $specifiers)) $positions[] = 'inlowerhalf'; */
                            if (in_array('dexter', $specifiers)) $phrases[] = $phrase->create('indexside');
                            if (in_array('sinister', $specifiers)) $phrases[] = $phrase->create('insinside');
                            if (in_array('upper', $specifiers)) $phrases[] = $phrase->create('inupperhalf');
                            if (in_array('lower', $specifiers)) $phrases[] = $phrase->create('inlowerhalf');
                            break;
                        case 'flank':
                        case 'chief':
                        case 'base':
                            $count = count($phrases);
                            if (in_array('dexter', $specifiers)) $phrases[] = $phrase->create('in' . 'dex' . $location->keyterm);
                            if (in_array('sinister', $specifiers)) $phrases[] = $phrase->create('in' . 'sin' . $location->keyterm);
                            if (in_array('middle', $specifiers)) $phrases[] = $phrase->create('in' . 'mid' . $location->keyterm);
                            if ($count == count($phrases)) { // didn't find any specs
                                $keyterm = 'in' . $location->keyterm;
                                // $positions[] = $keyterm;
                                $phrases[] = $phrase->create($keyterm);
                            }
                            break;
                    }
                }
            } else { // no quantifier or location, need to have "each"
                if (in_array('each', $specifiers)) {
                    if (in_array('first', $specifiers)) {
                        // $positions[] = 'ineach1st';
                        $phrases[] = $phrase->create('ineach1st');
                    } elseif (in_array('second', $specifiers)) {
                        // $positions[] = 'ineach2nd';
                        $phrases[] = $phrase->create('ineach2nd');
                    } else {
                        // $positions[] = 'ineach';
                        $phrases[] = $phrase->create('ineach');
                    }
                } else {
                    self::raiseError(self::ERROR_ORIGIN, 'Do not understand position', $phrase->create(languageDB::LOCATION));
                    $this->matcher->syncOver();
                }
            }
        }
        if (count($phrases) > 0) {
            $posArray = array();
            for ($i = 0; $i < count($phrases); $i++) {
                $posArray[] = $this->schema->makeModFromLexeme($phrases[$i], languageDB::POSITION);
            }
            return $posArray;
        } else
            return null;
    }

    public function getRoll() {
        $phrase = self::newPhrase();

        $phrase->add(self::findLex(languageDB::SIGNIFIER));
        if (!$phrase->add(self::findLex(languageDB::ROLL))) {
            self::discardPhrase($phrase);
            return null;
        }
        $phrase->add(self::findLex(languageDB::ROLL)); // allow, e.g. armorial roll
        $background = null;
        $title = null;
        $numCols = 3;
        do {
            $found = false;
            $phrase->add(self::findLex(languageDB::WITH));
            $phrase->add(self::findLex(languageDB::WORD_OF));
            $num = $phrase->add(self::findLex(languageDB::NUMBER));
            if (!is_null($num)) {
                $phrase->add(self::findLex(languageDB::PIECES));
                $numCols = $num->keyterm;
                $found = true;
            } elseif ($phrase->add(self::findLex(languageDB::BACKGROUND))) {
                $phrase->add(self::findLex(languageDB::WORD_OF));
                $background = self::tincture(true);
                $found = true;
            } elseif ($phrase->add(self::findLex(languageDB::TITLE))) {
                $title = $phrase->add(self::findLex(languageDB::VALUE));
                $found = true;
            }
        } while ($found);
        $phrase->add(self::findLex(languageDB::CONSISTING));
        $phrase->add(self::findLex(languageDB::WORD_OF));
        $node = $this->schema->makeNodeFromLexeme(blazonML::E_ROLL, $phrase->create());
        $node->setAttribute(blazonML::A_COLS, $numCols);
        if (!is_null($background)) {
            $node->appendChild($background);
        }
        if (!is_null($title)) {
            $node->setAttribute(blazonML::A_TITLE, $title->value); // default columns
        }

        $anotherShield = true;
        while($anotherShield && self::moreInput()) {
            if ($anotherShield) {
                $node->appendChild(self::getShield(false));
            }
            $anotherShield = false;
            while ($then = self::findLex(array(languageDB::NEXT, languageDB::SKIP))) {
                $anotherShield = true;
                if ($then->keyterm == languageDB::SKIP) {
                    $skipNum = self::findLex(languageDB::NUMBER);
                    $skips = is_null($skipNum) ? 1 : intval($skipNum->keyterm);
                    for ($i = 0; $i < $skips; $i++) {
                        $node->appendChild($this->schema->makeNode(blazonML::E_MISSING));
                    }
                }
                while (self::findLex(languageDB::NEXT)) {};
            }
        }
        $this->matcher->syncTo(array('NOT_A_CATEGORY'));

        return $node;

    }

    protected function getCrowns($foundTinc) // DONE
    {
        $crownType = 'default'; // default crown
        $modifierType = '';
        $phrase = self::newPhrase();
        while ($crown = $phrase->add(self::findLex(languageDB::CROWN_MODS))) {
            $modifierType = '';
            switch ($crown->keyterm) {
                case 'wreathed':
                    $crownType = 'wreath';
                    $modifierType = 'crowned';
                    break;
                case 'crowned':
                case 'gorged':
                    $modifierType = $crown->keyterm;
                    break;
                case 'crown': // just ignore (as in gorged with a naval crown
                default:
                    $crownType = $crown->keyterm;
                    break;
            }
        }
        if ($modifierType == '') { // TODO here, $crown can be null....?
            self::discardPhrase($phrase);
            return null;
        }
        $modifier = $this->schema->makeModFromLexeme($phrase->create($modifierType), languageDB::CROWN_MODS, $crownType);
        if ($foundTinc and ($tinc = self::tincture()))
            $modifier->appendChild($tinc);
        return $modifier;
    }

    protected function half_charge() {
        $phrase = self::newPhrase();
        if ($spec = $phrase->add(self::findLex(languageDB::SPECIFIER))) {
            if (!($half = $phrase->add(self::findLex(languageDB::HALF)))) {
                self::discardPhrase($phrase);
                return null;
            } // else
            $phrase->add(self::findLex(languageDB::WORD_OF));
            return $phrase->create($spec->keyterm . '-' . $half->keyterm);
        }
        return null;
    }

    protected function charge_prefix() // DONE
    {
        $phrase = self::newPhrase();
        $phrase->add(self::findLex(languageDB::SIGNIFIER));
        if (($keyterm = self::findLex(languageDB::CHARGE_PREFIX)) != null) {
            return ($this->schema->makeModFromLexeme($keyterm, languageDB::CHARGE_MODS));
        } elseif (($trad = $phrase->add(self::findLex(languageDB::TRADITION_PREFIX))) != null) {
            if (is_null($phrase->add(self::findLex(languageDB::TRADITION_DESCRIPTION)))) {
               self::discardPhrase($phrase);
               return null;
            }
            $phrase->add(self::findLex(languageDB::WORD_OF));
            return ($this->schema->makeModFromLexeme($phrase->create($trad->keyterm), languageDB::TRADITION_PREFIX));
        } elseif ($halfCharge = self::half_charge()) {
            return ($this->schema->makeModFromLexeme($halfCharge, languageDB::CHARGE_MODS));
        }
        return null;
    }

    protected function simpleCharge($assumeNum = false, $allowArr = false, $allowPos = false) // DONE
    {
        $phrase = self::newPhrase();
        $mods = array();
        // Look for a number
        $chgNum = $phrase->add(self::findLex(languageDB::QUANTITY));
        if ($chgNum === null) $chgNum = $phrase->add(self::findLex(languageDB::NUMBER));
        if ($chgNum === null) {
            if ($assumeNum)
                $chgNum = $phrase->add(new Lexeme( languageDB::NUMBER, '1','','',''));
            else {
                self::discardPhrase($phrase);
                return null;
            }
        }
        $phrase2 = self::newPhrase();
        // Look for words indicating a multiple charge (e.g. "2 bundles of arrows")
        if ($bundle = $phrase2->add(self::findLex(languageDB::BUNDLE))) {
            // Is the number made explicit?
            $number = $phrase2->add(self::findLex(languageDB::NUMBER));
            if (is_null($number)) {
                $num = ($bundle->keyterm == 'pair') ? '2' : '3';
            } else {
                $num = $number->keyterm;
            }
            $mods[] = $this->schema->makeModFromLexeme($phrase2->create(languageDB::BUNDLE), languageDB::BUNDLE, $num);
        }
        // Look for prefixes, (e.g. "a demi-lion")
        while (($mod = $this->charge_prefix()) != null)
            $mods[] = $mod;
        // Look for ordinaries that we might confuse with charges
        if (self::findLex(languageDB::ORDINARY_NOT_CHARGE)) {
            self::discardPhrase($phrase);
            return null;
        }
        // Now look for actual charges
        // TODO in herald knowledge replace special/same
        $chargeLex = $phrase->add(self::findLex(languageDB::CHARGE_NOT_ORDINARY));
        if (is_null($chargeLex) && $chgNum->keyterm > 1) {
            $chargeLex = $phrase->add(self::findLex(languageDB::ORDINARY_OR_CHARGE));
        }
        if (is_null($chargeLex)) {
            $chargeLex = $phrase->add(self::findLex(languageDB::CHARGE));
        }
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
        // In some cases we want the next input token as a "value" (e.g. "the word Karl" matches "words? *")
        if (!is_null($chargeLex->value))
            $mods[] = $this->schema->makeMod('value', $chargeLex->value, 'value');
        if ($keyterm == 'special/flag') {
            $keyterm = "special/escutcheon";
            $chargeNode->setAttribute("keyterm", $keyterm);
            $mods[] = $this->schema->makeMod('chargemod', null, 'shape-flag');
            $phrase3 = self::newPhrase();
            $phrase3->add(self::findLex(languageDB::USING));
            if ( $option = $phrase3->add(self::findLex(LanguageDB::DRAWN_MODS) )) {
                $value = strtolower($option->value);
                if ( $option->keyterm == "aspect" ) {
                    $mods[] = $this->schema->makeNodeFromLexeme(blazonML::E_ASPECT, $phrase3->create('aspect'), array(
                        blazonML::A_KEYTERM => $value)
                    );
                }
            }
        }
        if ($keyterm == 'special/escutcheon') {
            $this->matcher->syncOver(languageDB::BLAZONED);
            $mods[] = self::shield();
            if ($adjust = self::getAdjustments()) {
                $mods = array_merge($mods, $adjust);
            }
        }

        // Look for bundle again (e.g. "two arrow bundles"
        if ($bundle = self::findLex(languageDB::BUNDLE))
            $mods[] = $this->schema->makeModFromLexeme($bundle, languageDB::BUNDLE);
        // Now look for postfix stuff, including tinctures and arrangements
        $foundTinc = intval($chgNum->keyterm);
        if ($foundTinc == 0) $foundTinc = 1;
        $tincCount = 1;
        $gotMainTincture = false;
        while (self::moreInput()) {
            if (self::semicolon()) {
                break;
            }
            $this->matcher->syncOver();
            $phrase4 = self::newPhrase();
            $group = self::findLex(languageDB::GROUP);
            if ($chargeMods = self::findLex(languageDB::CHARGE_MODS))
                $mods[] = $this->schema->makeModFromLexeme($chargeMods, languageDB::CHARGE_MODS );
            elseif ($chargeMods = self::ofNumber())
                $mods[] = $this->schema->makeModFromLexeme($chargeMods, languageDB::CHARGE_MODS );
            elseif ($crowns = self::getCrowns($gotMainTincture))
                $mods[] = $crowns;
            elseif ($allowPos and $positions = self::getPositions())
                $mods = array_merge($mods, $positions);
            elseif ($allowArr and $arrangement = self::arranged())
                $mods[] = $this->schema->makeModFromLexeme($arrangement, languageDB::ARRANGEMENT);
            elseif ($num = $phrase4->add(self::findLex(languageDB::NUMBER))) {
                // This might be rows, but must be >1 number
                $this->matcher->syncOver(languageDB::ANDD);
                $rows = array($num->keyterm);
                $count = 1;
                while (($num = $phrase4->add(self::findLex(languageDB::NUMBER)))) {
                    $count += 1;
                    $rows[] = $num->keyterm;
                    if (self::semicolon(true)) break;
                    $this->matcher->syncOver(languageDB::ANDD);
                }
                if ($count == 1) {
                    self::discardPhrase($phrase4);
                    break;
                } elseif ($count > 1) {
                    $mods[] = $this->schema->makeModFromLexeme(
                        $phrase4->create('arrange-by-rows'), 'rows',  implode(',', $rows));
                }
            } elseif ($foundTinc > 0 && ($tinc = self::tincture())) {
                $gotMainTincture = true;
                if ($always_proper && is_object($tinc->firstChild) && $tinc->firstChild->nodeName != 'proper') {
                    self::raiseError(self::ERROR_ORIGIN, 'Charge is always proper', $phrase);
                } else {
                    $tinc->setAttribute(blazonML::A_INDEX, $tincCount++);
                    $mods[] = $tinc;
                    $foundTinc -= 1;
                }
            } elseif ($chargeFeatures = self::findLex(languageDB::CHARGE_FEATURES)) {
                $feature = $this->schema->makeModFromLexeme($chargeFeatures, blazonML::V_FEATURE);
                if (($gotMainTincture || $always_proper) && ($colour = self::colour())) {
                    $tinc = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_ORIGIN => 'given',
                        blazonML::A_INDEX => '1'));
                    $tinc->appendChild($colour);
                    $feature->appendChild($tinc);
                } else {
                    $feature->appendChild($this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_ORIGIN => 'pending',
                        blazonML::A_INDEX => '1')));
                }
                $mods[] = $feature;
                $this->matcher->syncOver(languageDB::ANDD);
                // Look for non-standard adjusments
            } elseif ($adjustments = self::getAdjustments()) {
                $mods = array_merge($mods, $adjustments);
            } elseif ($lexeme = self::findLex(languageDB::FONT)) {
                $mods[] = $this->schema->makeModFromLexeme($lexeme, languageDB::FONT);
            } else {
                $this->matcher->syncOver();
                break;
            }
            $this->matcher->syncOver();
        }

        if ($tincCount == 1) { // i.e. none found
            if ($always_proper || $optional_tincture) {
                $tinc = $this->schema->makeNode(blazonML::E_TINCTURE, array(blazonML::A_ORIGIN => 'given',
                    blazonML::A_INDEX => '1'));
                $tinc->appendChild($this->schema->makeNode(blazonML::E_PROPER));
                $mods[] = $tinc;
            } else {
                $mods[] = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_ORIGIN => 'pending',
                    blazonML::A_INDEX => '1'));
            }
        }
        $this->matcher->syncOver(languageDB::WITH);
        foreach ($mods as $mod)
            $chargeNode->appendChild($mod);
        return $chargeNode;
    }


    protected function getAdjustments() // DONE
    {
        $adjustments = array();
        $foundMuch = false;
        $foundShown = false;
        while ((($shownLex = self::findLex(languageDB::SHOWN_MODS)) != null) ||
            (($foundShown || $foundMuch) && ($shownLex = self::achievementLocation()) != null)) {
            $keyterm = $shownLex->keyterm;
            switch ($keyterm) {
                case 'than-normal':
                    break; // just ignore this (syntactic sugar)
                case 'shown':
                    $foundShown = true;
                    break;
                case 'stroked':
                    if (($colour = self::colour()) != null) {
                        $adjustments[] = $this->schema->makeModFromLexeme($shownLex, languageDB::SHOWN_MODS,
                            $colour->getAttribute(blazonML::A_KEYTERM));
                    } else {
                        $adjustments[] = $this->schema->makeModFromLexeme( $shownLex, languageDB::SHOWN_MODS,
                            'gray');
                    }
                    break;
                case 'much':
                case 'slightly':
                case 'very-much':
                    $foundMuch = $shownLex->keyterm;
                    break;
                default:
                    if ($foundMuch) $shownLex->keyterm = "$foundMuch-$keyterm";
                    $adjustments[] = $this->schema->makeModFromLexeme($shownLex, languageDB::SHOWN_MODS, null);
                    $foundMuch = false;
                    $foundShown = true;
                    break;
            }
            $this->matcher->syncOver(languageDB::ANDD);
        }
        if (count($adjustments))
            return $adjustments;
        return null;
    }

    protected function arranged() {
        $phrase = self::newPhrase();
        $phrase->add(self::findLex(languageDB::ARRANGED));
        $in = $phrase->add(self::findLex(languageDB::WORD_IN));
        $phrase->add(self::findLex(languageDB::WORD_THE));
        $phrase->add(self::findLex(languageDB::SIGNIFIER));
        if (!is_null($in) && $phrase->add(self::findLex(languageDB::INARRANGEMENT))) {
            return $phrase->create();
        } elseif ($phrase->add(self::findLex(languageDB::ARRANGEMENT))) {
            return $phrase->create();
        } // else
        self::discardPhrase($phrase);
        return null;
    }

    protected function charge($assumeNum = false, $allowArr = true, $allowPos = true) // DONE
    {
        $phrase = self::newPhrase();
        $chargeList = null;
        $mods = array();
        $posMods = array();
        while (true) {
            // Look for arrangements (e.g "in pale, three whatever...")
            if ($arrange = self::arranged()) {
                $mods[] = $this->schema->makeModFromLexeme( $arrange, languageDB::ARRANGEMENT);
                $allowArr = false;
                // Look for positions - may be more than one (e.g. "in chief and in base"
            } elseif ($positions = self::getPositions()) {
                $posMods = array_merge($mods, $positions);
                $mods = [];
                $allowPos = false;
            } else
                break;
        }
        while (true) {

            // over x a y, not needed, under x a y not implemented
            $this->matcher->syncOver(languageDB::LAYERING);

            /*
             * We support several types of grouped charges:
             *
             * 1) on ChargeA ChargeB
             * 2) ChargeA charged with ChargeB (and this is allowed recursively)
             * 3) ChargeB on ChargeA
             * 4) ChargeA between ChargeB
             * 5) between ChargeA ChargeB
             * 7) ChargeA holding a ChargeB
             *
             * And we generally ignore (for now)
             * 6) ChargeA above/below ChargeB
             * 7) above/below ChargeA ChargeB
             *
             * Bracketed numbers below refer to the groupings above
             */

            // Look for markers to associated charges, if found, remember for later
            $onMod = null;
            $betweenMod = null;
            $nearMod = null;
            $swapCharges = false;

            if ($phrase->add(self::findLex(languageDB::WORD_ON))) { // (1)
                $phrase->add(self::findLex(languageDB::EACH));
                $onMod = $this->schema->makeModFromLexeme($phrase->create('charged-with'), languageDB::CHARGE_WITH,
                    'charged-with');
            } elseif ($phrase->add(self::findLex(languageDB::CHARGE_BETWEEN))) { // (5)
                $betweenMod = $this->schema->makeModFromLexeme($phrase->create(), languageDB::CHARGE_BETWEEN, null);
                $swapCharges = true; // turn this into case (4)
            } elseif ($near = self::findLex(languageDB::CHARGE_NEAR)) { // (7)
                if ($near->keyterm == 'below') {
                    $near->keyterm = 'above';
                } else {
                    $swapCharges = true; // turn this into case (6)
                }
                $nearMod = $this->schema->makeModFromLexeme($near, languageDB::CHARGE_NEAR);
            }

            // Look for an actual charge
            if (($topCharge = self::simpleCharge($assumeNum, $allowArr, $allowPos)) == null) {
                self::discardPhrase($phrase);
                return null;
            }

            // Special handling for escutcheon (1)
            if (($onMod != null) && $topCharge->getAttribute(blazonML::A_KEYTERM) == 'special/escutcheon') {
                $onMod = null; // don't need to say "on an escutcheon..." (but allow it anyway)
            }

            // Now loop, looking for associated charges
            $currentCharge = $topCharge;
            $firstTime = true;
            do {
                $found = false; // looping control
                $holding = false;
                $keep = false;

                if (self::semicolon(true)) break; // semi colon ends input


                if ($onMod == null) {
                    if ($lexeme = self::findLex(languageDB::CHARGE_WITH)) { // (2)
                        $onMod = $this->schema->makeModFromLexeme($lexeme,languageDB::CHARGE_WITH);
                    }
                }

                if ($onMod == null) {
                    if ($lexeme = self::findLex(languageDB::HOLDING)) { // (7)
                        $onMod = $this->schema->makeModFromLexeme($lexeme, languageDB::HOLDING);
                        $holding = true;
                    }
                }

                if ($firstTime && $onMod == null) {
                    $phrase = self::newPhrase();
                    if ($phrase->add(self::findLex(languageDB::WORD_ON))) { // (3)
                        $phrase->add(self::findLex( languageDB::EACH));
                        $onMod = $this->schema->makeModFromLexeme($phrase->create('charged-with'), languageDB::CHARGE_WITH);
                        $swapCharges = true; // turn this into case (1rose)
                    }
                }

                if ($betweenMod == null) {
                    if ($lexeme = self::findLex(languageDB::CHARGE_BETWEEN)) { // (4)
                        $betweenMod = $this->schema->makeModFromLexeme($lexeme, languageDB::CHARGE_BETWEEN);
                    }
                }

                if ($nearMod == null) {
                    if ($lexeme = self::findLex(languageDB::CHARGE_NEAR)) { // (6)
                        if ($lexeme->keyterm == 'below') {
                            $lexeme->keyterm = 'above'; // collapse to a single case
                            $swapCharges = true;
                        }
                        $nearMod = $this->schema->makeModFromLexeme($lexeme, languageDB::CHARGE_NEAR);
                    }
                }

                if ($onMod == null && $betweenMod == null && $nearMod == null) {
                    break; // Nothing further to do, no associated charges
                }

                // There should be a charge (only) here
                if ((($otherCharge = self::simpleCharge(true, true, false)) == null) && (($otherCharge = self::ordinary()) == null)) {
                    $otherCharge = $this->schema->makeNode(blazonML::E_MISSING);
                    self::raiseError(self::ERROR_ORIGIN, "Expected something to go with charge", true);
                    $this->matcher->syncOver();
                    break;
                } // else

                if ( $holding ) {
                    $keep = self::semicolon(true);
                }
                else if ($swapCharges) { // collapse cases
                    if ($firstTime) {
                        $topCharge = $otherCharge;
                        $temp = $otherCharge;
                        $otherCharge = $currentCharge;
                        $currentCharge = $temp;
                    } else {
                        self::raiseError(self::ERROR_ORIGIN, "Ambiguous charge stacking (use charged with)", true);
                    }
                }

                if ($onMod != null) {
                    $theMod = $onMod;
                } elseif ($betweenMod != null) {
                    $theMod = $betweenMod;
                } else { // by default
                    $theMod = $nearMod;
                }

                $theMod->appendChild($otherCharge);
                $currentCharge->appendChild($theMod);
                if ( !$keep )
                    $currentCharge = $otherCharge;

                $found = true; // loop again, with a clean slate
                $onMod = null;
                $betweenMod = null;
                $nearMod = null;
                $swapCharges = false;
                $firstTime = false;
            } while ($found);


            // // Add anything found earlier
            // Are making a list?
            if ($chargeList != null) {
                foreach ($mods as $mod)
                    $chargeList->appendChild($mod);
                $chargeList->appendChild($topCharge);
            } else {
                foreach ($mods as $mod)
                    $topCharge->appendChild($mod);
            }
            // is there another charge coming?
            if (self::findLex(languageDB::CHGLIST)) {
                if ($chargeList == null)
                    $chargeList = $this->schema->makeNode(blazonML::E_LIST);
                $chargeList->appendChild($topCharge);
            } else {
                break;
            }
        }
        if ($chargeList != null) {
            foreach($posMods as $mod)
                $chargeList->appendChild($mod);
            return $chargeList;
        }
        // else
        foreach($posMods as $mod)
            $topCharge->appendChild($mod);
        return $topCharge;
    }

    protected function objects() // DONE
    {
        $node = $this->schema->makeNode(blazonML::E_OBJECTS);
        $assume_num = false;
        $found = false;

        $phrase = self::newPhrase();
        $phrase->add(self::findLex(languageDB::WITH));

        while (self::moreInput()) {
            if (self::findLex(languageDB::ANDD)) $assume_num = true;
            // We treat a counterchanged division a bit like an ordinary
            if ($counterchangedDivision = self::division(true)) {
                $node->appendChild($counterchangedDivision);
                $this->matcher->syncOver();
                $found = true;
            } elseif ($item = self::ordinary()) {
                $found = true;
                $node->appendChild($item);
                $this->matcher->syncOver();
                if (self::semicolon()) break;
            } elseif ( $item = self::charge($assume_num)) {
                $found = true;
                $assume_num = false; // reset if it was true
                $node->appendChild($item);
                $this->matcher->syncOver();
                // if (self::semicolon()) break;
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

    protected function cotticing() // DONE
    {
        $phrase = self::newPhrase();
        $num = null;
        if ($phrase->add(self::findLex(languageDB::BETWEEN))) {
            $num = $phrase->add(self::findLex(languageDB::NUMBER));
            if (!$phrase->add(self::findLex(languageDB::COTTICE))) {
                self::discardPhrase($phrase);
                return null;
            }
        }
        if (!$num) $num = $phrase->add(self::findLex(languageDB::COTTICE_MODS));
        if (!$num) { // still not found anything
            self::discardPhrase($phrase);
            return null;
        } // got something!
        $mod = $this->schema->makeModFromLexeme($phrase->create('cotticing'), languageDB::COTTICE_MODS, $num->keyterm);
        // now look for a linetype or colour
        $found = true;
        while ($found) {
            $num = null;
            if ($lexeme = self::linetypePrefix()) {
                $mod2 = $this->schema->makeModFromLexeme($lexeme, languageDB::LINETYPE_PREFIX);
                if ($lexeme= self::findLex(languageDB::LINETYPE))
                    $mod2->appendChild($this->schema->makeModFromLexeme($lexeme, blazonML::A_LINETYPE));
                if ($tinc = self::tincture(false))
                    $mod2->appendChild($tinc);
                if (!$mod2->hasChildNodes()) {
                    $mod2->appendChild($this->schema->makeNode(blazonML::E_MISSING));
                    self::raiseError(self::ERROR_ORIGIN, "Expected line type or colour", true);
                }
                $mod->appendChild($mod2);
            } elseif ($lexeme = self::findLex(languageDB::LINETYPE))
                $mod->appendChild($this->schema->makeModFromLexeme($lexeme, blazonML::A_LINETYPE));
            elseif ($tinc = self::tincture(false))
                $mod->appendChild($tinc);
            else
                $found = false;
        }
        return $mod;
    }

    protected function linetypePrefix()
    {
        $phrase = self::newPhrase();

        if ($phrase->add(self::findLex(languageDB::WORD_THE))) {
            $lexeme = $phrase->add(self::findLex(languageDB::LINETYPE_PREFIX));
            if (is_null($lexeme)) {
                self::discardPhrase($phrase);
                return null;
            } else {
                return $phrase->create();
            }
        }
        return null;
    }

    protected function lineTypePostfix() {
        $phrase = self::newPhrase();

        $phrase->add(self::findLex(languageDB::WORD_ON));
        $phrase->add(self::findLex(languageDB::WORD_AT));
        if ($phrase->isEmpty()) {
            return null;
        } // else
        $phrase->add(self::findLex(languageDB::WORD_THE));
        $lexeme = $phrase->add(self::findLex(languageDB::LINETYPE_POSTFIX));
        if (is_null($lexeme)) {
            self::discardPhrase($phrase);
            return null;
        } else {
            return $phrase->create();
        }
        return null;
    }

    protected function linetype() // DONE
    {
        $mod = null;
        // Look for modifiers that are followed a linetype (e.g. "the inner ones wavy")
        if ($lexeme = self::linetypePrefix()) {
            $mod = $this->schema->makeModFromLexeme($lexeme, languageDB::LINETYPE_PREFIX);
            if ($lexeme = self::findLex(languageDB::LINETYPE))
                $mod->appendChild($this->schema->makeModFromLexeme($lexeme, blazonML::A_LINETYPE));
            else {
                $mod->appendChild($this->schema->makeNode(blazonML::E_MISSING));
                self::raiseError(self::ERROR_ORIGIN, "Expected line type", true);
                $this->matcher->syncOver();
            }
            // Look for simple linetype modifiers (e.g. "dancetty")
        } elseif ($lexeme = self::findLex(languageDB::LINETYPE)) {
            $mod = $this->schema->makeModFromLexeme($lexeme, blazonML::A_LINETYPE);
        }
        if ($mod) { // also look for top/bottom modifiers
            if ($lexeme = self::linetypePostfix()) {
                $mod->appendChild($this->schema->makeModFromLexeme($lexeme, languageDB::LINETYPE_POSTFIX));
            }
        }
        return $mod;
    }

    protected function ordinary() // DONE
    {
        $insideMod = null;
        $phrase = self::newPhrase();
        $found = true;
        $ord = null;

        // look for positions (not all make sense, but we don't care at this stage)
        $pos = self::getPositions();
        if ($lexeme = self::findLex(languageDB::WORD_ON)) $onMod = $this->schema->makeModFromLexeme($lexeme, languageDB::WORD_ON);
        // if found, remember "inside" for later
        if ($lexeme = self::findLex(languageDB::BETWEEN)) $insideMod = $this->schema->makeModFromLexeme($lexeme, languageDB::BETWEEN);
        self::findLex(languageDB::LAYERING); // over x a y, not needed, under x a y not implementd
        // Look for a number (or assume one)
        $lexeme = self::findLex(languageDB::QUANTITY);
        if (!$lexeme) $lexeme = self::findLex(languageDB::NUMBER);
        if (!$lexeme) {
            $ordNum = 1;
        } else {
            $ordNum = $lexeme->keyterm;
        }
        $prefixes = [];
        // Look for various prefix modifiers (including positions, sometimes we get "in chief" or "in base"
        while ($lexeme = self::findLex(languageDB::ORDINARY_PREFIX)) {
                $prefixes[] = $this->schema->makeModFromLexeme($lexeme, languageDB::ORDINARY_PREFIX);
        }
        // Check that there is no confusion with charges that look a bit like ordinaries
        if (self::findLex(languageDB::CHARGE_NOT_ORDINARY)) {
            self::discardPhrase($phrase);
            return null;
        }
        // Now look for things that are definitely ordinaries
        $lexeme = self::findLex(languageDB::ORDINARY_NOT_CHARGE);
        // Now look for things that could be either but depend on the number given
        if (($lexeme == null) and $lexeme = self::findLex(languageDB::ORDINARY_OR_CHARGE)) {
            $parts = explode('/', $lexeme->keyterm);
            $type = $parts[1];
            if ($type == 'chevron') {  // TODO draw this (and 2 bends!)
                if ($ordNum > 3) {// must be a charge
                    self::discardPhrase($phrase);
                    return null;
                }
            } elseif ($ordNum > 1) { // must be a charge, e.g. 3 frets
                self::discardPhrase($phrase);
                return null;
            }
            $lexeme->keyterm = $type;
        } // check if found something earlier, or actually have a nice easy ordinary...
        if (($lexeme != null) || ($lexeme = self::findLex(languageDB::ORDINARY))) {
            $ord = $this->schema->makeNodeFromLexeme(blazonML::E_ORDINARY, $lexeme, array ( blazonML::A_NUMBER => "$ordNum"));
            foreach ($prefixes as $prefix)
                $ord->appendChild($prefix);
        } else { // no ordinary found, so backtrack.
            self::discardPhrase($phrase);
            return null;
        }
        // Bit of a fudge, we are always "within" a bordure or a tressure
        if (in_array($lexeme->keyterm, array("bordure", "tressure", "double-tressure"), true)) {
            $insideMod = null;
        }

        $foundTinc = false;
        $tincIndex = 1;
        while (self::moreInput()) { // Look for prefixes again, just in case, and other common ordinary modifiers (e.g. "a bend sinister")
            $this->matcher->syncOver(languageDB::ANDD);
            if ($lexeme = self::findLex(languageDB::ORDINARY_MODS)) {
                $ord->appendChild($this->schema->makeModFromLexeme($lexeme, languageDB::ORDINARY_MODS));
            } elseif ( $lexeme = self::findLex(LanguageDB::ORDINARY_PREFIX)) {
                $ord->appendChild($this->schema->makeModFromLexeme($lexeme, languageDB::ORDINARY_MODS));
            } elseif (($lexeme = self::findLex(languageDB::ORIENTATION)) != null) {
                $ord->appendChild($this->schema->makeModFromLexeme($lexeme, languageDB::ORIENTATION));
                // Look for modifiers that are normally followed by a colour (e.g. "a bend compony vert")
            } elseif ($lexeme = self::findLex(languageDB::COLOUR_MODS)) {
                $comp = $this->schema->makeModFromLexeme($lexeme, languageDB::COLOUR_MODS);
                $tinc2 = self::tincture(true); // Must have a tincture
                $tinc2->setAttribute('index', '2');
                $comp->appendChild($tinc2);
                $ord->appendChild($comp);
                // Look for cotticing (complicated, so moved to a separate function)
            } else if ($mod = self::cotticing())
                $ord->appendChild($mod);
            // similarly line types
            elseif ($mod = self::linetype()) {
                $ord->appendChild($mod);
                // Look for voided, (e.g. "a bend voided or")
            } elseif ($lexeme = self::findLex(languageDB::VOIDED)) {
                $mod = $this->schema->makeModFromLexeme($lexeme, languageDB::VOIDED);
                // If ordinary already has tincture, look for a void tincture (e.g. "a bend or voided sable")
                if ($foundTinc && ($tinc = self::tincture(false))) {
                    $mod->appendChild($tinc);
                }
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
        // Does this ordinary go "between" some charges?
        $lexeme = null;
        if (($insideMod != null) || ($lexeme = self::findLex(languageDB::BETWEEN))) {
            if (!isset($insideMod)) $insideMod = $this->schema->makeModFromLexeme($lexeme, languageDB::BETWEEN);
            if (($obj = self::charge(false, true, true)) or ($obj = self::ordinary())) {
                $insideMod->appendChild($obj);
            } else {
                $insideMod->appendChild($this->schema->makeNode(blazonML::E_MISSING));
                // Not necessarily an error - maybe need to rearrange previous charge?
                // self::raiseError(self::ERROR_ORIGIN,"Expected charge for within/inside", true);
            }
            // keep looking for more of the same
            while (!self::semicolon(true) && self::findLex(languageDB::ANDD) && (($obj = self::charge()) != null)) {
                $insideMod->appendChild($obj);
            }
            self::restoreAnd();
            $ord->appendChild($insideMod);
        }
        // Are there charges (or other ordinaries) "on" this one?
        $lexeme = null;
        if (isset($onMod) || ($lexeme = self::findLex(languageDB::CHARGE_WITH))) {
            $this->matcher->syncOver();
            if (!isset($onMod)) $onMod = $this->schema->makeModFromLexeme($lexeme, languageDB::CHARGE_WITH);
            // in some cases, prefer ordinaries to charges
            if (in_array($ord->getAttribute(blazonML::A_TYPE), array('chief', 'canton'))) {
                $item = self::ordinary();
                if ($item === null) $item = self::charge(false);
            } else {// but usually we expect to find charges
                $item = self::charge(false, true, false);
                if ($item === null) $item = self::ordinary(); // Some arrangements are valid
            }
            if (!$item) {
                $item = $this->schema->makeNode(blazonML::E_MISSING);
                self::raiseError(self::ERROR_ORIGIN, "Expect something to go on ordinary", true);
                $this->matcher->syncOver();
            }
            $onMod->appendChild($item);
            $ord->appendChild($onMod);
        }
        // Are we in a different position?
        if ($pos) {
            foreach ($pos as $p) {
                $ord->appendChild($p);
            }
        }
        return $ord;
    }

    protected function quartermarker($matchType = null): ?Lexeme // DONE
        // if matchType is null, we can match any quarterMarker
    {
        $state = self::save();
        $phrase = self::newPhrase();
        $phrase->add(self::findLex(languageDB::WORD_THE));
        // Look for an obvious quartering
        if ($matchType == null || $matchType == languageDB::QUARTERS_WORDS) {
            $lexeme = $phrase->add(self::findLex(languageDB::QUARTERS_WORDS));
            $phrase->add(self::findLex(languageDB::QUARTERMARK));
            if ($lexeme !== null) {
                return $phrase->create($lexeme->keyterm, languageDB::QUARTERS_WORDS);
            }
        }
        if ($matchType == null || $matchType == languageDB::QUARTERS_ORDINAL) {
            $lexeme = $phrase->add(self::findLex(languageDB::QUARTERS_ORDINAL));
            $phrase->add(self::findLex(languageDB::QUARTERMARK));
            if ($lexeme !== null) {
                return $phrase->create($lexeme->keyterm,  languageDB::QUARTERS_ORDINAL);
            }
        }
        if ($matchType == null || $matchType == languageDB::QUARTERS_ROMAN) {
            $lexeme = $phrase->add(self::findLex(languageDB::QUARTERS_ROMAN));
            $phrase->add(self::findLex(languageDB::QUARTERMARK));
            if ($lexeme !== null) {
                return $phrase->create($lexeme->keyterm, languageDB::QUARTERS_ROMAN);
            }
        }
        // Look for a number, but it MUST be followed by a quartermarker
        if ($matchType == null || $matchType == languageDB::NUMBER) {
            $lexeme = $phrase->add(self::findLex(languageDB::NUMBER));
            if (self::findLex(languageDB::QUARTERMARK)) {
                return $phrase->create($lexeme->keyterm, languageDB::NUMBER);
            }
        }
        // Look for a letter, but it MUST be followed by a quartermarker
        if ($matchType == null || $matchType == languageDB::LETTERS) {
            $lexeme = $phrase->add(self::findLex(languageDB::LETTERS));
            if (self::findLex(languageDB::QUARTERMARK)) {
                return $phrase->create($lexeme->keyterm, languageDB::LETTERS);
            }
        }
        self::discardPhrase($phrase);
        return null;
    }

    protected function quartered() // DONE
    {
        $subshields = array_fill(1, 64, null);

        $phrase = self::newPhrase();
        $node = null;
        $numberStated = 0;
        $highest = 0;

        if (($marshall = self::findLex(languageDB::QUARTERED)) != null) {
            $not_quarters = substr($marshall->keyterm, 0, 4) == "per-";
            $this->matcher->syncOver(languageDB::COLON); // allow quarterly: ...
            $quartered = $this->schema->makeNodeFromLexeme(blazonML::E_COMPLEX, $marshall, array('keyterm' => blazonML::V_QUARTERED));
            if (($qmod = self::findLex(languageDB::QUARTERMODS)) != null) {
                $quartered->appendChild($this->schema->makeModFromLexeme($qmod, languageDB::QUARTERMODS));
            }
            $this->matcher->syncOver();
            if ($lexeme = self::ofNumber(languageDB::PIECES)) { // like "quarterly of 4"
                $numberStated = $lexeme->value;
                $this->matcher->syncOver();
            }

            $foundQuarter = false;
            $quarterType = null; // accept any type of numbering
            do { // Look for quarters
                if (self::findLex(languageDB::DOUBLESEMI))
                    break;
                $found = false;
                $indices = array();
                while (($quarterMark = self::quartermarker($quarterType)) != null) {
                    $keyterm = $quarterMark->keyterm;
                    if ($quarterType == null ) {
                        $quarterType = $quarterMark->value;
                        if (in_array($quarterType, $this->previousTypes)) {
                            self::raiseError(self::ERROR_ORIGIN, "Sub quarters should use different numbering scheme", true);
                        }
                        $this->previousTypes[] = $quarterType;
                    }
                    $indices[] = $keyterm; // make a list of quarter numbers
                    if ($keyterm > $highest)
                        $highest = $keyterm;
                    $this->matcher->syncOver(languageDB::ANDD);
                    $found = $foundQuarter = true;
                }
                if ($found) {
                    // $ID = null;
                    $this->matcher->syncOver(languageDB::QUANTIFIER);
                    $quarter = self::shield(); // Look for the description of this quarter
                    // if ($quarter != null and count($indices) > 1) {
                        $ID = $quarter->getAttribute('ID');
                    // }
                    if ($subshields[$indices[0]] != null)
                        self::raiseError(self::ERROR_ORIGIN, 'Duplicate quarter', true);
                    else
                        $subshields[$indices[0]] = $quarter; // Set the first quarter to this description
                    for ($i = 1; $i < count($indices); $i++) {
                        // $reference = $this->schema->makeIDNode(blazonML::E_SHIELD, array(blazonML::A_IDREF => $ID)); // Set other quarters to point to this one
                        $reference = $quarter->cloneNode(true);
                        $reference->setAttribute('ID',unique('clone'));
                        if ($subshields[$indices[$i]] != null)
                            self::raiseError(self::ERROR_ORIGIN, 'Duplicate quarter', true);
                        else
                            $subshields[$indices[$i]] = $reference;
                    }
                } // Keep going as long as we find quarters
            } while ($found);
            array_pop($this->previousTypes);
            if ( ($marshall->keyterm == 'quarterly' || $not_quarters) and !$foundQuarter) {
                self::discardPhrase($phrase); // backtrack, assume this is quarterly division
                return null;
            }
            $order = $highest;
            if ($numberStated > 0) {
                $order = $numberStated;
                if ($numberStated >= $highest) {
                    $highest = $numberStated;
                } else {
                    self::raiseError(self::ERROR_ORIGIN, "Quarter given higher than stated", true);
                }
            }
            for ($i = 1; $i <= $highest; $i++) {
                if ($subshields[$i] == null) {
                    $pendingShield = $this->schema->makeIDNode(blazonML::E_SHIELD);
                    $pendingShield->setAttribute('index', $i);
                    $pendingShield->appendChild($this->schema->makeIDNode(blazonML::E_PENDING));
                    $subshields[$i] = $pendingShield;
                    // self::raiseError(self::ERROR_ORIGIN,'Missing quarter (' . $i . ')', true);
                } else {
                    $subshields[$i]->setAttribute(blazonML::A_INDEX, "$i");
                }
            }
            if ($marshall->keyterm == 'quartered-per-saltire') {
                $order = 4;
                $quartered->setAttribute("keyterm", "per-saltire");
            } else if ( $not_quarters ) {
                $quartered->setAttribute("keyterm", $marshall->keyterm);
            }
            $quartered->setAttribute('order', $order);
            for ($i = 1; $i <= $highest; $i++)
                $quartered->appendChild($subshields[$i]);
            $node = $this->schema->makeIDNode(blazonML::E_SHIELD, array(blazonML::A_INDEX => '0'));
            $node->appendChild($quartered);
        }
        return $node;
    }

    protected function halved() // DONE
    {

        $node = null;
        if (($lexeme = self::findLex(languageDB::SPLITS)) != null) {
            $half = $this->schema->makeNodeFromLexem(blazonML::E_COMPLEX, $lexeme);
            if (($shield1 = self::shield()) == null) {
                self::raiseError(self::ERROR_ORIGIN, 'Expected 1st shield description (replaced by blank)', $lexeme);
                $shield1 = $this->schema->makeNode(blazonML::E_MISSING);
            }
            $shield1->setAttribute('index', '1');
            $this->matcher->syncOver(languageDB::WITH);
            // if (!self::findLex(languageDB::WITH)) {
                // uncomment to require "impaled A with B"
                //      self::raiseError('warning','expected "with"');
                //      return $shield1;
            //}
            if (($shield2 = self::shield()) == null) {
                self::raiseError(self::ERROR_ORIGIN, 'Expected 2nd shield description (replaced by blank)', true);
                $shield2 = $this->schema->makeNode(blazonML::E_MISSING);
                $this->matcher->syncOver(languageDB::WITH);
            }
            $shield2->setAttribute('index', '2');
            $half->appendChild($shield1);
            $half->appendChild($shield2);
            $node = $this->schema->makeIDNode(blazonML::E_SHIELD,
                array(blazonML::A_INDEX => '0'));
            $node->appendChild($half);
        }
        return $node;
    }

    protected function fur() // DONE
    {
        $phrase = self::newPhrase();
        $phrase->add(self::findLex(languageDB::WORD_OF));
        $fur = $phrase->add(self::findLex(languageDB::FUR));
        if ($fur == null) {
            self::discardPhrase($phrase);
            return null;
        }
        return $this->schema->makeNodeFromLexeme(blazonML::E_FUR,
            $phrase->create());

    }

    protected function colour() // DONE
    {
        global $options;

        $phrase = self::newPhrase();
        $phrase->add(self::findLex(languageDB::WORD_OF));
        $heraldicOnly =  !is_null($phrase->add(self::findLex(languageDB::HERALDIC)));
        $colour = null;
        // if a colour set is enabled, those colours
        /// OVERRIDE heraldic colours
        if (!$heraldicOnly) {
            if ( $options['useTartanColours'])
                $colour = $phrase->add(self::findLex(languageDB::TARTANCOLS));
            if ($colour == null && $options['useWebColours'])
                $colour = $phrase->add(self::findLex(languageDB::WEBCOLS));
            if ($colour == null && $options['useWarhammerColours'])
                $colour = $phrase->add(self::findLex(languageDB::WHCOLS));
        }
        if ($colour == null)
            $colour = $phrase->add(self::findLex(languageDB::COLOUR));
        // If not enabled, we still search for them, but AFTER heraldic colours
        if ($colour == null && !$options['useTartanColours'])
            $colour = $phrase->add(self::findLex(languageDB::TARTANCOLS));
        if ($colour == null && !$options['useWebColours'])
            $colour = $phrase->add(self::findLex(languageDB::WEBCOLS));
        if ($colour == null && !$options['useWarhammerColours'])
            $colour = $phrase->add(self::findLex(languageDB::WHCOLS));
        // FFS! Have we found anything yet? If not, give up
        if ($colour == null) {
            self::discardPhrase($phrase);
            return null;
        }
        return $this->schema->makeNodeFromLexeme(blazonML::E_COLOUR, $phrase->create() );
    }

    protected function threadcount($value) {
        $colourCodes = [
            'FSB' => 'tartan/fs-blue',
            'LR' => 'tartan/light-red',
            'R' => 'tartan/red',
            'DR' => 'tartan/dark-red',
            'O' => 'tartan/orange',
            'DO' => 'tartan/dark-orange',
            'LY' => 'tartan/light-yellow',
            'Y' => 'tartan/yellow',
            'DY' => 'tartan/dark-yellow',
            'LG' => 'tartan/light-green',
            'G' => 'tartan/green',
            'DG' => 'tartan/dark-green',
            'L' => 'tartan/light-blue',
            'B' => 'tartan/blue',
            'DB' => 'tartan/dark-blue',
            'LP' => 'tartan/light-purple',
            'P' => 'tartan/purple',
            'DP' => 'tartan/dark-purple',
            'LN' => 'tartan/light-grey',
            'N' => 'tartan/grey',
            'DN' => 'tartan/dark-grey',
            'LT' => 'tartan/light-brown',
            'T' => 'tartan/brown',
            'DT' => 'tartan/dark-brown',
            'W' => 'tartan/white',
            'K' => 'tartan/black',
            'HSA' => 'tartan/hsa-blue',
            'WG' => 'tartan/weathered-green',
            'WB' => 'tartan/weathered-blue',
        ];
        $message = '';
        $retval = [];
        $value = strtoupper($value);
        $len = strlen($value);
        $defaultSize = 10;
        $lineNo = self::lineNo();
        $tokens = self::tokens();

        $symmetrical = true;
        $number = '';
        $colour = '';
        for ($i = 0; $i < $len; $i++) {
            switch ($value[$i]) {
                case '/':
                    $symmetrical = true;
                    break;
                case '.':
                    $symmetrical = false;
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    $number .= $value[$i];
                    break;
                case ' ':
                    if ($colour && $number) {
                        if (array_key_exists($colour, $colourCodes))
                            $colour = $colourCodes[$colour];
                        $colNode = $this->schema->makeNode(blazonML::E_COLOUR,
                            array(blazonML::A_KEYTERM => $colour), $tokens, $lineNo);
                        $mod = $this->schema->makeMod(languageDB::NUMBER, $number,
                            languageDB::NUMBER, $tokens, $lineNo);
                        $colNode->appendChild($mod);
                        $retval[] = $colNode;
                        $colour = '';
                        $number = '';
                    }
                    break;
                case 'L':
                case 'D':
                case 'R':
                case 'Y':
                case 'F':
                case 'S':
                case 'O':
                case 'G':
                case 'B':
                case 'P':
                case 'W':
                case 'N':
                case 'K':
                case 'T':
                case 'A':
                case 'H':
                    if ($colour && $number) {
                        if (!$number) $number = $defaultSize;
                        if (array_key_exists($colour, $colourCodes))
                            $colour = $colourCodes[$colour];
                        $colNode = $this->schema->makeNode(blazonML::E_COLOUR,
                            array(blazonML::A_KEYTERM => $colour), $tokens, $lineNo);
                        $mod = $this->schema->makeMod(languageDB::NUMBER, $number,
                            languageDB::NUMBER, $tokens, $lineNo);
                        $colNode->appendChild($mod);
                        $retval[] = $colNode;
                        $number = '';
                        $colour = '';
                    }
                    $colour .= $value[$i];
                    break;
                default:
                    $message .= ' [' . $value[$i] . '?]';
                    break;
            }
        }
        if ($message) {
            self::raiseError(self::ERROR_ORIGIN,
                "Unknown characters in tartan specifier: " . $message, true);
        }
        if ($colour) {
            if (!$number) $number = $defaultSize;
            if (array_key_exists($colour, $colourCodes))
                $colour = $colourCodes[$colour];
            $colNode = $this->schema->makeNode(blazonML::E_COLOUR,
                array(blazonML::A_KEYTERM => $colour), $tokens, $lineNo);
            $mod = $this->schema->makeMod(languageDB::NUMBER, $number,
                languageDB::NUMBER, $tokens, $lineNo);
            $colNode->appendChild($mod);
            $retval[] = $colNode;
        }
        if (!$symmetrical) {
            $retval[] = "no-repeat";
        }
        return $retval;

    }

    protected function tartan() { // DONE
        global $options;

        if (($tartan = self::findLex(languageDB::TARTAN)) == null)
            return null;

        $options['useTartanColours'] = true; // triggers search for tartan colours first
        $patternArray = [];
        $defaultSize = 10;
        $found = true;
        while ($found) {
            if (self::semicolon()) break;
            $this->matcher->syncOver(languageDB::ANDD);
            if (($colour = self::colour()) != null) {
                $size = null;
                if (!self::semicolon())
                    $size = self::number();
                if ($size == null) $size = $defaultSize;
                $mod = $this->schema->makeMod(languageDB::NUMBER,
                    $size, languageDB::NUMBER,
                    self::tokens(), self::lineNo());
                $colour->appendChild($mod);
                $patternArray[] = $colour;
            } else {
                $found = false;
            }
        }
        if (!count($patternArray)) { // didn't find anything, look for a string
            $state = self::save();
            self::findLex(languageDB::VALUE); // always gets something
            $value = $this->matcher->getValue();
            $patternArray = self::threadcount($value);
            if (count(($patternArray))) {
                self::discard($state);
            } else {
                self::restore($state);
            }
        }

        $node = $this->schema->makeNodeFromLexeme(blazonML::E_TREATMENT, $tartan);
        $last = count($patternArray) - 1;
        if ($last > 0 && $patternArray[$last] == "no-repeat") {
            $node->setAttribute(blazonML::A_KEYTERM, "tartan-no-repeat");
            array_pop($patternArray);
        }
        foreach ($patternArray as $tartanColour) {
            $node->appendChild($tartanColour);
        }
        return $node;
    }


  protected function number() { // DONE
      if (($num = self::findLex(languageDB::NUMBER)) == null)
          return null;
      if ($num->keyterm == 'bignum')
          $number = intval($num->tokens);
      else
          $number = $num->keyterm;
      return $number;
    }

  protected function treatment() { // DONE
        $phrase = self::newPhrase();
    $treatment = $phrase->add(self::findLex(languageDB::TREATMENT));
    if ( $treatment == null ) {
        self::discardPhrase($phrase);
      return null;
    }
    $ofNum = null;

    $numLexeme = $phrase->add(self::ofNumber(languageDB::POINTS));
    if (!is_null($numLexeme)) {
        $ofNum = $numLexeme->keyterm;
    }
    if ($treatment->keyterm == 'mulletty') {
        if ($ofNum == null || $ofNum < 5)
            $ofNum = 5;
        elseif ($ofNum > 10)
            $ofNum = 10;
        $treatment->keyterm .= '-' . $ofNum;
    }
    if ($treatment->keyterm == 'checky') {
        if ($ofNum == 9) {
            $keyterm = 'checky9';
     /*   } elseif ($ofNum == 3) {
            $keyterm = 'checky3'; */
        }
    }
    $node = $this->schema->makeNodeFromLexeme(blazonML::E_TREATMENT, $phrase->create($treatment->keyterm));
    if (substr($treatment->keyterm, 0, 5) == 'grady') {
        if ($inverse = self::findLex(languageDB::INVERSE)) {
            $node->appendChild($this->schema->makeModFromLexeme($inverse, languageDB::INVERSE));
        }
    }
    return $node;
  }

  protected function ofNumber($descriptor = null) {
        $phrase = self::newPhrase();
        if (is_null($phrase->add(self::findLex(languageDB::WORD_OF)))) return null;
        $number = $phrase->add(self::findLex(languageDB::NUMBER));
        if (!is_null($descriptor)) {
            $phrase->add(self::findLex($descriptor));
        }
        if (is_null($number)) {
            self::discardPhrase($phrase);
            return null;
        } // else
        $phrase->add(self::findLex(languageDB::POINTS));
        return $phrase->create('ofnum', $number->keyterm);
  }

  protected function division( $onlyCounterchanged = false ) { // DONE
    $need3Tinc = array ( 'per-pall', 'per-pall-arrondi', 'tierced-in-pale',
    'tierced-in-fess', 'tierced-in-bend', 'tierced-in-bend', 'tierced-in-chevron' );
    $opt3Tinc = array ( 'per-pile', );
    $opt4Tinc = array ( 'quarterly', 'quarterly-per-fess', 'quarterly-per-pale', 'per-saltire' );
    $optInfTinc = array ( 'per-fess', 'per-pale', 'paly', 'barry' );

    $phrase = self::newPhrase();
    $node = null;
    if ( ($divLexeme = self::findLex( languageDB::DIVISION )) != null ) {
        $keyterm = $divLexeme->keyterm;
        $this->matcher->syncOver(languageDB::COLON); // allow per pale:...
      $node = $this->schema->makeNodeFromLexeme(blazonML::E_DIVISION, $divLexeme);
      $this->matcher->syncOver();
      // generic modifiers, including tinctures
      $maxTinc = 2;
      $minTinc = 2;
      if (in_array( $keyterm, $need3Tinc)) {
        $maxTinc = 3;
        $minTinc = 3;
      } elseif (in_array($keyterm, $opt3Tinc)) {
        $maxTinc = 3;
      } elseif (in_array($keyterm, $opt4Tinc)) {
        $maxTinc = 4;
      } elseif (in_array($keyterm, $optInfTinc)) {
        $maxTinc = 64;
      }
      $tincCount = 0;
      $found = true;
      $foundCounterchange = false;
      while ( $found ) {
        if (self::semicolon(false)) {
            if ($tincCount < 2) { // still need another tincture, let's try again
                continue;
            } else {
                break;
            }
        }
        $this->matcher->syncOver(languageDB::ANDD);
        if ($lexeme = self::findLex(languageDB::LINETYPE)) {
          $node->appendChild( $this->schema->makeModFromLexeme($lexeme, blazonML::A_LINETYPE));
        } elseif ( $lexeme = self::findLex(languageDB::DIVISION_MODS)) {
          $node->appendChild($this->schema->makeModFromLexeme($lexeme, languageDB::DIVISION_MODS));
        } elseif ( $lexeme = self::ofNumber(languageDB::PIECES)) {
            $node->appendChild($this->schema->makeModFromLexeme($lexeme, languageDB::DIVISION_MODS));
        } elseif ($lexeme = self::findLex(languageDB::ORIENTATION)) {
          $node->appendChild($this->schema->makeModFromLexeme($lexeme, languageDB::ORIENTATION));
        } elseif ( $tinc = self::tincture(false) ) {
          $tincCount++;
          $tinc->setAttribute(blazonML::A_INDEX,"$tincCount");
          $node->appendChild($tinc);
          if ($tinc->firstChild != null && $tinc->firstChild->nodeName == blazonML::E_COUNTERCHANGED) {
            $foundCounterchange = true;
            $tincCount = 2;
            break;
          }
        } else
          $found = false;
      }
      if ($onlyCounterchanged && !$foundCounterchange) {
          self::discardPhrase($phrase);
        return null;
      }
      // Did we find all the tinctures that we need?
      while ( $tincCount < $minTinc ) {
        $node->appendChild($this->schema->makeNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => ++$tincCount,
            blazonML::A_ORIGIN => 'missing' )));
        self::raiseError(self::ERROR_ORIGIN,"Missing tincture(s) on division", $divLexeme);
      }
       if ($tincCount > $maxTinc) {
         self::raiseError(self::ERROR_ORIGIN,"Too many tinctures for division", $divLexeme);
       }
    }
    return $node;
  }


    protected function compartment() { // DONE
        $phrase = self::newPhrase();
        if ($phrase->add(self::findLex(languageDB::COMPARTMENT)) == null) {
            return;
        }
        $compTypeLex = $phrase->add(self::findLex(languageDB::COMP_TYPE));
        if ($compTypeLex == null) {
            $phrase->keyterm = 'mound';
        }
        $compartmentNode = $this->schema->makeNodeFromLexeme(blazonML::E_COMPARTMENT, $phrase->create());
        $tincNode = self::tincture();
        if ($tincNode != null) {
            $compartmentNode->appendChild($tincNode);
        }

        if (($adjust = $this->getAdjustments()) != null) {
            foreach ($adjust as $adj) {
                $compartmentNode->appendChild($adj);
            }
        }
        return $compartmentNode;
    }

    protected function ornaments() { // DONE
        // This function has 3 possible return values:
        // Null - nothing found
        // True - the word "ornament" on its own
        // A node representing a specific group of ornaments
        $phrase = self::newPhrase();
        $phrase->add(self::findLex(languageDB::SIGNIFIER));
        if (!$phrase->add(self::findLex(languageDB::ORNAMENT))) {
            self:$this->discardPhrase($phrase);
            return null;
        }
        $ornamentNode = true;

        $phrase->add(self::findLex(languageDB::WORD_OF));
        $phrase->add(self::findLex(languageDB::SIGNIFIER));
        $tradition = $phrase->add(self::findLex(languageDB::TRADITION));

        // try some actual hits now
        if (($churchRole = $phrase->add(self::findLex(languageDB::ECCLESIASTIC))) != null) {
            if (!$tradition->keyterm) $tradition->keyterm = 'catholic';
            $ornamentNode = $this->schema->makeNodeFromLexeme(blazonML::E_ORNAMENT, $phrase->create(), array (
                blazonML::A_KEYTERM => "{$tradition->keyterm}/{$churchRole->keyterm}"));
            if (($tincNode = self::tincture()) != null) {
                self::raiseError(self::ERROR_ORIGIN,"Ecclesiastic ornaments have fixed colours", true);
                $ornamentNode->appendChild($tincNode);
            }
        } // else look for other types of ornament

        if ($ornamentNode != null && ($adjust = $this->getAdjustments()) != null) {
            foreach ($adjust as $adj) {
                $ornamentNode->appendChild($adj);
            }
        }
        return $ornamentNode;
    }


    protected function mantling() // DONE
    {
        // First set some default values...
        $type = 'ribbons';
        $pavilion = '';
        $crowned = '';
        $found = false;

        $phrase = self::newPhrase();
        while (!is_null($lexeme = $phrase->add(self::findLex(
            array(languageDB::MANTLES, languageDB::MANTLING, languageDB::MANTLING_MODS))))) {
            switch ($lexeme->category) {
                case languageDB::MANTLING:
                    $phrase->add(self::findLex(languageDB::WORD_OF));
                    $phrase->add(self::findLex(languageDB::SIGNIFIER));
                    $found = true;
                    break;
                case languageDB::MANTLES:
                    $found = true;
                    $type = $lexeme->keyterm;
                    break;
                case languageDB::MANTLING_MODS:
                    if ($lexeme->keyterm == 'pavilion') $pavilion = '-pavilion';
                    elseif ($lexeme->keyterm == 'crowned') $crowned = '-crowned';
                    break;

            }
        }
        if (!$found) {
            self::discardPhrase($phrase);
            return null;
        }

        $type .= $pavilion . $crowned;
        $mantlingNode = $this->schema->makeNodeFromLexeme(blazonML::E_MANTLING, $phrase->create($type));

        // Got the mantling, now get the colours and any adjustments

        $tinctures = [];
        $locNode = null;
        $tincture = null;
        $adjust = null;
        $allowTincture = ($type == 'ribbons' || substr($type, 0, 5) == 'cloak' || $type == 'leaves');  // have complex colour scheme
        // Look for any combination of tinctures, adjustments and locations
        while (!is_null($lexeme = self::achievementLocation())
                    || (!is_null($tincture = self::tincture()))
                    || (!is_null($adjust = $this->getAdjustments()))) {
            if (!is_null($tincture)) {
                if (!$allowTincture) {
                    // Parse all colours and put in the AST but provide an error message
                    self::raiseError(self::ERROR_ORIGIN, "$type mantling has fixed colours", true);
                    $allowTincture = true; // suppress further errors
                }
                $tinctures[] = $tincture;
                $this->matcher->syncOver(languageDB::ANDD);
            } elseif (!is_null($adjust)) {
                foreach ( $adjust as $adjustment)
                    $mantlingNode->appendChild($adjustment);
            } elseif ($lexeme->category == languageDB::ACHIEVEMENT_LOC) {
                // Did we previously find another location?
                if (!is_null($locNode)) {
                    foreach ($tinctures as $tincture)
                        $locNode->appendChild($tincture);
                    $mantlingNode->appendChild($locNode);
                    $tinctures = [];
                } // save it for later
                $locNode = $this->schema->makeNodeFromLexeme(blazonML::E_LOCATION, $lexeme);
            }
        }
        // Do we have any tinctures left over?
        if (count($tinctures)) { // Yes,
            if ($locNode == null) { // use a default location
                $locNode = $this->schema->makeNode(blazonML::E_LOCATION, array(
                    blazonML::A_KEYTERM => 'default'));
            }
            foreach ($tinctures as $tincture)
                $locNode->appendChild($tincture);
            $mantlingNode->appendChild($locNode);
        }

        return $mantlingNode;
    }

    protected function achievementLocation() {
        $phrase = self::newPhrase();

        $phrase->add(self::findLex(languageDB::WORD_TO));
        $phrase->add(self::findLex(languageDB::WORD_IN));
        $phrase->add(self::findLex(languageDB::WORD_ON));
        $phrase->add(self::findLex(languageDB::WORD_OF));
        $phrase->add(self::findLex(languageDB::WORD_THE));
        if (!$loc = $phrase->add(self::findLex(languageDB::ACHIEVEMENT_LOC))) {
            self::discardPhrase($phrase);
            return null;
        }
        $this->matcher->syncOver(languageDB::ANDD);
        return $phrase->create($loc->keyterm);

    }

    protected function supporters() { // DONE
        if (is_null($supporters = self::findLex(languageDB::SUPPORTERS))) {
            return null;
        }
        $supportNode = $this->schema->makeNodeFromLexeme( blazonML::E_SUPPORTERS, $supporters);
        $locNode = null;
        while (true) {
            if (!is_null($loc = self::achievementLocation())) {
                $locNode = $this->schema->makeNodeFromLexeme(blazonML::E_LOCATION, $loc);
            }
            $charge = self::charge(true,false,false); // assume number, don't allow positions or arrangements
            if ($charge == null) {
                if ($locNode != null)
                    self::raiseError(self::ERROR_ORIGIN,"Expected charge for the supporter", true);
                break;
            } else {
                if ($locNode == null) { // use a default location
                    $locNode = $this->schema->makeNode(blazonML::E_LOCATION, array (
                        blazonML::A_KEYTERM => 'default'));
                }
                $locNode->appendChild($charge);
            }
            if (($adjust = $this->getAdjustments()) != null) {
                foreach ($adjust as $adj) {
                    $locNode->appendChild($adj);
                }
            }
            $supportNode->appendChild($locNode);
            $locNode = null;

            $this->matcher->syncOver(languageDB::ANDD);
        }
        return $supportNode;
    }

    protected function torse() { // DONE
        if (is_null($torse = self::findLex(languageDB::TORSE))) {
            return null;
        }
        $torseNode = $this->schema->makeNodeFromLexeme(blazonML::E_TORSE, $torse);
        do {
            $found = false;
            if (($tincNode = self::tincture()) != null) {
                $torseNode->appendChild($tincNode);
                $this->matcher->syncOver(languageDB::ANDD);
                $found = true;
            } else if (($adjust = $this->getAdjustments()) != null) {
                foreach ($adjust as $adj) {
                    $torseNode->appendChild($adj);
                }
                $found = true;
            }
        } while ( $found );
        return $torseNode;
    }

    protected function motto() // DONE
    {
        $phrase = self::newPhrase();
        $phrase->add(self::findLex(languageDB::SIGNIFIER));
        $typeLex = $phrase->add(self::findLex(languageDB::MOTTO_TYPE));
        $mottoLex = $phrase->add(self::findLex(languageDB::MOTTO));
        if ($mottoLex == null && $typeLex == null) {
            self::discardPhrase($phrase);
            return null;
        }
        // We have a motto!

        if ($typeLex == null) { // look again
            $phrase->add(self::findLex(languageDB::WORD_OF));
            $phrase->add(self::findLex(languageDB::SIGNIFIER));
            $typeLex = $phrase->add(self::findLex(languageDB::MOTTO_TYPE));
        }

        $type = !is_null($typeLex) ? $typeLex->keyterm : 'ribbon';

        $posLex = $phrase->add(self::findLex(languageDB::VERTICAL_POSITION));
        $position = !is_null($posLex) ? $posLex->keyterm : 'below';
        $keyterm = $type . '-' . $position;

        $textLex = $phrase->add(self::findLex(languageDB::MOTTO_TEXT)); // get the next word

        $mottoNode = $this->schema->makeNodeFromLexeme(blazonML::E_MOTTO, $phrase->create($mottoLex->keyterm),
                // override some attributes
            array ( blazonML::A_KEYTERM => $keyterm, blazonML::A_TEXT => $textLex->value)
        );



        while (self::moreInput()) {
            if (($colour = self::colour()) != null) {
                $mottoNode->appendChild($colour);
            } elseif (($adjust = $this->getAdjustments()) != null) {
                foreach ($adjust as $adj) {
                    $mottoNode->appendChild($adj);
                }
            } elseif ($chargeMod = self::findLex(languageDB::CHARGE_MODS)) {
                    $mottoNode->appendChild($this->schema->makeNodeFromLexeme(blazonML::E_MODIFIER, $chargeMod));
            } else {
                break;
            }
        }
        return $mottoNode;
    }

    protected function fringe() { // DONE
        if (is_null($fringe = self::findLex(languageDB::FRINGED))) {
            return null;
        }
        $fringeNode = $this->schema->makeNodeFromLexeme(blazonML::E_FRINGE, $fringe);
        if (!is_null($tincNode = self::tincture())) {
            $fringeNode->appendChild( $tincNode );
        }
        return $fringeNode;
    }

  protected function helmet() { // DONE
        $tincNode = null;
        $phrase = new Phrase();
        $phrase->add(self::findLex(languageDB::WORD_THE));
      $phrase->add(self::findLex(languageDB::SIGNIFIER));
    if (is_null($helmType = self::findLex(languageDB::CREST_HELM))) {
        self::discardPhrase($phrase);
      return null;
    }
    $helmetNode = $this->schema->makeNodeFromLexeme(blazonML::E_HELMET, $helmType);
    $locNode = null;
    do {
        $found = false;
        if (!is_null($loc = self::achievementLocation())) {
            $locNode = $this->schema->makeNodeFromLexeme(blazonML::E_LOCATION, $loc);
            $found = true;
        } else if (($tincNode = self::tincture()) != null) {
            $found = true;
        } else if (($adjust = $this->getAdjustments()) != null) {
            foreach ($adjust as $adj) {
                $helmetNode->appendChild($adj);
            }
            $found = true;
        }
    } while ($found);
    if ($locNode == null) { // use a default location
        $locNode = $this->schema->makeNode(blazonML::E_LOCATION, array (
            blazonML::A_KEYTERM => 'default'));
    }
    if (!is_null($tincNode))
        $locNode->appendChild($tincNode);
    $helmetNode->appendChild( $locNode );
    return $helmetNode;
  }

  // Look for a tincture, return a pending tincture if not found
  protected function tincture($required=false) { // DONE

    $found = true;
    $colourOrFur = null;
    $phrase = self::newPhrase();
    $node = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
        blazonML::A_ORIGIN => 'given'));

    if ( ($colourOrFur = self::colour())
        || ($colourOrFur = self::fur())
        || ($colourOrFur = self::tartan()) ) {
      // This could be [colour] [treatment] [colour],
      // unless it is followed by a semi colon
      if ( is_null(self::findLex(languageDB::COMMA)) ) {
        if ( ($treat = self::treatment()) ) {
          $keyterm = $treat->getAttribute('keyterm');
          $grady = (substr($keyterm,0,5) == 'grady');
          $tinc1 = $this->schema->makeIDNode(blazonML::E_TINCTURE, array ( blazonML::A_INDEX => '1'));
          $tinc1->appendChild($colourOrFur);
          $colourOrFur = null; // used
          $treat->appendChild($tinc1);
          $this->matcher->syncOver(languageDB::ANDD);
          // Some treatments only require one tincture
          if ( !in_array( $keyterm, array ( 'bezanty', 'hurty', 'platy', 'guzy', 'pommy', 'torty' ))) {
            $tinc2 = self::tincture(false);
              if (!$grady && is_null($tinc2)) {
                  $tinc2 = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
                      blazonML::A_ORIGIN => 'missing'));
              }
            if ($tinc2 != null) {
              $tinc2->setAttribute(blazonML::A_INDEX, '2');
              $treat->appendChild($tinc2);
            }
          }
          $node->appendChild($treat);
          // Could also be [colour] [semyde] [charge]
        } elseif ( $lexeme = self::findLex(languageDB::SEMY) ) {
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
            if (!self::semicolon() && self::findLex(languageDB::ANDD)) {
                if ($charge2 = self::simpleCharge(true)) {
                    $semyde->appendChild($charge2);
                } else {
                    self::discardPhrase($phrase2);
                }
            }
          $node->appendChild($semyde);
        }
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
      $grady = (substr($keyterm, 0, 5) == 'grady');
      $this->matcher->syncOver(languageDB::ANDD);
      $tinc2 = self::tincture(false); // grady might not need second tincture
        if (!$grady && is_null($tinc2)) {
            $tinc2 = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
                blazonML::A_ORIGIN => 'missing'));
        }
      if ($tinc2 != null) {
          $tinc2->setAttribute('index', 2);
          $treat->appendChild($tinc2);
      }
      $node->appendChild($treat);
      // Look for counterchange
    } elseif ( $lexeme = self::findLex(languageDB::COUNTERCHANGE))  {
      $counter = $this->schema->makeNodeFromLexeme(blazonML::E_COUNTERCHANGED, $lexeme);
      $node->appendChild($counter);
      // Look for division
    } elseif ( ($div1 = self::division()) ) {
      // Look for a second (normally counterchanged) division
      // if ( ( $div2 = self::division(true) ) ) $div1->appendChild($div2);
      $node->appendChild($div1);
      // Look for a back reference
    } elseif ( ($backref = self::findLex(languageDB::BACKREF)) ) {
        $numColours = count($this->foundColours);
        $numTinctures = count($this->foundTinctures);
        $refDist = 0;
        $missing = $this->schema->makeNode(blazonML::E_MISSING);
        switch ($backref->keyterm) {
            case "ofsame": // means the last TINCTURE of whatever kind
                if ($numTinctures < 1) {
                    $node = $missing;
                    self::raiseError(self::ERROR_ORIGIN, 'No tincture for "of the same"', true);
                } else {
                    $node = $this->foundTinctures[$numTinctures - 1];
                }
                break;
            case "oflast": // means the last COLOUR
                if ($numColours < 1) {
                    $node = $missing;
                    self::raiseError(self::ERROR_ORIGIN, 'No colour for "of the last"', true);
                } else {
                    $node = $this->foundColours[$numColours - 1 ];
                }
                break;
            case "offourth": // heraldically numbers seem to indicate colours NOT tinctures
                $refDist++; // flowthrough
            case "ofthird":
                $refDist++; // flowthrough
            case "ofsecond":
                $refDist++; // flowthrough
            case "offirst":
                if ($refDist + 1 > $numColours) {
                    $node = $missing;
                    self::raiseError(self::ERROR_ORIGIN, 'No colour for back reference', true);
                } else {
                    $node = $this->foundColours[$refDist];
                }
                break;
            default: // let the analyser resolve of the field
                  $node->setAttribute( blazonML::A_TOKENS, self::tokens());
                  $node->setAttribute( blazonML::A_LINENUMBER, self::lineNo());
                  break;
        }
        $node->setAttribute( blazonML::A_ORIGIN, $backref->keyterm);
      // Or a reference to proper
    } elseif ( $lexeme = self::findLex(languageDB::PROPER) ) {
      $proper = $this->schema->makeIDNodeFromLexeme(blazonML::E_PROPER,$lexeme);
      $node->appendChild($proper);
      // Look for a colour or a fur
    } else { // Didn't find anything
      self::discardPhrase($phrase);
      $found = false;
    }
    if ( $colourOrFur ) {
        $node->appendChild($colourOrFur); // we have a colour or fur on its own
        $this->foundColours[] = $node->cloneNode(true);
    }
    if ( $found ) {
        $this->foundTinctures[] = $node->cloneNode(true);
    } else {
      if ( $required ) {
        $node = $this->schema->makeIDNode(blazonML::E_TINCTURE, array(blazonML::A_INDEX => '1',
            blazonML::A_ORIGIN => 'pending'));
      } else {
        $node = null;
      }
    }
    return $node;
  }
}

