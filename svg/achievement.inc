<?php

function drawMantling($mantlingNode) {
    global $messages;
    if (is_null($mantlingNode)) return '';

    // keyterm of MantlingNode will contain the type (eventually)
    $default1 = rgb('argent');
    $default2 = rgb('gray');

    // default colours
    $features = array (
        'sinister1' => $default1,
        'sinister2' => $default2,
        'dexter1' => $default1,
        'dexter2' => $default2
    );
    $extras = array(); // not used at present (but required for getSVGFragment)

    // switch on type here
    $svg = file_get_contents('svg/components/mantling/ribbons.svg');

    // Sort out the actual colours in use
    for ($i = 0; $i < $mantlingNode->childNodes->length; $i++) {
        $location = $mantlingNode->childNodes->item($i);
        $colours = array();
        for($j = 0; $j < $location->childNodes->length; $j++) {
            $tincture = $location->childNodes->item($j);
            if ($tincture->firstChild->nodeName != 'colour') {
                $messages->addMessage('warning',"mantling tinctures may only be plain colours");
            } else {
                $colours[] = rgb($tincture->firstChild->getAttribute('keyterm'));
            }
        }
    }
    $numCols = count($colours);
    if ($numCols < 1) $colours[0] = rgb('gray');
    switch($location->getAttribute('keyterm')) {
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
            $messages->addMessage('internal','Unknown location for mantling tincture');
            break;
    }

    $retval = getSvgFragment($svg, $features, 'mantling', 'ribbons', $extras);
    return $retval['body'];
}

function drawMotto($mottoNode) {
    global $messages;

    if (is_null($mottoNode)) return '';

    $text = $mottoNode->getAttribute('text');
    $fontSize = 45;
    $up = 2;
    $textLen = strlen($text);
    //adjust for long mottos
    if ($textLen > 15) {
        $fontSize -= $textLen - 15; // one pixel smaller per extra letter
        $up = ($textLen - 15) / 3;
        if ($fontSize < 12) {
            $fontSize == 12; // but have a lower limit
            $up = 8;
        }
    }

    $svg = file_get_contents('svg/components/scroll/plain.svg');
    $svg = str_replace('%TEXT%',htmlentities($text),$svg);
    $svg = str_replace('%UP%',$up,$svg);
    $svg = str_replace('%FONT-SIZE%',"${fontSize}px",$svg);
    $messages->addMessage('licence','Motto: ' . "https://commons.wikimedia.org/wiki/User:F_l_a_n_k_e_r");
    
    return $svg;
}

function drawHelmet($helmetNode) {
    global $messages;

    $defaults = array (
        'peer' => 'gray',
        'royal' => 'or',
        'esquire' => 'argent',
        'baronet' => 'gray'
    );

    if ($helmetNode == null) {
        $style = 'esquire';
    } else {
        $style = $helmetNode->getAttribute(blazonML::A_KEYTERM);
    }
    // always draw a helmet if we are asked to
    $svg = file_get_contents("svg/components/helmet/$style.svg");
    $retval = getSvgFragment($svg, array(), 'helmet', 'baronet', array());
    if (array_key_exists($style, $defaults)) 
        $fill = $defaults[$style];
    else
        $fill = 'gray';
    $retval['fill'] = $fill;
    return $retval;
}
