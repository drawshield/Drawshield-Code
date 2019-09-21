<?php /* Copyright 2010 Karl R. Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

 $chg_data_cache = array();
 
 function get_chg_data() {
   global $xpath;
   global $chg_data_cache;
   
   /**
    * @var DOMElement $chg
    */
   foreach ( $xpath->query("//charge") as $chg ) { // TODO change this to use the ID as key
    if (substr($chg->getAttribute('keyterm'), 0, 8) == 'special/') continue;
     $chg_ref = unique('chg');
     $chg_data_cache[$chg_ref] = read_chg_file($chg);
     // Add a modifier to link to the cached charge data
     $chg->appendChild(createModifierNode(languageDB::CHARGE_MODS, 'chg_data', $chg_ref ));
   }
 }

function getBestMatchFilename($folder, $base, $mods) {
  global $messages;
  $lowestPenalty = 9999;
  $bestFilename = '';
  $bestMissing = '';
  $bestExtra = '';
  $presentNotRequiredPenalty = 10;
  $requiredNotPresentPenalty = 20;
  $positionWeight = 2;

  $dh = opendir($folder);
  if ($dh === false) {
    $messages->addMessage('internal',"Cannot open charge folder - $folder");
    $messages->addMessage('internal',"Current working dir - " . getcwd());
    return null;
  }
  $base .= $mods;
  // error_log("Testing for $base");
  $requiredParts = explode('-', $base);
  $numRequiredParts = count($requiredParts);

  while (false !== ($entry = readdir($dh))) {
    $penalty = 0;
    $currentRNPPenalty = $requiredNotPresentPenalty;
    $missing = '';
    $extra = '';
    if (substr($entry,-4) != '.svg') { continue; } // ignore non-svg files
    $filenameParts = preg_split('/(\.|-)/', $entry);
    $numFilenameParts = count($filenameParts) - 1; // last entry is 'svg', so ignore it
    // Base name of the item MUST match
    if ($requiredParts[0] != $filenameParts[0]) { continue; }

    // check for required, not present
    for ($i = 1; $i < $numRequiredParts; $i++) {
      $found = false;
      for ($j = 1; $j < $numFilenameParts; $j++) {
        if ($requiredParts[$i] == $filenameParts[$j]) {
          $found = true;
          break;
        }
      }
      if (!$found) {
        $penalty += $currentRNPPenalty;
        $missing .= $requiredParts[$i] . ' ';
      }
      $currentRNPPenalty += $positionWeight;
    }

    // check for present, not required
    for ( $j = 1; $j < $numFilenameParts; $j++) {
      $found = false;
      for ( $i = 1; $i < $numRequiredParts; $i++) {
        if ($requiredParts[$i] == $filenameParts[$j]) {
          $found = true;
          break;
        }
      }
      if (!$found) {
        $penalty += $presentNotRequiredPenalty;
        $extra .= $filenameParts[$j] . ' ';
      }
    }
    // error_log("$entry scores $penalty");
    // compare to previous best fit
    if ( $penalty < $lowestPenalty ) {
      $lowestPenalty = $penalty;
      $bestFilename = $entry;
      $bestMissing = $missing;
      $bestExtra = $extra;
    }
    if ($penalty == 0 ) { // perfect match, skip out
      break;
    }
  }
  closedir($dh);
  if ($bestMissing) $messages->addMessage('warning', $base . ' not drawn with ' . $bestMissing);
  // if ($bestExtra) $messages->addMessage('other', $base . ' is also ' . $bestExtra);
  return $bestFilename;
}

function featuresThatAreAlsoMods($node) {
  $retval = '';
  static $features = array (
    'head',
    'jessant',
    'voided',
    'holding-nut',
    'armoured',
    'fructed',
    'winged',
  );

  foreach ($features as $feature) {
    if (has_feature($node, $feature)) {
      $retval .= '-' . $feature;
    }
  }

  $bundle = getModifierNodeByType($node, 'bundle');
  if ($bundle) {
      $retval .= '-' . $bundle->getAttribute('keyterm') . "-" . $bundle->getAttribute('value');
  }
  return $retval;
}
   
