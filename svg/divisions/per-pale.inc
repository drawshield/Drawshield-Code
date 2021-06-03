<?php
$div_data = 'X500Y0a500c{%100*}e500G{%100*}';
$division['div2'] = createBody($div_data, getModifierByType($node, 'linetype', false), 60, $layout);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1200; $i += 80) {
    $rivetList .= '<g transform="translate(465,' . $i . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(515,' . $i . ')">' . $rivet . '</g>';
  }
  $division['divx'] = $rivetList;
}


if ( count($tinctures) > 2 )
{
    $thickness = $layout->size->width / count($tinctures);
    $margin = 100;
    for ( $i = 0; $i <= count($tinctures); $i++ )
    {
        $x = $thickness * $i;
        $builder = new PathBuilder();
        $builder->move($x, -$margin)
            ->right_plain($thickness)
            ->down($layout->size->height + $margin * 2)
            ->left_plain($thickness)
            ->up($layout->size->height + $margin * 2);

        $division["div".($i+1)] = createBody((string)$builder, $lineType, 60, $layout);
    }
}
