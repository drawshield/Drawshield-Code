<?php


$num_of = max((int)getModifierValueByKeyterm($node, 'points', true), 1);


$shape = new MutableShape();

$height = $layout->size->height / 3;
$width = $layout->size->width / max(1, $num_of-3);
$dx = $layout->size->width / max(1, $num_of-1);
$tip = $layout->size->height - $height;
$handle = $width/3;

$paths_arr = [];

if ( $num_of == 1 )
{
    $start = $layout->size->width / 2;
}
else if ( $num_of % 2 )
{
    $start = 0;
}
else
{
    $start = $width/4;
    $dx = ($layout->size->width-$width/2) / max(1, $num_of-1);;
}

for ( $i = 0; $i < $num_of / 2; $i++ )
{
    $path = new MutableShape();
    $tip_pos = new ShieldPos($start + $i * $dx, $tip);
    $path->move_to($tip_pos->x - $width / 2, $layout->size->height)
        ->quadratic_to(
            $tip_pos->translated(-$handle, 0),
            $tip_pos
        )
        ->quadratic_to(
            $tip_pos->translated($handle, 0),
            new ShieldPos($tip_pos->x + $width / 2, $layout->size->height)
        )
    ;

    $mirror_trans = new ShieldPos($layout->size->width - 2 * $tip_pos->x, 0);


    if ( $i != floor(($num_of - 0.5) / 2) )
    {
        $paths_arr[] = $path->to_svg_path();
        $path->translate($mirror_trans);
        $paths_arr[] = $path->to_svg_path();
    }
    else if ( $num_of % 2 )
    {
        $paths_arr[] = $path->to_svg_path();
    }
    else
    {
        $points = $path->last_path()->points();
        $arch_back = new CubicBezier($points[1]->pos, $points[1]->tan_out, $points[2]->tan_in, $points[2]->pos);
        $t = $arch_back->line_single_intersection_t(
            new ShieldPos($layout->size->width / 2, 0),
            new ShieldPos($layout->size->width / 2, $layout->size->height)
        );
        $bez = $arch_back->split($t)[0];
        $path->last_path()->points()[2]->pos = $bez->finish;
        $path->last_path()->points()[2]->tan_in = $bez->tan2;
        $path->last_path()->points()[1]->tan_out = $bez->tan1;
        $bez = $bez->translated(-$bez->finish->x, 0);
        $path->cubic_to(
                $layout->size->width / 2 - $bez->tan2->x, $bez->tan2->y,
                $layout->size->width / 2 - $bez->tan1->x, $bez->tan1->y,
                $layout->size->width / 2 - $bez->start->x, $bez->start->y
            )
            ->quadratic_to(
                $tip_pos->translated($dx+$handle, 0),
                $tip_pos->translated($dx+$width/2, $height)
            )
        ;
        $paths_arr[] = $path->to_svg_path();

    }
}

$paths = implode("", $paths_arr);

$ordinary['body'] = "<g style='stroke:#808080;stroke-width:3'>$paths</g>";
