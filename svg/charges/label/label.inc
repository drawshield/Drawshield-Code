<?php 

$suffix = '-straight';
if (existModifierWithKeyterm($node,'dovetailed')) {
  $suffix = '-dovetailed';
} elseif (existModifierWithKeyterm($node,'pattee')) {
  $suffix = '-pattee';
}

$num_of = getModifierValueByKeyterm($node, 'of-num', true);
if ( $num_of <= 4 )
  $num_of = 3;
elseif ( $num_of >= 6 )
  $num_of = 7;


$charge['file'] = 'label-of-' . $num_of . $suffix . '.svg';

