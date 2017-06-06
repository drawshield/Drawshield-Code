<?php
/**
 * Created by PhpStorm.
 * User: karlwilcox
 * Date: 31/08/15
 * Time: 22:47
 */

if ($field = getFieldTincture($node)) {
  if ($field->firstChild->getAttribute(blazonML::A_KEYTERM) == 'american-football-field') {
      $node->setAttribute(blazonML::A_KEYTERM,'sports/football-helmet');
      $charge['file'] = 'inkscape/football-helmet.svg';
  }
}