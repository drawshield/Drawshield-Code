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

include 'getcharge.inc';
include 'division.inc';
include 'effects.inc';
include "messages.inc";
include "furs.inc";
include "svgtools.inc";
include "placecharge.inc";
include "linetypes.inc";
include "tinctures.inc";
include "ordinary.inc";
include "arrangements.inc";
include "quartering.inc";
include "shield.inc";
include "shapes.inc";
include "achievement.inc";
include "pathtools.inc";
include "treatment.inc";
include "getwords.inc";

function getLine($node) {
  $retval = '';
  if ($node !== null) {
    $lineNo = $node->getAttribute(BlazonML::A_LINENUMBER);
    if ($lineNo != null) {
      $retval = " near line $lineNo";
    }
  }
  return $retval;
}


function makeOrdChgs( $node, $layout ) {
  $retval = '';
  foreach ( $node->childNodes as $child ) {
    switch ( $child->nodeName ) {
      case 'ordinary':
        $retval .= makeOrdinary( $child, $layout );
        break;
      case 'division':
        $retval .= makeDivision( $child, $layout );
        break;
      case 'list':
          $retval .= makeList( $child, $layout );
          break;
      case 'missing':
      case 'charge':
        $retval .= makeCharge( $child, $layout );
        break;
    }
  }
  return $retval;
}

function setOptionsFromBlazon( $node ) {
    global $options;

    foreach ($node->childNodes as $blazonOption) {
        switch ($blazonOption->nodeName) {
            case blazonML::E_COLOURSET:
                switch ($blazonOption->getAttribute('keyterm')) {
                    case 'web':
                        $options['useWebColours'] = true;
                        break;
                    case 'tartan':
                        $options['useTartanColours'] = true;
                        break;
                    case 'warhammer':
                        $options['useWarhammerColours'] = true;
                        break;
                    default:
                        // just ignore - should probably be an error message
                        break;
                }
                break;
            case blazonML::E_DEBUG:
                $options['debug'] = $blazonOption->getAttribute('keyterm');
                break;
            case blazonML::E_SHAPE:
                $options['shape'] = $blazonOption->getAttribute('keyterm');
                break;
            case blazonML::E_PALETTE:
                $options['palette'] = $blazonOption->getAttribute('keyterm');
                break;
            case blazonML::E_EFFECT:
                $options['effect'] = $blazonOption->getAttribute('keyterm');
                break;
            case blazonML::E_ASPECT:
                $ar = $blazonOption->getAttribute('keyterm');
                if ($ar != null) {
                    $options['aspectRatio'] = calculateAR($ar);
                }
                break;
        }
    }
    tidyOptions();
}

