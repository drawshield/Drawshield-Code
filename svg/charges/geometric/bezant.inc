<?php

foreach ( $node->childNodes as $child ) {
  if ( $child->nodeName == 'tincture') {
  	$node->replaceChild(createColourNode('or'),$child);
    break;
  }
}

$charge['file'] = 'bezant.svg';
