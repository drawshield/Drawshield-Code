<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see https://www.gnu.org/licenses/. */
/**
 * Created by PhpStorm.
 * User: karlwilcox
 * Date: 31/08/15
 * Time: 21:19
 */

$charge['file'] = 'american-football.svg'; // default
$charge['orient_to_ordinary'] = true;


if ($field = getFieldTincture($node)) {
        switch ($field->firstChild->getAttribute(blazonML::A_KEYTERM)) {
            case 'soccer-field':
                $charge['file'] = 'soccerball.svg';
                $node->setAttribute(blazonML::A_KEYTERM,'sports/soccerball');
                break;
            case 'tennis-court':
                $charge['file'] = 'tennis-ball.svg';
                $node->setAttribute(blazonML::A_KEYTERM,'sports/tennis-ball');
                break;
            case 'basket-ball':
                $charge['file'] = 'basketball.svg';
                $node->setAttribute(blazonML::A_KEYTERM,'sports/basketball');
                break;
            case 'american-football-field':
            default:
                $charge['file'] = 'american-football.svg';
                $node->setAttribute(blazonML::A_KEYTERM,'sports/american-football');
                break;
        }
}
