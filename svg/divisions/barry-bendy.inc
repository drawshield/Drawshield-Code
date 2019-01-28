<?php 

if ( existModifierWithKeyterm($node, 'sinister'))
  $div_data = array ( 250, 500, 'm62.5,125h125l62.5,-125h-125z h-62.5v125z m125,0h62.5v125h-125z m-125,250h-62.5v-125h125z h125l-62.5,125h-125z m125,0h62.5v-125z' );
else
  $div_data =  array ( 250, 500, 'l67.5,125h125l-67.5,-125z m0,125v125h125l-62.5,-125z m187.5,0l62.5,125v-125z m0,250h62.5v-125h-125z m-125,0l-62.5,-125v125z l62.5,125h125l-62.5,-125z' );

$retval = '<path d="';
list($width, $height, $pattern ) = $div_data;
for ( $y = 0; $y < 1200; $y += $height )
  for ( $x = 0; $x < 1000; $x += $width )
    $retval .= "M$x,$y $pattern ";
$retval .= '" />';
$division['div2'] = $retval;

