<?php

require_once("size.inc");


/**
 * @brief A cubic bezier with precompiled lengths
 * @note This should be treated as a read-only object
 */
class CubicBezier
{
    /// @var ShieldPos
    public $start;
    /// @var ShieldPos
    public $tan1;
    /// @var ShieldPos
    public $tan2;
    /// @var ShieldPos
    public $finish;

    public $length;
    public $linear;
    public $length_steps = [];


    function __construct(ShieldPos $start, ShieldPos $tan1, ShieldPos $tan2, ShieldPos $finish)
    {
        $this->start = $start;
        $this->tan1 = $tan1;
        $this->tan2 = $tan2;
        $this->finish = $finish;

        $this->linear = $tan1->x == $start->x && $tan1->y == $start->y && $tan2->x == $finish->x && $tan2->y == $finish->y;
        if ( $this->linear )
        {
            $this->length = $start->distance($finish);
        }
        else
        {
            $this->length = 0;
            $p0 = $this->start;
            $resolution = 10;
            for ( $i = 1; $i <= $resolution; $i++ )
            {
                $t = $i/$resolution;
                $p1 = $this->point($t);
                $dist = $p0->distance($p1);
                $this->length_steps["$t"] = $dist;
                $this->length += $dist;
                $p0 = $p1;
            }

        }
    }

    /**
     * @brief Bezier point at @p t [0,1]
     */
    function point($t)
    {
        return self::bezier_solve($t, $this->start, $this->tan1, $this->tan2, $this->finish);
    }

    // Cubic bezier solver coefficients
    private static function bezier_a($k0, $k1, $k2, $k3)
    {
        return -$k0 + $k1*3 + $k2 * -3 + $k3;
    }
    private static function bezier_b($k0, $k1, $k2)
    {
        return $k0 * 3 + $k1 * -6 + $k2 * 3;
    }
    private static function bezier_c($k0, $k1)
    {
        return $k0 * -3 + $k1 * 3;
    }
    private static function bezier_d($k0)
    {
        return $k0;
    }
    static function bezier_component($t, $k0, $k1, $k2, $k3)
    {
        return ((self::bezier_a($k0, $k1, $k2, $k3) * $t + self::bezier_b($k0, $k1, $k2) ) * $t +  self::bezier_c($k0, $k1) ) * $t + self::bezier_d($k0);
    }

    static function bezier_solve($t, ShieldPos $p1, ShieldPos $p2, ShieldPos $p3, ShieldPos $p4)
    {
        return new ShieldPos(
            self::bezier_component($t, $p1->x, $p2->x, $p3->x, $p4->x),
            self::bezier_component($t, $p1->y, $p2->y, $p3->y, $p4->y)
        );
    }

    /**
     * @brief Point along the bezier at a given distance from the start
     * @param $length in [0, $this->length]
     */
    function point_at_length($length)
    {
        if ( $length <= 0 )
            return clone $this->start;

        if ( $length >= $this->length )
            return clone $this->finish;

        if ( $this->linear )
            return $this->start->lerp($this->finish, $length / $this->length);

        return $this->point($this->t_at_length($length));
    }

    /**
     * @brief Approximates a @p t from length for non-linear segments
     */
    private function t_at_length($length)
    {
        $t0 = 0;
        foreach ( $this->length_steps as $t1 => $d1 )
        {
            $t1 = (float)$t1;

            if ( $d1 == $length )
                return $t1;

            if ( $d1 > $length )
                return $t0 + ($t1 - $t0) * $length / $d1;

            $t0 = $t1;
            $length -= $d1;
        }

        return 1;
    }
}


/**
 * @brief PolyBezier point
 */
class BezierPoint
{
    /// @var ShieldPos
    public $pos;
    /// @var ShieldPos
    public $tan_in;
    /// @var ShieldPos
    public $tan_out;

    function __construct(ShieldPos $pos, ShieldPos $tan_in = null, ShieldPos $tan_out = null)
    {
        $this->pos = $pos;
        $this->tan_in = $tan_in ?? clone $pos;
        $this->tan_out = $tan_out ?? clone $pos;
    }

    function translate(ShieldPos $p)
    {
        $this->pos = $this->pos->translated($p->x, $p->y);
        $this->tan_in = $this->tan_in->translated($p->x, $p->y);
        $this->tan_out = $this->tan_out->translated($p->x, $p->y);
    }

