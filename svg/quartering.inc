<?php
require_once('size.inc');

class Quarter
{
    function __construct($x, $y, $size, $scale, $rel_size)
    {
        $this->x = $x;
        $this->y = $y;
        $this->size = $size;
        $this->scale = $scale;
        $this->rel_size = $rel_size;
    }

    function svg_transform()
    {
        return "transform='translate({$this->x}, {$this->y}) scale({$this->scale})'";
    }
}

class Quartering
{
    public $size, $quarters;
    private $y;

    function __construct()
    {
        $this->size = ShieldSize::current();
        $this->y = 0;
        $this->quarters = [];
    }

    function add_row($n_columns, $height_mult = null)
    {
        if ( $height_mult === null )
            $height_mult = 1 - $this->y;

        $qsize = $this->size->scaled(1 / $n_columns, $height_mult);
        $norm_factor = $qsize->normalize_factor();
        $qsize_norm = $qsize->scaled($norm_factor);

        for ( $i = 0; $i < $n_columns; $i++ )
        {
            array_push($this->quarters, new Quarter(
                $i * $qsize->width,
                $this->y * $this->size->height,
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
            if ( $q->x > 0 )
                $grid .= "<path d='M {$q->x} {$q->y} v {$q->rel_size->height}' />";
            if ( $q->y > 0 )
                $grid .= "<path d='M {$q->x} {$q->y} h {$q->rel_size->width}' />";
        }
        $grid .= "</g>\n";
        return $grid;
    }


    function render_svg(DOMNodeList $children, $clips = null)
    {
        global $options;

        $retval = '';
        $saveShape = $options['shape'];
        $saveHeight = $options['flagHeight'] ?? null;

        $options['shape'] = 'quarter';

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

            if ( $saveShape == "flag" )
                $aspect_ratio = "10:10";
            else
                $aspect_ratio = $quarter->size->closest_aspect_ratio();

            $options['flagHeight'] = $height;

            $transform = $quarter->svg_transform();


            if ( $clips !== null )
            {
                $clipdef = add_def( 'clipPath', "<path d=\"{$clips[$index]}\" />");
                $retval .=
                    "<g clip-path='url(#$clipdef)'><g $transform >"
                    . makeshield($kid, $aspect_ratio)
                    . "</g></g>\n";
            }
            else
            {
                $clipdef = add_def( 'clipPath', "<path d=\"M0,0 h$width v$height h-$width v-$height Z\" />");
                $retval .=
                    "<g clip-path='url(#$clipdef)' $transform >"
                    . makeshield($kid, $aspect_ratio)
                    . "</g>\n";
            }

            region('pop');
        }

        $options['shape'] = $saveShape;
        if ( $saveHeight )
            $options['flagHeight'] = $saveHeight;
        $retval .= add_def();
        return $retval . ($clips !== null ? "" : $this->grid());
    }

}

function makeQuartered($across, $down, $children, $this_AR)
{
    $quartering = new Quartering();

    $h = 1 / $down;
    for ( $y = 0; $y < $down; $y++ )
        $quartering->add_row($across, $h);

    return $quartering->render_svg($children);
}

function makequartered4( DOMNodeList $children, $this_AR )
{
    global $options;

    $quartering = new Quartering();

    if ( $options["shape"] == "flag" || $this_AR != "10:12" )
    {
        $quartering->add_row(2, 0.5);
        $quartering->add_row(2);
    }
    else
    {
        $quartering->add_row(2, 0.417);
        $quartering->add_row(2);
    }

    return $quartering->render_svg($children);
}

function makequartered5( $children, $this_AR )
{
    global $options;

    $quartering = new Quartering();

    if ( $options["shape"] == "flag" || $this_AR != "10:12" )
    {
        $quartering->add_row(3, 0.5);
        $quartering->add_row(2);
    }
    else
    {
        $quartering->add_row(3, 0.417);
        $quartering->add_row(2);
    }

    return $quartering->render_svg($children);
}

function makequartered6( $children, $this_AR )
{
    global $options;

    $quartering = new Quartering();

    if ( $options["shape"] == "flag" || $this_AR != "10:12" )
    {
        $quartering->add_row(3, 0.5);
        $quartering->add_row(3);
    }
    else
    {
        $quartering->add_row(3, 0.417);
        $quartering->add_row(3);
    }

    return $quartering->render_svg($children);
}

function makequartered8( $children, $this_AR )
{
    global $options;

    $quartering = new Quartering();

    if ( $options["shape"] == "flag" || $this_AR != "10:12" )
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

function make_quartered_saltire( DOMNodeList $children, $this_AR )
{
    global $options;

    $quartering = new Quartering();

    if ( $options["shape"] == "flag" || $this_AR != "10:12" )
        $h = 0.5;
    else
        $h = 0.417;

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

    return $quartering->render_svg($children, $clips);
}