function read_chg_file(DOMElement $node) {
  global $messages;

  $title = '???';
  $chg_data = array();
  if ( is_null($node) or $node->nodeName == 'missing' ) {
    $title = 'missing';
    $chg_data['svgCode'] = '<svg height="100" width="100"><g stroke="none"><path fill-rule="evenodd" d="M0,0h100v100h-100zM10,10h80v80h-80z" />' .
        '<text x="50" y="55" font-size="15" text-anchor="middle">' . $title . '</text></g></svg>';
  } else {
    $splitArray = preg_split(':/:', $node->getAttribute('keyterm'));
    if (count($splitArray) != 2) {
      $messages->addMessage('internal',"Problem reading charge with keyterm " . $node->getAttribute('keyterm'));
      $chg_data['svgCode'] = '<svg height="100" width="100"><g stroke="none"><path fill-rule="evenodd" d="M0,0h100v100h-100zM10,10h80v80h-80z" />' .
        '<text x="50" y="55" font-size="15" text-anchor="middle">' . $title . '</text></g></svg>';
      return $chg_data;
    }
    list($group,$chargeKey) = $splitArray;
    $folder = "svg/charges/$group/";
    if ( ($title = $node->getAttribute('tokens')) == null ) $title = $chargeKey;

    // look for .inc file
    $file = $folder . $chargeKey . '.inc';
    if ( file_exists( $file )) {
      $chg_data = includeCharge( $node, $file, $chg_data, $folder, $chargeKey );
    }
    // TODO check if this still happens???
      // It is possible that some node attributes have been changed, so re-read them
    list($group,$chargeKey) = preg_split(':/:', $node->getAttribute('keyterm'));
    $folder = "svg/charges/$group/";

    if ( !array_key_exists('svgCode',$chg_data) and !array_key_exists('file',$chg_data) ) {
      // Have to treat head feature as a mod as well sometimes, + other things
      $modList = featuresThatAreAlsoMods($node);
      $ignoreMods = array("palewise", 'fesswise', 'bendwise', 'bendsinwise', 'haurient', 'conjoined', 'ensoleil');
      $otherArray = array_diff(array_keys(getModifiersByTypeAsArray($node,'chargemod')), $ignoreMods);
      $otherMods = implode('-',$otherArray);
      if ($otherMods) $modList .= '-' . $otherMods;
      $file = getBestMatchFilename($folder, $chargeKey, $modList);
      if ($file != '')
        $chg_data['file'] = $file;
    }
    // if we have a file, read it in, or leave existing code
    if ( !array_key_exists('svgCode',$chg_data) and array_key_exists('file', $chg_data) and file_exists( $folder . $chg_data['file'])) {
        $chg_data['svgCode'] = file_get_contents($folder . $chg_data['file']);
    }


    if ( !array_key_exists('svgCode',$chg_data)) {
      $messages->addMessage('warning',"Cannot draw $title");
      $chg_data['svgCode'] = '<svg height="100" width="100"><g stroke="none"><path fill-rule="evenodd" d="M0,0h100v100h-100zM10,10h80v80h-80z" />' .
              '<text x="50" y="55" font-size="15" text-anchor="middle">' . $title . '</text></g></svg>';
    }
    $chg_data = array_merge($chg_data, getExtraInfo($group, $chargeKey));
  }
  $chg_data['title'] = $title;
  if (array_key_exists('licence',$chg_data))
    $messages->addMessage('licence',$node->getAttribute('keyterm') . ' - ' . $chg_data['licence']);
  return $chg_data;
}


function makeCharge($charge, $placements = null) {
  if ( !$placements ) $placements = get_placement($charge);
  $retval = '';
  foreach ( explode('*',$placements) as $placement ) {
    $retval .= place_charges ( $charge, $placement );
  }
  return $retval;
}

// WARNINGS Accepted
function includeCharge( DOMElement $node, $includeFile, $charge, $folder, $chargeKey ) {
  global $messages, $options;
  include ( $includeFile );
  return $charge;
}

function getDemi ( $group, $charge ) {
  static $demi = null;

  if ( !$demi ) {
    include 'svg/demi.inc';
    $demi = new demiType();
  }

  return $demi->getDemi($group, $charge );
}


function getProper ( $group, $charge, $feature = null, $errorIfNone = true ) {
  static $proper = null;

  if ( !$proper ) {
    include 'svg/proper.inc';
    $proper = new properColour();
  }

  return $proper->getProper($group, $charge, $feature, $errorIfNone );
}

