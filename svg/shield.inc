<?php 

function makePlain( &$children, $layout ) {
  global $options;
  $field = '';
  $objects = '';
  $chief = '';
  // Does this shield contain a chief?
  $found = $children->item(0)->nodeName == blazonML::E_CHIEF;
  chief('push', $found ? 'CH' : 'NC');
  // Process the shield parts
  foreach ( $children as $kid ) {
    switch ( $kid->nodeName ) {
      case 'field':
        // Paint the field
        if (!array_key_exists('raw', $options)) {
            $field .= apply_tincture ( $kid->firstChild,
            '<rect x="0" y="0" width="1000" height="' . $layout->size->height . '" ><title>Field</title></rect>' ,
            $layout,
            '1000,' . $layout->size->height );
        }
        break;
      case 'objects':
        $objects .= makeOrdChgs($kid, $layout);
        break;
      case 'chief':
        // process ordinaries and charges
        $chief .= makeOrdChgs($kid, $layout);
        break;
    }
  }
  chief('pop');
  return $field . $objects . $chief; // chief always goes on top
}

function makeimpaled( DOMNodeList $children, $layout ) {
  global $messages;
  $retval = '';
  $imp1 = $imp2 = null; // remove warnings

  $size = $layout->size->scaled(0.5, 1);
  $clipI = add_def( 'clipPath', "<rect x='0' y='0' width='{$size->width}'  height='{$size->height}'/>" );
  $child_layout = new ShieldLayout($size, $layout->shape);
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
  $retval .= '<g clip-path="url(#' . $clipI . ')"  >' .  $imp1  . "</g>\n";
  $retval .= "<g clip-path='url(#$clipI)' transform='translate({$size->width},0)' >$imp2</g>\n";
  $retval .= add_def();

  return $retval . "<path stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\" d=\"M {$size->width} 0 v {$layout->size->height}\" />\n";
}
  
function makedimidiated( $children, $layout ) {
  $retval = '';
  
  $size = $layout->size->scaled(0.5, 1);
  $clip1 = add_def( 'clipPath', "<rect x='0' y='0' width='{$size->width}'  height='{$size->height}'/>" );
  $clip2 = add_def( 'clipPath', "<rect x='{$size->width}' y='0' width='{$size->width}'  height='{$size->height}'/>" );

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
  global $messages;
  
  if ( $node->hasAttribute('IDREF') ) { // shouldn't happen, but just in case
    $dom = $node->ownerDocument;
    $node = $dom->getElementById($node->getAttribute('IDREF'))->cloneNode(true);
    $node->setAttribute('ID',unique('clone'));
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
