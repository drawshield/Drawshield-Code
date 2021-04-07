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


$margin = 100;
$height = ($layout->size->height - $layout->division_offset->y) / 3;
$y1 = $layout->division_offset->y + $height;
$y2 = $y1 + $height;
$width = $layout->size->width + 2 * $margin;

$builder_2 = path_builder(-$margin, $y1)
    ->right($width)
    ->down_plain(2*$height + $margin)
    ->left_plain($width)
    ->up_plain(2*$height + $margin)
;
$builder_3 = path_builder(-$margin, $y2)
    ->right($width)
    ->down_plain($height + $margin)
    ->left_plain($width)
    ->up_plain($height + $margin)
;

$division['div2'] = createBody((string)$builder_2, $lineType, 60, $layout);
$division['div3'] = createBody((string)$builder_3, $lineType, 60, $layout);
if ( existModifierWithKeyterm($node, 'riveted')) {
	include '_rivet.inc';
	$rivetList = '';
	for ($i = 5; $i < 1000; $i += 80) {
		$rivetList .= '<g transform="translate(' . $i . ',365)">' . $rivet . '</g>';
		$rivetList .= '<g transform="translate(' . $i . ',415)">' . $rivet . '</g>';
		$rivetList .= '<g transform="translate(' . $i . ',765)">' . $rivet . '</g>';
		$rivetList .= '<g transform="translate(' . $i . ',815)">' . $rivet . '</g>';
	}
	$division['divx'] = $rivetList;
}
