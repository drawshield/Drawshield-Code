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

// These are for scaling into quarters & dimidiations
$aspect_ratio = '10:12';
$scale_factor = 1.0;

function makePlain( &$children ) {
  global $options;

  $retval = '';
  // Process the shield parts
  foreach ( $children as $kid ) {
    switch ( $kid->nodeName ) {
      case 'field':
        // Paint the field
        $retval .= apply_tincture ( $kid->firstChild, '<rect x="0" y="0" width="1000" height="' . $options['flagHeight'] . '" ><title>Field</title></rect>' , '1000,' . $options['flagHeight'] );
        break;
      case 'objects':
        // process ordinaries and charges
        $retval .= makeOrdChgs($kid);
        break;
    }
  }
  return $retval;
}

function makeimpaled( DOMNodeList $children, $this_AR ) {
  global $messages;
  $retval = '';
  switch ( $this_AR ) {
  case '10:12':
    $clipI = add_def( 'clipPath', '<path d="M250,0h500v1200h-500v-1200z" />' );
    $xOffset = 250;
    $child_AR = '10:24';
    break;
  case '10:10':
    $clipI = add_def( 'clipPath', '<path d="M250,0h500v1000h-500v-1000z" />' );
    $xOffset = 250;
    $child_AR = '10:20';
    break;
  case '10:14':
    $clipI = add_def( 'clipPath', '<path d="M285,0h430v1200h-430v-1200z" />' );
    $xOffset = 285;
    $child_AR = '10:28';
    break;
  default:
    //$messages->addMessage('warning', "Impaled shield too narrow" );
    $clipI = add_def( 'clipPath', '<path d="M285,0h430v1200h-430v-1200z" />' );
    $xOffset = 285;
    $child_AR = '10:28';
    break;
  }
  $imp1 = $imp2 = null; // remove warnings
  /**
   * @var DOMElement $kid
   */
  foreach ( $children as $kid ) {
    switch( $kid->getAttribute('index') ) {
    case '1':
      region('push','I1');
      $imp1 = makeshield( $kid, $child_AR ); 
      region('pop');
      break;
    case '2':
      region('push','I2');
      $imp2 = makeshield( $kid, $child_AR );
      region('pop');
      break;
    }
  }
  $retval .= '<g clip-path="url(#' . $clipI . ')" transform="translate(-' . (500 - $xOffset) . ',0)" >' .  $imp1  . "</g>\n";
  $retval .= '<g clip-path="url(#' . $clipI . ')" transform="translate(' . (500 - $xOffset) . ',0)" >' . $imp2 . "</g>\n";
  $retval .= add_def();

  return $retval;
}
  
function makedimidiated( $children, $this_AR ) {
  global $messages;
  $messages->addMessage('Dimidiated not supported for flags');

  return '';
}

function makeflag( DOMElement &$node ) {
  global $dom, $messages;
  global $aspect_ratio;
  
  if ( $node->hasAttribute('IDREF') ) {
    $node = $dom->getElementById($node->getAttribute('IDREF'));
  }


  $retval = '<g>';
  /**
   * @var DOMElement $child
   */
  foreach ( $node->childNodes as $child ) {
    $children = $child->childNodes;
    switch ( $child->nodeName ) {
      case 'simple':
        $retval .= makePlain($children);
        break;
      case 'complex':
        switch ( $child->getAttribute('keyterm') ) {
          case 'dimidiated':
            $retval .= makeDimidiated( $children );
            break;
          case 'impaled':
            $retval .= makeImpaled( $children );
            break;
          case 'quartered':
            switch ( $child->getAttribute('order') ) {
              case 4:
                $retval .= makeQuartered4($children);
                break;
              case 5:
                $retval .= makeQuartered5($children);
                break;
              case 6:
                $retval .= makeQuartered6($children);
                break;
              case 8:
                $retval .= makeQuartered8($children);
                break;
              case 9:
                $retval .= makeQuartered(3,3, $children);
                break;
              case 12:
                $retval .= makeQuartered(3,4, $children);
                break;
              case 16:
                $retval .= makeQuartered(4,4, $children);
                break;
              case 20:
                $retval .= makeQuartered(4,5, $children);
                break;
              case 25:
                $retval .= makeQuartered(5,5, $children);
                break;
              case 30:
                $retval .= makeQuartered(5,6, $children);
                break;
              case 36:
                $retval .= makeQuartered(6,6, $children);
                break;
              case 42:
                $retval .= makeQuartered(6,7, $children);
                break;
              case 49:
                $retval .= makeQuartered(7,7, $children);
                break;
              case 56:
                $retval .= makeQuartered(7,8, $children);
                break;
              case 64:
                $retval .= makeQuartered(8,8, $children);
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
        $aspect_ratio = '10:12';
        $retval .= makeOrdChgs($child->firstChild);
        break;
    }
  }
  return $retval . '</g>';
}
