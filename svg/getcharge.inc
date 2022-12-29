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

require_once("custom_charges.inc");


const PREPROPERTY = 0;
const INPROPERTY = 1;
const PREVALUE = 2;
const INVALUE = 3;

function getPropertyValuePairs($string) {
  $retval = [];
  $property = '';
  $value = '';
  $index = 0;
  $state = PREPROPERTY;
  while ($index < strlen($string)) {
    $ch = substr($string,$index++,1);
    switch ($state) {
      case PREPROPERTY:
        if (ctype_alnum($ch)) {
          $state = INPROPERTY;
          $property = $ch;
        }
        break;
      case INPROPERTY:
        if (ctype_alnum($ch) || $ch == '_') {
          $property .= $ch;
        } else {
          $state = PREVALUE;
        }
        break;
      case PREVALUE:
        if (ctype_alnum($ch)) {
          $state = INVALUE;
          $value = $ch;
        }
        break;
      case INVALUE:
        if (ctype_alnum($ch) || $ch == '-' || $ch == '.') {
          $value .= $ch;
        } else {
          $state = PREPROPERTY;
          $retval[$property] = $value;
//          error_log("Found $property as $value");
          $property = '';
          $value = '';
        }
        break;
    }
  }
  if ($property && $value) {
    $retval[$property] = $value;
//     error_log("Found $property as $value");
  }
  return $retval;
}

function getMetaDataFromSVG($svg, $chargeGroup = '', $chargeName = '') {
  global $messages;

    // Get metadata from the SVG
    @$SVGmetadata = $svg->xpath('//dc:title');
    if ( is_array($SVGmetadata) && count($SVGmetadata) > 0) {
        $licence = 'unknown';
        foreach($SVGmetadata as $metaitem) {
            if (strlen($metaitem)) {
                $licence = $metaitem;
                break;
            }
        }
        $messages->addMessage('licence', "$chargeGroup/$chargeName - $licence");
    }
    $retval = [];
    @$SVGmetadata = $svg->xpath('//dc:description');
    if ( is_array($SVGmetadata) and count($SVGmetadata) > 0) {
      foreach($SVGmetadata as $metaitem) {
          $metaitem = (string)$metaitem;
          if (strlen($metaitem)) {
            if ($metaitem[0] == '{') { // treat it as JSON
              $json = @json_decode($metaitem, true);
              if (is_null($json) || count($json) == 0) {
                $messages->addMessage('internal', "Bad JSON in $chargeGroup/$chargeName");
              } else {
                $retval = $json;
              }
            } else { // treat it as text
              $retval = getPropertyValuePairs($metaitem);
            }
          }
      }
   }
    return $retval;
}

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

function getBestMatchFilename($folder, $base, $mods, $warn = true) {
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
      // if the final part is something like (1), (2) etc. ignore that too
      if (preg_match("/([0-9])/",$filenameParts[$numFilenameParts])) $numFilenameParts -= 1;
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
        // Some things we don't really care about
          // english is the default tradition, so no worries if it is not found
        if (!in_array($requiredParts[$i], array('english', 'voided', 'upper', 'lower', 'half', 'demi',
            'winged', 'entire', 'head', 'leg', 'reversed', 'inverted', 'slipped', 'leaved' ))) {
          $missing .= $requiredParts[$i] . ' ';
        }
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
      // $bestExtra = $extra;
    }
    if ($penalty == 0 ) { // perfect match, skip out
      break;
    }
  }
  closedir($dh);
  if ($bestMissing && $warn )
    $messages->addMessage('warning', $base . ' may not be drawn with ' . $bestMissing);
    if (strstr($bestMissing, 'rampant')) {
      $messages->addMessage('warning', ' try salient instead of rampant?');
    }
  // if ($bestExtra) $messages->addMessage('other', $base . ' is also ' . $bestExtra);
  return $bestFilename;
}

