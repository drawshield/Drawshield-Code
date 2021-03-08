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

    function add_uniform_rows($n_rows, $n_columns)
    {
        $h = 1 / $n_rows;
        for ( $y = 0; $y < $n_rows; $y++ )
            $this->add_row($n_columns, $h);
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


    function render_svg( DOMNodeList $children, $this_AR)
    {
        global $options;

        $retval = '';
        $saveShape = $options['shape'];
        $saveHeight = $options['flagHeight'] ?? null;

        if ( $options["shape"] == "flag" )
            $this_AR = "10:10";

        $options['shape'] = 'quarter';

        foreach ( $children as $kid ) {
            $index = $kid->getAttribute('index');
            $index -= 1; // Arrays are zero based

            // error check
            if (!isset($this->quarters[$index])) {
                error_log("bad quarter index in " . $options['blazon'] );
                continue;
            }

            region('push','Q' . $index ); // Can lose this soon I hope...

            $quarter = $this->quarters[$index];
            $width = $quarter->size->width;
            $height = $quarter->size->height;
            $clipdef = add_def( 'clipPath', "<path d=\"M0,0 h$width v$height h-$width v-$height Z\" />");

            $options['flagHeight'] = $height;

            $transform = $quarter->svg_transform();
            $retval .=
                "<g clip-path='url(#$clipdef)' $transform >"
                . makeshield( $kid, $this_AR )
                . "</g>\n";
            region('pop');
        }

        $options['shape'] = $saveShape;
        if ( $saveHeight )
            $options['flagHeight'] = $saveHeight;
        $retval .= add_def();
        return $retval . $this->grid();
    }

}

function makeQuartered($across, $down, $children, $this_AR)
{
    $quartering = new Quartering();

    $h = 1 / $down;
    for ( $y = 0; $y < $down; $y++ )
        $quartering->add_row($across, $h);

    return $quartering->render_svg($children, $this_AR);
}

function makequartered4( DOMNodeList $children, $this_AR )
{
    global $options;

    $quartering = new Quartering();

    if ( $options["shape"] == "flag" || $this_AR != "10:12" )
    {
        $quartering->add_uniform_rows(2, 2);
    }
    else
    {
        $quartering->add_row(2, 0.417);
        $quartering->add_row(2);
    }

    return $quartering->render_svg($children, $this_AR);
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

    return $quartering->render_svg($children, $this_AR);
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

    return $quartering->render_svg($children, $this_AR);
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

    return $quartering->render_svg($children, $this_AR);
}