function getExtraInfo ( $group, $charge ) {
  static $extraInfo = null;

  if ( !$extraInfo ) {
    include 'svg/extrainfo.inc';
    $extraInfo = new extraInfoMap();
  }

  return $extraInfo->getExtra($group, $charge);
}

function getFlex ( $group, $charge ) {
  static $flex = null;

  if ( !$flex ) {
    include 'svg/flex.inc';
    $flex = new flexAmount();
  }

  return $flex->getFlex($group, $charge );
}

function makeSquareCharge(DOMElement $node ) {
  global $aspect_ratio;

  $oldAspect = $aspect_ratio;
  $aspect_ratio = 'square';
  $retval = makeCharge($node);
  $aspect_ratio = $oldAspect;
  return $retval;
}

function getCharge( DOMElement $node ) {
  global $chg_data_cache, $messages;

  list($chargeGroup, $chargeName ) = explode('/',$node->getAttribute('keyterm'));


    //-------------------------------------------------------------
  // Special handling for escutcheons
  if ($chargeName == 'escutcheon') {
    $chg_data = array();
    $shield = null;
    $crown = false;
    $chg_data['hflex'] = 0;
    $chg_data['wflex'] = 0;
    $chg_data['title'] = $chargeName;
    $shape = 'heater';
    if ( $node->hasChildNodes()) {
      foreach ( $node->childNodes as $child ) {
        if ( $child->nodeName == blazonML::E_SHIELD ) {
          $shield = $child;
        } elseif ($child->nodeName == blazonML::E_MODIFIER) {
          $modifier = $child->getAttribute('keyterm');
          if (substr($modifier,0,6) == 'shape-') {
            $shape = substr($modifier,6);
          }
        }
      }
    }
    if ($shield == null) {
      $messages->addMessage('Warning',"No data found for escutcheon");
      // just carry on with a blank escutcheon
    } else {
      $clipPath = getShape($shape);
      $clip = add_def( 'clipPath', '<path d="' . $clipPath . '" />' );
      $body = '<g clip-path="url(#' . $clip . ')" >' . makeshield( $shield, '10:12' ) . "</g>\n";
      $width = 1000;
      $height = 1200;
      // return $chg_data;
    }
  } else {
    //-------------------------------------------------------------
    $chg_ref = getModifierValueByKeyterm($node, 'chg_data');
    if ( !array_key_exists($chg_ref, $chg_data_cache) )
      $chg_data = read_chg_file($node);
    else
      $chg_data = $chg_data_cache[$chg_ref];

    //-------------------------------------------------------------
    // Set Flex amounts
    $flex = getFlex($chargeGroup, $chargeName);
    $chg_data['hflex'] = $flex[0];
    $chg_data['wflex'] = $flex[1];
    //-------------------------------------------------------------

  

    //------------------------------------------------------------
    // Get all tincture related information
    /**
     * @var DOMElement $charge_col
     */
    $charge_col = null;
    $feat_cols = array();
    $bodyTincture = null;
    /**
     * @var DOMElement $child
     */
    foreach ( $node->childNodes as $child ) { // TODO rewrite this logic?
      switch ( $child->nodeName ) {
        case 'tincture':
          $charge_col = resolveReference($child);
          break;
        case 'modifier':
          // TODO feature without child nodes, look for sibling feature with child nodes and use those?
          if (($child->getAttribute('type') == 'feature') and $child->hasChildNodes()){
            $featureTinc = $child->firstChild;
            $featureTinc = resolveReference($featureTinc);
            $featureName = $child->getAttribute('keyterm');
            if (!$featureTinc->hasChildNodes())
              $messages->addMessage('Warning',"No tincture for $featureName");
            else {
              $tinctureType = $featureTinc->firstChild;
              switch ($tinctureType->nodeName) {
                case 'colour':
                  $feat_cols[$featureName] = rgb($tinctureType->getAttribute('keyterm'));
                  break;
                case 'proper':
                  $feat_cols[$featureName] = getProper($chargeGroup, $chargeName, $featureName);
                  break;
                default: // treatment or something
                  $messages->addMessage('Warning', "Features can only be plain colours");
                  break;
              }
            }
          }
          break;
      }
    }
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Proper handling for body
    // By this point, $proper contains the proper colour code for the body (if given).
    // $feat_cols contains a array of named features => colour codes
    // We will now work out the actual colours to be used for the body and any unnamed features
    if ( $charge_col != null ) { 
      if ( $charge_col->hasChildNodes()) {
        $col_type = $charge_col->firstChild;
        if ( $col_type->nodeName == 'proper' ) {
          $col_type->setAttribute('spec', getProper($chargeGroup, $chargeName ));
          $feat_cols['proper'] = true; // Also use proper for any features not coloured explicitly
        }
      }
    }
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // Charges "internal" to this one
    $extras = array();
    // Special handling for charges "crowned"
    if ( $crown = getModifierNodeWithKeyterm($node, 'crowned', false) ) {
      $crownType = $crown->getAttribute('value');
      $extras['crowned'] = getSvgFragment(file_get_contents('svg/components/crown-as-worn/' . $crownType . '.svg'), array(), 'crown', $crownType, array());
      if ($crown->hasChildNodes()) {
        $feat_cols['crowned'] = get_rgb(getTinctureNode($crown));
      }
    }
    if ( $inscribe = getModifierNodeWithKeyterm($node, 'inscribed' ) ) {
      $extras['inscribed']['body'] = makeSquareCharge($inscribe->firstChild);
      $extras['inscribed']['width'] = 1000;
      $extras['inscribed']['height'] = 1000;
      $node->removeChild($inscribe);
    }

    //---------------------------------------------------------
    // So we now have $charge_col set to a valid tincture node and the feat_cols array set, apply this to the svg Data
    // dummy assignments
    $body = $height = $width = /* $licence = */ null;
    extract(getSvgFragment($chg_data['svgCode'], $feat_cols, $chargeGroup, $chargeName, $extras, $charge_col != null ));
    // $body now contains a valid fragment of SVG, ready for use
    // Also, $licence, $height and $width are set
    //----------------------------------------------------------
  }

  //----------------------------------------------------------
  // Demi handling
  if ( existModifierWithKeyterm($node,'demi') ) {
    if ( getDemi($chargeGroup, $chargeName) == 'horizontal' ) {
      $half = $height / 2;
      $clip = add_def ( 'clipPath', '<rect x="0" y="0" width="' . $width . '" height="' . $half . '" />' );
      $body = '<g clip-path="url(#' . $clip . ')">' . $body . '</g>';
      $height = $half;
    } else {
      $half = $width / 2;
      $clip = add_def ( 'clipPath', '<rect x="0" y="0" width="' . $half . '" height="' . $height . '" />' );
      $body = '<g clip-path="url(#' . $clip . ')">' . $body . '</g>';
      $width = $half;
    }
  } //----------------------------------------------------------


  //------------------------------------------------------------
  // Charges on or around this one
  $chargeOnMods = getModifiersByTypeAsArray($node,'chargewith', true); // get the actual nodes
  /**
   * @var DOMElement $chargeOn
   */
  foreach ($chargeOnMods as $chargeOn) {
    // What type of "on" is this?
    $chargeWithType = $chargeOn->getAttribute('keyterm');
    $chargeToAdd = $chargeOn->firstChild;
    $chargeToAddType = $chargeToAdd->getAttribute('keyterm');
    $chargeOnTop = false;
    switch ( $chargeWithType ) {
      case 'encircled-by':
        switch ( $chargeToAddType ) {
          case 'characters/word': // TODO move this logic to a separate function
            $chargeToAdd->appendChild(createModifierNode(languageDB::CHARGE_FEATURES,'encircled'));
            break;
          case 'symbol/laurel-wreath':
            // These are OK, but no further action required (already not on top)
            break;
          default:
           // $messages->addMessage('warning','charge inside another may not work properly');
            $chargeOnTop = true;
        }
        $charges = makeSquareCharge($chargeToAdd);
        // $charges contains 1000x1000 text circle
        // scale existing charge to fit inside
        $scale = min ( 500/$width, 500/$height);
        $xAdjust = 250 + (500 - ($width*$scale))/2;
        $yAdjust = 250 + (500 - ($height*$scale))/2;
        // and move to the centre
        if ( $chargeOnTop )
          $body = $charges . '<g transform="translate(' . $xAdjust . ',' . $yAdjust . ') scale(' . $scale . ')">' . $body . '</g>';
        else
          $body = '<g transform="translate(' . $xAdjust . ',' . $yAdjust . ') scale(' . $scale . ')">' . $body . '</g>' . $charges;
        $width = $height = 1000;
        $chg_data['hflex'] = $chg_data['wflex'] = 0; // don't flex, as circle  must stay round!
        break;
      // TODO Add more of the above!
      default:
        $charges = makeCharge($chargeToAdd);
        $scale = min ( $width/1400, $height/1400);
        // scale to fit onto charge - then move so charges are central
        // TODO fix this horrible bodge
        if ($node->getAttribute('keyterm') == 'sports/football-helmet' ) {
          $charges = sprintf('<g transform="translate(%2f,%2f) scale(%4f)">%s</g>', (($width / 2) - (550 * $scale) / 2), (($height / 2) - (1400 * $scale) / 2),
              $scale, $charges);
        } else {
          $charges = sprintf('<g transform="translate(%2f,%2f) scale(%4f)">%s</g>', (($width / 2) - (1000 * $scale) / 2), (($height / 2) - (1100 * $scale) / 2),
              $scale, $charges);
        }
        $body .= $charges;
        break;
    }
  }

  // Special handling for charges "en soleil"
  if ( existModifierWithKeyterm($node, 'ensoleil') ) {
    $sunOutline = '<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<svg
   xmlns:dc="http://purl.org/dc/elements/1.1/"
   xmlns:cc="http://creativecommons.org/ns#"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
   xmlns:svg="http://www.w3.org/2000/svg"
   xmlns="http://www.w3.org/2000/svg"
   xmlns:sodipodi="http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd"
   xmlns:inkscape="http://www.inkscape.org/namespaces/inkscape"
   height="400"
   width="400"
   version="1.0"
   id="svg2">

  <g
     id="g5158"
     style="fill:#ffff00">
    <path
       id="path4200-1"
       d="m 203,2 15,121 C 251,94.7 266,45.4 265,12.1 303,45.4 253,111 239,129 236,134 340,64.6 340,64.6 L 274,158 c 23,-13 69,-11 102,-47 -8,61 -78,41 -88,80 l 112,14 -116,19 c 48,15 61,47 102,46 -27,30 -88,-27 -108,-23 l 63,96 -89,-71 c -5,20 3,59 42,108 -81,-28 -44,-91 -69,-94 -1,12 -21,115 -21,115 L 186,286 c -29,29 -36,51 -43,104 -26,-69 25,-102 12,-119 L 60.6,346 127,257 C 74.5,271 68.7,253 21.2,290 42.5,243 95.8,249 112,232 L -4.04,207 112,182 C 61.5,165 60.6,137 8.08,140 38.4,111 72.6,136 121,162 134,169 58.6,58.6 58.6,58.6 L 154,130 C 155,68 141,37.9 112,23.2 155,2.03 177,88.7 181,121 187,122 203,2 203,2 z"
       style="fill:#ffff00;fill-opacity:1;stroke:#000000;stroke-width:1.2478801;stroke-linejoin:round;stroke-miterlimit:4;stroke-opacity:1;stroke-dasharray:none;display:inline" />
  </g>
</svg>';
    $sunArray = getSvgFragment($sunOutline,array(),'celestial','sun-outline', array());
    $enlargement = 1.4;
    $scale = (max($width,$height) / $sunArray['height']) * $enlargement;  // assume sun is square (!)
    $newSize = (max($width,$height)) * $enlargement;
    $Yshift = (($newSize - $height) / -2)/ $scale;
    $Xshift = (($newSize - $width) / -2) / $scale;
    $sunBody = '<g fill="' . rgb('or') . ' " stroke="' . rgb('charge-stroke') . '" transform=" scale(' . $scale . ') translate(' . "$Xshift,$Yshift" . ')">' . $sunArray['body'] . '</g>';
    $body = $sunBody . $body;
    $chg_data['hflex'] = $chg_data['wflex'] = 0; // don't flex, as sun must stay round!
  }

  if ( $crown ) {
    $body = '<g transform="scale(0.9,0.9) translate(' . ($width * 0.05) .',' . ($height * 0.1) . ')">' . $body . '</g>';
  }


  // $messages->addMessage('licence',$node->getAttribute('keyterm') . " $licence"); // TODO what about internal, and is this used below?
  // $chg_data['licence'] = $licence;
  $chg_data['body'] = $body;
  $chg_data['height'] = $height;
  $chg_data['width'] = $width;
  return $chg_data;
}
