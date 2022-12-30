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

require_once("edges/edge_type.inc");

function createBody($spec, $lineType, $feature_size, $layout)
{

    if ($spec instanceof MutableShape) {
        $compiled = $spec->compile();

        $feature_size *= 4;
        if ($lineType) {
            $edge = EdgeType::load($lineType);
            $modified = new MutableShape();
            $edge->apply($compiled, $modified, $feature_size, $feature_size);
        } else {
            $modified = $compiled;
        }

        return $modified->to_svg_path(["fill-rule" => "evenodd"]);
    }

    $spec = subPercent($spec, $layout);
    return '<path fill-rule="evenodd" d="' . makePath($spec, $lineType, $feature_size) . '" />';
}

function makeVoid($spec, $layout, $lineType = 'none', $featureSize = 50)
{
    $spec = subPercent($spec, $layout);
    $retval = '';
    // Create the outer edge as normal
    $retval .= makePath($spec, $lineType, $featureSize);
    $innerSpec = parallelSpec($spec, 'i', $featureSize);
    $retval .= ' ' . makePath($innerSpec, 'none', 0);
    return $retval;
}

function createVoid($spec, $lineType, $feature_size, $layout)
{
    if ($spec instanceof MutableShape) {
        $new_shape = new MutableShape();
        $offsetSize = $feature_size;
        foreach ($spec->sub_paths() as $path) {
            $new_shape->add_path($path);
            $off = $path->offset_copy(-$offsetSize);
            $off->reverse();
            $off->make_plain();
            $new_shape->add_path($off);
        }
        return createBody($new_shape, $lineType, $feature_size, $layout);
    }

    $spec = subPercent($spec, $layout);
    $retval = '<path fill-rule="evenodd" d="' . makeVoid($spec, $layout, $lineType, $feature_size) . '" />';
    return $retval;
}

function createCotise($spec, $num, $lineType, $layout, $cotiseSize, $feature_size)
{
    $spec = subPercent($spec, $layout);
    $outerSize = $cotiseSize * ($num * 2);
    $innerSize = $cotiseSize * (($num * 2) - 1);
    $outerSpec = parallelSpec($spec, 'o', $outerSize);
    $innerSpec = parallelSpec($spec, 'o', $innerSize);
    $innerSpec = reverseSpec($innerSpec);
    $retval = '<path fill-rule="evenodd" d="';
    $retval .= makePath($outerSpec, $lineType, $feature_size);
    $retval .= ' ' . makePath($innerSpec, $lineType, $feature_size);
    $retval .= '" />';
    return $retval;
}


function createCottices($spec, $numberCottices, $lineType, $layout)
{
    $cotiseSize = 40;
    $feature_size = 18;

    if ($spec instanceof MutableShape) {
        $new_shape = new MutableShape();
        foreach ($spec->sub_paths() as $path) {
            $off = $cotiseSize;

            for ($i = 0; $i < $numberCottices; $i++) {
                $inner = $path->offset_copy($off, -1);
                $inner->reverse();
                $inner->make_plain();

                $outer = $path->offset_copy($cotiseSize + $off, -1);
                if ($i != $numberCottices - 1)
                    $outer->make_plain();

                $new_shape->add_path($inner);
                $new_shape->add_path($outer);

                $off += $cotiseSize * 2;
            }
        }
        return createBody($new_shape, $lineType, $feature_size, $layout);
    }

    $cotiseSVG = '';
    for ($i = 1; $i <= $numberCottices; $i++)
        $cotiseSVG .= createCotise($spec, $i, $lineType, $layout, $cotiseSize, $feature_size);
    return $cotiseSVG;
}

