<?php 

$fillet = $node && $node->getAttribute('keyterm') == "fillet-cross";


$width = 200;

if ( $fillet )
{
    $width = 100;
    $ordinary['feature_size'] = 15;
    $ordinary['chargesWithin'] = 'x/250,750,180,180:750,750/x/250,250,180,180:750,250:250,750:750,750/x/x';
}
else
{
    $ordinary['chargesWithin'] = 'x/175,750,180,300:825,750/x/175,200,180,160:825,200:175,840:825:840/s/s';
}

$cx = $layout->center_point->x;
$cy = $layout->center_point->y;
$margin = 100;
$arm_l = $cx - $width / 2 + $margin;
$arm_r = ($layout->size->width - $cx) - $width / 2 + $margin;
$arm_u = $cy - $width / 2 + $margin;
$arm_d = ($layout->size->height - $cy) - $width / 2 + $margin;


if ( !$fillet && existModifierWithKeyterm($node, 'nowy')) { // special handling for some linetypes
  $ordinary['body'] = '<path d="m 350,-100 v 342.74 c -43.63,25.5 -80.8,62.09 -107.12,107.26 h -492.88 v 300 h 492.95 c 26.3,45.14 63.45,81.72 107.05,107.22 v 542.78 h 300 v -542.74 c 43.63,-25.5 80.8,-62.09 107.12,-107.26 h 492.88 v -300 h -492.95 c -26.3,-45.14 -63.45,-81.72 -107.05,-107.22 v -342.78 z" />';
} elseif ( !$fillet && existModifierWithKeyterm($node, 'quadrate')) { // special handling for some linetypes
  $ordinary['body'] = '<path d="m -243.73,411.73 v 199.97 h 415.45 v 200.02 h 200.01 v 528.58 h 199.97 v -528.58 h 200.01 v -200.01 h 484.59 v -199.97 h -484.59 v -200.01 h -200.01 v -415.45 h -199.97 v 415.45 h -200.01 v 200.01 z" />';
} else {

    $ordinary['shape_spec'] = (string)path_builder(-$margin, $cy-$width/2)
                ->right($arm_l)
                ->up($arm_u)
                ->right_plain($width)
                ->down($arm_u)
                ->right($arm_r)
                ->down_plain($width)
                ->left($arm_r)
                ->down($arm_d)
                ->left_plain($width)
                ->up($arm_d)
                ->left($arm_l)
                ->up_plain($width)
    ;
}

if ( !$fillet )
{
    $on_extent = 200;
    $on_size = new ShieldSize($on_extent, $on_extent);
    $on_center = new ShieldRect(clone $layout->center_point, $on_size);

    $dx = ($layout->center_point->x + 100) / 2;
    $dy = ($layout->center_point->y + 100) / 2;

    $ordinary['chargesOn'] = array_to_placement([
        [$on_center],
        "",
        "",
        "",
        "",
        [
            $on_center->translated(-$dx, 0),
            $on_center->translated(0, -$dy),
            $on_center->translated($dx, 0),
            $on_center->translated(0, $dy),
            $on_center,
            $on_center->translated(0, $dy*2),
        ]
    ]);
}

$within_margin = 50;
$within_ll = $within_margin;
$within_lr = $layout->center_point->x - $width/2 - $within_margin;
$within_rl = $layout->center_point->x + $width/2 + $within_margin;
$within_rr = $layout->size->width - $within_margin;
$within_tt = $within_margin;
$within_tb = $layout->center_point->y - $width/2 - $within_margin;
$within_bt = $layout->center_point->y + $width/2 + $within_margin;
$within_bb = $within_bt + $within_tb - $within_tt;

$ordinary['chargesWithin'] = array_to_placement([
    [],
    [
        ShieldRect::from_corners($within_ll, $within_bt, $within_lr, $within_bb),
        ShieldRect::from_corners($within_rl, $within_bt, $within_rr, $within_bb),
    ],
    [],
    [
        ShieldRect::from_corners($within_ll, $within_bt, $within_lr, $within_bb),
        ShieldRect::from_corners($within_rl, $within_bt, $within_rr, $within_bb),

        ShieldRect::from_corners($within_ll, $within_tt, $within_lr, $within_tb),
        ShieldRect::from_corners($within_rl, $within_tt, $within_rr, $within_tb),
    ],
    [],
    [],
]);


if ( existModifierWithKeyterm($node, 'riveted')) {
    include '_rivet.inc';
    $rivetList = '';
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

    $ordinary['body3'] = $rivetList;
}