function featuresThatAreAlsoMods($node) {
  $retval = '';
  static $features = array (
    'head',
    'claw',
    'leg',
    'clasped',
    'slipped',
    'leaved',
    'collared',
    'jessant',
      'jessed',
      'leathered',
    'voided',
    'holding-nut',
    'armoured',
    'fructed',
    'winged',
      'wreathed',
      'spouting',
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
    $folder = __dir__ . "/charges/$group/";
    $subfolder = '';
    if (($title = $node->getAttribute('tokens')) == null ) $title = $chargeKey;
    $tincture = getTinctureNode($node);
    if (!is_null($tincture) && !is_null($tincture->firstChild) && $tincture->firstChild->nodeName == blazonML::E_PROPER) {
        // Charge is proper let's see if there is a special version for this
        if (file_exists($folder . "proper/" . $chargeKey . '.svg')) {
            $subfolder .= "proper/";
        }
    }

    // look for .inc file
    $file = $folder . $subfolder . $chargeKey . '.inc';
    if ( $group == "custom" ) {
        $chg_data = SmartChargeGroup::instance()->charge_data($node, $chg_data, $chargeKey);
    } else if ( file_exists( $file )) {
      $chg_data = includeCharge( $node, $file, $chg_data, $folder, $chargeKey );
    }
    // TODO check if this still happens???
      // It is possible that some node attributes have been changed, so re-read them
    list($group,$chargeKey) = preg_split(':/:', $node->getAttribute('keyterm'));
    $folder = __dir__ . "/charges/$group/$subfolder";

    $modList = '';
    if ( !array_key_exists('svgCode',$chg_data) and !array_key_exists('file',$chg_data) ) {
        // If there is a different tradition specified we put that first
        if (($trad = getModifierByType($node,languageDB::TRADITION_PREFIX)) != null)
            $modList .= '-' . $trad;
      // Have to treat head feature as a mod as well sometimes, + other things
      $modList .= featuresThatAreAlsoMods($node);
      $ignoreMods = array("palewise", 'fesswise', 'bendwise', 'bendsinwise', 'haurient', 'conjoined', 'ensoleil');
      $otherArray = array_diff(array_keys(getModifiersByTypeAsArray($node,'chargemod')), $ignoreMods);
      if (existModifierWithKeyterm($node,'fancy')) $otherArray[] = 'fancy';
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
    // Get additional metadata, but allow existing entries (picked up from the charge itself)
    // to have priority
    $chg_data = array_merge(getMetadata($group, $chargeKey), $chg_data);
  }
  $chg_data['title'] = $title;
  $chg_data['verbatim'] = false;
  if ($subfolder) { // must be proper, so don't try to colour in
    $chg_data['verbatim'] = true;
  }
  if (array_key_exists('licence',$chg_data))
    $messages->addMessage('licence',$node->getAttribute('keyterm') . ' - ' . $chg_data['licence']);
  return $chg_data;
}


function makeCharge($charge, $layout, $placements = null) {
  $retval = '';
  if (empty($charge)) return $retval;
  if ( !$placements ) $placements = get_placement($charge, $layout);
  foreach ( explode('*',$placements) as $placement ) {
    $retval .= place_charges ( $charge, $placement, $layout );
  }
  return $retval;
}

function makeList($chgList, $layout) {
    $charges = [];
    foreach ($chgList->childNodes as $child) {
        $charge[] = array('body' => makeCharge($child, ShieldLayout::square() ),
            'width' => 1000, 'height' => 1000);
    }

}

function chargeInAnnulo($node, $charge, $base_name, $dir)
{
    static $attitudes = [];

    if ( !isset($attitudes[$base_name]) )
    {
        $attitudes[$base_name] = [];
        $re_base = preg_quote($base_name);
        foreach ( scandir($dir) as $fname )
        {
            $matches = [];
            if ( preg_match('/^'.$re_base.'-([a-z]+)\.svg/', $fname, $matches) )
                $attitudes[$base_name][] = $matches[1];
        }
    }

    $in_annulo = getModifierNodeWithKeyterm($node, "inannulo", false);

    if ( $in_annulo )
    {
        foreach ( $attitudes[$base_name] as $att )
        {
            if ( existModifierWithKeyTerm($node, $att, false) )
                return;
        }

        setRenderInfo($in_annulo, 'used');
        $charge['file'] = "$base_name-in-annulo.svg";
    }
}

// WARNINGS Accepted
function includeCharge( DOMElement $node, $includeFile, $charge, $folder, $chargeKey ) {
  global $messages;
  include ( $includeFile );
  return $charge;
}

function getProper ( $group, $charge, $feature = null, $errorIfNone = true ) {
  static $proper = null;

  if ( !$proper ) {
    include __dir__ . '/proper.inc';
    $proper = new properColour();
  }

  return $proper->getProper($group, $charge, $feature, $errorIfNone );
}

function getMetadata ( $group, $charge ) {
  static $metadata = null;

  if ( !$metadata ) {
    include __dir__ . '/metadata.inc';
    $metadata = new metadataMap();
  }

  return $metadata->getMeta($group, $charge);
}

function makeSquareCharge(DOMElement $node ) {
  $retval = makeCharge($node, new ShieldLayout(new ShieldSize(1000, 1000), "square", null, null, false));
  return $retval;
}

function getCharge( DOMElement $node, $layout ) {
    global $chg_data_cache, $messages, $options;
    if (is_null($node))
        return null;

    $keyterm = $node->getAttribute('keyterm');
    if (strpos($keyterm, '/') === false) {
        error_log("bad charge keyterm - $keyterm");
        return null;
    }
    list($chargeGroup, $chargeName ) = explode('/', $keyterm);

    $filters = [];
    static $filter_id = 0;
    $hold_point = null;
    $hold_radius = 1;
    $bounding_box = null;

    //-------------------------------------------------------------
    // Special handling for escutcheons
    if ($chargeName == 'escutcheon')
    {
        $chg_data = array();
        $shield = null;
        $crown = false;
        $chg_data['hflex'] = 0;
        $chg_data['wflex'] = 0;
        $chg_data['title'] = $chargeName;
        $shape = 'heater';

        if ( $node->hasChildNodes())
        {
            foreach ( $node->childNodes as $child )
            {
                if ( $child->nodeName == blazonML::E_SHIELD )
                {
                    $shield = $child;
                }
                elseif ( $child->nodeName == blazonML::E_MODIFIER )
                {
                    $modifier = $child->getAttribute(blazonML::A_KEYTERM);
                    if (substr($modifier, 0, 6) == 'shape-')
                    {
                        $shape = substr($modifier, 6);
                    } elseif ($child->getAttribute(blazonML::A_TYPE) == languageDB::TRADITION_PREFIX) {
                        $shape = $modifier;
                    }

                    // we don't need the pending tincture node
                }
                elseif ( $child->nodeName == blazonML::E_TINCTURE )
                {
                    $child->setAttribute(blazonML::A_ORIGIN, blazonML::V_IMPLIED);
                }
            }
        }
        if ($shield == null)
        {
            $messages->addMessage('Warning',"No data found for escutcheon");
            // just carry on with a blank escutcheon
        }
        else
        {
            $width = 1000;

            if ( $shape == "flag" )
            {
                $xpath = new DomXPath($node->ownerDocument);
                $aspect = $xpath->evaluate("string(aspect/@keyterm)", $node);
                if ( !$aspect )
                    $aspect = 0.6;
                else
                    $aspect = calculateAR($aspect);

                $height = $width * $aspect;
                $hold_point = new ShieldPos(0, 0);
                $child_layout = new ShieldLayout(new ShieldSize($width, $height), $shape);
            }
            else
            {
                $height = ShieldLayout::best_height($shape);
                $child_layout = new ShieldLayout(new ShieldSize($width, $height), $shape);
                $hold_point = $child_layout->cross_point;
            }

            $hold_radius = $width / 15;

            $clipPath = getShape($child_layout);
            $clip = add_def( 'clipPath', '<path d="' . $clipPath . '" />' );
            $body = '<g clip-path="url(#' . $clip . ')" >' . makeshield( $shield, $child_layout ) . "</g>\n";
            $bounding_box = ShieldRect::from_corners(0, 0, $width, $height);
        }
  } else {
        //-------------------------------------------------------------
        $chg_ref = getModifierValueByKeyterm($node, 'chg_data');
        if (!array_key_exists($chg_ref, $chg_data_cache))
            $chg_data = read_chg_file($node);
        else
            $chg_data = $chg_data_cache[$chg_ref];

        //-------------------------------------------------------------

        // Turn the svg_code into a SimpleXML object
        libxml_use_internal_errors(true);
        if (($svg = @simplexml_load_string($chg_data['svgCode'], null, LIBXML_NOENT)) == null) {
            $messages->addMessage('internal', "XML error reading charge data");
            error_log("XML error reading charge data - " . $chargeGroup . '/' . $chargeName);
            // Fallback item
            $svg = simplexml_load_string('<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                    <svg
                    xmlns="http://www.w3.org/2000/svg"
                    width="100"
                    height="100"
                    version="1.1" >
                    <g style="fill:#ffff00;fill-opacity:1;fill-rule:evenodd;stroke:#000000;stroke-width:3;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-opacity:1;stroke-dasharray:none" id="body"><path fill-rule="evenodd" d="M0,0h100v100h-100zM15,15h70v70h-70z" />
                    <text x="50" y="65" font-size="30" text-anchor="middle">????</text></g>
                    </svg>'
            );
        }
        libxml_clear_errors();
        //------------------------------------------------------------

        // Get metadata from the SVG
        $chg_data = array_merge($chg_data, getMetaDataFromSVG($svg, $chargeName, $chargeGroup));

        //------------------------------------------------------------
        // Get all tincture related information
        /**
         * @var DOMElement $charge_col
         */
        $charge_col = null;
        $strokeCol = null;
        $feat_cols = array();
        $defs = [];
        $extras = [];

        /**
         * @var DOMElement $child
         */
        foreach ($node->childNodes as $child) { // TODO rewrite this logic?
            switch ($child->nodeName) {
                case 'tincture':
                    $charge_col = resolveReference($child);
                    break;
                case 'modifier':
                    // TODO feature without child nodes, look for sibling feature with child nodes and use those?
                    if (($child->getAttribute('type') == 'feature') and $child->hasChildNodes()) {
                        $featureTinc = $child->firstChild;
                        $featureTinc = resolveReference($featureTinc);
                        $featureName = $child->getAttribute('keyterm');
                        if ($featureTinc == null || !$featureTinc->hasChildNodes())
                            $messages->addMessage('Warning', "No tincture for $featureName");
                        else {
                            $tinctureType = $featureTinc->firstChild;
                            switch ($tinctureType->nodeName) {
                                case 'proper':
                                    $feat_cols[$featureName] = getProper($chargeGroup, $chargeName, $featureName);
                                    break;
                                default:
                                    $feat_cols[$featureName] = rgb($tinctureType->getAttribute('keyterm'));

                                    $tincture_data = new TinctureData($featureTinc);
                                    $tincture_data->pattern_transform = "scale(0.5)";
                                    $tincture_data->parse_fill($layout);
                                    if ($tincture_data->fill) {
                                        $feat_cols[$featureName] = $tincture_data->fill;
                                        $defs = array_merge($defs, $tincture_data->defs);
                                    }
                                    break;
                            }
                        }
                    } elseif ($child->getAttribute(blazonML::A_KEYTERM) == 'stroked') {
                        $strokeCol = rgb($child->getAttribute(blazonML::A_PARAM));
                    } else if ($child->getAttribute('type') == languageDB::HOLDING and $child->hasChildNodes()) {
                        $chargeToAdd = $child->firstChild;
                        if ($chargeToAdd->nodeName == 'missing' || $chargeToAdd->nodeName == 'ordinary')
                            break;
                        $chargeWithType = $child->getAttribute('keyterm');
                        $extras[$chargeWithType] = getCharge($chargeToAdd, $layout);
                    }
            }
        }
        //--------------------------------------------------------------------------

        //--------------------------------------------------------------------------
        // Proper handling for body
        // By this point, $proper contains the proper colour code for the body (if given).
        // $feat_cols contains a array of named features => colour codes
        // We will now work out the actual colours to be used for the body and any unnamed features
        $body_col = null;
        if ($charge_col != null) {
            if ($charge_col->hasChildNodes()) {
                $col_type = $charge_col->firstChild;
                if ($col_type->nodeName == 'proper') {
                    if (isset($chg_data['proper'])) { // TODO handle proper as an array, with this one as 'body'
                        if (is_array($chg_data['proper'])) {
                            foreach ($chg_data['proper'] as $feature => $value) {
                                if ($feature == 'body') {
                                    $body_col = $value;
                                } else {
                                    $feat_cols[$feature] = $value;
                                }
                            }
                        } else {
                            $body_col = rgb($chg_data['proper']);
                        }
                    } else {
                        $body_col = getProper($chargeGroup, $chargeName, null, !$chg_data['verbatim']);
                    }
                    $col_type->setAttribute('spec', $body_col);
                    $feat_cols['proper'] = true; // Also use proper for any features not coloured explicitly
                }
            }
        }
        //------------------------------------------------------------------------------
        // set a default stroke colour based on whether the body colour is light or dark
        if (!$chg_data['verbatim'] && !$strokeCol)
            $strokeCol = getStroke($charge_col, array_key_exists('fixStroke', $chg_data));

        //------------------------------------------------------------------------------
        // Charges "internal" to this one

        // Special handling for charges "crowned"
    $crowns = getModifiersByTypeAsArray($node, languageDB::CROWN_MODS, true);
    foreach ($crowns as $crown) {
            // if ( $crown = getModifierNodeWithKeyterm($node, 'crowned', false) ) {
            $crownType = $crown->getAttribute('value');
            $modType = $crown->getAttribute('keyterm'); // crowned, gorged or wreathed
            if ($crownType == 'default') $crownType = 'ducal';
            $crownCol = $charge_col;
            if ($temp = getTinctureNode($crown)) {
                $crownCol = $temp;
                $feat_cols[$modType] = get_rgb($crownCol);
            }
            $crownFile = __dir__ . '/components/crown-as-worn/' . $crownType . '.svg';
            if (!file_exists($crownFile)) {
                $crownFile = __dir__ . '/charges/crown/' . $crownType . '.svg';
            }
            if (!file_exists($crownFile)) {
                $messages->addMessage('internal', "No crown file for $crownType");
            } else {
                $extras[$modType] = getSvgFragment(simplexml_load_file($crownFile),
                    array(), 'crown', $crownType, array(), getStroke($crownCol), 2); // multiply size of crown strokes
            }
    }
    if ( $inscribe = getModifierNodeWithKeyterm($node, 'inscribed' ) ) {
      $extras['inscribed']['body'] = makeSquareCharge($inscribe->firstChild);
      $extras['inscribed']['width'] = 1000;
      $extras['inscribed']['height'] = 1000;
      $node->removeChild($inscribe);
    }


  if ( getModifierNodeWithKeyterm($node, 'voided') != null && (!isset($chg_data["file"]) || strpos($chg_data["file"], "voided") === false) )
  {
      $filter_id++;
      $svg_id = "ds_filter_voided_$filter_id";

      if ( $charge_col->firstChild && $charge_col->firstChild->nodeName == "colour" )
      {
            $solid_color = get_rgb($charge_col);
            $filters[$svg_id] = <<<HERE
                <feMorphology operator="erode" in="SourceGraphic" radius="5" result="back" />
                <feFlood flood-opacity="1" flood-color="$solid_color" result="flood" />
                <feComposite in="flood" in2="SourceGraphic" operator="in" result="filled" />
                <feComposite in2="filled" in="back" operator="xor" result="final" />
HERE;
      }
      else
      {
            $filters[$svg_id] = <<<HERE
                <feMorphology operator="erode" in="SourceGraphic" radius="5" result="back" />
                <feComposite in2="SourceGraphic" in="back" operator="xor" result="final" />
HERE;
      }
  }

    // special handling for fimbriated (overwrites strokeCol)
    if ( ($fimNode = getModifierNodeWithKeyterm($node, 'fimbriated')) != null ) {
      if ( array_key_exists('fimbriate', $chg_data) && $chg_data['fimbriate'] == true && count($filters) == 0 ) {
        $tincNode = getTinctureNode($fimNode);
        $strokeCol = get_rgb($tincNode);
      } else {
        $fimbTincNode = getTinctureNode($fimNode);
        $fimbStrokeCol = get_rgb($fimbTincNode);
        $filter_id++;
        $svg_id = "ds_filter_fimbriated_$filter_id";
        $filters[$svg_id] = <<<HERE
          <feMorphology operator="dilate" in="SourceGraphic" radius="3" result="dilated" />
          <feFlood flood-opacity="1" flood-color="$fimbStrokeCol" result="flood" />
          <feComposite in="flood" in2="dilated" operator="in" result="back" />
          <feComposite in="SourceGraphic" in2="back" operator="over" result="final" />
HERE;
      }
    }

    //---------------------------------------------------------
    // So we now have $charge_col set to a valid tincture node and the feat_cols array set, apply this to the svg Data
    if ($chg_data['verbatim']) {
        $body = preg_replace('/<\?xml.*?>/i','', $chg_data['svgCode']);
        if ( $options['palette'] == 'outline' || $options['palette'] == 'hatching') {
            $body = "<g filter=\"url('#grayscale')\">" . $body . '</g>';
        }
        $chg_data = array_merge($chg_data, getSvgFragment($svg, $feat_cols, $chargeGroup, $chargeName, $extras, $strokeCol, null, $chg_data['verbatim'] ));
     } else {
        $chg_data = array_merge($chg_data, getSvgFragment($svg, $feat_cols, $chargeGroup, $chargeName, $extras, $strokeCol));
        $body = $chg_data['body'];
    }
        $height = $chg_data['height'];
        $width = $chg_data['width'];

    //----------------------------------------------------------
  }

  $bounding_box = ShieldRect::from_corners(0, 0, $width, $height);
    //----------------------------------------------------------
  // demi / half handling
  if ( existModifierWithKeyterm($node,'demi') ) {
    if ( array_key_exists('demi', $chg_data) && $chg_data['demi'] == 'horizontal' ) {
      $half = $height / 2;
      $bounding_box->size->height -= $half;
      $clip = add_def ( 'clipPath', '<rect x="0" y="0" width="' . $width . '" height="' . $half . '" />' );
      $body = '<g clip-path="url(#' . $clip . ')">' . $body . '</g>';
      $height = $half;
    } else {
      $half = $width / 2;
      $bounding_box->size->width -= $half;
      $clip = add_def ( 'clipPath', '<rect x="0" y="0" width="' . $half . '" height="' . $height . '" />' );
      $body = '<g clip-path="url(#' . $clip . ')">' . $body . '</g>';
      $width = $half;
    }
  } //----------------------------------------------------------

  
    //----------------------------------------------------------
    // NON-STANDARD EXTENSION - half handling
    if ( existModifierWithKeyterm($node,'upper-half') )
    {
        $half = $height / 2;
        $bounding_box->size->height -= $half;
        $clip = add_def ( 'clipPath', '<rect x="0" y="0" width="' . $width . '" height="' . $half . '" />' );
        $body = '<g clip-path="url(#' . $clip . ')">' . $body . '</g>';
        $height = $half;
    }
    else if ( existModifierWithKeyterm($node,'dexter-half') )
    {
        $half = $width / 2;
        $bounding_box->size->width -= $half;
        $clip = add_def ( 'clipPath', '<rect x="0" y="0" width="' . $half . '" height="' . $height . '" />' );
        $body = '<g clip-path="url(#' . $clip . ')">' . $body . '</g>';
        $width = $half;
    }
    else if ( existModifierWithKeyterm($node,'sinister-half') )
    {
        $half = $width / 2;
        if ( $hold_point )
            $hold_point->x -= $half;
        $bounding_box->size->width -= $half;
        $bounding_box->pos->x -= $half / 2;
        $clip = add_def ( 'clipPath', '<rect x="' . $half . '" y="0" width="' . $half . '" height="' . $height . '" />' );
        $body = '<g transform="translate(-' .  $half . ',0)" clip-path="url(#' . $clip . ')">' . $body . '</g>';
        $width = $half;
    }
    else if ( existModifierWithKeyterm($node,'lower-half') )
    {
        $half = $height / 2;
        if ( $hold_point )
            $hold_point->y -= $half;
        $bounding_box->size->height -= $half;
        $bounding_box->pos->y -= $half / 2;
        $clip = add_def ( 'clipPath', '<rect x="0" y="' . $half . '" width="' . $width . '" height="' . $half . '" />' );
        $body = '<g transform="translate(0, -' .  $half . ')" clip-path="url(#' . $clip . ')">' . $body . '</g>';
        $height = $half;
    } //----------------------------------------------------------

    if ( count($filters) )
    {
        foreach ( $filters as $id => $filter )
        {
            $defs[] = "<filter id='$id'>$filter</filter>";
            $body = "<g style='filter:url(#$id)'>$body</g>";
        }
    }
    if ( isset($defs) && count($defs) )
    {
        $body = "<defs>" . implode("", $defs) . "</defs>" . $body;
    }

  //------------------------------------------------------------
  // Charges on or around this one
  $chargeOnMods = getModifiersByTypeAsArray($node,'chargewith', true); // get the actual nodes
  $new_layout = new ShieldLayout(new ShieldSize(1000, 1000), "square", null, null, false);
  /**
   * @var DOMElement $chargeOn
   */
  foreach ($chargeOnMods as $chargeOn) {
    $chargeToAdd = $chargeOn->firstChild;
    if ($chargeToAdd->nodeName == 'missing') {
      // error already rasied in parser
      // $messages->addMessage('error', "Missing charge for 'on'"); 
      continue;
    }    
    // What type of "on" is this?
    $chargeWithType = $chargeOn->getAttribute('keyterm');
    if ($chargeWithType == 'charged-each') {
      $num_of = getModifierValueByKeyterm($node, 'ofnum');
      if ($num_of > 0) {
      }
      $chargeWithType = 'charged-with';
    }
    $chargeToAddType = $chargeToAdd->getAttribute('keyterm');
    if ( $chargeToAdd->nodeName == 'ordinary') {
      //$messages->addMessage('Warning', "Charge charged with ordinary not supported");
      $new_ord = makeOrdinary( $chargeToAdd, $new_layout );
      $patt_id = add_def ( 'pattern width="100%" height="100%" patternUnits="objectBoundingBox" preserveAspectRatio="none" viewBox="0 0 1000 1000"',
      "<desc>Ordinary used to fill $chargeName</desc>" . $new_ord ); 
      $body = add_def() . $body;
      $body .= '<g fill="url(#' . $patt_id . ')"  >' . $body . '</g>';   
      continue;
    }
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
        if ($chargeGroup == 'label') {
          $body .= place_charges($chargeToAdd, $chg_data['placement'], $new_layout);
          break;
        }
        $charges = makeCharge($chargeToAdd, $new_layout);
        // We now have one or more superposed charges scaled to fit into a 1000x1000 box
        // We want to shrink the superposed charges by a factor (so they leave plenty of space
        // to allow the underlying charge to be seen)
        $xPos = $yPos = 50; // Centre Position of superposed charge, can be varied, as a percentage (0 = left/top edge)
        $shrinkFactor = 72;  // <<< As percentage of existing charge size
        if (array_key_exists('superposition', $chg_data)) {
          $supers = explode(',',$chg_data['superposition']);
          switch (count($supers)) {
            case 3:
              $shrinkFactor = $supers[2];
              /* flow through */
            case 2:
              $yPos = $supers[1];
              /* flow through */
            case 1:
              $xPos = $supers[0];
            break;
          }
        }
        // The underlying charge is $width x $height, we need to work out how to scale
        // the superposed charges so that they fit
        $targetSize = min ( $width * ($shrinkFactor / 100), $height * ($shrinkFactor / 100));
        $scale = $targetSize / 1000;
        $targetCentre = $targetSize / 2;
        $xShift = ($width * ($xPos / 100)) - $targetCentre; // convert percentage to factor
        $yShift = ($height * ($yPos / 100)) - $targetCentre; // convert percentage to factor
        // scale to fit onto charge - then move so charges are central
        $charges = sprintf('<g class="881" transform="translate(%2f,%2f) scale(%4f)">%s</g>',
                $xShift, 
                $yShift,
                $scale, 
                $charges);
        $body .= $charges;
        break;
    }
  }

  // Special handling for charges "en soleil"
  if ( existModifierWithKeyterm($node, 'ensoleil') ) {
    $sunOutline = simplexml_load_string('<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<svg
   xmlns:svg="http://www.w3.org/2000/svg"
   xmlns="http://www.w3.org/2000/svg"
   height="400"
   width="400"
   version="1.0"
   id="svg2">

  <g
     style="fill:#ffff00">
    <path
       d="m 203,2 15,121 C 251,94.7 266,45.4 265,12.1 303,45.4 253,111 239,129 236,134 340,64.6 340,64.6 L 274,158 c 23,-13 69,-11 102,-47 -8,61 -78,41 -88,80 l 112,14 -116,19 c 48,15 61,47 102,46 -27,30 -88,-27 -108,-23 l 63,96 -89,-71 c -5,20 3,59 42,108 -81,-28 -44,-91 -69,-94 -1,12 -21,115 -21,115 L 186,286 c -29,29 -36,51 -43,104 -26,-69 25,-102 12,-119 L 60.6,346 127,257 C 74.5,271 68.7,253 21.2,290 42.5,243 95.8,249 112,232 L -4.04,207 112,182 C 61.5,165 60.6,137 8.08,140 38.4,111 72.6,136 121,162 134,169 58.6,58.6 58.6,58.6 L 154,130 C 155,68 141,37.9 112,23.2 155,2.03 177,88.7 181,121 187,122 203,2 203,2 z"
       style="fill:#ffff00;fill-opacity:1;stroke:#000000;stroke-width:1.2478801;stroke-linejoin:round;stroke-miterlimit:4;stroke-opacity:1;stroke-dasharray:none;display:inline" />
  </g>
</svg>');
    $sunArray = getSvgFragment($sunOutline,array(),'celestial','sun-outline', array(), null); // don't set stroke, we will do that below
    $enlargement = 1.4;
    $scale = (max($width,$height) / $sunArray['height']) * $enlargement;  // assume sun is square (!)
    $newSize = (max($width,$height)) * $enlargement;
    $Yshift = (($newSize - $height) / -2)/ $scale;
    $Xshift = (($newSize - $width) / -2) / $scale;
    // always use a light stroke as the sun is yellow
    $sunBody = '<g fill="' . rgb('or') . ' " stroke="' . rgb('stroke-light') . '" transform=" scale(' . $scale . ') translate(' . "$Xshift,$Yshift" . ')">' . $sunArray['body'] . '</g>';
    $body = $sunBody . $body;
    $chg_data['hflex'] = $chg_data['wflex'] = 0; // don't flex, as sun must stay round!
  }

  // some of these may have changed, so we put them back into the array
  $chg_data['node'] = $node;
  $chg_data['layout'] = $layout;
  $chg_data['body'] = $body;
  $chg_data['height'] = $height;
  $chg_data['width'] = $width;
  $chg_data['hold_point'] = $hold_point;
  $chg_data['hold_radius'] = $hold_radius;
  $chg_data['bounding_box'] = $bounding_box;
  $chg_data['name'] = $chargeName;
  $chg_data['group'] = $chargeGroup;
  return $chg_data;
}
