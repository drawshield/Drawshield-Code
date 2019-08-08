<?php

foreach ( $node->childNodes as $child ) {
  if ( $child->nodeName == 'tincture') {
  	$node->replaceChild(createColourNode('sable'),$child);
    break;
  }
}

$charge['file'] = 'roundel.svg';
