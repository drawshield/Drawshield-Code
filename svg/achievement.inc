<?php

function drawSupporters($supportNode, $layout) {
    if ($supportNode == null) return '';
    $boundX = 300;
    $boundY = 600;
    $centreY = 600;
    $dexterX = $boundX / 2;
    $sinisterX = 1000 - $dexterX;

    $supportCount = 0;
    $retval = '';
    for ($i = 0; $i < $supportNode->childNodes->length; $i++) {
        $location = $supportNode->childNodes->item($i);
        switch ($location->getAttribute('keyterm')) {
            case 'default':
                if ($supportCount == 0) { // assume dexter first
                    $retval .= placeCharge($location->firstChild, $layout, $boundX, $boundY, $dexterX, $centreY, 0, 0, 1);
                    $supportCount++;
                } else {
                    $retval .= placeCharge($location->firstChild, $layout, $boundX, $boundY, $sinisterX, $centreY, 0, 0, 0);
                }
                break;
            case 'dexter':
                $retval .= placeCharge($location->firstChild, $layout, $boundX, $boundY, $dexterX, $centreY, 0, 0, 1);
                break;
            case 'sinister':
                $retval .= placeCharge($location->firstChild, $layout, $boundX, $boundY, $sinisterX, $centreY, 0, 0, 0);
                break;
        }
    }
    if ($supportCount == 1) { // only one supplied, so repeat it
        $retval .= placeCharge($location->firstChild, $layout, $boundX, $boundY, $sinisterX, $centreY, 0, 0, 0);
    }
    return $retval;
}

function drawMantling($mantlingNode) {
    global $messages;

    $hat = null;
    $staff = null;

    if (is_null($mantlingNode)) {
        $type = 'ribbons';
        $default1 = rgb('argent');
        $default2 = rgb('gray');
    } else {
        $type = $mantlingNode->getAttribute(blazonML::A_KEYTERM);
        switch($type) {
            case 'priest-catholic':
            case 'canon-catholic':
            case 'vicar-catholic':
                $default1 = $default2 = $hat = rgb('sable');
                break;
            case 'prelate-catholic':
                $default1 = $default2 = $hat = rgb('purpure');
                break;
            case 'bishop-catholic':
                $default1 = $default2 = $hat = rgb('vert');
                $staff = rgb('or');
                break;
            case 'chaplain-catholic':
                $default1 = $default2 = rgb('purpure');
                $hat = rgb('sable');
                break;
            default:
                $default1 = $default2 = $hat = rgb('gray');
                break;
        }
    }

    // default colours
    $features = array (
        'sinister1' => $default1,
        'sinister2' => $default2,
        'dexter1' => $default1,
        'dexter2' => $default2
    );
    $extras = array(); // not used at present (but required for getSVGFragment)

    // switch on type here
    $svg = file_get_contents("svg/components/mantling/$type.svg");
    $defs = [];

     if ($mantlingNode != null) {
        // Sort out the actual colours in use
        for ($i = 0; $i < $mantlingNode->childNodes->length; $i++) {
            $location = $mantlingNode->childNodes->item($i);
            $colours = array();
            for($j = 0; $j < $location->childNodes->length; $j++) {
                $tincture = $location->childNodes->item($j);
                $tincture_data = new TinctureData($tincture);
                $tincture_data->pattern_transform = "scale(0.3)";
                $tincture_data->parse_fill(new ShieldLayout(new ShieldSize(1000, 1000), "square"));
                if ( $tincture_data->fill )
                {
                    $colours[] = $tincture_data->fill;
                    $defs = array_merge($defs, $tincture_data->defs);
                }
                else
                {
                    $messages->addMessage('error',"complex mantling tinctures are not supported");
                }
            }
            $numCols = count($colours);
            if ($numCols < 1) $colours[0] = rgb('gray');
            switch($location->getAttribute('keyterm')) {
                case 'default':
                    $features['sinister1'] = $colours[0];
                    $features['dexter1'] = $colours[0];
                    $staff = $colours[0];
                    $hat = $colours[0];
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
                    $messages->addMessage('internal','Unknown location for mantling tincture');
                    break;
            }
        }
    }
    if ($staff) {
         $features['staff'] = $staff;
    }
    if ($hat) {
        $features['hat'] = $hat;
    }

    $retval = getSvgFragment($svg, $features, 'mantling', $type, $extras, getStroke(null, true));

    $defs_svg  = "";
    if ( count($defs) )
        $defs_svg  = "<defs>" . implode("", $defs) . "</defs>";

    return $defs_svg . $retval['body'];
}

