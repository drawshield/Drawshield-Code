<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

     DrawShield is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
 */


require_once("shield_shape_utils.inc");

$margin = 68;
$width = 100;

if ( !is_null($layout->section) && strpos($layout->section, "quarter") !== false )
{
    $width *= 2;
    $margin *= 2;
    $ext = min($layout->size->width, $layout->size->height) - 2 * $margin;
    $width = min($ext / 4, $width);
}

$shape = ShieldShape::from_layout($layout);

$path_builder = new MutableShape();

$shape->svg_path($layout, $margin, false, $path_builder);
$shape->svg_path($layout, $margin + $width, true, $path_builder);

$ordinary['shape_spec'] = $path_builder;

$ordinary['chargesOn'] = 'x///////////125,300,80,80:875,300:500,120:500,1070:125,700:875,700:250,120:750,120:250,970:750,970:125,500:875,500';

$ordinary['chargesWithin'] = $layout->rect_area()->margin(250, 0, 250, 350);

// Change orle fur into on an orle X 12 ermine spots Y
$tinc = getTinctureNode($node);
if ( $tinc->hasChildNodes() and $tinc->firstChild->nodeName == 'fur' ) {
  $spot_col = null;
  $orle_col = null;
  switch ($tinc->firstChild->getAttribute('keyterm')) {
    case 'ermine':
      $orle_col = createColourNode('argent');
      $spot_col = createColourNode('sable');
      break;
    case 'erminois':
      $orle_col = createColourNode('or');
      $spot_col = createColourNode('sable');
      break;
    case 'ermines':
      $orle_col = createColourNode('sable');
      $spot_col = createColourNode('argent');
      break;
    case 'pean':
      $orle_col = createColourNode('sable');
      $spot_col = createColourNode('or');
      break;
  }
  if ( $orle_col != null ) {
    $node->replaceChild($orle_col,$tinc);
    $on = createModifierNode('wordon','on');
    $chg = createChargeNode('emblem/ermine-spot',12);
    $chg->appendChild($spot_col);
    $on->appendChild($chg);
    $node->appendChild($on);
  }
}



if ( existModifierWithKeyterm($node,'compony1') )
{
    $ordinary['body2'] = $shape->compony($layout, $margin, $width)->to_svg_path();
}
elseif (existModifierWithKeyterm($node,'compony2') )
{
    $body2 = $shape->compony($layout, $margin, $width);
    $shape->svg_path($layout, $margin + $width / 2, true, $body2);
    $ordinary['body2'] = $body2->to_svg_path();
}


$ordinary['chargesOn'] = $shape->charge_placement($layout, $margin, $width);
$ordinary['feature_size'] = 30;
