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


$ordinary['body'] = array ( 'NC' => '<path fill-rule="evenodd" d="M 68.69,64.58 C 68.69,64.58 74.47,824.24 74.75,826.24 C 96.97,988.24 389.91,1092.92 496.99,1123.22 C 593.96,1094.94 914.82,998.29 925.29,826.24 C 925.50,822.90 927.32,64.58 927.32,64.58 L 68.69,64.58 z
        M 173.74,175.70 C 173.74,175.70 173.09,783.92 173.74,785.83 C 226.27,939.75 389.91,985.84 496.99,1016.15 C 593.96,987.86 759.26,949.80 828.32,785.83 C 829.62,782.75 828.32,175.70 828.32,175.70 L 173.74,175.70 z" />',
        'CH' => '<path fill-rule="evenodd" d="M 68.69,364.58 C 68.69,364.58 74.47,824.24 74.75,826.24 C 96.97,988.24 389.91,1092.92 496.99,1123.22 C 593.96,1094.94 914.82,998.29 925.29,826.24 C 925.50,822.90 927.32,364.58 927.32,364.58 L 68.69,364.58 z
        M 173.74,475.70 C 173.74,475.70 173.09,783.92 173.74,785.83 C 226.27,939.75 389.91,985.84 496.99,1016.15 C 593.96,987.86 759.26,949.80 828.32,785.83 C 829.62,782.75 828.32,475.70 828.32,475.70 L 173.74,475.70 z" />');

$ordinary['chargesOn'] = chiefPresent() ? 'x///////////125,800,80,80:875,800:500,420:500,1070:125,800:875,800:250,420:750,420:250,970:750,970:125,600:875,600' :
  'x///////////125,300,80,80:875,300:500,120:500,1070:125,700:875,700:250,120:750,120:250,970:750,970:125,500:875,500';

$ordinary['chargesWithin'] = array ('w',chiefPresent() ? '250,550,500,300' : '250,250,500,600');

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
    $chg = createChargeNode('symbol/ermine-spot',12);
    $chg->appendChild($spot_col);
    $on->appendChild($chg);
    $node->appendChild($on);
  }
}