function draw() {
    global $dom;
    global $options;
    global $version;
    global $messages;

    $messages = new messageStore($dom);

    // Get all the charge data (once for each charge type)
    get_chg_data();
    $mantling = '';
    $supports = '';
    $compartment = '';
    $motto = '';
    $ornament = '';
    $helmets = [];
    $crests = [];
    $fringe = '';
    $shieldAdjustments = [];
    $y1Location = 0;
    $extraDefs = '';
    $extraSpace = 0;
    if ($options['palette'] == 'hatching') {
        require("hatching.inc");
        $extraDefs = mkHatches();
    }

    $rdf = '<rdf:RDF>
      <cc:Work
         rdf:about="">
        <dc:format>image/svg+xml</dc:format>
        <dc:type
           rdf:resource="http://purl.org/dc/dcmitype/StillImage" />
        <dc:rights>
          <cc:Agent>
            <dc:title>This work is licensed under a &lt;a rel=&quot;license&quot; href=&quot;http://creativecommons.org/licenses/by-nc-sa/4.0/&quot;&gt;Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License&lt;/a&gt;.</dc:title>
          </cc:Agent>
        </dc:rights>
      </cc:Work>
      </rdf:RDF>';

    $blazonNode = $dom->documentElement;
    $layout = ShieldLayout::from_options($options);
    $effect = $options['effect'];

    if ($layout->shape == 'flag') {
        $body = '';
        foreach ( $blazonNode->childNodes as $node ) {
            switch ( $node->nodeName ) {
                case 'shield':
                $body .= makeshield($node, $layout);
                break;
                case 'achievement':
                foreach ($node->childNodes as $subNode) {
                    switch ($subNode->nodeName) {
                        case blazonML::E_FRINGE:
                            $fringe = drawFringe($subNode, $layout);
                            break;
                        // other achievement parts don't really make sense (do they?)
                        default:
                            break;
                    }
                }
                break;
                // other elements for future expansion
            }
        }
        if ($fringe) {
            $extraSpace = 150;
        }

        $outline = getShape($layout);

        $displayScale = $options['size'] / 1000;
        $displayWidth = $options['size'];
        $displayHeight = (int)(($layout->size->height + 40 + $extraSpace) * $displayScale);

        $header = '<?xml version="1.0" encoding="utf-8" ?>
    <svg version="1.1"
         baseProfile="full"
         xmlns="http://www.w3.org/2000/svg"
         xmlns:xlink="http://www.w3.org/1999/xlink"
         xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
         xmlns:cc="http://creativecommons.org/ns#"
         xmlns:dc="http://purl.org/dc/elements/1.1/" 
         xmlns:blazonML="http://drawshield.net/blazonML" ' .
         'preserveAspectRatio="xMidYMid meet" height="' . $displayHeight . '" width="' . $displayWidth . '" ';
        $header .= 'viewBox="'. getEffect($effect, 'viewbox',1000, $layout->size->height + $extraSpace + 40) . ' ">';
        $meta = '';
        $defs = '<defs>' . getEffect($effect, 'defs') . $extraDefs . '</defs>';
        $trailer =  "</svg>\n";


    } else {
        $header = '<?xml version="1.0" encoding="utf-8" ?>
    <svg version="1.1"
        baseProfile="full"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink" 
         xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
         xmlns:cc="http://creativecommons.org/ns#"
         xmlns:dc="http://purl.org/dc/elements/1.1/" 
        xmlns:blazonML="http://drawshield.net/blazonML" ' .
        'preserveAspectRatio="xMidYMid meet" height="' . (int)($options['size'] * 1.2) . '" width="' . $options['size'];
        $header .= '" viewBox="'. getEffect($effect, 'viewbox') . ' ">';
        $meta = '';
        $defs = '<defs>' . getEffect($effect, 'defs') . $extraDefs .  '</defs>';
        $trailer =  "</svg>\n";
        if ( $options['palette'] == 'outline' || $options['palette'] == 'hatching') {
            $header .= '  <filter id="grayscale">
    <feColorMatrix type="saturate" values="0.10"/>
  </filter>';
        }

        $blazonNode = $dom->documentElement;
        $body = '';
        foreach ( $blazonNode->childNodes as $node ) {
            switch ( $node->nodeName ) {
                case 'complex':
                    $body .= makeAlongside($node, $layout);
                    break;
                case 'shield':
                    foreach ($node->childNodes as $childNode) {
                        if ($childNode->nodeName == blazonML::E_MODIFIER) {
                            $shieldAdjustments[] = $childNode->getAttribute(blazonML::A_KEYTERM);
                        }
                    }
                    $body .= makeshield($node, $layout);
                    $layout = ShieldLayout::from_options($options);
                    // options may have changed in makeShield
                    $outline = getShape($layout);
                    $effect = $options['effect'];
                    if (!array_key_exists('nomask', $options)) {
                        $clipShield = add_def('mask', '<path fill="#FFFFFF" d="' . $outline . '" />' );
                        $body = '<g mask="url(#' . $clipShield . ')">' . $body . '</g>';
                    }
                    if ( $options['effect'] == 'inked')
                        $body .=  '<path fill="none" stroke="#000000" stroke-width="4" d="' . $outline . '" />';
                    break;
                case 'achievement':
                    if ($layout->shape != 'pauldron' && $layout->shape != 'flag')
                if ($node->firstChild == null) { # just the word 'achievement'
                    $helmets[] = drawHelmet(null,null);
                } else {
                    foreach ($node->childNodes as $subNode) {
                        switch ($subNode->nodeName) {
                        case blazonML::E_SUPPORTERS:
                            $supports = drawSupporters($subNode, $layout);
                            break;
                        case blazonML::E_ORNAMENT:
                                $ornament = drawOrnament($subNode);
                                break;
                        case blazonML::E_MANTLING:
                            $mantling = drawMantling($subNode);
                            break;
                        case blazonML::E_FRINGE:
                            $fringe = drawFringe($subNode, $layout);
                            break;
                        case blazonML::E_COMPARTMENT:
                            $compartment = drawCompartment($subNode);
                            break;
                        case blazonML::E_MOTTO:
                            $motto = drawMotto($subNode, $layout);
                            break;
                        case blazonML::E_HELMET: // can't draw immediately in case there is a torse
                            if (count($crests) < count($helmets)) {
                                $crests[] = null; // keep crests in step with helmets
                            }
                            $helmets[] = drawHelmet($subNode);
                            break;
                        case blazonML::E_CREST:
                            $crests[] = $subNode->cloneNode(true);
                            break;
                        default:
                          // not implemented yet
                          break;
                        }
                    }
                }
                break;
            }
        }
    }
    if (!array_key_exists('nomask', $options)) {
        if ($options['effect'] == 'shiny') { // this only gets applied to the escutcheon
            $body .= add_def();
            $body = getEffect($effect, 'below') . '<g ' . getEffect($effect, 'filter') . '>' . $body . getEffect($effect, 'above') . '</g>';
        }
        // Add outline of shield shape, only if required
        if ( $options['palette'] == 'outline') {
            $outline = getShape($layout);
            $body .=  '<path fill="none" stroke="' . rgb('division-stroke') . '" stroke-width="4" d="' . $outline . '" />';
        }
        // Rest depends on output format
        if ( !$options['asFile'] /* && !$options['printable'] */ ) { // minimal decoration, no metadata
            $y1Location = '1190';
            if ($layout->shape == 'flag')
                $y1Location = $layout->size->height + $extraSpace + 40;
            if ( $options['outputFormat'] == 'svg' || $options['outputFormat'] == 'json') {
                $messages->addMessage('legal',"Heraldic images provided on this site are for education and information only. The use of heraldic devices is restricted in many countries, independently of copyright. The granting of a Coat of Arms is solely the right of the appropriate Heraldic Authority for your nationality and country of residence.");
                $messages->insertMessages();
                $xpath = new DOMXPath($dom);
                $nodes = array_reverse(iterator_to_array($xpath->evaluate('//*') ) );
                foreach ($nodes as $node) {
                    $replacement = $dom->createElementNS('http://drawshield.net/blazonML', 'blazonML:'.$node->localName);
                    foreach ($xpath->evaluate('node()|@*', $node) as $childNode) {
                        $replacement->appendChild($childNode);
                    }
                    $node->parentNode->replaceChild($replacement, $node);
                }
                $dom->formatOutput = true;
                $blazonMLText = $dom->saveXml(); // Need to remove the processing instruction
                $blazonMLText = preg_replace('/<\?xml version="1\.0"( encoding="utf-8")?\?>/i', '', $blazonMLText);
                $meta = "\n<metadata>\n" . $rdf . $blazonMLText . "\n</metadata>\n";
            }
        }
    }
    // At this point $body contains the shield in a 1000x1200 box, does it have any helmets etc. to add on top?
    $centreY = "350";
    if (count($helmets)) {
        if (count($crests)) {
            $body = "<g transform=\"translate(174,420) scale(0.65) \">$body</g>";
        } else {
            $body = "<g transform=\"translate(100,240) scale(0.8) \">$body</g>";
            $centreY = "180";
        }
    }
    switch(count($helmets)) {
        case 0:
            break; // nothing to do
        case 1:
            if (count($crests) && $crests[0] != null) { // at least one
                $crestCharges = makeCharge($crests[0]->firstChild, ShieldLayout::square());
                $body .= "<g transform=\"translate(350,10) scale(0.30) \"> $crestCharges</g>";
                $crests[0] = null; // mark this crest as used
            }
            if (!empty($helmets[0]))
                $body .= quickPlace($helmets[0]['body'], $helmets[0]['width'], $helmets[0]['height'], 300, 300, 500, $centreY, rgb($helmets[0]['fill']), $helmets[0]['adjustments']);
            // TODO add crown if present
            break;
        case 2:
            if (count($crests) && $crests[0] != null) { // at least one
                $crestCharges = makeCharge($crests[0]->firstChild, ShieldLayout::square());
                $body .= "<g transform=\"translate(200,10) scale(0.30) \"> $crestCharges</g>";
                $crests[0] = null; // mark this crest as used
            }
            $body .= quickPlace($helmets[0]['body'], $helmets[0]['width'], $helmets[0]['height'], 300, 300, 360, $centreY, rgb($helmets[0]['fill']), $helmets[0]['adjustments']);
            // TODO add crown if present
            if (count($crests) > 1 && $crests[1] != null) { // at least one
                $crestCharges = makeCharge($crests[1]->firstChild, ShieldLayout::square());
                $body .= "<g transform=\"translate(500,10) scale(0.30) \"> $crestCharges</g>";
                $crests[1] = null; // mark this crest as used
            }
            $body .= '<g transform="scale(-1,1) translate(-1320,0)">' . quickPlace($helmets[1]['body'], $helmets[1]['width'], $helmets[1]['height'], 300, 300, 660, $centreY,
                    rgb($helmets[1]['fill']), $helmets[1]['adjustments']) . '</g>';
            // TODO add crown if present
            break;
        default:
            if (count($helmets) > 3) {
                $messages->addMessage('internal',"3 helmets maximum");
            }
            if (count($crests) && $crests[0] != null) { // at least one
                $crestCharges = makeCharge($crests[0]->firstChild, ShieldLayout::square());
                $body .= "<g transform=\"translate(100,10) scale(0.30) \"> $crestCharges</g>";
                $crests[0] = null; // mark this crest as used
            }
            $body .= quickPlace($helmets[0]['body'], $helmets[0]['width'], $helmets[0]['height'], 300, 300, 260, $centreY, rgb($helmets[0]['fill']), $helmets[0]['adjustments']);
            // TODO add crown if present
            if (count($crests) > 1 && $crests[1] != null) { // at least one
                $crestCharges = makeCharge($crests[1]->firstChild, ShieldLayout::square());
                $body .= "<g transform=\"translate(350,10) scale(0.30) \"> $crestCharges</g>";
                $crests[1] = null; // mark this crest as used
            }
            $body .= quickPlace($helmets[1]['body'], $helmets[1]['width'], $helmets[1]['height'], 300, 300, 500, $centreY,
                rgb($helmets[1]['fill']), $helmets[1]['adjustments']);
            // TODO add crown if present
            if (count($crests) > 2 && $crests[2] != null) { // at least one
                $crestCharges = makeCharge($crests[2]->firstChild, ShieldLayout::square());
                $body .= "<g transform=\"translate(580,10) scale(0.30) \"> $crestCharges</g>";
                $crests[2] = null; // mark this crest as used
            }
            $body .= '<g transform="scale(-1,1) translate(-1400,0)">' . quickPlace($helmets[2]['body'], $helmets[2]['width'], $helmets[2]['height'], 300, 300, 660, $centreY,
                    rgb($helmets[2]['fill']), $helmets[2]['adjustments']) . '</g>';
            // TODO add crown if present
            break;
    }
    switch(count($crests)) {
        case 0:
            break; // nothing to do
        case 1:
            if (!empty($crests[0])) {
                $crestCharges = makeCharge($crests[0]->firstChild, ShieldLayout::square());
                $body .= "<g transform=\"translate(300,75) scale(0.4) \"> $crestCharges</g>";
            }
            break;
        case 2:
            if (!empty($crests[0])) {
                $crestCharges = makeCharge($crests[0]->firstChild, ShieldLayout::square());
                $body .= "<g transform=\"translate(200,135) scale(0.30) \"> $crestCharges</g>";
            }
            if (!empty($crests[1])) {
                $crestCharges = makeCharge($crests[1]->firstChild, ShieldLayout::square());
                $body .= "<g transform=\"translate(500,135) scale(0.30) \"> $crestCharges</g>";
            }
            break;
        default:
            if (count($crests) > 3) {
                $messages->addMessage('internal',"3 helmets maximum");
            }
            if (!empty($crests[0])) {
                $crestCharges = makeCharge($crests[0]->firstChild, ShieldLayout::square());
                $body .= "<g transform=\"translate(100,135) scale(0.30) \"> $crestCharges</g>";
            }
            if (!empty($crests[1])) {
                $crestCharges = makeCharge($crests[1]->firstChild, ShieldLayout::square());
                $body .= "<g transform=\"translate(350,135) scale(0.30) \"> $crestCharges</g>";
            }
            if (!empty($crests[2])) {
                $crestCharges = makeCharge($crests[2]->firstChild, ShieldLayout::square());
                $body .= "<g transform=\"translate(580,135) scale(0.30) \"> $crestCharges</g>";
            }
            break;
    }

    // We have added any helmets etc, if present, now do we need to adjust the whole thing?
    if (count($shieldAdjustments)) {
        $body = quickPlace($body, 1000, 1200, 1000, 1200, 500, 600, null, $shieldAdjustments);
    }

    // assorted combinations of achievements
    if ($ornament) {
        $body = $ornament['body'] . '<g transform="' . $ornament['transform'] . '">' . $body . '</g>';
    }
    if ($mantling) {
        $body = $mantling['body'] . '<g transform="' . $mantling['transform'] . '">' . $body . '</g>';
    }
    if ($motto && $supports == '') {
        if ($layout->shape == 'pauldron') {
            $body = '<g transform="translate(0,-100) ">' . $body . '</g>' . $motto;
        } else {
            if ($helmets == [] && $mantling == '') { // only motto
                $body = '<g transform="translate(57,0) scale(0.88) ">' . $body . '</g>';
            }
            $body .= quickPlace($motto['body'], $motto['width'], $motto['height'], 1000, 200, 500, 985, null, $motto['adjustments']);
        }
    }
    if ($fringe) {
        $yPos = '995';
        if ($layout->shape == 'flag') $yPos = $layout->size->height - 5;
        $body .= "<g transform=\"translate(0,$yPos)\">$fringe</g>";
    }
    if ($supports) {
        $body = '<g transform="translate(150,0) scale(0.7) ">' . $body . '</g>' . $supports;
        if ($compartment) {
            $body = quickPlace($compartment['body'], $compartment['width'], $compartment['height'], 1000, 300, 500, 910, rgb($compartment['fill']), $compartment['adjustments']) . $body;
        }
        if ($motto) {
            $body .= quickPlace($motto['body'], $motto['width'], $motto['height'], 1000, 200, 500, 985, null, $motto['adjustments']);
            // $body .= '<g transform="translate(0, 885)">' . $motto . '</g>';
        }
    }
    if (!$options['asFile']) {
        $body .= '<text id="test1" x="10" y="' . $y1Location . '" font-size="30" >' . $version['website'] . '</text><text id="release-id" x="990" y="' . $y1Location . '" text-anchor="end" font-size="30" >' . $version['release'] . '</text>';
    }
    if ($effect != 'shiny') { // other effects are applied everywhere
        $body .= add_def();
        $body = getEffect($effect, 'below') . '<g ' . getEffect($effect, 'filter') . '>' . $body . getEffect($effect, 'above') . '</g>';
    }

    if (array_key_exists('debug', $options) && strpos($options['debug'],'grid') !== false) {
        $ordinary = [];
        include "ordinaries/grid.inc";
        $body .= '<g fill="#555555">' . $ordinary['body'] . '</g>';
    }
    return $header . $meta . $defs . $body . $trailer;
}
