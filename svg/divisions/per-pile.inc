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
$div2 = new MutableShape();
$div3 = new MutableShape();

if ( existModifierWithKeyterm($node, 'reversed') or  existModifierWithKeyterm($node, 'reversed') )
{
    $div2->move_to(0, $layout->size->height + 200)
        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->line_to($layout->size->width / 2, 0)
        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->line_to($layout->size->width, $layout->size->height + 200)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->close()
    ;
    $div3->move_to($layout->size->width / 2, -200)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->line_to($layout->size->width / 2, 0)
        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->line_to($layout->size->width, $layout->size->height + 200)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->line_to($layout->size->width + 200, $layout->size->height + 200)
        ->line_to($layout->size->width + 200, -200)
        ->close()
    ;
}
else
{
    $div2->move_to(0, -200)
        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->line_to($layout->size->width / 2, $layout->size->height)
        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->line_to($layout->size->width, -200)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->close()
    ;

    $div3->move_to($layout->size->width / 2, $layout->size->height)
        ->tag_segment(BezierSegmentFlags::NORMAL)
        ->line_to($layout->size->width, -200)
        ->tag_segment(BezierSegmentFlags::PLAIN)
        ->line_to($layout->size->width + 200, -200)
        ->line_to($layout->size->width + 200, $layout->size->height + 200)
        ->line_to($layout->size->width / 2, $layout->size->height + 200)
        ->close()
    ;
}


$division['div2'] = createBody($div2, $lineType, 40, $layout);
$division['div3'] = createBody($div3, $lineType, 40, $layout);
