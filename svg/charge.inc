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
       $chg_ref = unique('chg');
       $chg_data_cache[$chg_ref] = read_chg_file($chg);
       // Add a modifier to link to the cached charge data
       $chg->appendChild(createModifierNode(languageDB::CHARGE_MODS, 'chg_data', $chg_ref ));
     }
   }
   
function read_chg_file(DOMElement $node) {
  global $messages;

  $title = '???';
  $chg_data = array();
  if ( is_null($node) or $node->nodeName == 'missing' ) {
    $title = 'missing';
  } else {
    $splitArray = preg_split(':/:', $node->getAttribute('keyterm'));
    if (count($splitArray) != 2) {
      $messages->addMessage('internal',"Problem reading charge with keyterm " . $node->getAttribute('keyterm'));
      $chg_data['svgCode'] = '<svg height="100" width="100"><g stroke="none"><path fill-rule="evenodd" d="M0,0h100v100h-100zM10,10h80v80h-80z" />' .
        '<text x="50" y="55" font-size="15" text-anchor="middle">' . $title . '</text></g></svg>';
      return $chg_data;
    }
    list($folder,$chargeKey) = $splitArray;
    $folder = "svg/charges/$folder/";
    if ( ($title = $node->getAttribute('tokens')) == null ) $title = $chargeKey;
    // Look for a common file first, applies to all in this category
    // TODO This can set a default 'file' - and add a warning?
    $file = $folder . '_prefix.inc';
    if ( file_exists( $file )) {
      $chg_data = includeCharge( $node, $file, $chg_data, $folder, $chargeKey );
    }
    // Then look for .inc file
    $file = $folder . $chargeKey . '.inc';
    if ( file_exists( $file )) {
      $chg_data = includeCharge( $node, $file, $chg_data, $folder, $chargeKey );
    }
      // It is possible that some node attributes have been changed, so re-read them
    list($folder,$chargeKey) = preg_split(':/:', $node->getAttribute('keyterm'));
    $folder = "svg/charges/$folder/";
    // Another chance to set file, is it a simple name match?
    $file =  'inkscape/' . $chargeKey . '.svg';
    if ( !array_key_exists('file',$chg_data) and file_exists( $folder . $file ) )
      $chg_data['file'] = $file;
    // And for another common file last, applies to all in this category
    $file = $folder . '_postfix.inc';
    if ( file_exists( $file )) {
      $chg_data = includeCharge( $node, $file, $chg_data, $folder, $chargeKey );
    }
    // if we have a file, read it in, or leave existing code
    if ( array_key_exists('file', $chg_data) and file_exists( $folder . $chg_data['file']))
      $chg_data['svgCode'] = file_get_contents($folder . $chg_data['file']);
    if ( !array_key_exists('svgCode',$chg_data)) {
      $messages->addMessage('warning',"Cannot draw $title");
      $chg_data['svgCode'] = '<svg height="100" width="100"><g stroke="none"><path fill-rule="evenodd" d="M0,0h100v100h-100zM10,10h80v80h-80z" />' .
              '<text x="50" y="55" font-size="15" text-anchor="middle">' . $title . '</text></g></svg>';
    }
  }
  $chg_data['title'] = $title;
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

  $chg_ref = getModifierValueByKeyterm($node, 'chg_data');
  if ( !array_key_exists($chg_ref, $chg_data_cache) )
    $chg_data = read_chg_file($node);
  else
    $chg_data = $chg_data_cache[$chg_ref];

  //-------------------------------------------------------------
  // Set Flex amounts
  list($chargeGroup, $chargeName ) = explode('/',$node->getAttribute('keyterm'));
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
            $messages->addMessage('internal',"Malformed tincture on $featureName");
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
  if ( $charge_col != null and $charge_col->getAttribute('origin') != 'implied') { // fixed colour
    if ( !$charge_col->hasChildNodes()) {
      // $messages->addMessage('internal','Malformed tincture ID= ' . $charge_col->getAttribute('ID'));
    } else {
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
    $extras['crowned'] = getSvgFragment(file_get_contents('svg/charges/crown-as-worn/inkscape/' . $crownType . '.svg'), array(), 'crown', $crownType, array());
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
  $body = $height = $width = $licence = null;
  extract(getSvgFragment($chg_data['svgCode'], $feat_cols, $chargeGroup, $chargeName, $extras ));
  // $body now contains a valid fragment of SVG, ready for use
  // Also, $licence, $height and $width are set
  //----------------------------------------------------------

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
        $scale = min ( 600/$width, 600/$height);
        $xAdjust = 200 + (600 - ($width*$scale))/2;
        $yAdjust = 200 + (600 - ($height*$scale))/2;
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
    $sunArray = getSvgFragment(file_get_contents('svg/charges/celestial/inkscape/sun-outline.svg'),array(),'celestial','sun-outline', array());
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


  $messages->addMessage('licence',$node->getAttribute('keyterm') . " $licence"); // TODO what about internal, and is this used below?
  // $chg_data['licence'] = $licence;
  $chg_data['body'] = $body;
  $chg_data['height'] = $height;
  $chg_data['width'] = $width;
  return $chg_data;
}
