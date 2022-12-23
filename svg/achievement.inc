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

function drawFrontSupporters($supportNode, $layout) {
    if ($supportNode == null) return '';
    $boundX = 300;
    $boundY = 600;
    $centreY = 900;
    $dexterX = $boundX / 2;
    $sinisterX = 1000 - $dexterX;

    $supportCount = 0;
    $retval = '';
    for ($i = 0; $i < $supportNode->childNodes->length; $i++) {
        $location = $supportNode->childNodes->item($i);
        switch ($location->getAttribute('keyterm')) {
            case 'default':
                if ($supportCount == 0) { // assume dexter first
                    $retval .= placeCharge($location->firstChild, $layout, $boundX, $boundY, $dexterX, $centreY, 1, 0, 0, 1);
                    $supportCount++;
                } else {
                    $retval .= placeCharge($location->firstChild, $layout, $boundX, $boundY, $sinisterX, $centreY, 1, 0, 0, 0);
                }
                break;
            case 'dexter':
                $retval .= placeCharge($location->firstChild, $layout, $boundX, $boundY, $dexterX, $centreY, 1, 0, 0, 1);
                break;
            case 'sinister':
                $retval .= placeCharge($location->firstChild, $layout, $boundX, $boundY, $sinisterX, $centreY, 1, 0, 0, 0);
                break;
            case 'centre':
                // This is dealt with elsewhere
                break;
            }
    }
    if ($supportCount == 1) { // only one supplied, so repeat it
        $retval .= placeCharge($location->firstChild, $layout, $boundX, $boundY, $sinisterX, $centreY, 1, 0, 0, 0);
    }
    return $retval;
}

function drawRearSupporters($supportNode, $layout) {
    if ($supportNode == null) return '';
    $boundX = 1000;
    $boundY = 1200;
    $centreY = $layout->size->height / 2;
    $centreX = 500;

    $retval = '';
    for ($i = 0; $i < $supportNode->childNodes->length; $i++) {
        $location = $supportNode->childNodes->item($i);
        if ($location->getAttribute('keyterm') == 'centre') {
                $retval .= placeCharge($location->firstChild, $layout, $boundX, $boundY, $centreX, $centreY, 0, 0, 0);
        }
    }
    return $retval;
}

function drawOrnament($ornamentNode)
{
    global $messages, $options;

    $adjustments = [];
    $type = $ornamentNode->getAttribute(blazonML::A_KEYTERM);
    if (!file_exists("svg/components/ornaments/$type.svg")) {
        $messages->addMessage('internal', "Unable to draw mantling type: $type");
        return null;
    }
    foreach ( $ornamentNode->childNodes as $child) {
        if ($child->nodeName == blazonML::E_MODIFIER) {
            $adjustments[] = $child->getAttribute(blazonML::A_KEYTERM);
        }
    }
    $rawFile = file_get_contents("svg/components/ornaments/$type.svg");
    $svg = simplexml_load_string($rawFile);
    getMetaDataFromSVG($svg, 'achievement', 'mantling'); // just for the credit licence
    $retval['body'] = preg_replace('/<\?xml.*?>/i', '', $rawFile);
    if ($options['palette'] == 'outline' || $options['palette'] == 'hatching') {
        $retval['body'] = "<g filter=\"url('#grayscale')\">" . $retval['body'] . '</g>';
    }
    $retval['adjustments'] = $adjustments;
    $retval['transform'] = "translate(191.667,350) scale(0.61667)";

    return $retval;
}

