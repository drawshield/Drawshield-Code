<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

     DrawShield is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */

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
        if ( $this->linear )
            return $this->start->lerp($this->finish, $t);
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

        return $this->point($this->unchecked_t_at_length($length));
    }

    function t_at_length($length)
    {
        if ( $length <= 0 )
            return 0;

        if ( $length >= $this->length )
            return 1;

        if ( $this->linear )
            return $length / $this->length;

        return $this->unchecked_t_at_length($length);
    }

    /**
     * @brief Approximates a @p t from length for non-linear segments
     */
    private function unchecked_t_at_length($length)
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

    private function split_points($t)
    {

        $p01 = $this->start->lerp($this->tan1, $t);
        $p12 = $this->tan1->lerp($this->tan2, $t);
        $p23 = $this->tan2->lerp($this->finish, $t);

        $p012 = $p01->lerp($p12, $t);
        $p123 = $p12->lerp($p23, $t);

        $p0123 = $p012->lerp($p123, $t);

        return [
            $p01, $p012, $p0123, $p123, $p23
        ];
    }

    function split($t)
    {
        if ( $this->linear )
        {
            $mid = $this->point($t);
            return [
                new CubicBezier(clone $this->start, clone $this->start, clone $mid, clone $mid ),
                new CubicBezier(clone $mid, clone $mid, clone $this->finish, clone $this->finish),
            ];
        }

        list($p01, $p012, $p0123, $p123, $p23) = $this->split_points($t);

        return [
            new CubicBezier(clone $this->start, $p01, $p012, $p0123),
            new CubicBezier($p0123, $p123, $p23, clone $this->finish)
        ];
    }

    function multi_split($positions)
    {
        if ( !count($positions) )
            return [$this];

        $t1 = array_shift($positions);

        list($seg1, $seg2) = $this->split($t1);
        $segs = [$seg1];

        foreach ( $positions as $t2 )
        {
            $t = ($t2-$t1) / (1-$t1);
            list($seg1, $seg2) = $seg2->split($t);
            $t1 = $t;
            $segs[] = $seg1;
        }

        $segs[] = $seg2;

        return $segs;
    }

    function single_root_t($tolerance = 0.01)
    {
        if ( abs($this->start->y) < $tolerance )
            return 0;

        if ( abs($this->finish->y) < $tolerance )
            return 1;

        if ( $this->linear )
        {
            $ss = $this->start->y > 0 ? 1 : -1;
            $sf = $this->finish->y > 0 ? 1 : -1;
            if ( $ss == $sf )
                return null;

            $dy = $this->finish->y - $this->start->y;

            $t = abs($this->start->y) / $dy;
            if ( $t < 0 || $t > 1 )
                return null;
            return $t;
        }


        $t0 = 0;
        $p0 = $this->start;
        for ( $i = 1; $i < count($this->length_steps); $i++ )
        {
            $t1 = $i/count($this->length_steps);
            $p1 = self::bezier_solve($t1, $this->start, $this->tan1, $this->tan2, $this->finish);
            if ( $p1->y == 0 )
                return $t1;

            $s0 = $p0->y > 0 ? 1 : -1;
            $s1 = $p1->y > 0 ? 1 : -1;
            // Signs are different, the root is in [$t0, $t1]
            if ( $s0 != $s1 )
            {
                // Bisect until under tolerance (max 10 steps)
                for ( $j = 0; abs($p1->y - $p0->y) > $tolerance && $j < 10; $j++ )
                {
                    $tm = ($t0+$t1) / 2;
                    $pm = self::bezier_solve($tm, $this->start, $this->tan1, $this->tan2, $this->finish);
                    if ( $pm->y == 0 )
                        return $tm;
                    $sm = $pm->y > 0 ? 1 : -1;


                    if ( $sm == $s0 )
                        list($t0, $p0, $s0) = [$tm, $pm, $sm];
                    else
                        list($t1, $p1, $s1) = [$tm, $pm, $sm];
                }

                return ($t0+$t1) / 2;
            }
            $t0 = $t1;
            $p0 = $p1;
        }

        return null;
    }

    function translated($dx, $dy)
    {
        $copy = clone $this;
        $copy->start = $copy->start->translated($dx, $dy);
        $copy->tan1 = $copy->tan1->translated($dx, $dy);
        $copy->tan2 = $copy->tan2->translated($dx, $dy);
        $copy->finish = $copy->finish->translated($dx, $dy);
        return $copy;
    }

    function rotated($angle, ShieldPos $origin = null)
    {
        if ( !$origin )
            $origin = new ShieldPos(0, 0);

        $copy = clone $this;
        $copy->start = $copy->start->rotated($angle, $origin);
        $copy->tan1 = $copy->tan1->rotated($angle, $origin);
        $copy->tan2 = $copy->tan2->rotated($angle, $origin);
        $copy->finish = $copy->finish->rotated($angle, $origin);
        return $copy;
    }

    function line_single_intersection_t(ShieldPos $p0, ShieldPos $p1)
    {
        $seg = $this->translated(-$p0->x, -$p0->y)->rotated(-$p0->angle_to($p1));
        return $seg->single_root_t();
    }

    function line_single_intersection(ShieldPos $p0, ShieldPos $p1)
    {
        $t = $this->line_single_intersection_t($p0, $p1);
        if ( $t === null )
            return null;
        return $this->point($t);
    }

    function tangent($t)
    {
        if ( $this->linear )
            return $this->start->angle_to($this->finish);

        list($p01, $p012, $p0123, $p123, $p23) = $this->split_points($t);
        return $p0123->angle_to($p123);
    }

    function reversed()
    {
        return new CubicBezier($this->finish, $this->tan2, $this->tan1, $this->start);
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
            if ( $this->start()->pos->distance($this->finish()->pos) > 1 )
                $this->add_point(clone $this->start());
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
        $sub = 0;
        if ( $this->closed && $this->size() > 2 )
        {
            $sub = 1;
            if ( $this->start()->pos->distance($this->finish()->pos) > 1 )
                $this->points[] = clone $this->start();
        }

        foreach ( $this->points as $point )
            $point->reverse();
        $this->points = array_reverse($this->points);

        if ( empty($this->segment_tags) )
            return;

        $tags = [];
        $tag_old = $this->segment_tags[0] ?? BezierSegmentFlags::NORMAL;
        foreach ( $this->segment_tags as $index => $tag )
        {
            if ( $index != 0 )
                $tags[$this->size() - $sub - $index] = $tag_old;
            $tag_old = $tag;
        }
        $tags[0] = $tag_old;

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

        $inter = $this->insersect($l[0], $l[1], $r[0], $r[1]);
        $vals = [];

        if ( $inter && $miter_limit > 0 && $c->pos->distance($inter) > abs($amount) * $miter_limit )
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

    function points()
    {
        return $this->points;
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

    private function on_parse_d($d, ?MutableShape $bezier = null) : MutableShape
    {
        $add_spaces = preg_replace(["/([a-zA-Z])([-+0-9.])/", "/([-+0-9.])([-a-zA-Z])/"], "$1 $2", $d);
        $this->tokens = preg_split('/(,|\s)+/', $add_spaces);
        $this->bezier = $bezier ?? new MutableShape();
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

class SegmentAtData
{
    function __construct($index, CubicBezier $segment, $t, CompiledBezierSegment $parent)
    {
        $this->index = $index;
        $this->segment = $segment;
        $this->t = $t;
//         $this->parent = $parent;
    }

    function split()
    {
        return $this->segment->split($this->t);
    }

    function point() : ShieldPos
    {
        return $this->segment->point($this->t);
    }

    function tangent()
    {
        return $this->segment->tangent($this->t);
    }

    function normal()
    {
        return $this->tangent() + M_PI/2;
    }

    // Relative point in polar coordinates, taking tangent() as reference angle
    function polar_relative($distance, $angle) : ShieldPos
    {
        $p = $this->point();
        $angle = $this->tangent() + $angle;
        return new ShieldPos(
            $p->x + $distance * cos($angle),
            $p->y + $distance * sin($angle)
        );
    }
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

    function size()
    {
        return count($this->sub_segments);
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

    function segment_at_length($length) : SegmentAtData
    {
        if ( $length >= $this->length )
            return new SegmentAtData(
                count($this->sub_segments) - 1,
                $this->sub_segments[count($this->sub_segments) - 1],
                1,
                $this
            );

        for ( $i = 0; $i < count($this->sub_segments); $i++ )
        {
            $seg = $this->sub_segments[$i];
            if ( $length < $seg->length )
                return new SegmentAtData(
                    $i,
                    $seg,
                    $seg->t_at_length($length),
                    $this
                );
            $length -= $seg->length;
        }

        return new SegmentAtData(
            count($this->sub_segments) - 1,
            $this->sub_segments[count($this->sub_segments) - 1],
            1,
            $this
        );
    }

    function length()
    {
        return $this->length;
    }

    function flags()
    {
        return $this->flags;
    }

    function reversed()
    {
        $copy = clone $this;
        $copy->sub_segments = [];
        foreach ( array_reverse($this->sub_segments) as $seg )
            $copy->sub_segments[] = $seg->reversed();
        return $copy;
    }

    function line_single_intersection(ShieldPos $p0, ShieldPos $p1) : ?SegmentAtData
    {
        for ( $i = 0; $i < count($this->sub_segments); $i++ )
        {
            $seg = $this->sub_segments[$i];
            $inter = $seg->line_single_intersection_t($p0, $p1);
            if ( $inter !== null )
                return new SegmentAtData($i, $seg, $inter, $this);
        }
        return null;
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

    function empty()
    {
        return count($this->segments) == 0;
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


    function reversed()
    {
        $copy = clone $this;
        $copy->segments = [];
        foreach ( array_reverse($this->segments) as $seg )
            $copy->segments[] = $seg->reversed();
        return $copy;
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

