<?php

function build_bend($layout, $width, $margin, $offset = 0, $path_builder = null)
{
    $base = min($layout->center_point->x, $layout->center_point->y) - $margin;
    $length = $base * M_SQRT2 * 2;
    $offset_comp = $offset / M_SQRT2;

    if ( $path_builder == null )
        $path_builder = new PathBuilder();

    return $path_builder
        ->move($layout->center_point->x - $base + $offset_comp, $layout->center_point->y - $base - $offset_comp)
        ->up_right_plain($width/2)
        ->down_right($length)
        ->down_left_plain($width)
        ->up_left($length)
        ->up_right_plain($width/2)
    ;
}


function placement_in_bend($boundingBox, $sinister, $angle)
{
    // ensure we are working with a square
    $min_extent = min($boundingBox->size->width, $boundingBox->size->height);
    $mult = $sinister ? 1 : -1;
    if ( $boundingBox->size->width > $boundingBox->size->height )
        $boundingBox->pos->x += $mult * ($boundingBox->size->width - $min_extent) / 2;
    else
        $boundingBox->pos->y -= ($boundingBox->size->height - $min_extent) / 2;

    $boundingBox->size->width = $boundingBox->size->height = $min_extent;

    $placement_arr = [];
    for ( $n = 1; $n <= 8; $n++ )
    {
        $plac = [];
        $size = 1 / $n * 0.8;
        $start = 0.1 + $size / 2;
        for ( $i = 0; $i < $n; $i++ )
        {
            $pos = $start + $size * $i;
            $plac[] = box_placement(
                $boundingBox->relative_box(
                    $sinister ? 1 - $pos : $pos,
                    $pos,
                    .23,
                    .23
                ),
                $angle
            );
        }

        $placement_arr[] = $plac;

    }
    return array_to_placement($placement_arr);
}
