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

function get_clipdata ( $AR ) {
  $clip_data = array (
    '10:12' => '0,0,1000,1200',
    '10:10' => '0,0,1000,1000',
    '10:14' => '70,0,860,1200',
    '10:24' => '250,0,500,1200',
    '10:20' => '250,0,500,1000',
    '10:28' => '285,0,430,1200',
    '10:6' => '0,200,1000,600',
  );

  $retval = array();
  list($x1,$y1,$w,$h) = explode(',',$clip_data[$AR]);
  $retval['clippath'] = "<path d=\"M$x1,$y1 h$w v$h h-$w v-$h Z\" />";
  $retval['xOffset'] = $x1;
  $retval['yOffset'] = $y1;
  return $retval;
}

$halfHeights = array (
    '10:12' => 500,
    '10:10' => 500,
    '10:14' => 600,
    '10:24' => 600,
    '10:20' => 500,
    '10:28' => 500,
    '10:6' => 300,  
);

$quarterHeights = array (
    '10:12' => 250,
    '10:10' => 250,
    '10:14' => 300,
    '10:24' => 300,
    '10:20' => 250,
    '10:28' => 250,
    '10:6' => 150, 
);

$thirdHeights = array (
    '10:12' => 400,
    '10:10' => 400,
    '10:14' => 400,
    '10:24' => 400,
    '10:20' => 400,
    '10:28' => 400,
    '10:6' => 200, // ???
);

function get_hierarchy( DOMElement $item ) {
  $hierarchy = '';
  
  do {
    if ( $item->nodeName == 'shield') {
      $index = $item->getAttribute('index');
      switch ( $item->parentNode->nodeName ) {
      case 'blazon':
        $hierarchy = 'PL' . $hierarchy;
        break;
      case 'quartered':
        $hierarchy = 'Q' . $index . $hierarchy;
        break;
      case 'impaled':
        $hierarchy = 'I' . $index . $hierarchy;
        break;
      case 'dimidiated':
        $hierarchy = 'D' . $index . $hierarchy;
        break;
      }
    }
    $item = $item->parentNode;
  } while ($item->nodeName != 'blazon');
  
  return $hierarchy;
}

function makePlain( &$children, $drawnAR = '10:12' ) {
  global $aspect_ratio;
  
  $aspect_ratio = $drawnAR;
  
  $retval = '';
  // Does this shield contain a chief?
  $found = false;
  foreach ( $children as $kid ) {
    if ( !$found and $kid->nodeName == 'objects' ) $found = find_chief ( $kid );
  }
  chief('push', $found ? 'CH' : 'NC');
  // Process the shield parts
  foreach ( $children as $kid ) {
    switch ( $kid->nodeName ) {
      case 'field':
        // Paint the field
        $retval .= apply_tincture ( $kid->firstChild, '<rect x="0" y="0" width="1000" height="1200" ><title>Field</title></rect>' , '1000,1200' );
        break;
      case 'objects':
        // process ordinaries and charges
        $retval .= makeOrdChgs($kid);
        break;
    }
  }
  chief('pop');
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
  $retval = '';
  
  switch ( $this_AR ) {
  case '10:12':
    $clip1 = add_def( 'clipPath', '<path d="m0,0L500,0 500,1200 0,1200Z" />' );
    $clip2 = add_def( 'clipPath', '<path d="m500,0L1000,0 1000,1200 500,1200Z" />' );
    $child_AR = '10:12';
    break;
  case '10:10':
    $clip1 = add_def( 'clipPath', '<path d="m0,0L500,0 500,1000 0,1000Z" />' );
    $clip2 = add_def( 'clipPath', '<path d="m500,0L1000,0 1000,1000 500,1000Z" />' );
    $child_AR = '10:12';
    break;
  case '10:14':
    $clip1 = add_def( 'clipPath', '<path d="m70,0L430,0 430,1200 0,1200Z" />' );
    $clip2 = add_def( 'clipPath', '<path d="m500,0L860,0 860,1200 500,1200Z" />' );
    $child_AR = '10:14';
    break;
  default:
    $messages->addMessage('warning', "Dimidiated shield too narrow" );
    $clip1 = add_def( 'clipPath', '<path d="m70,0L430,0 430,1200 0,1200Z" />' );
    $clip2 = add_def( 'clipPath', '<path d="m500,0L860,0 860,1200 500,1200Z" />' );
    $child_AR = '10:14';
    break;
  }
  /**
   * @var DOMElement $kid
   */
  foreach ( $children as $kid ) {
    switch( $kid->getAttribute('index') ) {
    case '1':
      region('push','D1');
      $retval .= '<g clip-path="url(#' . $clip1 . ')" >' . makeshield( $kid, $child_AR ) . "</g>\n";
      region('pop');
      break;
    case '2':
      region('push','D2');
      $retval .= '<g clip-path="url(#' . $clip2 . ')" >' . makeshield( $kid, $child_AR ) . "</g>\n";
      region('pop');
      break;
    }
  }
  $retval .= add_def();
  return $retval;
}

function makeshield( DOMElement &$node, $this_AR ) {
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
        $retval .= makePlain($children, $this_AR);
        break;
      case 'complex':
        switch ( $child->getAttribute('keyterm') ) {
          case 'dimidiated':
            $retval .= makeDimidiated( $children, $this_AR );
            break;
          case 'impaled':
            $retval .= makeImpaled( $children, $this_AR );
            break;
          case 'quartered':
            switch ( $child->getAttribute('order') ) {
              case 4:
                $retval .= makeQuartered4($children, $this_AR);
                break;
              case 5:
                $retval .= makeQuartered5($children, $this_AR);
                break;
              case 6:
                $retval .= makeQuartered6($children, $this_AR);
                break;
              case 8:
                $retval .= makeQuartered8($children, $this_AR);
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
