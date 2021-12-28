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

function getAchievementParts($node, $layout) {
    $retval = [];
    $crests = [];
    $crestHeight = $node->getAttribute(blazonML::A_KEYTERM);

    foreach ($node->childNodes as $subNode) {
        switch ($subNode->nodeName) {
            case blazonML::E_SUPPORTERS:
                $retval['supports'] = drawSupporters($subNode, $layout);
                break;
            case blazonML::E_ORNAMENT:
                $retval['ornament'] = drawOrnament($subNode);
                break;
            case blazonML::E_MANTLING:
                $retval['mantling'] = drawMantling($subNode);
                break;
            case blazonML::E_FRINGE:
                $retval['fringe'] = drawFringe($subNode, $layout);
                break;
            case blazonML::E_COMPARTMENT:
                $retval['compartment'] = drawCompartment($subNode);
                break;
            case blazonML::E_MOTTO:
                $retval['motto'] = drawMotto($subNode, $layout);
                break;
            case blazonML::E_CREST:
                $crests[] = drawCrest($subNode, $crestHeight);
                break;
            default:
                // not implemented yet
                break;
        }
    }
    // if all are null, just return empty array
    $retval['crestHeight'] = $crestHeight;
    $retval['crests'] = $crests;
    return $retval;
}

/*
 * return either a 1000x1000 box containing a charge or a helmet
 * OR a 1000x1800 box if there is a helmet somewhere in the achievement
 */
function drawCrest($crest, $crestHeight) {
    $crestCharges = '';
    $helmet = '';
    $torse = '';
    // Do we have a helmet here?
    $helmetPresent = false;
    foreach ($crest->childNodes as $child) {
        if ($child->nodeName == blazonML::E_HELMET) {
            $helmetPresent = true;
            break;
        }
    }
    // Now do the actual drawing
    foreach ($crest->childNodes as $child) {
       switch ($child->nodeName) {
           case blazonML::E_CHARGE:
               $crestCharges = makeCharge($child, ShieldLayout::square());
               if (!$helmetPresent && $crestHeight == 'tall-crest') {
                   $crestCharges = '<g transform="translate(0,800)">' . $crestCharges . '</g>';
               }
               break;
           case blazonML::E_HELMET:
               $helmetData = drawHelmet($child);
               $helmet = quickPlace($helmetData['body'], $helmetData['width'], $helmetData['height'], 1000,
                   1000, 500, 500, rgb($helmetData['fill']), $helmetData['adjustments']);
               if ($crestHeight == 'tall-crest') {
                   $helmet = '<g transform="translate(0,800)">' . $helmet . '</g>';
               }
               break;
           case blazonML::E_TORSE:
               $torseData = drawTorse($child);
               $torse = quickPlace($torseData['body'], $torseData['width'], $torseData['height'], 1000,
                   1000, 500, 500, rgb('gray'), $torseData['adjustments']);
               if ($helmetPresent) {
                   if ($crestHeight == 'tall-crest') { // tall crest, helmet near bottom
                       $torse = '<g transform="translate(150,480) scale(0.7)">' . $torse . '</g>';
                   } else { // short crest, helmet near top
                       $torse = '<g transform="translate(150,-280) scale(0.7)">' . $torse . '</g>';
                   }
               } else { // torse on its own
                   if ($crestHeight == 'tall-crest') { // tall crest, no helmet
                       $torse = '<g transform="translate(150,1200) scale(0.7)">' . $torse . '</g>';
                   } else { // short crest, no helmet
                       $torse = '<g transform="translate(0,260)">' . $torse . '</g>';
                   }
               }
               break;
       }
    }
    // make sure we have the correct layering
    return $helmet . $crestCharges . $torse;
}

