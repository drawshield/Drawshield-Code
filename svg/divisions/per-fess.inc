<?php 

$lineType = getModifierByType($node, 'linetype', false);
if ( $lineType == 'nebuly' or $lineType == 'potenty' )
  $div_data = array ( 'CH' =>'X-100Y700A1016c508e1016g608' , 'NC' =>'X-100Y{%50}A1200c608e1200g608' );
else
  $div_data = array ( 'CH' =>'X-8Y700A1016c508e1016g508' , 'NC' =>'X-8Y{%50}A1016c708e1016g708' );

$division['div2'] = createBody(choose_fit($div_data), $lineType, 60);

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  for ($i = 5; $i < 1000; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',465)">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . $i . ',515)">' . $rivet . '</g>';
  }
  $division['divx'] = $rivetList;
}



