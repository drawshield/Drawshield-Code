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

function getLine($node)
{
    $retval = '';
    if ($node !== null) {
        $lineNo = $node->getAttribute(BlazonML::A_LINENUMBER);
        if ($lineNo != null) {
            $retval = " near line $lineNo";
        }
    }
    return $retval;
}


function makeOrdChgs($node, $layout)
{
    $retval = '';
    foreach ($node->childNodes as $child) {
        switch ($child->nodeName) {
            case 'ordinary':
                $retval .= makeOrdinary($child, $layout);
                break;
            case 'division':
                $retval .= makeDivision($child, $layout);
                break;
            case 'list':
                $retval .= makeList($child, $layout);
                break;
            case 'missing':
            case 'charge':
                $retval .= makeCharge($child, $layout);
                break;
        }
    }
    return $retval;
}

function setOptionsFromBlazon($node)
{
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

function drawShield($node, $layout) {
    global $options;

    $partsList = [];
    $body = makeshield($node, $layout);
    $layout = ShieldLayout::from_options($options);
    // options may have changed in makeShield
    $outline = getShape($layout);
    if (!array_key_exists('nomask', $options)) {
        $clipShield = add_def('mask', '<path fill="#FFFFFF" d="' . $outline . '" />');
        $body = '<g mask="url(#' . $clipShield . ')">' . $body . '</g>';
    }
    if ($options['effect'] == 'inked')
        $body .= '<path fill="none" stroke="#000000" stroke-width="4" d="' . $outline . '" />';
    foreach ($node->childNodes as $childNode) {
        if ($childNode->nodeName == blazonML::E_ACHIEVEMENT) {
            $partsList[] = getAchievementParts($childNode, $layout);
        }
    }
    return array ('body' => $body, 'parts' => $partsList);
}

function draw()
{
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
    $extraSpace = 0;
    $partsList = [];
    $vbHeight = null;
    $layout = ShieldLayout::from_options($options);
    if ($options['palette'] == 'hatching') $options['addHatching'] = $layout->size->height;

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
    $effect = $options['effect'];

    $svgHeight = (int)($options['size'] * 1.2);
    $svgWidth = $options['size'];

    $meta = '';
    $trailer = "</svg>\n"; // TODO this code is redundant..?
    $extraHeader = '';
    if ($options['palette'] == 'outline' || $options['palette'] == 'hatching') {
        $extraHeader .= '  <filter id="grayscale">
    <feColorMatrix type="saturate" values="0.10"/>
  </filter>';
    }

    $blazonNode = $dom->documentElement;
    $body = '';
    foreach ($blazonNode->childNodes as $node) {
        switch ($node->nodeName) {
            case blazonML::E_COMPLEX:
                $body .= makeAlongside($node, $layout);
                break;
            case blazonML::E_ROLL:
                $columns = $node->getAttribute(blazonML::A_COLS);
                $title = $node->getAttribute(blazonML::A_TITLE);
                $scale = 1 / $columns;
                $colCount = -1;
                $rowCount = 0;
                $gap = 100;
                $offset = ($gap /2) + $gap * $scale * -1;
                if (!is_null($title)) {
                    $builder = new WordBuilder();
                    $builder->setOption(WordBuilder::TEXT1, $title);
                    $builder->setOption(WordBuilder::ALIGN, WordBuilder::CENTRE);
                    $builder->setOption(WordBuilder::UNDERLINE, true);
                    // leave everything else as a default for now
                    $builder->compile();
                    $result = $builder->getSVG(true);
                    $capWidth = $result['width'];
                    $capHeight = $result['height'];
                    // work out how to scale & place it - it needs to go into a box 100 * 1000 at 0,100
                    // first, will it fit if we just scale the height?
                    $capScale = ($gap * 0.8) / $capHeight;
                    $newWidth = $capWidth * $capScale;
                    if ($newWidth > 1000) { // fit width instead
                        $capScale = 940 / $capWidth;
                        $newWidth = 940;
                    }
                    $xShift = (1000 - $newWidth) / 2;
                    $yShift = ($gap * 0.1);
                    $body .= "<g transform=\"translate($xShift,$yShift) scale($capScale)\">{$result['svg']}</g>";
                    $offset += 100;
                }
                $background = null;
                foreach ($node->childNodes as $childNode) {
                    if ($childNode->nodeName == blazonML::E_TINCTURE) {
                        $background = $childNode;
                        continue;
                    }
                    $colCount += 1;
                    if ($colCount >= $columns) {
                        $colCount = 0;
                        $rowCount += 1;
                        $offset += 100 * $scale;
                    }
                    $xShift = $colCount * (1000 * $scale);
                    $yShift = $offset + $rowCount * (1200 * $scale);
                    if ($childNode->nodeName != blazonML::E_MISSING) {
                        $rollEntry = "<g transform=\"translate($xShift,$yShift) scale($scale)\">";
                        $entry = drawShield($childNode, $layout);
                        $rollEntry .= $entry['body'];
                        if (count($partsList)) {
                            foreach ($partsList as $parts) {
                                $rollEntry .= addParts($rollEntry, $parts, $layout);
                            }
                        }
                        if ($caption = $childNode->getAttribute(blazonML::A_CAPTION)) {
                            $builder = new WordBuilder();
                            $builder->setOption(WordBuilder::TEXT1, $caption);
                            $builder->setOption(WordBuilder::ALIGN, WordBuilder::CENTRE);
                            // leave everything else as a default for now
                            $builder->compile();
                            $result = $builder->getSVG(true);
                            $capWidth = $result['width'];
                            $capHeight = $result['height'];
                            // work out how to scale & place it - it needs to go into a box 100 * 1000 at 0,1200
                            // first, will it fit if we just scale the height?
                            $capScale = ($gap * 0.8) / $capHeight;
                            $newWidth = $capWidth * $capScale;
                            if ($newWidth > 1000) { // fit width instead
                                $capScale = 940 / $capWidth;
                                $newWidth = 940;
                            }
                            $xShift = (1000 - $newWidth) / 2;
                            $yShift = 1200 + ($gap * 0.1);
                            $rollEntry .= "<g transform=\"translate($xShift,$yShift) scale($capScale)\">{$result['svg']}</g>";
                        }
                        $rollEntry .= '</g>';
                        $body .= $rollEntry;
                    }
                }
                $vbHeight =  ($gap /2) + (($rowCount + 1) * ($gap + 1200)) * $scale;
                if (!is_null($title)) {
                    $vbHeight += 100;
                }
                if (!is_null($background)) {
                    $body = apply_tincture($background, "<path d=\"m0 0 h1000 v$vbHeight h-1000 z\"/>", $layout) . $body;
                }
                break;
            case blazonML::E_SHIELD:
                $body .= makeshield($node, $layout);
                $layout = ShieldLayout::from_options($options);
                // options may have changed in makeShield
                $outline = getShape($layout);
                $effect = $options['effect'];
                if (!array_key_exists('nomask', $options)) {
                    $clipShield = add_def('mask', '<path fill="#FFFFFF" d="' . $outline . '" />');
                    $body = '<g mask="url(#' . $clipShield . ')">' . $body . '</g>';
                }
                if ($options['effect'] == 'inked')
                    $body .= '<path fill="none" stroke="#000000" stroke-width="4" d="' . $outline . '" />';
                foreach ($node->childNodes as $childNode) {
                    if ($childNode->nodeName == blazonML::E_ACHIEVEMENT) {
                        $partsList[] = getAchievementParts($childNode, $layout);
                    }
                }
                break;
        }
    }
    if (!array_key_exists('nomask', $options)) {
        if ($options['effect'] == 'shiny') { // this only gets applied to the escutcheon
            $body .= add_def();
            $body = getEffect($effect, 'below') . '<g ' . getEffect($effect, 'filter') . '>' . $body . getEffect($effect, 'above') . '</g>';
        }
        // Add outline of shield shape, only if required
        if ($options['palette'] == 'outline' || $options['asFile'] == 'printable') {
            $outline = getShape($layout);
            $body .= '<path fill="none" stroke="' . rgb('gray') . '" stroke-width="4" d="' . $outline . '" />';
        }
        // Rest depends on output format
        if ($options['asFile'] != '1' ) { // minimal decoration, no metadata
            if ($layout->shape == 'flag') {
                if (count($partsList)) $extraSpace = 120; // should really check for fringe being present
            }
            if ($options['outputFormat'] == 'svg' || $options['outputFormat'] == 'json') {
                $messages->addMessage('legal', "Heraldic images provided on this site are for education and information only. The use of heraldic devices is restricted in many countries, independently of copyright. The granting of a Coat of Arms is solely the right of the appropriate Heraldic Authority for your nationality and country of residence.");
                $messages->addMessage('legal', "{$version['website']} Version {$version['release']} {$version['email']}");
                $messages->insertMessages();
                $xpath = new DOMXPath($dom);
                $nodes = array_reverse(iterator_to_array($xpath->evaluate('//*')));
                foreach ($nodes as $node) {
                    $replacement = $dom->createElementNS('http://drawshield.net/blazonML', 'blazonML:' . $node->localName);
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
        foreach ($partsList as $parts) {
            $body = addParts($body, $parts, $layout);
        }
    }
    if ($effect != 'shiny') { // other effects are applied everywhere
        $body .= add_def();
        $body = getEffect($effect, 'below') . '<g ' . getEffect($effect, 'filter') . '>' . $body . getEffect($effect, 'above') . '</g>';
    }

    if (array_key_exists('debug', $options) && strpos($options['debug'], 'grid') !== false) {
        $ordinary = [];
        include "ordinaries/grid.inc";
        $body .= '<g fill="#555555">' . $ordinary['body'] . '</g>';
    }
    $extraDefs = '';
    if (array_key_exists('addHatching', $options)) {
        require("hatching.inc");
        $extraDefs = mkHatches($options['addHatching']);
    }
    if (is_null($vbHeight)) {
        $vbHeight = 1200;
        if ($options['shape'] == 'flag') { // need to scale tall flags to fit
            if ($options['aspectRatio'] > 1.18) {
                // $shrinkFactor = 1.18 / $options['aspectRatio'];
                // $shiftFactor = (1000 *  (1-$shrinkFactor)) / 2;
                // $body = "<g transform=\"translate($shiftFactor, 0) scale($shrinkFactor)\">$body</g>";
                $svgHeight = $svgWidth * $options['aspectRatio'];
                $vbHeight = 1000 * $options['aspectRatio'];
            } elseif ($options['aspectRatio'] < 1.22) {
                $svgHeight = $svgWidth * $options['aspectRatio'];
                $vbHeight = 1000 * $options['aspectRatio'];
            }
        }
    }
    /*   if ($options['shape'] == "flag") {
           $svgWidth = $options['printSize'];
           $svgHeight = $svgWidth * $options['aspectRatio'];
       } */

    $header = '<?xml version="1.0" encoding="utf-8" ?>
    <svg version="1.1"
        baseProfile="full"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink" 
         xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
         xmlns:cc="http://creativecommons.org/ns#"
         xmlns:dc="http://purl.org/dc/elements/1.1/" 
        xmlns:blazonML="http://drawshield.net/blazonML" ' .
        'preserveAspectRatio="xMidYMid meet" height="' . $svgHeight . '" width="' . $svgWidth;
    $header .= '" viewBox="' . getEffect($effect, 'viewbox' , 1000, $vbHeight ) . ' ">';
    $defs = '<defs>' . getEffect($effect, 'defs') . $extraDefs . $extraHeader . '</defs>';
    return $header . $meta . $defs . $body . $trailer;
}