function drawMantling($mantlingNode)
{
    global $messages, $options;

    $default1 = rgb('argent');
    $default2 = rgb('gray');

    if (is_null($mantlingNode)) {
        $type = 'ribbons';
    } else {
        $type = $mantlingNode->getAttribute(blazonML::A_KEYTERM);
    }

    $extras = array(); // not used at present (but required for getSVGFragment)
    $features = array();
    $adjustments = [];
    $useRawSVG = true;

    $yShift = 160;
    $scale = 0.7;
    switch($type) {
        case 'grandee-of-spain-crowned':
            $yShift = 470;
            $scale = 0.55;
            break;
        case 'grandee-of-spain':
            $yShift = 360;
            $scale = 0.57;
            break;
        case 'prince-hre-crowned':
            $yShift = 460;
            $scale = 0.55;
            break;
        case 'royal':
            $yShift = 0;
            $scale = 0.8;
            break;
        case 'prince-hre':
            $yShift = 260;
            $scale = 0.55;
            break;
        case 'cloak':
        case 'cloak-ermine':
            $useRawSVG = false;
            $features['background'] = rgb('argent');
            $features['tails'] = rgb('sable');
            $yShift = 270;
            break;
        case 'cloak-pean':
            $useRawSVG = false;
            $features['background'] = rgb('sable');
            $features['tails'] = rgb('or');
            $yShift = 270;
            $type = 'cloak-ermine';
            break;
        case 'cloak-counter-ermine':
            $useRawSVG = false;
            $features['background'] = rgb('sable');
            $features['tails'] = rgb('argent');
            $yShift = 270;
            $type = 'cloak-ermine';
            break;
        case 'cloak-erminois':
            $useRawSVG = false;
            $features['background'] = rgb('or');
            $features['tails'] = rgb('sable');
            $yShift = 270;
            $type = 'cloak-ermine';
            break;
        case 'napoleanic-pavilion':
        case 'cloak-pavilion':
            $yShift = 360;
            $scale = 0.65;
            break;
        case 'peer-of-france':
            $yShift = 350;
            $scale = 0.5;
            break;
        case 'napoleanic-pavilion-crowned':
            $yShift = 550;
            $scale = 0.5;
            break;
        case 'peer-of-france-crowned':
            $yShift = 460;
            $scale = 0.55;
            break;
        case 'leaves':
            $useRawSVG = false;
            $yShift = 270; $scale = 0.65;
            $default1 = $default2 = rgb('vert');
            break;
        case 'ribbons':
            $useRawSVG = false;
            $yShift = 170; $scale = 0.75;
            break;
    }
    $xShift = (1000 * (1-$scale)) / 2;

    if (!file_exists("svg/components/mantling/$type.svg")) {
        $messages->addMessage('internal', "Unable to draw mantling type: $type");
        return null;
    }
    // switch on type here
    $rawFile = file_get_contents("svg/components/mantling/$type.svg");
    $svg = simplexml_load_string($rawFile);
    getMetaDataFromSVG($svg, 'achievement', 'mantling'); // just for the credit licence
    $defs = [];

    if (!$useRawSVG){ // have complex colour scheme
        // default colours
        $features = array_merge( $features, array(
            'sinister1' => $default1,
            'sinister2' => $default2,
            'dexter1' => $default1,
            'dexter2' => $default2
        ));
        // Sort out the actual colours in use
        for ($i = 0; $i < $mantlingNode->childNodes->length; $i++) {
            $location = $mantlingNode->childNodes->item($i);
            if ($location->nodeName != blazonML::E_LOCATION) break;
            $colours = array();
            for ($j = 0; $j < $location->childNodes->length; $j++) {
                $tincture = $location->childNodes->item($j);
                $tincture_data = new TinctureData($tincture);
                $tincture_data->pattern_transform = "scale(0.3)";
                $tincture_data->parse_fill(new ShieldLayout(new ShieldSize(1000, 1000), "square"));
                if ($tincture_data->fill) {
                    $colours[] = $tincture_data->fill;
                    $defs = array_merge($defs, $tincture_data->defs);
                } else {
                    $messages->addMessage('error', "complex mantling tinctures are not supported");
                }
            }
            $numCols = count($colours);
            if ($numCols < 1) $colours[0] = rgb('gray');
            switch ($location->getAttribute('keyterm')) {
                case 'default':
                    $features['sinister1'] = $colours[0];
                    $features['dexter1'] = $colours[0];
                    if ($numCols < 2) {
                        // check no clash with default colours
                        if ($colours[0] == $default2) {
                            $features['sinister2'] = $default1;
                            $features['dexter2'] = $default1;
                        }
                    } else {
                        $features['sinister2'] = $colours[1];
                        $features['dexter2'] = $colours[1];
                    }
                    break;
                case 'sinister':
                    $features['sinister1'] = $colours[0];
                    if ($numCols < 2) {
                        if ($colours[0] == $default2) {
                            $features['sinister2'] = $default1;
                        }
                    } else {
                        $features['sinister2'] = $colours[1];
                    }
                    break;
                case 'dexter':
                    $features['dexter1'] = $colours[0];
                    if ($numCols < 2) {
                        if ($colours[0] == $default2) {
                            $features['dexter2'] = $default1;
                        }
                    } else {
                        $features['dexter2'] = $colours[1];
                    }
                    break;
                default:
                    $messages->addMessage('internal', 'Unknown location for mantling tincture');
                    break;
            }
        }
        $retval = getSvgFragment($svg, $features, 'mantling', $type, $extras, getStroke(null, true));
    } else {
        $retval['body'] = preg_replace('/<\?xml.*?>/i', '', $rawFile);
    }
    $retval['transform'] = "translate($xShift,$yShift) scale($scale)";
    if (count($defs))
        $retval['body'] = "<defs>" . implode("", $defs) . "</defs>" . $retval['body'];
    foreach ( $mantlingNode->childNodes as $child) {
        if ($child->nodeName == blazonML::E_MODIFIER) {
            $adjustments[] = $child->getAttribute(blazonML::A_KEYTERM);
        }
    }
    $retval['adjustments'] = $adjustments;

    return $retval;
}

