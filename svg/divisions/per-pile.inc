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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */
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
