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

function makePlain(&$children, $layout)
{
    global $options;
    $retval = '';

    // Does this shield contain a chief?
    $chief = null;
    $field = null;
    $objects = null;

    // Process the shield parts
    foreach ($children as $kid) {
        switch ($kid->nodeName) {
            case 'field':
                $field = $kid;
                break;
            case 'objects':
                $objects = $kid;
                break;
            case "chief":
                if ($layout->chief)
                    $chief = $kid;
                break;

        }
    }

    $layout_field = $layout;
    $layout_objects = $layout;
    $layout_chief = null;

    $off = 0;
    $clip_attr = "";
    if ($chief) {
        $layout_chief = new ShieldLayout($layout->chief->size, "chief", null, null, false);
        $new_size = new ShieldSize($layout->size->width, $layout->size->height - $layout->chief->size->height);
        $cross_c = new ShieldPos($new_size->width / 2, $new_size->height / 2);
        if ($layout->cross_point->y < $layout->size->height / 2)
            $cross_c->y = 400;
        $layout_objects = new ShieldLayout($new_size, $layout->shape, $cross_c, null, false, $layout->section);
        $layout_field = clone $layout;
        $off = $layout_chief->size->height;
        $layout_field->cross_point = $layout_objects->cross_point->translated(0, $off);
        $layout_field->division_offset = new ShieldPos(0, $off);
        list($clip_id, $clip) = make_def("clipPath", $layout_objects->rect_area()->margin(-100, 0, -100, -100)->svg_rect());
        $retval .= "<defs>$clip</defs>";
        $clip_attr = " clip-path='url(#$clip_id)'";
    }

    // Paint the field
    if ($field && !array_key_exists('raw', $options)) {
        $retval .= apply_tincture(
            $field->firstChild,
            "<rect x='0' y='0' width='1000' height='{$layout_field->size->height}'><title>Field</title></rect>",
            $layout_field,
            $layout_field->size
        );
    }

    if ($objects) {
        // process ordinaries and charges
        $retval .= "<g transform='translate(0,$off)'$clip_attr>" . makeOrdChgs($objects, $layout_objects) . "</g>";
    }


    if ($chief)
        $retval .= makeChief($chief, $layout_chief);

    return $retval;
}

function makeChief($node, $layout)
{
    return makeOrdinary($node->firstChild, $layout);
}

function makeAlongside(DOMNode $node, $layout)
{
    global $options;
    $arrangement = $node->getAttribute(blazonML::A_KEYTERM) ?? 'alongside';
    $retval = '';
    $partsList = [];

    /**
     * @var DOMElement $kid
     */
    foreach ($node->childNodes as $kid) {
        if ($kid->nodeName == blazonML::E_SHIELD ) {
            $shield = makeshield($kid, $layout);
            // layout may have changed
            $layout = ShieldLayout::from_options($options);
            $outline = getShape($layout);
            $clipShield = add_def('mask', '<path fill="#FFFFFF" d="' . $outline . '" />');
            $shield = '<g mask="url(#' . $clipShield . ')">' . $shield . '</g>';
            $shield .= '<path fill="none" stroke="#848484" stroke-width="4" d="' . $outline . '" />';
            if ($options['effect'] == 'inked' || $options['asFile'] == 'printable' )
                $shield .= '<path fill="none" stroke="#000000" stroke-width="4" d="' . $outline . '" />';
            foreach($kid->childNodes as $grandchild) {
                if ($grandchild->nodeName == blazonML::E_ACHIEVEMENT) {
                    $parts = getAchievementParts($grandchild, $layout);
                    $shield = addParts($shield, $parts, $layout);
                }
            }
            switch ($kid->getAttribute('index')) {
                case '1':
                    switch ($arrangement) {
                        case 'joined':
                            $retval .= '<g transform="translate(10,100) scale(0.48) rotate(20,500,500)">' . $shield . "</g>\n";
                            break;
                        case 'alongside':
                        default:
                            $retval .= '<g transform="translate(10,100) scale(0.48)">' . $shield . "</g>\n";
                            break;
                    }
                    break;
                case '2':
                    switch ($arrangement) {
                        case 'joined':
                            $retval .= '<g transform="translate(520,100) scale(0.48) rotate(-20,500,500)">' . $shield . "</g>\n";
                            break;
                        case 'alongside':
                        default:
                            $retval .= '<g transform="translate(520,100) scale(0.48)">' . $shield . "</g>\n";
                            break;
                    }
                    break;
            }
        } elseif ($kid->nodeName == blazonML::E_ACHIEVEMENT) {
            $partsList[] = getAchievementParts($kid, $layout);
        }
    }
    foreach ($partsList as $parts) {
        if (!is_null($parts) && count($parts))
            $retval = addParts($retval, $parts, $layout);
    }
    $retval .= add_def();
    return $retval;
}