function drawTorse($torseNode)
{
    global $messages;

    if (is_null($torseNode)) {
        $type = 'torse';
    } else {
        $type = $torseNode->getAttribute(blazonML::A_KEYTERM);
    }

    $features = [];
    $extras = array(); // not used at present (but required for getSVGFragment)

    // switch on type here
    $svg = simplexml_load_file("svg/components/torse/$type.svg");
    $retval = getMetaDataFromSVG($svg, 'achievement', 'torse');
    $adjustments = [];

    if ($torseNode != null) {
        // Sort out the actual colours in use
        $colours = array();
        foreach ($torseNode->childNodes as $child) {
            if ($child->nodeName == blazonML::E_TINCTURE) {
                if ($child->firstChild->nodeName != 'colour') {
                    $messages->addMessage('error', "torse tinctures may only be plain colours");
                } else {
                    $colours[] = $child->firstChild->getAttribute('keyterm');
                }
            } elseif ($child->nodeName == blazonML::E_MODIFIER) {
                $adjustments[] = $child->getAttribute(blazonML::A_KEYTERM);
            }
        }
        switch (count($colours)) {
            case 0:
                $features['tincture1'] = rgb('gray');
                $features['tincture2'] = rgb('argent');
                break;
            case 1:
                $features['tincture1'] = rgb($colours[0]);
                if ($colours[0] == 'argent') {
                    $features['tincture2'] = rgb('gray');
                } else {
                    $features['tincture2'] = rgb('argent');
                }
                break;
            case 2:
                $features['tincture1'] = rgb($colours[0]);
                $features['tincture2'] = rgb($colours[1]);
        }
    }
    $retval['adjustments'] = $adjustments;
    $retval = array_merge($retval, getSvgFragment($svg, $features, 'torse', 'torse',
        $extras, getStroke(null, true)));
    return $retval;
}

