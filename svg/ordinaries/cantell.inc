<?php 

$ordinary['if_ord_on'] = 'scale';
$ordinary['feature_size'] = 40;
$ordinary['shape_spec'] = (chiefPresent() ? 
        'X{%101}Y{%24*}c{%31}E{%31}G{%31}a{%31}' :
        'X{%101}Y-{%1}c{%31}E{%31}G{%31}a{%31}');
$ordinary['chargesOn'] = array ('w', (chiefPresent() ? '{%72},{%27*},{%26},{%26}' : '{%72},{%2},{%26},{%26}'));