function division_to_modifier(DOMElement &$node, $type)
{
    // check for tincture being a quarterly division, if so handle separately & rewrite modifiers
    /**
     * @var DOMElement $item
     */
    $item = null;
    // See if there is a division of the required type
    if ($node != null) {
        foreach ($node->childNodes as $child) {
            if ($child->nodeName == 'tincture' && $child->hasChildNodes()) {
                $firstChild = resolveReference($child)->firstChild;
                if ($firstChild != null && $firstChild->nodeName == 'division' && $firstChild->getAttribute('keyterm') == $type) {
                    $item = $child;
                    break;
                }
            }
        }
    }
    if ($item == null) return false; // not found

    /**
     * @var DOMElement[] $children
     */
    $children = array();
    // We cannot just iterate through childNodes as we want to make changes, so put in an array
    foreach ($item->firstChild->childNodes as $child) {
        $children[] = (object)$child;
    }
    // We can now remove the original tincture
    $node->removeChild($item);
    // Create a new modifier node
    $mod = createModifierNode('div2ord', $type);
    // Now copy any tinctures back into the original node, and save any other modifiers
    for ($i = 0; $i < count($children); $i++)
        if ($children[$i]->nodeName == 'tincture') {
            // We swap the colours over, as body2 draws the first one (other way round for normal divisions)
            if ($children[$i]->getAttribute('index') == '1')
                $children[$i]->setAttribute('index', '2');
            else
                $children[$i]->setAttribute('index', '1');
            $node->appendChild($children[$i]);
        } else
            $mod->appendChild($children[$i]);
    // Create a modifier to show which division we replaced
    $node->appendChild($mod);

    return true;
}


function includeOrdinary(DOMElement &$node, $layout)
{
    global $messages;

    $ordinaryType = $node->getAttribute('keyterm');
    $ordinary = array();
    $file = null;
    if ($layout->shape == 'flag') { // look in the flag sub-directory first
        $file = "ordinaries/flag/$ordinaryType.inc";
        if (stream_resolve_include_path($file) === false) $file = null;
    }
    if ($file == null) {
        $file = "ordinaries/$ordinaryType.inc";
        if (stream_resolve_include_path($file) === false) $file = null;
    }
    $calcHeight = $layout->size->height; // value to use for calculating drawing height
    if ($file != null) { // calcHeight is available within the included file
        include($file);
    } else { // can't draw this ordinary
        $messages->addMessage('warning', "Cannot draw $ordinaryType, substituting bend" . getLine($node));
        include('ordinaries/bend.inc');
    }
    return $ordinary;
}

function getSpecial($type, $node, $layout)
{
    global $options, $messages;
    $special = [];

    $filename = $layout->shape;

    $linetype = getModifierByType($node, 'linetype', false);

    if ($linetype != null)
        $filename .= '-' . $linetype;

    if ($layout->size->height < 1000)
        $filename .= '-chief';

    $compony = getModifierByType($node, 'compony');

    if ($compony != null)
        $filename .= '-compony1-of16';

    $folder = __dir__ . "/ordinaries/$type/";

    $subfile = getBestMatchFilename($folder, $filename, null, false);

    if ($subfile == null)
        return includeOrdinary($node, $layout);;

    $basename = basename($subfile, ".svg");
    if (strpos($basename, "-") !== false && (strpos($filename, "-") === false || $basename != $filename))
        return includeOrdinary($node, $layout);;

    $svg = simplexml_load_file($folder . $subfile);

    $paths = $svg->xpath('//svg:path');
    $tempBody = '';
    foreach ($paths as $path) {
        $tempBody .= $path->asXML();
    }
    $special['body'] = $tempBody;

    $paths = $svg->xpath('//svg:g[@id="body2"]/*');
    if (is_array($paths) && count($paths)) {
        $tempBody = '';
        foreach ($paths as $path) {
            $tempBody .= $path->asXML();
        }
        $special['body2'] = $tempBody;
    }

    switch ($layout->shape) {
        case 'drawshield':
            break;
        case 'french':
            break;
        case 'italian':
            break;
        case 'polish':
            break;
        case 'spanish':
            break;
        case 'sca':
            break;
        case 'english':
            break;
        case 'oval':
            break;
        case 'german':
            break;
        case 'square':
            break;
        case 'circle':
            break;
        case 'lozenge':
            break;
        case 'pauldron':
            $options['notrim'] = true; // don't put the grey translucent border on
            break;
        case 'swiss':
            break;
        default:
            break;
    }

    $special['chargesWithin'] = $layout->rect_area()->margin(250, 0, 250, 350);

    $special['chargesOn'] = 'x///////////500,30,55,55:250,30:750,30:500,1150:35,500:965,500:35,200:965,200:35,800:965,800:200,1050:800,1050';

    return ($special);

}

