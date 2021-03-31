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
        $d = "M {$p0->pos->x} {$p0->pos->y} \nC";
        for ( $i = 1; $i < count($this->points); $i++ )
        {
            $p1 = $this->points[$i];
            $d .= " {$p0->tan_out->x},{$p0->tan_out->y} {$p1->tan_in->x},{$p1->tan_in->y} {$p1->pos->x},{$p1->pos->y}\n";
            $p0 = $p1;
        }

        if ( $this->closed )
            $d .= " Z";

        return $d;
    }

    function tag_segment($flags)
    {
        if ( $this->empty() )
            $this->segment_tags[0] = $flags;
        else
            $this->segment_tags[$this->size()-1] = $flags;
        return $this;
    }

    function clear_tags()
    {
        $this->segment_tags = [];
        return $this;
    }

    function make_plain()
    {
        $this->segment_tags = [BezierSegmentFlags::PLAIN];
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

        $tags = [];
        foreach ( $this->segment_tags as $index => $tag )
            $tags[$this->size() - 1 - $index] = $tag;
        $this->segment_tags = $tags;
    }

    function __clone()
    {
        $cloned = [];
        foreach ( $this->points as $p )
        {
            $cloned[] = clone $p;
        }
        $this->points = $cloned;
    }

    private function offset_side($segment_start, $segment_finish, $amount)
    {
        $angle = $segment_start->angle_to($segment_finish);
        $normal_angle = $angle - M_PI/2;
        $dx = cos($normal_angle);
        $dy = sin($normal_angle);
        return [
            $segment_start->translated($amount * $dx, $amount * $dy),
            $segment_finish->translated($amount * $dx, $amount * $dy)
        ];
    }

    private function offset_helper(BezierPoint $p_left, BezierPoint $c, BezierPoint $p_right, $amount, $miter_limit)
    {
//         $seg_left = $c->tan_in->distance($c->pos) < 1 ? $p_left->pos : $c->tan_in;
//         $seg_right = $c->tan_out->distance($c->pos) < 1 ? $p_right->pos : $c->tan_out;
        $seg_left = $p_left->pos;
        $seg_right = $p_right->pos;
        $l = $this->offset_side($seg_left, $c->pos, $amount);
        $r = $this->offset_side($c->pos, $seg_right, $amount);

        $inter = $this->insersect($l[0], $l[1], $r[0], $r[1], $miter_limit);
        $vals = [];

        if ( $inter && $c->pos->distance($inter) > abs($amount) * $miter_limit )
        {
            $d = $c->pos->distance($inter);
            $dx = ($inter->x - $c->pos->x) / $d;
            $dy = ($inter->y - $c->pos->y) / $d;
            if ( $amount < 0 )
            {
                $dx = -$dx;
                $dy = -$dy;
            }
            $inter = $c->pos->translated($dx * $amount, $dy * $amount);

        }

        if ( $inter )
        {
            $new_p_left = $new_p_right = new BezierPoint($inter);
            $vals = [$new_p_left];
        }
        else
        {

            $new_p_left = new BezierPoint($l[1]);
            $new_p_right = new BezierPoint($r[0]);
            $vals = [$new_p_left, $new_p_right];
        }

        $new_p_left->tan_in = $this->adjust_tangent($c->tan_in, $c->pos, $new_p_left->pos);
        $new_p_right->tan_out = $this->adjust_tangent($c->tan_out, $c->pos, $new_p_right->pos);

        return $vals;
    }

    private function adjust_tangent($tangent, $pos_old, $pos_new)
    {
        return new ShieldPos(
            $tangent->x - $pos_old->x + $pos_new->x,
            $tangent->y - $pos_old->y + $pos_new->y
        );
    }

    private function insersect($lp1, $lp2, $rp1, $rp2)
    {
        if ( $lp2->distance($rp1) < 1 )
            return $lp2;

        $a = $lp2->translated(-$lp1->x, -$lp1->y);
        $b = $rp1->translated(-$rp2->x, -$rp2->y);
        $c = $lp1->translated(-$rp1->x, -$rp1->y);

        $denominator = $a->y * $b->x - $a->x * $b->y;
        if ( $denominator == 0 )
            return null;

        $na = ($b->y * $c->x - $b->x * $c->y) / $denominator;

        return $lp1->translated($a->x * $na, $a->y * $na);
    }

    function offset_copy($amount, $miter_limit = 2)
    {
        $bez = new PolyBezier();

        if ( $this->size() < 3 )
            return $bez;

        $tag = BezierSegmentFlags::NORMAL;

        if ( isset($this->segment_tags[0]) )
            $bez->segment_tags[0] = $tag = $this->segment_tags[0];

        $bez->add_point(clone $this->points[0]);

        $points = $this->points;
        $n_points = count($points);
        if ( $points[0]->pos->distance($points[$n_points - 1]->pos) > 1 )
        {
            $points[] = $points[0];
            $n_points++;
        }

        for ( $i = 1; $i < $n_points - 1; $i++ )
        {
            $p_left = $points[$i - 1];
            $c = $points[$i];
            $p_right = $points[$i+1];
            $vals = $this->offset_helper($p_left, $c, $p_right, $amount, $miter_limit);

            if ( count($vals) == 2 )
            {
                if ( $tag != BezierSegmentFlags::PLAIN )
                    $bez->segment_tags[$bez->size()] = BezierSegmentFlags::PLAIN;

                $tag = BezierSegmentFlags::PLAIN;

                $bez->add_point($vals[0]);

                if ( isset($this->segment_tags[$i]) )
                {
                    $tag = $this->segment_tags[$i];

                    if ( $tag != BezierSegmentFlags::PLAIN )
                        $bez->segment_tags[$bez->size()] = $tag;
                }

                $bez->add_point($vals[1]);
            }
            else
            {
                if ( isset($this->segment_tags[$i]) )
                {
                    $tag = $this->segment_tags[$i];
                    $bez->segment_tags[$bez->size()] = $tag;
                }
                $bez->add_point($vals[0]);
            }
        }

        if ( $this->closed )
        {
            $c = $points[0];
            $p_right = $points[1];

            $p_left = $points[$n_points - 2];

            $vals = $this->offset_helper($p_left, $c, $p_right, $amount, $miter_limit);

            if ( count($vals) == 2 )
            {
                $bez->points[0] = $vals[1];
                $bez->points[] = $vals[0];
            }
            else
            {
                $bez->points[0] = $vals[0];
                $bez->points[] = clone $vals[0];
            }

            $bez->closed = true;
        }
        else
        {
            $last = $points[$n_points - 1];
            $off = $this->offset_side($points[$n_points - 2]->pos, $last->pos, $amount);
            $tan = $this->adjust_tangent($last->tan_in, $last->pos, $off[1]);
            $bez->points[] = new BezierPoint($off[1], $tan);


            $first = $points[0];
            $off = $this->offset_side($first->pos, $points[1]->pos, $amount);
            $tan = $this->adjust_tangent($first->tan_out, $first->pos, $off[0]);
            $bez->points[0] = new BezierPoint($off[0], $tan);
        }

        return $bez;
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
    abstract function to_svg_d();

    function to_svg_path($attrs=[])
    {
        $attrs["d"] = $this->to_svg_d();
        $path = "<path";
        foreach ( $attrs as $name => $val )
            $path .= " $name='$val'";
        return $path . "/>";
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

    function last_path() : PolyBezier
    {
        return $this->sub_paths[count($this->sub_paths)-1];
    }

    private function finish() : BezierPoint
    {
        return $this->last_path()->finish();
    }

    private function has_finish()
    {
        return !$this->empty() && !$this->last_path()->empty();
    }

    private function relpoint()
    {
        return $this->has_finish() ? $this->finish()->pos : new ShieldPos(0, 0);
    }

    private function get_args($args, $n)
    {
        $relative = false;

        if ( is_bool($args[count($args)-1]) )
            $relative = array_pop($args);

        if ( count($args) == $n )
        {
            $new_args = $args;
        }
        else
        {
            if ( count($args) != 2*$n )
                throw new Exception("Must pass $n points");

            $new_args = [];
            for ( $i = 0; $i < $n; $i++ )
                $new_args[] = new ShieldPos($args[$i*2], $args[2*$i+1]);
        }

        if ( !$relative )
            return $new_args;

        $last_pos = $this->relpoint();
        $offset_args = [];
        foreach ( $new_args as $a )
            $offset_args[] = $a->translated($last_pos->x, $last_pos->y);
        return $offset_args;
    }

    private function new_path_if_needed()
    {
        if ( $this->empty() || $this->last_path()->closed )
        {
            $new_bez = new PolyBezier();
            $new_bez->add_point($this->relpoint());
            $this->sub_paths[] = $new_bez;
        }
    }

    function move_to()
    {
        list($p) = $this->get_args(func_get_args(), 1);
        if ( !$this->empty() && $this->last_path()->size() < 2 )
        {
            $this->last_path()->points = [new BezierPoint($p)];
        }
        else
        {
            $new_bez = new PolyBezier();
            $new_bez->add_point($p);
            $this->sub_paths[] = $new_bez;
        }
        return $this;
    }

    function line_to()
    {
        list($p) = $this->get_args(func_get_args(), 1);
        $this->new_path_if_needed();
        $this->last_path()->line_to($p);
        return $this;
    }

    function quadratic_to()
    {
        list($handle, $dest) = $this->get_args(func_get_args(), 2);
        $this->new_path_if_needed();
        $this->last_path()->quadratic_to($handle, $dest);
        return $this;
    }

    function cubic_to()
    {
        list($handle1, $handle2, $dest) = $this->get_args(func_get_args(), 3);
        $this->new_path_if_needed();
        $this->last_path()->cubic_to($handle1, $handle2, $dest);
        return $this;
    }

    function horizontal($x, $relative = false)
    {
        if ( $relative )
            $x += $this->relpoint()->x;
        $y = $this->relpoint()->y;
        $this->new_path_if_needed();
        $this->line_to($x, $y);
        return $this;
    }

    function vertical($y, $relative = false)
    {
        if ( $relative )
            $y += $this->relpoint()->y;
        $x = $this->relpoint()->x;
        $this->new_path_if_needed();
        $this->line_to($x, $y);
        return $this;
    }

    function close()
    {
        if ( !$this->empty() )
            $this->last_path()->close();
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
            $cb = $bez->compile();
            if ( $cb->size() != 0 )
                $compiled[] = $cb;
        }

        return new CompiledShape($compiled);
    }

    function add_path(PolyBezier $path)
    {
        $this->sub_paths[] = $path;
    }

    function remove_path($index)
    {
        array_splice($this->sub_paths, $index, 1);
    }

    function combine_from(MutableShape $other)
    {
        foreach ( $other->sub_paths as $bez )
            $this->add_path(clone $bez);
    }

    function translate()
    {
        list($p) = $this->get_args(func_get_args(), 1);
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

    function reverse_last()
    {
        if ( $this->sub_paths[$this->size()-1]->size() < 2 )
            $this->sub_paths[$this->size()-2]->reverse();
        else
            $this->sub_paths[$this->size()-1]->reverse();

        return $this;
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
        else
            $this->new_path_if_needed();
        $this->last_path()->tag_segment($flags);
        return $this;
    }

    function to_svg_d()
    {
        $d = "";
        foreach ( $this->sub_paths() as $bez )
        {
            if ( $bez->size() > 1 )
                $d .= $bez->to_svg_d();
        }
        return $d;
    }
}

/**
 * @brief Minimal parser for the SVG path d attribute
 */
class SvgDParser
{
    private $tokens, $tokpos, $letter, $relative;
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
        {
            $this->bezier->close();
            $this->letter = "M";
        }
    }

    private function on_parse_d($d) : MutableShape
    {
        $add_spaces = preg_replace(["/([a-zA-Z])([-+0-9.])/", "/([-+0-9.])([-a-zA-Z])/"], "$1 $2", $d);
        $this->tokens = preg_split('/(,|\s)+/', $add_spaces);
        $this->bezier = new MutableShape();
        $this->bezier->move_to(new ShieldPos(0, 0));
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
            else if ( $tok == "" )
            {
                $this->tokpos++;
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
                $this->bezier->move_to($this->point(), $this->relative);
                $this->letter = "L";
                return;
            case 'L':
                $this->bezier->line_to($this->point(), $this->relative);
                return;
            case 'H':
                $this->bezier->horizontal($this->coord(), $this->relative);
                return;
            case 'V':
                $this->bezier->vertical($this->coord(), $this->relative);
                return;
            case 'Q':
                $this->bezier->quadratic_to($this->point(), $this->point(), $this->relative);
                return;
            case 'C':
                $this->bezier->cubic_to($this->point(), $this->point(), $this->point(), $this->relative);
                return;
            default:
                user_error("SVG path {$this->letter} not supported", E_USER_WARNING);
                $this->tokpos++;
                while ( $this->tokpos < count($this->tokens) && !ctype_alpha($this->tokpos) )
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
        return new ShieldPos($x, $y);
    }
}

class BezierSegmentFlags
{
    // Paint the pattern normally
    const NORMAL        = 0x00;
    // Don't paint the pattern
    const PLAIN         = 0x01;
    // Safe to paint the pattern on the whole segment (rather than repeating)
    const STRAIGHT      = 0x02;

    // Mark the "under" side for embattled
    const UNDERSIDE     = 0x10;
    // Try to make it look the same on both ends of the edge
    const SYMMETRICAL   = 0x20;
    // Mirror the y component of the pattern
    const MIRROR        = 0x40;
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
    private $length = 0;

    function __construct($closed, $segments = [])
    {
        $this->segments = $segments;
        $this->closed = $closed;
        foreach ( $segments as $seg )
            $this->length += $seg->length();
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
        $this->length += $segment->length();
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

    function point_at_length($length)
    {
        if ( $length <= 0 )
            return $this->start();
        if ( $length >= $this->length )
            return $this->finish();

        foreach ( $this->segments as $seg )
        {
            if ( $length < $seg->length() )
                return $seg->point_at_length($length);
            $length -= $seg->length();
        }

        return $this->finish();
    }

    function length()
    {
        return $this->length;
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


    function to_svg_d()
    {
        $d = "";
        foreach ( $this->sub_paths() as $bez )
        {
            $d .= $bez->to_svg_d();
        }
        return $d;
    }
}

