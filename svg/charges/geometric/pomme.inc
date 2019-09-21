<?php

foreach ( $node->childNodes as $child ) {
  if ( $child->nodeName == 'tincture') {
  	$node->replaceChild(createColourNode('vert'),$child);
    break;
  }
}

$charge['file'] = 'roundel.svg';