    function scale($x, $y)
    {
        $this->pos = new ShieldPos($this->pos->x * $x, $this->pos->y * $y);
        $this->tan_in = new ShieldPos($this->tan_in->x * $x, $this->tan_in->y * $y);
        $this->tan_out = new ShieldPos($this->tan_out->x * $x, $this->tan_out->y * $y);
    }

    function __clone()
    {
        $this->pos = clone $this->pos;
        $this->tan_in = clone $this->tan_in;
        $this->tan_out = clone $this->tan_out;
    }

    function reverse()
    {
        list($this->tan_in, $this->tan_out) = [$this->tan_out, $this->tan_in];
    }
}

/**
 * @brief A continuous sequence of bezier data, which might be closed to loop along itself
 */
class PolyBezier
{
    public $points = [];
    public $closed = false;
    public $segment_tags = [];

    function start() : BezierPoint
    {
        return $this->points[0] ?? null;
    }

    function finish() : BezierPoint
    {
        return $this->points[count($this->points)-1] ?? null;
    }

    function add_point($point)
    {
        if ( $point instanceof BezierPoint )
            $this->points[] = $point;
        else
            $this->points[] = new BezierPoint($point);
        return $this;
    }

    function empty()
    {
        return count($this->points) == 0;
    }

    function size()
    {
        return count($this->points);
    }

    function line_to(ShieldPos $p)
    {
        if ( !$this->empty() )
            $this->finish()->tan_out = clone $this->finish()->pos;

        $this->points[] = new BezierPoint($p);
        return $this;
    }

    function quadratic_to(ShieldPos $handle, ShieldPos $dest)
    {
        if ( !$this->empty() )
            $this->finish()->tan_out = $this->finish()->pos->lerp($handle, 2/3);

        $this->points[] = new BezierPoint($dest, $dest->lerp($handle, 2/3));
        return $this;
    }

    function cubic_to(ShieldPos $handle1, ShieldPos $handle2, ShieldPos $dest)
    {
        if ( !$this->empty() )
            $this->finish()->tan_out = $handle1;

        $this->points[] = new BezierPoint($dest, $handle2);
        return $this;
    }

    function close()
    {
        if ( $this->size() > 1 )
        {
            if ( $this->start()->pos->distance($this->finish()->pos) < 1 )
            {
                $this->start()->tan_in = $this->finish()->tan_out;
                $this->finish()->pos = $this->start()->pos;
            }
            else
            {
                $this->start()->tan_in = $this->start()->pos;
                $this->finish()->tan_out = $this->start()->pos;
            }
        }
        $this->closed = true;
        return $this;
    }

    function to_svg_d()
    {
        if ( $this->empty() )
            return "";


        $p0 = $this->points[0];
        $d = "M {$p0->pos->x} {$p0->pos->y} C";
        for ( $i = 1; $i < count($this->points); $i++ )
        {
            $p1 = $this->points[$i];
            $d .= " {$p0->tan_out->x},{$p0->tan_out->y} {$p1->tan_in->x},{$p1->tan_in->y} {$p1->pos->x},{$p1->pos->y}";
            $p0 = $p1;
        }

        if ( $this->closed )
            $d .= " Z";

        return $d;
    }

    function tag_segment($flags)
    {
        $this->segment_tags[$this->size()] = $flags;
        return $this;
    }

    function compile()
    {
        $compiled = new CompiledPolyBezier($this->closed);
        if ( $this->size() > 1 )
        {
            $flags = $this->segment_tags[0] ?? BezierSegmentFlags::NORMAL;
            $cubics = [];

            $p0 = $this->points[0];
            for ( $i = 1; $i < $this->size(); $i++ )
            {
                $p1 = $this->points[$i];
                $cubics[] = new CubicBezier($p0->pos, $p0->tan_out, $p1->tan_in, $p1->pos);
                $p0 = $p1;


                if ( isset($this->segment_tags[$i]) )
                {
                    if ( count($cubics) )
                        $compiled->add_segment(new CompiledBezierSegment($cubics, $flags));
                    $cubics = [];
                    $flags = $this->segment_tags[$i];
                }
            }

            if ( $this->closed )
            {
                $p1 = $this->points[0];
                $cubics[] = new CubicBezier($p0->pos, $p0->tan_out, $p1->tan_in, $p1->pos);
            }

            if ( count($cubics) )
                $compiled->add_segment(new CompiledBezierSegment($cubics, $flags));
        }
        return $compiled;
    }

