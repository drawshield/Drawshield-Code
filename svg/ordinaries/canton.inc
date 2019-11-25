<?php 

$ordinary['if_ord_on'] = 'scale';
$ordinary['feature_size'] = 40;
$ordinary['shape_spec'] = (chiefPresent() ? 
            'X-{%1}Y{%24*}a{%31}C{%31}E{%31}g{%31}' : 
            'X-{%1}Y-{%1}a{%31}C{%31}E{%31}g{%31}');
if ( $node->getAttribute('number') == '2') 
    $ordinary['shape_spec'] .=  (chiefPresent() ? 
            '    X{%101}Y{%24*}c{%31}E{%31}G{%31}a{%31}' :
            '    X{%101}Y-{%1}c{%31}E{%31}G{%31}a{%31}');
$ordinary['chargesOn'] = array ('w', (chiefPresent() ? '{%2},{%27*},{%26},{%26}' : '{%2},{%2},{%26},{%26}'));