function makeimpaled(DOMNodeList $children, $layout)
{
    $retval = '';
    $imp1 = $imp2 = null; // remove warnings

    $size = $layout->size->scaled(1, 2);
    $clipI = add_def('clipPath', "<rect x='0' y='0' width='{$size->width}'  height='{$size->height}'/>");
    $cp = $layout->cross_point->translated(0, $layout->cross_point->y);
    $child_layout_1 = new ShieldLayout($size, $layout->shape, $cp, null, true, "dexter");
    $child_layout_2 = new ShieldLayout($size, $layout->shape, $cp, null, true, "sinister");
    /**
     * @var DOMElement $kid
     */
    foreach ($children as $kid) {
        switch ($kid->getAttribute('index')) {
            case '1':
                $imp1 = makeshield($kid, $child_layout_1);
                break;
            case '2':
                $imp2 = makeshield($kid, $child_layout_2);
                break;
        }
    }
    $retval .= "<g clip-path='url(#$clipI)' transform='scale(0.5)' >$imp1</g>\n";
    $retval .= "<g clip-path='url(#$clipI)' transform='scale(0.5) translate({$size->width},0)' >$imp2</g>\n";
    $retval .= add_def();

    return $retval . "<path stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\" d=\"M {$size->width} 0 v {$layout->size->height}\" />\n";
}

function makedimidiated($children, $layout)
{
    $retval = '';

    $size = $layout->size->scaled(0.5, 1);
    $clip1 = add_def('clipPath', "<rect x='0' y='0' width='{$size->width}'  height='{$size->height}'/>");
    $clip2 = add_def('clipPath', "<rect x='{$size->width}' y='0' width='{$size->width}'  height='{$size->height}'/>");

    /**
     * @var DOMElement $kid
     */
    foreach ($children as $kid) {
        switch ($kid->getAttribute('index')) {
            case '1':
                $retval .= '<g clip-path="url(#' . $clip1 . ')" >' . makeshield($kid, $layout) . "</g>\n";
                break;
            case '2':
                $retval .= '<g clip-path="url(#' . $clip2 . ')" >' . makeshield($kid, $layout) . "</g>\n";
                break;
        }
    }
    $retval .= add_def();
    return $retval . "<path stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\" d=\"M 499 0 v 1200\" />\n";
}

