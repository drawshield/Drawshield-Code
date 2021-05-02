<?php 

function makePlain( &$children, $layout ) {
    global $options;
    $retval = '';

    // Does this shield contain a chief?
    $chief = null;
    $field = null;
    $objects = null;

    // Process the shield parts
    foreach ( $children as $kid )
    {
        switch ( $kid->nodeName ) {
            case 'field':
                $field = $kid;
                break;
            case 'objects':
                $objects = $kid;
                break;
            case "chief":
                if ( $layout->chief )
                    $chief = $kid;
                break;

        }
    }

    $layout_field = $layout;
    $layout_objects = $layout;
    $layout_chief = null;

    $off = 0;
    $clip_attr = "";
    if ( $chief )
    {
        $layout_chief = new ShieldLayout($layout->chief->size, "chief", null, null, false);
        $new_size = new ShieldSize($layout->size->width, $layout->size->height - $layout->chief->size->height);
        $cross_c = new ShieldPos($new_size->width / 2, $new_size->height / 2);
        if ( $layout->cross_point->y < $layout->size->height /2 )
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
    if ( $field && !array_key_exists('raw', $options) )
    {
        $retval .= apply_tincture(
            $field->firstChild,
            "<rect x='0' y='0' width='1000' height='{$layout_field->size->height}'><title>Field</title></rect>",
            $layout_field,
            $layout_field->size
        );
    }

    if ( $objects )
    {
        // process ordinaries and charges
        $retval .= "<g transform='translate(0,$off)'$clip_attr>" . makeOrdChgs($objects, $layout_objects) . "</g>";
    }


    if ( $chief )
        $retval .= makeChief($chief, $layout_chief);

    return $retval;
}

function makeChief($node, $layout)
{
    return makeOrdinary($node->firstChild, $layout);
}

function makeimpaled( DOMNodeList $children, $layout ) {
  global $messages;
  $retval = '';
  $imp1 = $imp2 = null; // remove warnings

  $size = $layout->size->scaled(1, 2);
  $clipI = add_def( 'clipPath', "<rect x='0' y='0' width='{$size->width}'  height='{$size->height}'/>" );
  $cp = $layout->cross_point->translated(0, $layout->cross_point->y);
  $child_layout_1 = new ShieldLayout($size, $layout->shape, $cp, null, true, "dexter");
  $child_layout_2 = new ShieldLayout($size, $layout->shape, $cp, null, true, "sinister");
  /**
   * @var DOMElement $kid
   */
  foreach ( $children as $kid ) {
    switch( $kid->getAttribute('index') ) {
    case '1':
      $imp1 = makeshield( $kid, $child_layout_1 );
      break;
    case '2':
      $imp2 = makeshield( $kid, $child_layout_2 );
      break;
    }
  }
  $retval .= "<g clip-path='url(#$clipI)' transform='scale(0.5)' >$imp1</g>\n";
  $retval .= "<g clip-path='url(#$clipI)' transform='scale(0.5) translate({$size->width},0)' >$imp2</g>\n";
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
      $retval .= '<g clip-path="url(#' . $clip1 . ')" >' . makeshield( $kid, $layout ) . "</g>\n";
      break;
    case '2':
      $retval .= '<g clip-path="url(#' . $clip2 . ')" >' . makeshield( $kid, $layout ) . "</g>\n";
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
            switch ( $child->getAttribute('order') ) {
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
