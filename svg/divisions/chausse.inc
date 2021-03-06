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
 * Time: 22:21
 */

$builder = new MutableShape();
$builder->move_to($layout->size->width / 2, $layout->size->height)
    ->tag_segment(BezierSegmentFlags::NORMAL)
    ->line_to($layout->size->width, $layout->size->height * .134)
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->line_to($layout->size->width + 100, $layout->size->height + 100)
     ->line_to(-100, $layout->size->height + 100)
    ->line_to(0, $layout->size->height * .134)
    ->tag_segment(BezierSegmentFlags::NORMAL)
    ->line_to($layout->size->width / 2, $layout->size->height)
    ->close();

// $division['div2'] = subPercent('<path d="m500,{%100} l500,-866 0,866 -1000,0 0,-866 500,866z" />', $layout);

$lineType = getModifierByType($node, 'linetype', false);
$division['div2'] = createBody($builder, $lineType, 30, $layout);
