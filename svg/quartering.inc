<?php
require_once('size.inc');

class Quarter
{
    function __construct($pos, $size, $scale, $rel_size)
    {
        $this->pos = $pos;
        $this->size = $size;
        $this->scale = $scale;
        $this->rel_size = $rel_size;
    }

    function svg_transform()
    {
        return "transform='translate({$this->pos->x}, {$this->pos->y}) scale({$this->scale})'";
    }

    function to_layout()
    {
        return new ShieldLayout($this->size, "quarter");
    }
}

class Quartering
{
    public $size, $quarters;
    private $y;

    function __construct($layout)
    {
        $this->layout = $layout;
        $this->y = 0;
        $this->quarters = [];
    }

    function add_row($n_columns, $height_mult = null)
    {
        if ( $height_mult === null )
            $height_mult = 1 - $this->y;

        $qsize = $this->layout->size->scaled(1 / $n_columns, $height_mult);
        $norm_factor = $qsize->normalize_factor();
        $qsize_norm = $qsize->scaled($norm_factor);

        for ( $i = 0; $i < $n_columns; $i++ )
        {
            array_push($this->quarters, new Quarter(
                new ShieldPos(
                    $i * $qsize->width,
                    $this->y * $this->layout->size->height
                ),
                $qsize_norm,
                1 / $norm_factor,
                $qsize
            ));
        }

        $this->y += $height_mult;

    }

    function grid()
    {
        $grid = "<g fill='none' stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\">\n";
        foreach ( $this->quarters as $q )
        {
            if ( $q->pos->x > 0 )
                $grid .= "<path d='M {$q->pos->x} {$q->pos->y} v {$q->rel_size->height}' />";
            if ( $q->pos->y > 0 )
                $grid .= "<path d='M {$q->pos->x} {$q->pos->y} h {$q->rel_size->width}' />";
        }
        $grid .= "</g>\n";
        return $grid;
    }


    function render_svg(DOMNodeList $children, $clips = null)
    {
        global $options;

        $retval = '';

        foreach ( $children as $kid )
        {
            $index = $kid->getAttribute('index');
            $index -= 1; // Arrays are zero based

            // error check
            if (!isset($this->quarters[$index]))
            {
                error_log("bad quarter index in " . $options['blazon'] );
                continue;
            }

            region('push','Q' . $index ); // Can lose this soon I hope...

            $quarter = $this->quarters[$index];
            $width = $quarter->size->width;
            $height = $quarter->size->height;

            $transform = $quarter->svg_transform();
            $layout = $quarter->to_layout();

            if ( $clips !== null )
            {
                $clipdef = add_def( 'clipPath', "<path d=\"{$clips[$index]}\" />");
                $retval .=
                    "<g clip-path='url(#$clipdef)'><g $transform >"
                    . makeshield($kid, $layout)
                    . "</g></g>\n";
            }
            else
            {
                $clipdef = add_def( 'clipPath', "<path d=\"M0,0 h$width v$height h-$width v-$height Z\" />");
                $retval .=
                    "<g clip-path='url(#$clipdef)' $transform >"
                    . makeshield($kid, $layout)
                    . "</g>\n";
            }

            region('pop');
        }

        $retval .= add_def();
        return $retval . ($clips !== null ? "" : $this->grid());
    }

}

function makeQuartered($across, $down, $children, ShieldLayout $layout)
{
    $quartering = new Quartering($layout);

    $h = 1 / $down;
    for ( $y = 0; $y < $down; $y++ )
        $quartering->add_row($across, $h);

    return $quartering->render_svg($children);
}

function makequartered4( DOMNodeList $children, ShieldLayout $layout )
{
    $quartering = new Quartering($layout);

    $h = $layout->center_point_ratio()->y;
    $quartering->add_row(2, $h);
    $quartering->add_row(2);

    return $quartering->render_svg($children);
}

function makequartered5( $children, ShieldLayout $layout )
{
    $quartering = new Quartering($layout);

    $h = $layout->center_point_ratio()->y;
    $quartering->add_row(3, $h);
    $quartering->add_row(2);

    return $quartering->render_svg($children);
}

function makequartered6( $children, ShieldLayout $layout )
{
    $quartering = new Quartering($layout);

    $h = $layout->center_point_ratio()->y;
    $quartering->add_row(3, $h);
    $quartering->add_row(3);

    return $quartering->render_svg($children);
}

function makequartered8( $children, ShieldLayout $layout )
{
    $quartering = new Quartering($layout);

    if ( $layout->shape == "flag" || $layout->size->height != 1200 )
    {
        $quartering->add_row(3, 1/3);
        $quartering->add_row(3, 1/3);
        $quartering->add_row(2);
    }
    else
    {
        $quartering->add_row(3, 0.3);
        $quartering->add_row(3, 0.3);
        $quartering->add_row(2);
    }

    return $quartering->render_svg($children);
}

function make_quartered_saltire( DOMNodeList $children, ShieldLayout $layout )
{
    $quartering = new Quartering($layout);

    $h = $layout->center_point_ratio()->y;

    $quartering->quarters[] = new Quarter(
        0,
        0,
        $quartering->size->scaled(1, $h),
        1,
        $quartering->size->scaled(1, $h)
    );

    $side_size = $quartering->size->scaled(0.5, $h * 2);
    $norm_factor = $side_size->normalize_factor();
    $side_size_norm = $side_size->scaled($norm_factor);

    $quartering->quarters[] = new Quarter(
        0,
        0,
        $side_size_norm,
        1/$norm_factor,
        $side_size
    );
    $quartering->quarters[] = new Quarter(
        $quartering->size->width / 2,
        0,
        $side_size_norm,
        1/$norm_factor,
        $side_size
    );

    $quartering->quarters[] = new Quarter(
        0,
        $h * $quartering->size->height,
        $quartering->size->scaled(1, 1-$h),
        1,
        $quartering->size->scaled(1, 1-$h)
    );

    $center_x = $quartering->size->width / 2;
    $center_y = $quartering->size->height * $h;
    $bottom_s = $quartering->size->height * $h * 2;
    $bottom = $quartering->size->height;
    $right = $quartering->size->width;
    $clips = [
        "M 0 0 L $center_x $center_y $right 0 z",
        "M 0 0 L $center_x $center_y 0 $bottom_s z",
        "M $right 0 L $center_x $center_y $right $bottom_s z",
        "M $center_x $center_y L $right $bottom_s $right $bottom 0 $bottom 0 $bottom_s z",
    ];

    $grid = "<g fill='none' stroke-width=\"3\" stroke=\"" . rgb('marshalling-stroke') . "\">\n";
    $grid .= "<path d='M 0 0 L $right $bottom_s M $right 0 L 0 $bottom_s'/>\n";
    $grid .= "</g>";
    return $quartering->render_svg($children, $clips) . $grid;
}
