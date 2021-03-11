<?php 

function makePlain( &$children, $layout ) {
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
        if (!array_key_exists('raw', $options)) {
            $retval .= apply_tincture ( $kid->firstChild,
            '<rect x="0" y="0" width="1000" height="' . $layout->size->height . '" ><title>Field</title></rect>' ,
            $layout,
            '1000,' . $layout->size->height );
        }
        break;
      case 'objects':
        // process ordinaries and charges
        $retval .= makeOrdChgs($kid, $layout);
        break;
    }
  }
  chief('pop');
  return $retval;
}

function makeimpaled( DOMNodeList $children, $layout ) {
  global $messages;
  $retval = '';
  switch ( $layout->size->closest_aspect_ratio() ) {
  case '10:12':
    $clipI = add_def( 'clipPath', '<path d="M250,0h500v1200h-500v-1200z" />' );
    $xOffset = 250;
    break;
  case '10:10':
    $clipI = add_def( 'clipPath', '<path d="M250,0h500v1000h-500v-1000z" />' );
    $xOffset = 250;
    break;
  case '10:14':
    $clipI = add_def( 'clipPath', '<path d="M285,0h430v1200h-430v-1200z" />' );
    $xOffset = 285;
    break;
  default:
    //$messages->addMessage('warning', "Impaled shield too narrow" );
    $clipI = add_def( 'clipPath', '<path d="M285,0h430v1200h-430v-1200z" />' );
    $xOffset = 285;
    break;
  }
  $imp1 = $imp2 = null; // remove warnings

  $child_layout = new ShieldLayout($layout->size->scaled(0.5, 1), $layout->shape);
  /**
   * @var DOMElement $kid
   */
  foreach ( $children as $kid ) {
    switch( $kid->getAttribute('index') ) {
    case '1':
      region('push','I1');
      $imp1 = makeshield( $kid, $child_layout );
      region('pop');
      break;
    case '2':
      region('push','I2');
      $imp2 = makeshield( $kid, $child_layout );
      region('pop');
      break;
    }
  }
  $retval .= '<g clip-path="url(#' . $clipI . ')" transform="translate(-' . (500 - $xOffset) . ',0)" >' .  $imp1  . "</g>\n";
  $retval .= '<g clip-path="url(#' . $clipI . ')" transform="translate(' . (500 - $xOffset) . ',0)" >' . $imp2 . "</g>\n";
  $retval .= add_def();

  return $retval . "<path stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\" d=\"M 499 0 v 1200\" />\n";
}
  
function makedimidiated( $children, $layout ) {
  global $messages, $options;
  $retval = '';
  
  switch ( $this_AR ) {
  case '10:12':
    $clip1 = add_def( 'clipPath', '<path d="m0,0L500,0 500,1200 0,1200Z" />' );
    $clip2 = add_def( 'clipPath', '<path d="m500,0L1000,0 1000,1200 500,1200Z" />' );
    break;
  case '10:10':
    $clip1 = add_def( 'clipPath', '<path d="m0,0L500,0 500,1000 0,1000Z" />' );
    $clip2 = add_def( 'clipPath', '<path d="m500,0L1000,0 1000,1000 500,1000Z" />' );
    break;
  case '10:14':
    $clip1 = add_def( 'clipPath', '<path d="m70,0L430,0 430,1200 0,1200Z" />' );
    $clip2 = add_def( 'clipPath', '<path d="m500,0L860,0 860,1200 500,1200Z" />' );
    break;
  default:
    $messages->addMessage('warning', "Dimidiated shield too narrow" );
    $clip1 = add_def( 'clipPath', '<path d="m70,0L430,0 430,1200 0,1200Z" />' );
    $clip2 = add_def( 'clipPath', '<path d="m500,0L860,0 860,1200 500,1200Z" />' );
    break;
  }

  /**
   * @var DOMElement $kid
   */
  foreach ( $children as $kid ) {
    switch( $kid->getAttribute('index') ) {
    case '1':
      region('push','D1');
      $retval .= '<g clip-path="url(#' . $clip1 . ')" >' . makeshield( $kid, $layout ) . "</g>\n";
      region('pop');
      break;
    case '2':
      region('push','D2');
      $retval .= '<g clip-path="url(#' . $clip2 . ')" >' . makeshield( $kid, $layout ) . "</g>\n";
      region('pop');
      break;
    }
  }
  $retval .= add_def();
  return $retval . "<path stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\" d=\"M 499 0 v 1200\" />\n";
}

function makeshield( DOMElement &$node, $layout ) {
  global $dom, $messages;
  
  if ( $node->hasAttribute('IDREF') ) {
    $node = $dom->getElementById($node->getAttribute('IDREF'));
  }

  $retval = '<g>';
  if (!is_object($node)) return $retval;
  /**
   * @var DOMElement $child
   */
  foreach ( $node->childNodes as $child ) {
    $children = $child->childNodes;
    switch ( $child->nodeName ) {
      case 'simple':
        $retval .= makePlain($children, $layout);
        break;
      case 'complex':
        switch ( $child->getAttribute('keyterm') ) {
          case 'dimidiated':
            $retval .= makeDimidiated( $children, $layout );
            break;
          case 'impaled':
            $retval .= makeImpaled( $children, $layout );
            break;
          case 'quartered':
            switch ( $child->getAttribute('order') ) {
              case '4-saltire':
                $retval .= make_quartered_saltire($children, $layout);
                break;
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
                $retval .= makeQuartered(3,3, $children, $layout);
                break;
              case 12:
                $retval .= makeQuartered(3,4, $children, $layout);
                break;
              case 16:
                $retval .= makeQuartered(4,4, $children, $layout);
                break;
              case 20:
                $retval .= makeQuartered(4,5, $children, $layout);
                break;
              case 25:
                $retval .= makeQuartered(5,5, $children, $layout);
                break;
              case 30:
                $retval .= makeQuartered(5,6, $children, $layout);
                break;
              case 36:
                $retval .= makeQuartered(6,6, $children, $layout);
                break;
              case 42:
                $retval .= makeQuartered(6,7, $children, $layout);
                break;
              case 49:
                $retval .= makeQuartered(7,7, $children, $layout);
                break;
              case 56:
                $retval .= makeQuartered(7,8, $children, $layout);
                break;
              case 64:
                $retval .= makeQuartered(8,8, $children, $layout);
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
  }
  return $retval . '</g>';
}