function drawTorse($torseNode) {
    global $messages;

    if (is_null($torseNode)) {
        $type = 'torse';
    } else {
        $type = $torseNode->getAttribute(blazonML::A_KEYTERM);
    }

    // keyterm of torseNode will contain the type (possibly)
    $default1 = rgb('argent');
    $default2 = rgb('gray');

    // default colours
    $features = array (
        'tincture1' => $default1,
        'tincture2' => $default2,
    );
    $extras = array(); // not used at present (but required for getSVGFragment)

    // switch on type here
    $svg = file_get_contents("svg/components/torse/$type.svg");

     if ($torseNode != null) {
        // Sort out the actual colours in use
        for ($i = 0; $i < $torseNode->childNodes->length; $i++) {
            $location = $torseNode->childNodes->item($i);
            $colours = array();
            for($j = 0; $j < $location->childNodes->length; $j++) {
                $tincture = $location->childNodes->item($j);
                if ($tincture->firstChild->nodeName != 'colour') {
                    $messages->addMessage('error',"torse tinctures may only be plain colours");
                } else {
                    $colours[] = rgb($tincture->firstChild->getAttribute('keyterm'));
                }
            }
            $numCols = count($colours);
            if ($numCols < 1) $colours[0] = rgb('gray');
            switch($location->getAttribute('keyterm')) {
                case 'default':
                    $features['tincture1'] = $colours[0];
                    if ($numCols < 2) {
                        // check no clash with default colours
                        if ($colours[0] == $default2) {
                            $features['tincture2'] = $default1;
                        }
                    } else {
                        $features['tincture2'] = $colours[1];
                    }
                    break;
                default:
                    $messages->addMessage('internal','Unknown location for torse tincture');
                    break;
            }
        }
    }
    return getSvgFragment($svg, $features, 'torse', 'torse', $extras, getStroke(null, true));
}

function drawMotto($mottoNode, $layout) {
    global $messages;

    if (is_null($mottoNode)) return '';

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
    if ($layout->shape == 'pauldron') {
        $builder = new WordBuilder('hooge');
        $svg = file_get_contents('svg/components/scroll/plaque.svg');
        $svg .= "<g transform=\"translate(50,1000)\">\n" .
        $builder->fitWordsToRect($text, 160, 900, $colour) . "</g>\n";
        $svg = '<g transform="scale(0.9,0.9) translate(50,180)">' . $svg . "</g>\n";
        return $svg;
    } else {
        $retval = [];

        $fontSize = 45;
        $up = 2;
        $textLen = strlen($text);
        //adjust for long mottos
        if ($textLen > 10) {
            $fontSize -= (0.7 * ($textLen - 10)); // one pixel smaller per extra letter
            $up = ($textLen - 10) / 1.5;
            if ($fontSize < 15) {
                $fontSize = 15; // but have a lower limit
                $up = 12;
            }
        }

        $svg = file_get_contents('svg/components/scroll/plain.svg');
        $svg = str_replace('%TEXT%', htmlentities($text, ENT_XML1), $svg);
        $svg = str_replace('%UP%', $up, $svg);
        $svg = str_replace('%COLOUR%', $colour, $svg);
        $svg = str_replace('%FONT-SIZE%', "${fontSize}px", $svg);
        $retval = getSvgFragment($svg, array(), 'scroll', 'plain', array(), getStroke(null, true));
        $retval['adjustments'] = $adjustments;
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
    if ($fill == null) {
        switch ($compType) {
            case 'mound':
                $fill = 'vert';
                break;
            default:
                $fill = 'gray';
                break;
        }
    }
    $retval = getSvgFragment(file_get_contents("svg/components/compartments/$compType.svg"), array(), 'compartments', $compType,
        array(), getStroke($fill));
    $retval['fill'] = $fill;
    $retval['adjustments'] = $adjustments;
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
    $svg = getSvgFragment(file_get_contents("svg/components/fringes/$shape.svg"), array(), 'fringes', $shape,
    array(), getStroke($tinc));
    return (apply_tincture($tinc, $svg['body'], $layout));

}

function drawHelmet($helmetNode) {
    global $messages;

    $defaults = array (
        'peer' => 'gray',
        'royal' => 'or',
        'esquire' => 'argent',
        'baronet' => 'gray'
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
    $svg = file_get_contents("svg/components/helmet/$style.svg");
    if ($fill == null) {
        if (array_key_exists($style, $defaults)) 
            $fill = $defaults[$style];
        else
            $fill = 'gray';
    }
    $retval = getSvgFragment($svg, $features, 'helmet', $style, $extras, getStroke($fill));
    $retval['fill'] = $fill;
    $retval['adjustments'] = $adjustments;
    return $retval;
}