    function translate(ShieldPos $p)
    {
        foreach ( $this->points as $point )
            $point->translate($p);
    }

    function scale($x, $y = null)
    {
        if ( $y == null )
            $y = $x;
        foreach ( $this->points as $point )
            $point->scale($x, $y);
    }

    function reverse()
    {
        foreach ( $this->points as $point )
            $point->reverse();
        $this->points = array_reverse($this->points);
    }

    function __clone()
    {
        $cloned = [];
        foreach ( $this->points as $p )
            $cloned[] = clone $p;
        $this->points = $cloned;
    }

    private function closest_get_vt($closest_index, ShieldPos $current, $round_distance, $round_corner)
    {
        if ( $closest_index < 0 )
            $closest_index += count($this->points);
        $closer_v = $this->points[$closest_index]->pos;
        $distance = $current->distance($closer_v);
        $new_pos_perc = $distance != 0 ? min($distance/2, $round_distance) / $distance : 0;
        $vert = $current->lerp($closer_v, $new_pos_perc);
        $tan_x = - ($vert->x - $current->x) * $round_corner;
        $tan_y = - ($vert->y - $current->y) * $round_corner;
        return [$vert, new ShieldPos($tan_x + $vert->x, $tan_y + $vert->y)];
    }

    function rounded($round_distance)
    {
        $cloned = new PolyBezier();
        $cloned->closed = $this->closed;
        $round_corner = 0.5519;

        for ( $i = 0; $i < $this->size(); $i++ )
        {
            $current = $this->points[$i]->pos;
            if ( !$this->closed  && ($i == 0 || $i == count($this->points) - 1) )
            {
                $cloned->add_point($this->points[$i]);
            }
            else
            {
                list($vert1, $out_t) = $this->closest_get_vt($i - 1, $current, $round_distance, $round_corner);
                $cloned->add_point(new BezierPoint($vert1, $vert1, $out_t));
                list($vert2, $in_t) = $this->closest_get_vt(($i+1) % count($this->points), $current, $round_distance, $round_corner);
                $cloned->add_point(new BezierPoint($vert2, $in_t, $vert2));
            }
        }

        return $cloned;
    }
}

/**
 * @brief A bezier shape, which might contain multiple disconnected beziers
 */
abstract class ShapeInterface
{
    /**
     * @brief Returns the sub-paths
     */
    abstract function sub_paths();

    /**
     * @brief Converts to the 'd' attribute for a svg path element
     */
    function to_svg_d()
    {
        $d = "";
        foreach ( $this->sub_paths() as $bez )
            $d .= $bez->to_svg_d();
        return $d;
    }

    /**
     * @brief Whether the shape has no sub paths
     */
    function empty()
    {
        return count($this->sub_paths()) == 0;
    }

    /**
     * \brief Number of sub-paths
     */
    function size()
    {
        return count($this->sub_paths());
    }
}

/**
 * @brief A shape made up of PolyBezier objects
 */
class MutableShape extends ShapeInterface
{
    private $sub_paths = [];

    function sub_paths()
    {
        return $this->sub_paths;
    }

    private function last_bez() : PolyBezier
    {
        return $this->sub_paths[count($this->sub_paths)-1];
    }

    private function finish() : BezierPoint
    {
        return $this->last_bez()->finish();
    }

    private function has_finish()
    {
        return !$this->empty() && !$this->last_bez()->empty();
    }

    function move_to(ShieldPos $p)
    {
        if ( !$this->empty() && $this->last_bez()->size() < 2 )
        {
            $this->last_bez()->points = [new BezierPoint($p)];
        }
        else
        {
            $new_bez = new PolyBezier();
            $new_bez->add_point($p);
            $this->sub_paths[] = $new_bez;
        }
        return $this;
    }

    function line_to(ShieldPos $p)
    {
        $this->last_bez()->line_to($p);
        return $this;
    }

    function quadratic_to(ShieldPos $handle, ShieldPos $dest)
    {
        $this->last_bez()->quadratic_to($handle, $dest);
        return $this;
    }

    function cubic_to(ShieldPos $handle1, ShieldPos $handle2, ShieldPos $dest)
    {
        $this->last_bez()->cubic_to($handle1, $handle2, $dest);
        return $this;
    }

