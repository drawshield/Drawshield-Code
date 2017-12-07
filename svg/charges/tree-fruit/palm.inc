<?php

if (($arrangement = getModifierNodeWithKeyterm($node, 'insaltire')) != null) {
  $num = $node->getAttribute('number');
  if ($num == 2) {
    $node->setAttribute('number','1');
    if (existModifierWithKeyterm($node,'leaf')) {
    	$charge['file'] = "palm-leaf-insaltire-2.svg";
    } else {
    	$charge['file'] = "palm-tree-insaltire-2.svg";
    }
    $node->removeChild($arrangement);
  }
}