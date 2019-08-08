<?php
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
