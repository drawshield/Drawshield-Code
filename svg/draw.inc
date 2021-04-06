<?php 

// Global variables for parsing
$svg_region = 'PL';
$svg_chief = 'NC';

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


function region ( $action, $region = null ) {
  global $svg_region, $messages;
  static $stack = array();
  
//  $messages->addMessage('internal', 'region: ' . $action . " " . $region);

  switch ( $action ) {
    case 'push':
      array_push( $stack, $svg_region );
      $svg_region = $region;
      break;
    case 'swap':
      $svg_region = $region;
      break;
    case 'pop':
      $svg_region = array_pop( $stack );
      break;
  }
}

function chief ( $action, $chief = null ) {
  global $svg_chief;
  static $stack = array();

  switch ( $action ) {
    case 'push':
      array_push( $stack, $svg_chief );
      $svg_chief = $chief;
      break;
    case 'swap':
      $svg_chief = $chief;
      break;
    case 'pop':
      $svg_chief = array_pop( $stack );
      break;
  }
}

function chiefPresent() {
  global $svg_chief;

  return ( $svg_chief == 'CH' );
}

function choose_fit( $choices ) {
  global $svg_region, $svg_chief, $messages;
  $retval = null;
  $found = null;
  
  //  $messages->addMessage('internal', "Searching for $svg_region + $svg_chief");


  if ( !is_array($choices) ) {
      $retval = $choices;
      $found = "(fixed)";
  }
    // Most detailed, look for the combination
  $combo = $svg_region . $svg_chief;
  if ( $found == null && array_key_exists($combo, $choices) ) {
      $retval =  $choices[$combo];
      $found = $combo;
  }
  // Look for chief/no chief first
  if ( $found == null && array_key_exists($svg_chief, $choices) ) {
      $retval =  $choices[$svg_chief];
      $found = $svg_chief;
  }
  // Look for region alone next
  if ( $found == null && array_key_exists($svg_region, $choices) ) {
      $retval =  $choices[$svg_region];
      $found = $svg_region;
  }

  if ( $retval == null ) {
      $messages->addMessage('internal', 'Cannot find placement for region');     
  }
  
  // raise error
  return $retval;
}


function find_chief ( $ord_chgs ) {
  /**
   * @var DOMElement $child
   */
  foreach ( $ord_chgs->childNodes as $child ) {
    if ( $child->nodeName == 'ordinary' and $child->getAttribute('keyterm') == 'chief' ) return true;
  }
  return false;
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
      case 'missing':
      case 'charge':
        $retval .= makeCharge( $child, $layout );
        break;
    }
  }
  return $retval;
}