    function close()
    {
        $new_bez = new PolyBezier();
        if ( !$this->empty() )
        {
            $this->last_bez()->close();
            if ( !$this->last_bez()->empty() )
                $new_bez->add_point($this->last_bez()->finish());
        }
        $this->sub_paths[] = $new_bez;
        return $this;
    }

    function clean()
    {
        $cleaned = [];
        foreach ( $this->sub_paths as $bez )
        {
            if ( $bez->size() > 1 )
                $cleaned[] = $bez;
        }

        $this->sub_paths = $cleaned;

        return $this;
    }

    function compile()
    {
        $compiled = [];
        foreach ( $this->sub_paths as $bez )
        {
            if ( $bez->size() > 1 )
                $compiled[] = $bez->compile();
        }

        return new CompiledShape($compiled);
    }

    function add_path(PolyBezier $path)
    {
        $this->sub_paths[] = $path;
    }

    function combine_from(MutableShape $other)
    {
        foreach ( $other->sub_paths as $bez )
            $this->add_path(clone $bez);
    }

    function translate(ShieldPos $p)
    {
        foreach ( $this->sub_paths as $path )
            $path->translate($p);
    }

    function scale($x, $y = null)
    {
        if ( $y == null )
            $y = $x;
        foreach ( $this->sub_paths as $path )
            $path->scale($x, $y);
    }

    function reverse()
    {
        foreach ( $this->sub_paths as $path )
            $path->reverse();
    }

    function __clone()
    {
        $cloned = [];
        foreach ( $this->sub_paths as $p )
            $cloned[] = clone $p;
        $this->sub_paths = $cloned;
    }

    function tag_segment($flags)
    {
        if ( $this->empty() )
            $this->sub_paths[] = new PolyBezier();
        $this->last_bez()->tag_segment($flags);
        return $this;
    }

    function rounded($round_distance)
    {
        $clone = new MutableShape();
        foreach ( $this->sub_paths as $bez )
            $clone->add_path($bez->rounded($round_distance));
        return $clone;
    }

}

/**
 * @brief Minimal parser for the SVG path d attribute
 */
class SvgDParser
{
    private $tokens, $tokpos, $letter, $relative, $relpoint;
    public $bezier;

    static function parse_d($d) : MutableShape
    {
        return (new SvgDParser())->on_parse_d($d);
    }

    private function curtok()
    {
        return $this->tokens[$this->tokpos];
    }

    private function parse_alpha($tok)
    {
        $this->letter = strtoupper($tok);
        $this->tokpos++;
        $this->relative = ctype_lower($tok);
        if ( strtolower($tok) == "z" )
            $this->bezier->close();
    }

    private function on_parse_d($d) : MutableShape
    {
        $this->relpoint = new ShieldPos(0, 0);
        $add_spaces = preg_replace(["/([a-zA-Z])([-+0-9.])/", "/([-+0-9.])([a-zA-Z])/"], "$1 $2", $d);
        $this->tokens = preg_split('/(,|\s)+/', $add_spaces);
        $this->bezier = new MutableShape();
        $this->bezier->move_to(clone $this->relpoint);
        $this->tokpos = 0;
        $this->letter = "M";
        $this->relative = false;

        for ( ; $this->tokpos < count($this->tokens); )
        {
            $tok = $this->curtok();
            if ( ctype_alpha($tok) )
            {
                $this->parse_alpha($tok);
            }
            else
            {
                $this->parse_param();
            }
        }

        $this->bezier->clean();

        return $this->bezier;
    }

    private function parse_param()
    {
        switch ( $this->letter )
        {
            case 'M':
                $this->relpoint = $this->point();
                $this->bezier->move_to(clone $this->relpoint);
                $this->letter = "L";
                return;
            case 'L':
                $this->relpoint = $this->point();
                $this->bezier->line_to(clone $this->relpoint);
                return;
            case 'H':
                $x = $this->coord();
                if ( $this->relative )
                    $x += $this->relpoint->x;
                $this->relpoint = new ShieldPos($x, $this->relpoint->y);
                $this->bezier->line_to(clone $this->relpoint);
                return;
            case 'V':
                $y = $this->coord();
                if ( $this->relative )
                    $y += $this->relpoint->y;
                $this->relpoint = new ShieldPos($this->relpoint->x, $y);
                $this->bezier->line_to(clone $this->relpoint);
                return;
            case 'Q':
                $handle = $this->point();
                $this->relpoint = $this->point();
                $this->bezier->quadratic_to($handle, clone $this->relpoint);
                return;
            case 'C':
                $handle1 = $this->point();
                $handle2 = $this->point();
                $this->relpoint = $this->point();
                $this->bezier->cubic_to($handle1, $handle2, clone $this->relpoint);
                return;
            default:
                user_error("SVG path {$this->letter} not supported", E_USER_WARNING);
                $this->tokpos++;
                return;
        }
    }

