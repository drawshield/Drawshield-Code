<?php

switch ($options['shape']) {
	case 'drawshield':
		break;
    case 'french':
        break;
    case 'italian': 
        break;
    case 'polish': 
        break;
    case 'spanish': 
        break;
    case 'sca':
        break;
    case 'english': 
        break;
    case 'oval': 
        break;
    case 'german':
        break;
    case 'square':
        break;
    case 'circle':
        break;
    case 'lozenge':
        break;
    case 'pauldron': 
        $options['notrim'] = true; // don't put the grey translucent border on
	    break;
    case 'swiss':
		break;
    default:
        break;
}

$ordinary['chargesWithin'] = array ('w',chiefPresent() ? '120,420,760,560' : '120,120,760,860');

// only really applies to drawshield
$ordinary['chargesOn'] = chiefPresent() ? 'x///////////500,330,55,55:250,330:750,330:500,1150:35,700:965,700:35,450:965,450:50,900:950,900:200,1050:800,1047' : 'x///////////500,30,55,55:250,30:750,30:500,1150:35,500:965,500:35,200:965,200:35,800:965,800:200,1050:800,1050';

