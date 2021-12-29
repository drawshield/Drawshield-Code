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
    $partsList= [];
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
                    foreach ($node->childNodes as $childNode) {
                        if ($childNode->nodeName == blazonML::E_ACHIEVEMENT) {
                            $partsList[] = getAchievementParts($childNode, $layout);
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
    if (count($partsList)) {
        foreach($partsList as $parts ) {
            $body = addParts($body, $parts);
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