function drawMotto($mottoNode, $layout) {
    global $messages, $options;

    if (is_null($mottoNode)) return '';

    $position = $mottoNode->getAttribute(blazonML::A_KEYTERM);
    $text = $mottoNode->getAttribute('text');
    $colour = rgb('sable');
    $adjustments = null;
    foreach($mottoNode->childNodes as $child) {
        switch($child->nodeName) {
            case 'modifier':
                $adjustments[] = $child->getAttribute(blazonML::A_KEYTERM);
                break;
            case 'colour':
                $colour = rgb($child->getAttribute(blazonML::A_KEYTERM));
                break;
            default:
                break;
        }
    }
    if ($options['shape'] == 'pauldron') {
        $text = htmlentities($text, ENT_XML1);
        $builder = new WordBuilder('hooge');
        $svgText = file_get_contents("svg/components/motto/plaque.svg");
        $svgText .= "<g transform=\"translate(50,1000)\">\n" .
        $builder->fitWordsToRect($text, 160, 900, $colour) . "</g>\n";
        $svgText = '<g transform="scale(0.9,0.9) translate(50,180)">' . $svgText . "</g>\n";
        return $svgText;
    } else {
        // Some hard-coded tweaks (sorry about this, has to be judged by eye)
        $maxHeight = 50;
        switch ($position) {
            case 'ribbon-above':
                $maxHeight = 55;
                break;
        }

        // open the motto file as SVG, extract the body & size etc.
        $svgDOM = new DOMDocument;
        $svgDOM->load("svg/components/motto/$position.svg");
        $svgXML = simplexml_import_dom($svgDOM);
        $retval = getSvgFragment($svgXML, array(), 'motto', $position, array(), getStroke(null, true));

        // Just extract the position for use later
        if (($dash = strpos($position,'-')) > 0) {
            $position = substr($position,$dash + 1);
        }

        // We have the SVG text as the body, find an path with the ID textPath
        $svg = $retval['body'];
        preg_match('/<path\s+?id="textPath".*?d="(.*?)".*?\/>/s',$svg,$matches, PREG_OFFSET_CAPTURE);
        $start = $matches[0][1];
        $end = strlen($matches[0][0]);
        $d = $matches[1][0];

        // Get ourselves a word builder and set up the options
        $builder = new WordBuilder();
        $text = htmlentities($text, ENT_XML1);
        // set some defaults
        $builder->setOption(WordBuilder::FONT, 'serif');
        $builder->setOption(WordBuilder::TEXT1, $text);
        $builder->setOption(WordBuilder::GIVENPATH, $d);
        // set the user options
        $builder->setOptionsFromModifiers($mottoNode);
        // overwrite the layout, just in case
        $builder->setOption(WordBuilder::LAYOUT, WordBuilder::GIVEN);
        $builder->setOption(WordBuilder::FILL, $colour);
        $builder->setOption(WordBuilder::MAXHEIGHT, $maxHeight);
        $builder->setOption(WordBuilder::STANDALONE, false);
        $builder->compile();
        $mottoSVG = $builder->getSVG(); // get SVG fragment

        // now replace the path we found with the SVG fragment
        $svg = substr_replace($svg, $mottoSVG, $start, $end);

        // Set the rest of the  parameters
        $retval['body'] = $svg;
        $retval['adjustments'] = $adjustments;
        $retval['position'] = $position;
        return $retval;
    }
    return null;
}

function drawCompartment($compartmentNode) {
    global $messages;

    $compType = $compartmentNode->getAttribute(blazonML::A_KEYTERM);
    if (!file_exists("svg/components/compartments/$compType.svg")) {
        $messages->addMessage('internal', "Not able to draw compartment $compType");
        return null;
    }
    $adjustments = [];
    $transform = 'translate(75,0) scale(0.85)';
    $proper = 'gray';
    switch ($compType) {
        case 'castle':
            $transform = 'translate(50,0) scale(0.9)';
            break;
        case 'wall':
            $proper = 'gules';
            $transform = 'translate(50,0) scale(0.9)';
            break;
        case 'mound':
            $proper = 'vert';
            break;
        case 'pedestal':
            $transform = 'translate(25,0) scale(0.95)';
            break;
        default:
            break;
    }
    $fill = null;
    foreach ($compartmentNode->childNodes as $child) {
        switch($child->nodeName) {
            case blazonML::E_TINCTURE:
                if ($child->firstChild->nodeName != blazonML::E_COLOUR) {
                    $messages->addMessage('error', 'Compartment tinctures may only be plain colours');
                } else {
                    $fill = $child->firstChild->getAttribute(blazonML::A_KEYTERM);
                }
                break;
            case blazonML::E_MODIFIER:
                $adjustments[] = $child->getAttribute(blazonML::A_KEYTERM);
                break;
        }
    }
    if ($fill == null) $fill = $proper;
    $retval = getSvgFragment(simplexml_load_file("svg/components/compartments/$compType.svg"), array(), 'compartments', $compType,
        array(), getStroke($fill));
    $retval['fill'] = $fill;
    $retval['adjustments'] = $adjustments;
    $retval['transform'] = $transform;
    return $retval;

}


function drawFringe($fringeNode, $layout) {
    global $messages;

    $shape = $layout->shape;
    if (!file_exists("svg/components/fringes/$shape.svg")) {
        $messages->addMessage('blazon', "Fringe not available for $shape shaped shields");
        return null;
    }
    if ($fringeNode->hasChildNodes() && $fringeNode->firstChild->nodeName == 'tincture') {
        $tinc = $fringeNode->firstChild;
    } else {
        $tinc = createColourNode('or' );
        $fringeNode->appendChild($tinc);
    }
    $svg = getSvgFragment(simplexml_load_file("svg/components/fringes/$shape.svg"), array(), 'fringes', $shape,
    array(), getStroke($tinc));
    return (apply_tincture($tinc, $svg['body'], $layout));

}

