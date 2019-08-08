<?php

foreach ( $node->childNodes as $child ) {
  if ( $child->nodeName == 'tincture') {
  	$node->replaceChild(createColourNode('sanguine'),$child);
    break;
  }
}

$charge['file'] = 'roundel.svg';