function addParts($body, $parts, $shieldAdjustments = []) {
    global $messages;

    extract($parts); // creates $crests, $crestHeight and other variables
    // At this point $body contains the shield in a 1000x1200 box, does it have any helmets etc. to add on top?

    if (count($crests)) {
        $crestOffsetY = 0;
        $crestScale = 0.3;
        $bodyScale = 1.0;
        if ($crestHeight == 'tall-crest') {
            $bodyScale = 0.65;
            $crestScale = 0.275;
        } elseif ($crestHeight == 'short-crest') {
            $bodyScale = 0.8;
            $crestOffsetY = 20;
        }
        $bodyWidth = 1000 * $bodyScale;
        $bodyOffsetX = (1000 - $bodyWidth) / 2;
        $bodyOffsetY = (1200 * (1 - $bodyScale));
        $body = "<g transform=\"translate($bodyOffsetX,$bodyOffsetY) scale($bodyScale) \">$body</g>";
        switch (count($crests)) {
            case 0:
                break; // nothing to do
            case 1:
                $crestOffsetX = (1000 * (1 - $crestScale)) / 2;
                $body .= "<g transform=\"translate($crestOffsetX,$crestOffsetY) scale($crestScale) \"> ${crests[0]}</g>";
                // TODO add crown if present
                break;
            case 2:
                $w = 1000 * $crestScale;
                $gap = ($bodyWidth - $w - $w) / 3;
                $crestOffsetX = $bodyOffsetX + $gap;
                $body .= "<g transform=\"translate($crestOffsetX,$crestOffsetY) scale($crestScale) \"> ${crests[0]}</g>";
                $crestOffsetX = $bodyOffsetX + $gap + $w + $gap;
                $body .= "<g transform=\"translate($crestOffsetX,$crestOffsetY) scale($crestScale) \"> ${crests[1]}</g>";
                // TODO add crown if present
                break;
            default:
                if (count($crests) > 3) {
                    $messages->addMessage('internal', "3 helmets/crests maximum");
                }
                $w = 1000 * $crestScale;
                $gap = ($bodyWidth - $w - $w - $w) / 4;
                $crestOffsetX = $bodyOffsetX + $gap;
                $body .= "<g transform=\"translate($crestOffsetX,$crestOffsetY) scale($crestScale) \"> ${crests[0]}</g>";
                $crestOffsetX = $bodyOffsetX + $gap + $w + $gap;
                $body .= "<g transform=\"translate($crestOffsetX,$crestOffsetY) scale($crestScale) \"> ${crests[1]}</g>";
                $crestOffsetX = $bodyOffsetX + $gap + $w + $gap + $w + $gap;
                $body .= "<g transform=\"translate($crestOffsetX,$crestOffsetY) scale($crestScale) \"> ${crests[2]}</g>";
                // TODO add crown if present
                break;
        }
    }

    // We have added any helmets etc, if present, now do we need to adjust the whole thing?
    if (count($shieldAdjustments)) {
        $body = quickPlace($body, 1000, 1200, 1000, 1200, 500, 600, null, $shieldAdjustments);
    }

    // assorted combinations of achievements
    if (isset($ornament)) {
        $body = $ornament['body'] . '<g transform="' . $ornament['transform'] . '">' . $body . '</g>';
    }
    if (isset($mantling)) {
        $body = $mantling['body'] . '<g transform="' . $mantling['transform'] . '">' . $body . '</g>';
    }
    if (isset($motto) && !isset($supports)) {
        if ($layout->shape == 'pauldron') {
            $body = '<g transform="translate(0,-100) ">' . $body . '</g>' . $motto;
        } else {
            if ($helmets == [] && !isset($mantling)) { // only motto
                $body = '<g transform="translate(57,0) scale(0.88) ">' . $body . '</g>';
            }
            $body .= quickPlace($motto['body'], $motto['width'], $motto['height'], 1000, 200, 500, 985, null, $motto['adjustments']);
        }
    }
    if (isset($fringe)) {
        $yPos = '995';
        if ($layout->shape == 'flag') $yPos = $layout->size->height - 5;
        $body .= "<g transform=\"translate(0,$yPos)\">$fringe</g>";
    }
    if (isset($supports)) {
        $body = '<g transform="translate(150,0) scale(0.7) ">' . $body . '</g>' . $supports;
        if (isset($compartment)) {
            $body = quickPlace($compartment['body'], $compartment['width'], $compartment['height'], 1000, 300, 500, 910, rgb($compartment['fill']), $compartment['adjustments']) . $body;
        }
        if (isset($motto)) {
            $body .= quickPlace($motto['body'], $motto['width'], $motto['height'], 1000, 200, 500, 985, null, $motto['adjustments']);
            // $body .= '<g transform="translate(0, 885)">' . $motto . '</g>';
        }
    }
    return $body;
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
    $parts = [];
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
                        if ($childNode->nodeName == blazonML::E_ACHIEVEMENT) {
                            $parts = getAchievementParts($childNode, $layout);
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
    if (count($parts)) {
        $body = addParts($body, $parts);
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
