<?php


function build_chevron(ShieldLayout $layout, $width, $offset, PathBuilder $builder, $y_align = 0)
{
    $extent = max($layout->size->width, $layout->size->height);

    return $builder
        ->move($layout->center_point->x, $layout->center_point->y + $offset + $y_align * $width * M_SQRT2)
        ->chevron()
        ->down_left($extent)
        ->up_left_plain($width)
        ->up_right($extent + $width)
        ->down_right($extent + $width)
        ->down_left_plain($width)
        ->up_left($extent);
}


function chevron_width(ShieldLayout $layout, $number)
{
    return min(200, $layout->center_point->y / ($number+0.5));
}
