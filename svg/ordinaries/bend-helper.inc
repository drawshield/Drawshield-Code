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

function bend_within($layout, $width)
{

    $cx = $layout->center_point->x;
    $cy = $layout->center_point->y;
    $lw = $layout->size->width;
    $lh = min($layout->size->height, $layout->size->width);
    $width_delta = $width / 2 / M_SQRT2 + 10;
    $within_margin = 50;
    $within_2 = [
        ShieldRect::from_corners($cx + $width_delta, $within_margin, $lw - $within_margin, $cy - $width_delta),
        ShieldRect::from_corners($within_margin, $cy + $width_delta, $cx - $width_delta, $lh - $within_margin)
    ];
    $within_4 = [
        ShieldRect::from_corners($cx, $within_margin, $within_2[0]->left() + $within_2[0]->size->width / 3, $within_2[0]->bottom() - $within_2[0]->size->height / 3),
        ShieldRect::from_corners($within_2[0]->left() + $within_2[0]->size->width / 3, $within_2[0]->bottom() - $within_2[0]->size->height / 3, $lw - $within_margin, $cy),

        ShieldRect::from_corners($within_margin, $cy, $within_2[1]->right() - $within_2[1]->size->width / 3, $within_2[1]->top() + $within_2[1]->size->height / 3),
        ShieldRect::from_corners($within_2[1]->right() - $within_2[1]->size->width / 3, $within_2[1]->top() + $within_2[1]->size->height / 3, $cx, $lh - $within_margin),
    ];

    return array_to_placement([
        "",
        $within_2,
        [
            $within_2[0],
            $within_4[2],
            $within_4[3],
        ],
        $within_4,
        "",
        [
            new ShieldRect($within_4[0]->pos, $within_4[0]->size->scaled(0.8)),
            new ShieldRect($within_4[1]->pos, $within_4[1]->size->scaled(0.8)),
            new ShieldRect(new ShieldPos($within_4[1]->pos->x, $within_4[0]->pos->y), $within_4[0]->size->scaled(0.8)),

            new ShieldRect($within_4[2]->pos, $within_4[2]->size->scaled(0.8)),
            new ShieldRect($within_4[3]->pos, $within_4[3]->size->scaled(0.8)),
            new ShieldRect(new ShieldPos($within_4[2]->pos->x, $within_4[3]->pos->y), $within_4[0]->size->scaled(0.8)),
        ]

    ]);
}