function makeshield(DOMElement &$node, $layout)
{
    global $messages, $options;

    if ($node->hasAttribute('IDREF')) { // shouldn't happen, but just in case
        $dom = $node->ownerDocument;
        $node = $dom->getElementById($node->getAttribute('IDREF'))->cloneNode(true);
        $node->setAttribute('ID', unique('clone'));
    }

    $retval = '<g>';
    if (!is_object($node)) return '';

    /**
     * @var DOMElement $child
     */
    // go through children once to set options (overrides layout argument)
    foreach ($node->childNodes as $child) {
        if ($child->nodeName == blazonML::E_INSTRUCTIONS) {
            setOptionsFromBlazon($child);
            $layout = ShieldLayout::from_options($options);
            // The palette can be overwritten later so set a separate flag to
            // cause the hatching patterns to be created at the maximum height needed
            if ($options['palette'] == 'hatching' && !array_key_exists('addHatching', $options)) {
                    $options['addHatching'] = $layout->size->height;
            }
        }
    }
    // If at a later date we find a taller shield, use that height instead
    if (array_key_exists('addHatching', $options) && $options['addHatching'] < $layout->size->height) {
        $options['addHatching'] = $layout->size->height;
    }
// then again to draw the shields
    foreach ($node->childNodes as $child) {
        $children = $child->childNodes;
        switch ($child->nodeName) {
            case 'simple':
                $retval .= makePlain($children, $layout);
                break;
            case 'complex':
                switch ($child->getAttribute('keyterm')) {
                    case 'dimidiated':
                        $retval .= makeDimidiated($children, $layout);
                        break;
                    case 'impaled':
                        $retval .= makeImpaled($children, $layout);
                        break;
                    case 'per-saltire':
                        $retval .= make_quartered_saltire($children, $layout);
                        break;
                    case "per-pale":
                        $retval .= makeQuartered($child->getAttribute('order'), 1, $children, $layout);
                        break;
                    case "per-fess":
                        $retval .= makeQuartered(1, $child->getAttribute('order'), $children, $layout);
                        break;
                    case 'quartered':
                        switch ($child->getAttribute('order')) {
                            case 4:
                                $retval .= makeQuartered4($children, $layout);
                                break;
                            case 5:
                                $retval .= makeQuartered5($children, $layout);
                                break;
                            case 6:
                                $retval .= makeQuartered6($children, $layout);
                                break;
                            case 8:
                                $retval .= makeQuartered8($children, $layout);
                                break;
                            case 9:
                                $retval .= makeQuartered(3, 3, $children, $layout);
                                break;
                            case 12:
                                $retval .= makeQuartered(3, 4, $children, $layout);
                                break;
                            case 16:
                                $retval .= makeQuartered(4, 4, $children, $layout);
                                break;
                            case 20:
                                $retval .= makeQuartered(4, 5, $children, $layout);
                                break;
                            case 25:
                                $retval .= makeQuartered(5, 5, $children, $layout);
                                break;
                            case 30:
                                $retval .= makeQuartered(5, 6, $children, $layout);
                                break;
                            case 36:
                                $retval .= makeQuartered(6, 6, $children, $layout);
                                break;
                            case 42:
                                $retval .= makeQuartered(6, 7, $children, $layout);
                                break;
                            case 49:
                                $retval .= makeQuartered(7, 7, $children, $layout);
                                break;
                            case 56:
                                $retval .= makeQuartered(7, 8, $children, $layout);
                                break;
                            case 64:
                                $retval .= makeQuartered(8, 8, $children, $layout);
                                break;
                            default:
                                $messages->addMessage('internal', 'No code to draw ' . $children->length . ' quarters yet');
                                break;
                        }
                }
                break;
            case 'missing':
                break;
            case 'overall':
                $retval .= makeOrdChgs($child->firstChild, $layout);
                break;
        }
        if ($layout->shape == 'italian') { // another horrible fudge
            $retval .= '<g>
                    <path
                        d="m 344.81,3.6726 38.87,68.8004 -38.87,44.027 c 98.25,-11.08 203.99,-10.83 294.76,0 l -25.1,-43.029 25.1,-69.7984 c -91.97,12.5284 -196.51,13.0914 -294.76,0 z"
                        style="fill:#f3f1ee;stroke:#808080;stroke-width:7.17741;stroke-miterlimit:1" />
                    <path
                        d="m 7.3198,182.27 c 153.3802,0 216.1402,-145.737 308.6002,-145.737 38.95,0 47.72,17.957 47.72,37.931 -21.32,6.107 -38.06,7.221 -62.77,-1.998 3.71,24.327 27.69,43.914 60.28,43.914 30.41,0 66.46,-18.364 66.46,-53.9 0,-29.654 -35.64,-58.8906 -97.85,-58.8906 -140.82,0 -162.71,123.0406 -322.4402,178.6806 z"
                        style="fill:#f3f1ee;stroke:#808080;stroke-width:7.17741;stroke-miterlimit:1" />
                    <path
                        d="m 991.87,182.27 c -152.89,0 -215.82,-145.737 -308.28,-145.737 -38.95,0 -47.72,17.957 -47.72,37.931 21.33,6.107 38.06,7.221 62.77,-1.998 -3.62,24.327 -27.6,43.914 -60.19,43.914 -30.5,0 -66.55,-18.364 -66.55,-53.9 0,-29.654 35.65,-58.8906 97.85,-58.8906 141.06,0 162.79,123.0406 322.12,178.6806 z"
                        style="fill:#f3f1ee;stroke:#808080;stroke-width:7.17741;stroke-miterlimit:1" />
                    </g>';
        } elseif ($layout->shape == 'pauldron' && !isset($options['notrim'])) {
            $retval .= '          <path
                    d="m 41.536147,871.59393 c 0,0 7.47903,-641.82667 458.680763,-641.93028 451.20173,-0.10361 456.7417,646.64289 456.7417,646.64289 z M 1.2817646,953.9012 c 0,0 -3.0191322,-756.24112 499.0203454,-754.95935 502.03949,1.28177 496.91075,760.08641 496.91075,760.08641 z"
                    style="fill:#f3f1ee;stroke:#808080;stroke-width:7.17741;stroke-miterlimit:1;fill-rule:evenodd;opacity:0.5" />';
        }
    }
    return $retval . '</g>';
}