    private function coord()
    {
        $v = (float)$this->curtok();
        $this->tokpos++;
        return $v;
    }

    private function point()
    {
        $x = $this->coord();
        $y = $this->coord();
        if ( $this->relative )
        {
            $x += $this->relpoint->x;
            $y += $this->relpoint->y;
        }
        return new ShieldPos($x, $y);
    }
}

class BezierSegmentFlags
{
    const NORMAL    = 0x00;
    const PLAIN     = 0x01;
    const STRAIGHT  = 0x02;
    const REVERSE   = 0x10;
}

/**
 * @brief A continuous sequence of compiled bezier data
 */
class CompiledBezierSegment
{
    /// @var CubicBezier[]
    private $sub_segments = [];
    private $length = 0;
    private $flags;

    function __construct($sub_segments, $flags)
    {
        $this->sub_segments = $sub_segments;
        $this->flags = $flags;
        foreach ( $sub_segments as $seg )
            $this->length += $seg->length;
    }

    function start()
    {
        if ( count($this->sub_segments) )
            return $this->sub_segments[0]->start;
        return null;
    }

    function finish()
    {
        if ( count($this->sub_segments) )
            return $this->sub_segments[count($this->sub_segments)-1]->finish;
        return null;
    }

    function empty()
    {
        return count($this->sub_segments) == 0;
    }

    function to_svg_d()
    {
        if ( $this->empty() )
            return "";

        $d = "";
        $letter = "";
        foreach ( $this->sub_segments as $segment )
        {
            if ( $segment->linear )
            {
                if ( $letter != "L" )
                    $d .= " L";
                $letter = "L";
                $d .= " {$segment->finish->x},{$segment->finish->y}";
            }
            else
            {
                if ( $letter != "C" )
                    $d .= " C";
                $letter = "C";
                $d .= " {$segment->tan1->x},{$segment->tan1->y} {$segment->tan2->x},{$segment->tan2->y} {$segment->finish->x},{$segment->finish->y}";
            }
        }

        return $d;
    }

    function sub_segments()
    {
        return $this->sub_segments;
    }

    function point_at_length($length)
    {
        if ( $length >= $this->length )
            return $this->finish();

        foreach ( $this->sub_segments as $seg )
        {
            if ( $length < $seg->length )
                return $seg->point_at_length($length);
            $length -= $seg->length;
        }

        return $this->finish();
    }

    function length()
    {
        return $this->length;
    }

    function flags()
    {
        return $this->flags;
    }
}

class CompiledPolyBezier
{
    private $closed = false;
    private $segments = [];

    function __construct($closed, $segments = [])
    {
        $this->segments = $segments;
        $this->closed = $closed;
    }

    function closed()
    {
        return $this->closed;
    }

    function to_svg_d()
    {
        $d = "";

        $p0 = $this->start();
        if ( $p0 )
            $d .= "M {$p0->x} {$p0->y}";

        foreach ( $this->segments as $segment )
        {
            $d .= $segment->to_svg_d();
        }

        if ( $this->closed )
            $d .= " Z";

        return $d;
    }

    function size()
    {
        return count($this->segments);
    }

    function segments()
    {
        return $this->segments;
    }

    function add_segment(CompiledBezierSegment $segment)
    {
        $this->segments[] = $segment;
    }

    function start()
    {
        if ( count($this->segments) )
            return $this->segments[0]->start();
        return null;
    }

    function finish()
    {
        if ( count($this->segments) )
            return $this->segments[count($this->segments)-1]->finish();
        return null;
    }
}

/**
 * @brief A shape made up of CompiledPolyBezier objects
 */
class CompiledShape extends ShapeInterface
{
    private $sub_paths = [];

    function sub_paths()
    {
        return $this->sub_paths;
    }

    function __construct($sub_paths)
    {
        $this->sub_paths = $sub_paths;
    }
}

