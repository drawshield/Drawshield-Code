<?php 

if ( existModifierWithKeyterm($node, 'nowy')) { // special handling for some linetypes
  $ordinary['body'] = '<path d="m 350,-100 v 342.74 c -43.63,25.5 -80.8,62.09 -107.12,107.26 h -492.88 v 300 h 492.95 c 26.3,45.14 63.45,81.72 107.05,107.22 v 542.78 h 300 v -542.74 c 43.63,-25.5 80.8,-62.09 107.12,-107.26 h 492.88 v -300 h -492.95 c -26.3,-45.14 -63.45,-81.72 -107.05,-107.22 v -342.78 z" />';
} elseif ( existModifierWithKeyterm($node, 'quadrate')) { // special handling for some linetypes
  $ordinary['body'] = '<path d="m -243.73,411.73 v 199.97 h 415.45 v 200.02 h 200.01 v 528.58 h 199.97 v -528.58 h 200.01 v -200.01 h 484.59 v -199.97 h -484.59 v -200.01 h -200.01 v -415.45 h -199.97 v 415.45 h -200.01 v 200.01 z" />';
} else {
  $ordinary['shape_spec'] = array ( 'NC' => 'X-100Y350A450G450a300C450A450c300E450C650e300G650E450g300',
                                  'CH' => 'X400Y200a200C400A500c200E500C500e200G500E500g200A500G400' );
}

$ordinary['chargesOn'] = chiefPresent() ? '500,700,160,160////250,700,160,160:750,700:500,450:500,700:500,950/x' :
  '500,500,200,200//200,500,220,220:800,500:500,250/200,500,200,200:500,220:800,500:500,800//200,500,200,200:500,200:800,500:500,500:500,800:500,1050';

$ordinary['chargesWithin'] = chiefPresent() ? 'x/200,450,180,160:800,450/x/200,450,180,160:800,450:200,940:800:840/s/s' : 'x/175,750,180,300:825,750/x/175,200,180,160:825,200:175,840:825:840/s/s';

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  if ( chiefPresent() ) {
    for ($i = 10; $i < 1000; $i += 80) {
      if ($i < 400 or $i >= 600) {
        $rivetList .= '<g transform="translate(' . $i . ',615)">' . $rivet . '</g>';
        $rivetList .= '<g transform="translate(' . $i . ',765)">' . $rivet . '</g>';
      }
    }
    for ($i = 315; $i < 1200; $i += 80) {
      if ($i <= 600 or $i >= 790) {
        $rivetList .= '<g transform="translate(415,' . $i . ')">' . $rivet . '</g>';
        $rivetList .= '<g transform="translate(565,' . $i . ')">' . $rivet . '</g>';
      }
    }
  } else {
    for ($i = 10; $i < 1000; $i += 80) {
      if ($i < 350 or $i >= 650) {
        $rivetList .= '<g transform="translate(' . $i . ',365)">' . $rivet . '</g>';
        $rivetList .= '<g transform="translate(' . $i . ',615)">' . $rivet . '</g>';
      }
    }
    for ($i = 5; $i < 1200; $i += 80) {
      if ($i < 350 or $i >= 640) {
        $rivetList .= '<g transform="translate(365,' . $i . ')">' . $rivet . '</g>';
        $rivetList .= '<g transform="translate(615,' . $i . ')">' . $rivet . '</g>';
      }
    }
  }

  $ordinary['body3'] = $rivetList;
}
