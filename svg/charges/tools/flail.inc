<?php

if (($arrangement = getModifierNodeWithKeyterm($node, 'insaltire')) != null) {
  $num = $node->getAttribute('number');
  if ($num == 2) {
    $node->setAttribute('number','1');
    $charge['file'] = "flail-insaltire-2.svg";
    $node->removeChild($arrangement);
  }
}