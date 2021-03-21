<?php

  $ordinary['shape_spec'] = 'X-100Y-8U1a1200c308E1200g308';
  $ordinary['chargesOn'] = array ('h','100,50,800,200');
if ( existModifierWithKeyterm($node,'couped') ) {
  $ordinary['shape_spec'] = 'X100Y-8U1a800c408E800g408';
  $ordinary['chargesOn'] = array ('h','200,50,600,200');
} elseif ( existModifierWithKeyterm($node, 'couped-dexter')) {
  $ordinary['shape_spec'] = 'X-8Y-8U1a908c408E908g408';
  $ordinary['chargesOn'] = array ('h','200,50,600,200');
} elseif ( existModifierWithKeyterm($node,'couped-sinister')) {
  $ordinary['shape_spec'] = 'X100Y-8U1a908c408E908g408';
  $ordinary['chargesOn'] = array ('h','200,50,600,200');
} elseif ( existModifierWithKeyterm($node,'lozengy')) {
  $ordinary['body'] = '<path d="M0,0l83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 83.3,150 83.3,-150 v300 l-83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 -83.3,-150 -83.3,150 " />';
  $ordinary['chargesOn'] = array ('h','100,50,800,200'); // Or an error?
} elseif ( existModifierWithKeyterm($node, 'voided')) {
  $ordinary['shape_spec'] = 'X-100Y300U1a1200c40E1200g40';
  $ordinary['chargesOn'] = array ('h','100,30,800,180');
} elseif ( existModifierWithKeyterm($node, 'pointed')) {
  $ordinary['body'] = '<path d="M -20,250 500,350 1020,250 V -20 H -20 Z" />';
  $ordinary['chargesOn'] = array ('h','100,50,800,200');
} else {
  $ordinary['shape_spec'] = (string)path_builder(-100, -8)
    ->draw_all()
    ->right_plain($layout->size->width+200)
    ->down_plain($layout->size->height+8)
    ->left($layout->size->width+200)
    ->up_plain($layout->size->height+8);
  $ordinary['chargesOn'] = array ('h','100,50,800,200');
}

 if ( existModifierWithKeyterm($node,'compony1') )
  $ordinary['body2'] = '<path d="M0,0h250v450h-250z M500,0h250v450h-250z" />';
elseif (existModifierWithKeyterm($node,'compony2') )
  $ordinary['body2'] =  '<path d="M0,0h250v150h-250z M250,150h250v300h-250z M500,0h250v150h-250z M750,150h250v300h-250z" />';

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',265)">' . $rivet . '</g>';
  }
  $ordinary['body3'] = $rivetList;
}
