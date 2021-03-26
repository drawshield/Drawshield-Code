<?php /* Copyright 2010 Karl R. Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */


require_once("shield_shape_utils.inc");

$margin = 68;
$width = 100;

if ( strpos($layout->section, "quarter") !== false )
{
    $width *= 2;
    $margin *= 2;
    $ext = min($layout->size->width, $layout->size->height) - 2 * $margin;
    $width = min($ext / 4, $width);
}

$shape = ShieldShape::from_name($layout->shape);

$path_builder = new SvgPathBuilder();

$shape->svg_path($layout, $margin, false, $path_builder);
$shape->svg_path($layout, $margin + $width, true, $path_builder);

$ordinary['body'] = $path_builder->to_path();

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
    $ordinary['body2'] = $shape->compony($layout, $margin, $width)->to_path();
}
elseif (existModifierWithKeyterm($node,'compony2') )
{
    $body2 = $shape->compony($layout, $margin, $width);
    $shape->svg_path($layout, $margin + $width / 2, true, $body2);
    $ordinary['body2'] = $body2->to_path();
}


$ordinary['chargesOn'] = $shape->charge_placement($layout, $margin, $width);
