<?php
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 06/09/14
 * Time: 19:56
 */

$lineType = getModifierByType($node, 'linetype', false);
/*if ( ($sinister = existModifierWithKeyterm($node, 'sinister'))) {
  $div2_data = array ( 'NC' => 'X-100Y800H1696c1700e1200g500', 'CH' => 'X-100Y1000H1696c1700e1200g500' );
  $div3_data = array ( 'NC' => 'X1100Y200c1100e1100H1696', 'CH' => 'X1100Y400c1100e1100H1696' );
} else {
  $div2_data = array ( 'NC' => 'X-100Y-100a1200c1500F1696g300', 'CH' => 'X-100Y100a1200c1500F1696g300' );
  $div3_data = array ( 'NC' => 'X200Y-100a1000c1000F1414',  'CH' => 'X200Y100a1000c1000F1414' );
}*/

$base = max($layout->size->width, $layout->size->height);
$length = $base * M_SQRT2 * 2;
$width = 400;

if ( $sinister = existModifierWithKeyterm($node, 'sinister') )
{{
    $builder_2 = path_builder($layout->center_point->x + $base, $layout->center_point->y - $base)
        ->up_left_plain($width/2)
        ->down_left($length)
        ->right_plain(($length+$width/2) / M_SQRT2)
        ->up_plain(($length-$width/2) / M_SQRT2)
    ;

    $builder_3 = path_builder($layout->center_point->x + $base, $layout->center_point->y - $base)
        ->down_right_plain($width/2)
        ->down_left($length)
        ->right_plain(($length-$width/2) / M_SQRT2)
        ->up_plain(($length-$width/2) / M_SQRT2)
    ;
}
}
else
{
    $builder_2 = path_builder($layout->center_point->x - $base, $layout->center_point->y - $base)
        ->down_left_plain($width/2)
        ->down_right($length)
        ->up_plain(($length+$width/2) / M_SQRT2)
        ->left_plain(($length-$width/2) / M_SQRT2)
    ;

    $builder_3 = path_builder($layout->center_point->x - $base, $layout->center_point->y - $base)
        ->up_right_plain($width/2)
        ->down_right($length)
        ->up_plain(($length-$width/2) / M_SQRT2)
        ->left_plain(($length-$width/2) / M_SQRT2)
    ;
}


$division['div2'] = createBody((string)$builder_2, $lineType, 60, $layout);
$division['div3'] = createBody((string)$builder_3, $lineType, 60, $layout);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $offset = $layout->center_point->y - 500;
  for ($i = 1075; $i > -80; $i -= 80) {
    if ( $sinister ) {
      $rivetList .= '<g transform="translate(' . (630 - $i) . ',' . ($i  + $offset). ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (680 - $i) . ',' . ($i + 50 + $offset) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (930 - $i) . ',' . ($i +300 + $offset). ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . (980 - $i) . ',' . ($i + 350 + $offset) . ')">' . $rivet . '</g>';
    } else {
      $rivetList .= '<g transform="translate(' . ($i + 350 - $offset) . ',' . $i . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i + 300 - $offset) . ',' . ($i + 50) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i + 50 - $offset) . ',' . ($i + 300) . ')">' . $rivet . '</g>';
      $rivetList .= '<g transform="translate(' . ($i - $offset) . ',' . ($i + 350) . ')">' . $rivet . '</g>';
    }
  }
  $division['divx'] = $rivetList;
}



