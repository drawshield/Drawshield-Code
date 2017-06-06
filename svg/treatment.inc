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


function getSvgTreatment ($filename) {
  global $messages;
  $retval = array();

  if ( ($svg = simplexml_load_file($filename)) == null ) {
    $messages->addMessage('internal',"XML error reading charge data");
    return null;
  }
  $retval['height'] = floatval($svg['height']);
  $retval['width'] = floatval($svg['width']);
  @$metadata = $svg->xpath('//cc:Agent/dc:title');
  if ( is_array($metadata) and count($metadata) > 0) {
    $messages->addMessage('licence', $metadata[0]);
  }
  $fragment = '';
  foreach( $svg->children() as $child ) {
    if ( $child->getName() != 'metadata' and $child->getName() != 'defs' )
      $fragment .= $child->asXML();
  }
  $retval['body'] = $fragment;
  return $retval;
}


function makeTreatment ( $node ) {
  global $messages;
  $folder = 'svg/treatments/inkscape/';

  $name = $node->getAttribute('keyterm');
  $tinctures = array();
  $children = $node->childNodes;
  $parent = $node->parentNode;
  // This file can set $filename and/or overwrite $tinctures  (and has access to the other variables above)
  if ( file_exists("svg/treatments/$name.inc"))
    include "svg/treatments/$name.inc";
  foreach ( $children as $child ) {
    if ( $child->nodeName == 'tincture' )
      $tinctures[$child->getAttribute('index')] = $child;
  }
  if ( !isset($filename))
  $filename = "$folder$name.svg";
  if ( file_exists($filename))
    $treatment_data = getSvgTreatment($filename);
  else {
    $treatment_data = getSvgTreatment("${folder}checky.svg");
    $messages->addMessage('warning',"Checky substituted for treatment $name");
  }
  $treatment = $treatment_data['body'];
  $size_x = $treatment_data['width'];
  $size_y = $treatment_data['height'];
  $base = apply_tincture ( $tinctures[1], '<rect x="0" y="0" width="' .
                $size_x . '" height="' . $size_y . '" />', "$size_x,$size_y" );
  $pattern = apply_tincture ( $tinctures[2], $treatment, "$size_x,$size_y" );
  $retval = array ( 'width' =>  $size_x, 'height' => $size_y, 'body' => $base . '<g stroke-width="2" stroke="' . rgb('pattern-stroke') . '" >' .$pattern . '</g>' );
  return $retval;
}

function makeSemy( $node ) {
  global $messages;
  $children = $node->childNodes;
  $background = $charge = null;
  foreach ( $children as $child ) {
    if ( $child->nodeName == 'tincture' )
      $background = $child;
   elseif ( $child->nodeName == 'charge' )
     $charge = $child;
  }
  if ( $background == null or $charge == null )
    $messages->addMessage('internal', 'Bad semyde element' );
  else {
    $semy = PlaceCharge($charge, 100, 100, 100, 100);
    $semy .= PlaceCharge($charge, 100, 100, 0, 0);
    $semy .= PlaceCharge($charge, 100, 100, 200, 0);
    $semy .= PlaceCharge($charge, 100, 100, 0, 200);
    $semy .= PlaceCharge($charge, 100, 100, 200, 200);
    $base = apply_tincture ( $background, '<rect x="0" y="0" width="200" height="200" />' );
    return array (  'link' => 'http://karlwilcox.com/parker/s/seme', 'width' => 200, 'height' => 200, 'body' => $base . $semy );
 }
}