function drawHelmet($helmetNode) {
    global $messages;

    $defaults = array (
        'peer' => 'gray',
        'royal' => 'or',
        'esquire' => 'argent',
        'baronet' => 'gray',
        'knight' => 'or'
    );
    if ($helmetNode == null) return null; 
    $torse = null;
    if ($helmetNode->hasChildNodes()) {
        foreach($helmetNode->childNodes as $childNode) {
            if ($childNode->nodeName == blazonML::E_TORSE) {
                $torse = drawTorse($childNode);
                break;
            }
        }
    }
    $fill = null;
    $adjustments = [];
    $extras = [];
    $features = array ('visor' => rgb('or'));
    if ($torse != '') {
        $extras['torse'] = $torse;
    }
    if ($helmetNode == null) {
        $style = 'esquire';
    } else {
        $style = $helmetNode->getAttribute(blazonML::A_KEYTERM);
        foreach($helmetNode->childNodes as $child) {
            switch($child->nodeName) {
                case 'location':
                    $locNode = $child;
                    if ($locNode->hasChildNodes() && $locNode->firstChild->nodeName == 'tincture') {
                        $tincture = $locNode->firstChild;
                        if ($tincture->firstChild->nodeName != 'colour') {
                            $messages->addMessage('error', "Helmet tinctures may only be plain colours");
                        } else {
                            $fill = $tincture->firstChild->getAttribute('keyterm');
                        }
                    }
                    break;
                case 'modifier':
                    $adjustments[] = $child->getAttribute('keyterm');
                    break;
                default:
                    break;
            }
        }
    }
    // always draw a helmet if we are asked to
    $svg = simplexml_load_file("svg/components/helmet/$style.svg");
    if ($fill == null) {
        if (array_key_exists($style, $defaults)) 
            $fill = $defaults[$style];
        else
            $fill = 'gray';
    }
    $retval = getSvgFragment($svg, $features, 'helmet', $style, $extras, getStroke($fill));
    $retval['fill'] = $fill;
    $retval['adjustments'] = $adjustments;
    $retval = array_merge($retval, getMetaDataFromSVG($svg,'helmet',$style));
    return $retval;
}