function draw() {
    global $dom;
    global $options;
    global $version;
    global $messages;

    $messages = new messageStore($dom);

    // Get all the charge data (once for each charge type)
    get_chg_data();
    $achievement = false;
    $mantling = '';
    $supports = '';
    $compartment = '';
    $motto = '';
    $helmets = [];
    $crests = [];
    $fringe = '';
    $shieldAdjustments = [];
    $y1Location = 0;
    $extraDefs = '';
    $extraSpace = 0;
    if ($options['palette'] == 'hatching') {
        include("hatching.inc");
        $extraDefs = mkHatches();
    }

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
         xmlns:blazonML="http://drawshield.net/blazonML" ' .
         'preserveAspectRatio="xMidYMid meet" height="' . $displayHeight . '" width="' . $displayWidth . '" ';
        $header .= 'viewBox="'. getEffect($effect, 'viewbox',1000, $layout->size->height + $extraSpace + 40) . ' ">';
        $meta = '';
        $defs = '<defs>' . getEffect($effect, 'defs') . $extraDefs . '</defs>';
        $trailer =  "</svg>\n";


    } else {
        $outline = getShape($layout);
        $header = '<?xml version="1.0" encoding="utf-8" ?>
    <svg version="1.1"
        baseProfile="full"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink" ' .
        'preserveAspectRatio="xMidYMid meet" height="' . (int)($options['size'] * 1.2) . '" width="' . $options['size'];
        $header .= '" viewBox="'. getEffect($effect, 'viewbox') . ' ">';
        $meta = '';
        $defs = '<defs>' . getEffect($effect, 'defs') . $extraDefs .  '</defs>';
        $trailer =  "</svg>\n";

        $blazonNode = $dom->documentElement;
        $body = '';
        $addMantling = false;
        foreach ( $blazonNode->childNodes as $node ) {
            switch ( $node->nodeName ) {
            case 'shield':
                $body .= makeshield($node, $layout);
                if ($layout->shape == 'italian') { // another horrible fudge
                    $body .= '<g>
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
                    $body .= '          <path
                    d="m 41.536147,871.59393 c 0,0 7.47903,-641.82667 458.680763,-641.93028 451.20173,-0.10361 456.7417,646.64289 456.7417,646.64289 z M 1.2817646,953.9012 c 0,0 -3.0191322,-756.24112 499.0203454,-754.95935 502.03949,1.28177 496.91075,760.08641 496.91075,760.08641 z"
                    style="fill:#f3f1ee;stroke:#808080;stroke-width:7.17741;stroke-miterlimit:1;fill-rule:evenodd;opacity:0.5" />';
                }
                break;
            case 'achievement':
                if ($layout->shape != 'pauldron' && $layout->shape != 'flag')
                    $achievement = true;
                if ($node->firstChild == null) {
                    $addMantling = true;
                    $helmets[] = drawHelmet(null,null);
                } else {
                    foreach ($node->childNodes as $subNode) {
                        switch ($subNode->nodeName) {
                        case blazonML::E_SUPPORTERS:
                            $supports = drawSupporters($subNode, $layout);
                            break;
                        case blazonML::E_MANTLING:
                            $mantling = drawMantling($subNode);
                            break;
                        case blazonML::E_SHIELD:
                            foreach ($subNode->childNodes as $childNode) {
                                if ($childNode->nodeName == blazonML::E_MODIFIER) {
                                    $shieldAdjustments[] = $childNode->getAttribute(blazonML::A_KEYTERM);
                                }
                            }
                            break;
                        case blazonML::E_FRINGE:
                            $fringe = drawFringe($subNode, $layout);
                            $achievement = false; // can't combine these
                            break;
                        case blazonML::E_COMPARTMENT:
                            $compartment = drawCompartment($subNode);
                            break;
                        case blazonML::E_MOTTO:
                            $motto = drawMotto($subNode, $layout);
                            break;
                        case blazonML::E_HELMET: // can't draw immediately in case there is a torse
                            $addMantling = true;
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
                // tidy up the combinations
                if (!$mantling && $addMantling && $layout->shape != 'pauldron')
                    $mantling = drawMantling(null);
                break;
            // other elements for future expansion
            }
        }
    }
    if (!array_key_exists('nomask', $options)) {
        $clipShield = add_def('mask', '<path fill="#FFFFFF" d="' . $outline . '" />' );
        $body = '<g mask="url(#' . $clipShield . ')">' . $body . '</g>';
        if ( $options['effect'] == 'inked')
            $body .=  '<path fill="none" stroke="#000000" stroke-width="4" d="' . $outline . '" />';

        // Add effects (if any) and Clip to shield shape
        if ($options['effect'] == 'shiny') { // this only gets applied to the escutcheon
            $body .= add_def();
            $body = getEffect($effect, 'below') . '<g ' . getEffect($effect, 'filter') . '>' . $body . getEffect($effect, 'above') . '</g>';
        }
        // Add outline of shield shape, only if required
        if ( $options['palette'] == 'outline')
            $body .=  '<path fill="none" stroke="' . rgb('division-stroke') . '" stroke-width="4" d="' . $outline . '" />';
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
                $meta = "\n<metadata>\n"  . $blazonMLText . "\n</metadata>\n";
            }
        }
    }
    if (count($shieldAdjustments)) {
        $body = quickPlace($body, 1000, 1200, 1000, 1200, 500, 600, null, $shieldAdjustments);
    }

    // assorted combinations of achievements
    if ($mantling || count($helmets) || count($crests)) {
        $body = $mantling . '<g transform="translate(191.667,400) scale(0.61667) ">' . $body . '</g>';
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
    if ($achievement) {
        foreach ($crests as $crest) {
            if ($crest == null) continue;  // some helmets may not have crests
            $body .= placeCharge($crest->firstChild, $layout, 300, 300, 500, (count($helmets) ? 150 : 270));
        }
        if (count($helmets)) {
            $body .= quickPlace($helmets[0]['body'], $helmets[0]['width'], $helmets[0]['height'], 300, 300, 500, 350, rgb($helmets[0]['fill']), $helmets[0]['adjustments']);
        }
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

    // Uncomment following 3 lines for an overall grid
    // $ordinary = [];
    // include "ordinaries/grid.inc";
    // $body .= '<g fill="#555555">' . $ordinary['body'] . '</g>';
    return $header . $meta . $defs . $body . $trailer;
}
