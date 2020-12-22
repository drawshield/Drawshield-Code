<?php

if (($arrangement = getModifierNodeWithKeyterm($node, 'insaltire')) != null) {
  $num = $node->getAttribute('number');
  if ($num == 2) {
    $node->setAttribute('number','1');
    $charge['file'] = "banner-insaltire-2.svg";
    $node->removeChild($arrangement);
  } elseif ($num == 4) {
    $node->setAttribute('number','1');
    $charge['file'] = "banner-insaltire-4.svg";
    $node->removeChild($arrangement);
  }
}