function getAchievementParts($node, $layout) {
    $retval = [];
    $crests = [];
    $mottoes = [];
    $adjustments = [];
    $shieldStroke = '';
    $crestHeight = $node->getAttribute(blazonML::A_KEYTERM);
    $retval['mantlingType'] = null;

    foreach ($node->childNodes as $subNode) {
        switch ($subNode->nodeName) {
            case blazonML::E_SUPPORTERS:
                $retval['frontSupports'] = drawFrontSupporters($subNode, $layout);
                $retval['rearSupports'] = drawRearSupporters($subNode, $layout);
                break;
            case blazonML::E_ORNAMENT:
                $retval['ornament'] = drawOrnament($subNode);
                break;
            case blazonML::E_MANTLING:
                $retval['mantling'] = drawMantling($subNode);
                $retval['mantlingType'] = $subNode->getAttribute(blazonML::A_KEYTERM);
                break;
            case blazonML::E_FRINGE:
                $retval['fringe'] = drawFringe($subNode, $layout);
                break;
            case blazonML::E_COMPARTMENT:
                $retval['compartment'] = drawCompartment($subNode);
                break;
            case blazonML::E_MOTTO:
                $mottoes[] = drawMotto($subNode, $layout);
                break;
            case blazonML::E_SHIELD:
                foreach ($subNode->childNodes as $child) {
                    if ($child->getAttribute(blazonML::A_KEYTERM) == 'stroked') {
                        $colour = $child->getAttribute(blazonML::A_PARAM);
                        if ($colour == '') $colour = 'gray';
                        $shieldStroke = '<path fill="none" stroke-width="8" stroke="' . rgb($colour) . '" d="' . getShape($layout) . '" ></path>';
                    } else {
                        $adjustments[] = $child->getAttribute(blazonML::A_KEYTERM);
                    }
                }
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
    $retval['mottoes'] = $mottoes;
    $retval['adjustments'] = $adjustments;
    $retval['shieldStroke'] = $shieldStroke;
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
                        $torse = '<g transform="translate(150,515) scale(0.7)">' . $torse . '</g>';
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

function addParts($body, $parts, $layout) {
    global $messages, $options;

    extract($parts); // creates $crests, $crestHeight and other variables
    // At this point $body contains the shield in a 1000x1200 box, does it have any helmets etc. to add on top?
    // If there is some mantling, the type is available in $mantlingType as some things need to be scaled
    // and positioned differently

    if ($shieldStroke != '') $body .= $shieldStroke;

    $bodyScale = 1;
    if (count($crests)) {
        $crestOffsetY = 0;
        $crestScale = 0.3;
        $bodyScale = 1.0;
        if ($crestHeight == 'tall-crest') {
            switch ($mantlingType) {
                case 'ribbons':
                    $bodyScale = 0.8;
                    $crestScale = 0.3;
                    $crestOffsetY = -200;
                    break;
                case 'leaves':
                    $bodyScale = 0.9;
                    $crestScale = 0.35;
                    $crestOffsetY = -400;
                    break;
                default:
                    $bodyScale = 0.65;
                    $crestScale = 0.275;
                    break;
            }
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
    if (count($adjustments)) {
        $body = quickPlace($body, 1000, 1200, 1000, 1200, 500, 600, null, $adjustments);
    }

    // Ornaments
    if (isset($ornament)) {
        // $body = $ornament['body'] . '<g transform="' . $ornament['transform'] . '">' . $body . '</g>';
        $body = quickPlace($ornament['body'], 1000, 1200, 1000, 1200,
            500, 600, null, $ornament['adjustments'])
            . '<g transform="' . $ornament['transform'] . '">' . $body . '</g>';
    }

    // Mantling
    if (isset($mantling)) {
        // $body = $mantling['body'] . '<g transform="' . $mantling['transform'] . '">' . $body . '</g>';
        $body = quickPlace($mantling['body'], 1000, 1200, 1000, 1200,
                500, 600, null, $mantling['adjustments'])
            . '<g transform="' . $mantling['transform'] . '">' . $body . '</g>';
    }

    // Fringe
    if (isset($fringe)) {
        $yPos = '995';
        if ($layout->shape == 'flag') $yPos = $layout->size->height - 5;
        $body .= "<g transform=\"translate(0,$yPos)\">$fringe</g>";
    }

    // Supporters - try to fit in making maximum use of space, so a bit complicated
    if (isset($rearSupports) && $rearSupports != '') {
        // Always scale body down just a bit so at least some of the charge is visible
        $rearSupportScale = 0.8;
        $rearSupportX = (1000 * (1-$rearSupportScale)) / 2;
        $rearSupportY = (1200 * (1-$rearSupportScale)) / 2;
        $body = "$rearSupports<g transform=\"translate($rearSupportX,$rearSupportY) scale($rearSupportScale) \">$body</g>";
    }
    if (isset($frontSupports) && $frontSupports != '') {
        $supportScale = 0.7 / $bodyScale; // modify internal scaling based on presence of a crest
        $supportX = (1000 * (1-$supportScale)) / 2;
        $body = "<g transform=\"translate($supportX,0) scale($supportScale) \">$body</g>$frontSupports";
        // But might need to scale down if there isn't a compartment
        if (!isset($compartment)) {
            $body = '<g transform="translate(50,0) scale(0.9)">' . $body . '</g>';
        }
    }

    // specials for pauldrons
    if (count($mottoes) && $options['shape'] == 'pauldron') {
        $body = '<g transform="translate(0,-100) ">' . $body . '</g>' . $mottoes[0];
    }

    if (isset($compartment)) {
        $scale = 1000 / $compartment['width'];
        $body = quickPlace($compartment['body'], $compartment['width'], $compartment['height'],
                1000, 1200, 500,  1200 - ($compartment['height'] * $scale)/2,
                rgb($compartment['fill']), $compartment['adjustments']) .
                 '<g transform="' . $compartment['transform'] . '">' . $body . '</g>';
    }
    if ($options['shape'] != 'pauldron') {
        foreach ($mottoes as $motto) {
            $position = $motto['position'] ?? 'below';
            switch ($position) {
                case 'above':
                    $body .= quickPlace($motto['body'], $motto['width'], $motto['height'],
                        1000, 200, 500, 105, null, $motto['adjustments']);
                    break;
                default: // might add some more later
                case 'below':
                    $body .= quickPlace($motto['body'], $motto['width'], $motto['height'],
                        1000, 200, 500, 1085, null, $motto['adjustments']);
                    break;
            }
        }
    }

    return $body;
}