function makeOrdinary(DOMElement &$node, $layout)
{
    /**
     * @var
     */
    global $options, $messages;

    // Get information about the shapes to be drawn
    // We do this first in case the ordinary code changes any of the modifiers, e.g. a bend quarterly
    $subtype = $node->getAttribute('keyterm');
    // special handling for bordures (as lots of options)
    if ($subtype == 'bordure') {
        $ord_data = getSpecial('bordures', $node, $layout);
    } else {
        $ord_data = includeOrdinary($node, $layout);
    }
    /**
     * @var DOMElement $rev
     */
    if ($rev = getModifierNodeWithKeyterm($node, 'reversed', true)) { // reversed means inverted for most ordinaries
        switch($subtype) {
            case 'baton':
                $rev->setAttribute('keyterm', 'sinister');
                break;
            default:
                $rev->setAttribute('keyterm', 'inverted');
                break;
        }
    }

    // On X another Y means "voided", so we fix it here
    // Note, both $on_ord and $on_object might be used later on
    if ((($on_ord = getModifierNodeWithKeyterm($node, 'on')) != null) or (($on_ord = getModifierNodeWithKeyterm($node, 'charged-with')) != null)) {
        $onObject = $on_ord->firstChild;
        if ($onObject->getAttribute('keyterm') == 'another') {
            $voidTincture = getTinctureNode($onObject);
            $myVoided = createModifierNode('ordmods', 'voided');
            $myVoided->appendChild($voidTincture);
            $node->replaceChild($myVoided, $on_ord);
            $on_ord = null;
        }
    }

    // Some defaults (either from the ordinary data or from modifiers)
    if (array_key_exists('feature_size', $ord_data)) {
        $feature_size = $ord_data['feature_size'];
    } elseif ($layout->shape == 'flag') {
        $feature_size = (int)($layout->size->height * 0.05);
    } else {
        $feature_size = 40;
    }
    /**
     * @var DOMElement $voided
     */
    $voided = getModifierNodeWithKeyterm($node, 'voided');
    $tinc1 = getTinctureNode($node, '1');
    $cotiseSVG = '';

    // Create the basic shape
    $bodySVG = '';
    if (array_key_exists('shape_spec', $ord_data)) { //  we are given a path specification to convert to SVG
        $shape_spec = $ord_data['shape_spec'];
        if ($voided != null) {
            if (($void_col = $voided->firstChild) != null)
                $bodySVG .= apply_tincture($void_col, createBody($shape_spec, 'none', 40, $layout), $layout);
            $bodySVG .= createVoid($shape_spec, getModifierByType($node, 'linetype', false), $feature_size, $layout);
        } else {
            $bodySVG .= createBody($shape_spec, getModifierByType($node, 'linetype', false), $feature_size, $layout);
        }
        // Now do the cotises
        /**
         * @var DOMElement $cot_node
         */
        if (($cot_node = getModifierNodeWithKeyterm($node, 'cotticing', false)) != null) { // Need the value later
            $cot_linetype = getModifierByType($cot_node, 'linetype', false);
            $cot_tinc = getTinctureNode($cot_node);
            if (!$cot_tinc)
                $cot_tinc = $tinc1;
            $numberCottices = $cot_node->getAttribute('value');
            $cotiseSVG .= createCottices($shape_spec, $numberCottices, $cot_linetype, $layout);
            $cotiseSVG = apply_tincture($cot_tinc, $cotiseSVG, $layout);
            existModifierWithKeyterm($node, 'cotticing'); // mark as used
        }
    } else { // we are given direct SVG data
        if ($voided != null) {
            if (!array_key_exists('voided', $ord_data)) {
                $messages->addMessage('warning', 'cannot draw ' . $subtype . ' as voided' . getLine($node));
                $bodySVG .= subPercent($ord_data['body'], $layout);
            } else {
                if (($void_col = $voided->firstChild) != null)
                    $bodySVG .= apply_tincture($void_col, subPercent($ord_data['body']), $layout);
                $bodySVG .= subPercent($ord_data['voided'], $layout);
            }
        } else {
            $bodySVG .= subPercent($ord_data['body'], $layout);
        }
    } // We now have $body and $cotise set.

    // Find out how we are arranged
    if (array_key_exists('rotate_pattern', $ord_data) && !is_null($tinc1) && $tinc1->hasChildNodes() &&
        $tinc1->firstChild->nodeName != blazonML::E_COUNTERCHANGED)
        $rot = $ord_data['rotate_pattern'];
    else
        $rot = 0;
    $mirrored = existModifierWithKeyterm($node, 'sinister') || $subtype == 'scarpe';
    $inverted = existModifierWithKeyterm($node, 'inverted');

    // Start to build return string
    $retval = apply_tincture($tinc1, $bodySVG, $layout, null, $rot, $mirrored, $inverted);

    // Do we need to add a second tincture as an overlay?
    if (array_key_exists('body2', $ord_data)) {
        $body2_col = getTinctureNode($node, '2');
        if ($body2_col == null) {
            $messages->addMessage('warning', 'Second colour required for ordinary, gray substituted');
            $body2_col = createColourNode('gray', '2');
        }
        $body2_spec = subPercent($ord_data['body2'], $layout);
        if ($body2_spec[0] != '<') {
            $linetype = null;
            // Is there a linetype here also
            if (($div2ord = getModifierNodeWithKeyterm($node, 'div2ord')) != null)
                $linetype = getModifierByType($node, 'linetype', false);
            $body2_spec = '<path d="' . makePath($body2_spec, $linetype, 30) . '" />';
        }
        $body2 = apply_tincture($body2_col, $body2_spec, $layout, null, $rot, $mirrored, $inverted);
        $body2_fill = add_def('pattern patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="0" ' .
            'y="0" width="' . $layout->size->width . '" height="' . $layout->size->height . '"', $body2);
        $retval .= add_def() . '<g fill="url(#' . $body2_fill . ')">' . $bodySVG . '</g>';
    } elseif ((getModifierNodeWithKeyterm($node, 'compony1', false)) != null or
        (getModifierNodeWithKeyterm($node, 'compony2', false)) != null) {
        $messages->addMessage('warning', 'Sorry, compony not supported for ' . $subtype);
    }

    // Fimbriation goes on top of everything else
    if (($fimbriated = getModifierNodeWithKeyterm($node, 'fimbriated')) != null) {
        $fim_rgb = get_rgb(getTinctureNode($fimbriated));
        $fim_width = 15;
        switch ($subtype) {
            case 'crancelin':
                $fim_width = 6;
                break;
            default:
                break;
        }
        $retval .= '<g stroke="' . $fim_rgb . '" stroke-width="' . $fim_width . '" fill="none">' . "$bodySVG</g>";
    }
    if (existModifierWithKeyterm($node, 'depressed'))
        $retval .= '<g stroke="#000000" stroke-width="7">' . "$bodySVG</g>";
    if (array_key_exists('body3', $ord_data)) { // Other embellishments
        $retval .= subPercent($ord_data['body3'], $layout);
    }


    // Now add anything that is "on" the ordinary
    $onObjectSVG = '';
    /**
     * @var DOMElement $on_ord
     */
    if ($on_ord != null) {
        /**
         * @var DOMElement $onObject
         */
        foreach ($on_ord->childNodes as $onObject) {
            switch ($onObject->nodeName) {
                case 'ordinary':
                    // If the underlying ordinary is mirrored, we need to mirror the "on" item so it ends up back the right way
                    if ($mirrored) {
                        if (($sin = getModifierNodeWithKeyterm($onObject, 'sinister')) != null)
                            $onObject->removeChild($sin);
                        else
                            $onObject->appendChild(createModifierNode('chargemod', 'sinister'));
                    }
                    // Make a normal ordinary, ignore any chief present
                    $new_ord = makeOrdinary($onObject, $layout);
                    // Does the main ordinary want this handled specially (scaling and clipping to fit entirely)?
                    if (array_key_exists('if_ord_on', $ord_data) and $ord_data['if_ord_on'] == 'scale') {
                        $type = $onObject->getAttribute('keyterm'); // TODO can we find this out from a flag?
                        $height = ($type == 'per-bend' || $type == 'per-bend-sinister' || $type == 'quarterly') ? 1000 : 1200;
                        $patt_id = add_def('pattern width="100%" height="100%" patternUnits="objectBoundingBox" preserveAspectRatio="none" viewBox="0 0 1000 ' . $height . '"',
                            "<desc>Ordinary on ordinary</desc>" . $new_ord);
                        $onObjectSVG .= '<g fill="url(#' . $patt_id . ')"  >' . $bodySVG . '</g>';
                    } else {
                        $fill = add_def('pattern patternContentUnits="userSpaceOnUse" patternUnits="userSpaceOnUse" x="0" ' .
                            'y="0" width="1000" height="1200"', $new_ord);
                        // TODO should this be a mask? Or a clip path?
                        $onObjectSVG .= '<g fill="url(#' . $fill . ')">' . $bodySVG . '</g>';
                    }
                    break;
                case 'charge':
                    // If the underlying ordinary is mirrored, we need to mirror the "on" item so it ends up back the right way
                    if ($mirrored) {
                        if (($sin = getModifierNodeWithKeyterm($onObject, 'reversed')) != null)
                            $onObject->removeChild($sin);
                        else
                            $onObject->appendChild(createModifierNode('chargemod', 'reversed'));
                    }
                    if (array_key_exists('chargesOn', $ord_data)) {
                        $onPlacement = $ord_data['chargesOn'];
                        $arr = getModifierByType($onObject, 'arrangement', true);
                        if ($arr == "inchiefthrough" && $subtype == "chief" && $layout->shape == "chief") {
                            $onPlacement = calcPlace($onObject, 'h', $layout->rect_area());
                        }
                        if (is_array($onPlacement)) { // Means we have to call calcPlace
                            $onPlacement = calcPlace($onObject, $onPlacement[0], subPercent($onPlacement[1], $layout));
                        } else {
                            $onPlacement = subPercent($onPlacement, $layout);
                        }
                    } elseif (array_key_exists('chargePlacement', $ord_data)) {
                        $onPlacement = adjustPlacement(get_placement($onObject, $layout), $ord_data['chargePlacement']);
                    } else {
                        $onPlacement = 'n'; // error if no placement for ordinary with charges on it, but let placeCharges handle it
                    }
                    $onObjectSVG .= makeCharge($onObject, $layout, $onPlacement);
                    break;
            }
        }
    }

    // Is this ordinary "between" anything?
    $chargeWithinSVG = '';
    /**
     * @var DOMElement $btwn_ord
     */
    foreach (getModifiersByTypeAsArray($node, 'between', true) as $chargesWithin) {
        $chargesWithin = (object)$chargesWithin->firstChild;
        // If the underlying ordinary is mirrored, we need to mirror the "on" item so it ends up back the right way
        if ($mirrored) {
            if (($sin = getModifierNodeWithKeyterm($chargesWithin, 'reversed')) != null)
                $chargesWithin->removeChild($sin);
            else
                $chargesWithin->appendChild(createModifierNode('chargemod', 'reversed'));
        }
        if (array_key_exists('chargesWithin', $ord_data)) {
            $withinPlacement = $ord_data['chargesWithin'];
            if (is_array($withinPlacement)) { // Means we have to call calcPlace
                $withinPlacement = calcPlace($chargesWithin, $withinPlacement[0], $withinPlacement[1]);
            }
        } else {
            $withinPlacement = 'n'; // error if no placement for ordinary with charges on it, but let placeCharges handle it
        }
        $chargeWithinSVG = makeCharge($chargesWithin, $layout, $withinPlacement);
    }

    // Gather all the parts together
    $retval = add_def() . $retval . $cotiseSVG . $onObjectSVG . $chargeWithinSVG;

    // Are there any modifiers that affect over all position?
    $transform = '';
    if ($mirrored) {
        $transform .= "translate({$layout->size->width},0) scale(-1,1) ";
    }
    if (existModifierWithKeyterm($node, 'enhanced')) {
        $yshift = $layout->size->height * -0.1666; // Adjust to change movement for enhanced (match in layout.inc)
        $transform .= 'translate(0,' . $yshift . ') ';
    }
    if (existModifierWithKeyterm($node, 'abased')) {
        $yshift = $layout->size->height * 0.1666; // As above
        $transform .= 'translate(0, ' . $yshift . ') ';
    }
    if ($inverted) {
        $transform .= "translate(0,{$layout->size->height}) scale(1,-1) ";
    }
    if ($transform != '') {
        $retval = '<g ' . trace(__FILE__, __LINE__) . ' transform="' . $transform . '">' . $retval . "</g>\n";
    }
    // have we been moved to a different position (or more than one)?
    // For ease of use in switch statemes we combine any "sinister" modifier
    if (existModifierWithKeyTerm($node, 'sinister')) $subtype .= '-sinister';
    $output = '';
    $positionNodes = getModifiersByTypeAsArray($node, 'position', true);
    if ($positionNodes) {
        foreach ($positionNodes as $positionNode) {
            // only certain combinations of position and ordinary actually make sense
            $position = $positionNode->getAttribute('keyterm');
            $valid = false;
            switch ($position) {
                case 'inchief':
                case 'inbase':
                    switch ($subtype) {
                        case 'bar':
                        case 'closet':
                        case 'barrulet':
                        case 'humet':
                        case 'fess':
                            // move from being centered on height to centered on 1/5 of height
                            if ($position == 'inchief') {
                                $yshift = $layout->size->height * -0.3;
                            } else {
                                $yshift = $layout->size->height * 0.6;
                            }
                            $transform .= 'translate(0, ' . $yshift . ') ';
                            $output .= '<g transform="' . $transform . '">' . $retval . "</g>\n";
                            $valid = true;
                            break;
                        default:
                            break;
                    }
                case 'indexside':
                case 'insinside':
                    switch ($subtype) {
                        case 'palet':
                        case 'endorse':
                        case 'pale':
                            // move from being centered on width to centered on 1/5 of width
                            if ($position == 'indexside') {
                                $xshift = $layout->size->width * -0.25;
                            } else {
                                $xshift = $layout->size->width * 0.5;
                            }
                            $transform .= 'translate(' . $xshift . ', 0) ';
                            $output .= '<g transform="' . $transform . '">' . $retval . "</g>\n";
                            $valid = true;
                            break;
                        case 'pile':
                            // move from being centered on width to centered on 1/5 of width
                            if ($position == 'indexside') {
                                $xshift = $layout->size->width * -0.25;
                            } else {
                                $xshift = $layout->size->width * 0.5;
                            }
                            $transform .= 'translate(' . $xshift . ', 0) scale(0.5)';
                            $output .= '<g transform="' . $transform . '">' . $retval . "</g>\n";
                            $valid = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case 'inQ2':
                    switch ($subtype) {
                        case 'bendlet':
                        case 'bend':
                        case 'shakefork':
                            $xShift = $layout->size->width * 0.5;
                            $transform = 'translate(' . $xShift . ',0) scale(0.5)';
                            $output .= '<g transform="' . $transform . '">' . $retval . "</g>\n";
                            $valid = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case 'inQ1':
                    switch ($subtype) {
                        case 'bendlet-sinister':
                        case 'bend-sinister':
                        case 'shakefork':
                            $transform = 'translate(0,0) scale(0.5)';
                            $output .= '<g transform="' . $transform . '">' . $retval . "</g>\n";
                            $valid = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case 'inQ3':
                    switch ($subtype) {
                        case 'bendlet-sinister':
                        case 'bend-sinister':
                        case 'shakefork':
                            $xShift = $layout->size->width * 0.5;
                            $yshift = $layout->size->height - $xShift;
                            $transform = 'translate(' . $xShift . ',' . $yshift . ') scale(0.5)';
                            $output .= '<g transform="' . $transform . '">' . $retval . "</g>\n";
                            $valid = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case 'inQ4':
                    switch ($subtype) {
                        case 'bendlet':
                        case 'bend':
                        case 'shakefork':
                            $xShift = $layout->size->width * 0.5;
                            $yshift = $layout->size->height - $xShift;
                            $transform = 'translate(0,' . $yshift . ') scale(0.5)';
                            $output .= '<g transform="' . $transform . '">' . $retval . "</g>\n";
                            $valid = true;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        if (!$valid) {
            $messages->addMessage('error', "Placement not supported: $subtype $position");
        } else {
            $retval = $output;
        }
    }

    // And finally...
    $retval = '<g><title>' . $subtype . '</title>' . $retval . '</g>';

    if ($options['palette'] == 'outline')
        $retval = '<g stroke-width="3" stroke="' . rgb('ordinary-stroke') . '" >' . $retval . '</g>';

    return $retval;
}

