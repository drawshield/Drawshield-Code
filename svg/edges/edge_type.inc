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

require_once(__dir__."/../bezier.inc");


abstract class EdgeType
{
    abstract function apply_bezier_segment(CompiledBezierSegment $source, MutableShape $target, $feature_width, $feature_height);

    function apply_bezier(CompiledPolyBezier $source, MutableShape $target, $feature_width, $feature_height)
    {
        $p0 = $source->start();
        $target->move_to($p0);

        foreach( $source->segments() as $path )
            $this->apply_bezier_segment($path, $target, $feature_width, $feature_height);

        if ( $source->closed() )
            $target->close();
    }


    function apply(CompiledShape $source, MutableShape $target, $feature_width, $feature_height = null)
    {
        if ( $feature_height == null )
            $feature_height = $feature_width;

        foreach( $source->sub_paths() as $path )
            $this->apply_bezier($path, $target, $feature_width, $feature_height);
    }

    private static function get_meta_flag($element, $attribute, $default = 0)
    {
        if ( $element->hasAttribute("data-drawshield-$attribute") )
        {
            $val = $element->getAttribute("data-drawshield-$attribute");
            if ( is_numeric($default) )
                return (float)$val;
            return $val;
        }
        return $default;
    }

    protected function skip_segment(CompiledBezierSegment $source, MutableShape $target)
    {
        foreach ( $source->sub_segments() as $bez )
            $target->cubic_to($bez->tan1, $bez->tan2, $bez->finish);
    }

    private static function load_path($xpath, $n, $filename)
    {
        $element = $xpath->evaluate("//*[local-name()='path'][$n]")[0];
        if ( !$element || !$element->hasAttribute("d") )
        {
            user_error("Missing path element in $filename", E_USER_WARNING);
            return [null, null];
        }

        $path = SvgDParser::parse_d($element->getAttribute("d"));
        if ( $path->empty() || $path->sub_paths()[0]->empty() )
        {
            user_error("Empty path data in $filename", E_USER_WARNING);
            return [null, null];
        }

        // Flip y coordinates before applying
        if ( self::get_meta_flag($element, "flip-y") )
            $path->scale(1, -1);

        // Reverse the order of the points
        if ( self::get_meta_flag($element, "reverse") )
            $path->reverse();

        return [$element, $path];
    }

    static function load($name) : EdgeType
    {
        static $cache = [];

        if ( isset($cache[$name]) )
            return $cache[$name];

        $filename = __dir__ . "/$name.svg";
        if ( !file_exists($filename) )
        {
            user_error("No edge type file for $name", E_USER_WARNING);
            return new EdgeTypeNull();
        }

        $dom = new DOMDocument();
        $dom->load($filename);
        $xpath = new DOMXPath($dom);

        list($element, $path) = self::load_path($xpath, 1, $filename);
        if ( !$path )
            return new EdgeTypeNull();

        // Edge class ["full", "repeating"]
        $type = self::get_meta_flag($element, "type", "repeating");
        switch ( $type )
        {
            default:
                user_error("$type edge types are not supported yet", E_USER_WARNING);

            case "repeating":
            case "once":
                // What to do on reverse edges (see constants in EdgeTypeRepeating)
                $reverse_behaviour = self::get_meta_flag($element, "reverse-behaviour", EdgeTypeRepeating::REVERSE_NORMAL);

                // Pattern is symmetrical (can be cut in half if needed)
                $symmetrical = self::get_meta_flag($element, "symmetrical");

                // How much to stretch the pattern by along the path
                $scale_x = self::get_meta_flag($element, "scale-x", 1);
                // How much to stretch the pattern by along the normal
                $scale_y = self::get_meta_flag($element, "scale-y", 1);

                if ($type == 'once') {
                    $edge = new EdgeTypeOnce($path->sub_paths()[0], $symmetrical, $reverse_behaviour, $scale_x, $scale_y);
                } else {
                    $edge = new EdgeTypeRepeating($path->sub_paths()[0], $symmetrical, $reverse_behaviour, $scale_x, $scale_y);
                }
                break;

            case "full":
                $edge = new EdgeTypeFull($path->sub_paths()[0]);
                break;

            case "flory":
                $path_bottom = self::load_path($xpath, 2, $filename)[1];
                $path_bottom_small = self::load_path($xpath, 3, $filename)[1];
                $edge = new EdgeTypeFlory($path->sub_paths()[0], $path_bottom->sub_paths()[0], $path_bottom_small->sub_paths()[0]);
                break;
        }

        $cache[$name] = $edge;
        return $edge;
    }
}

abstract class EdgeTypeBezier extends EdgeType
{
    function __construct(PolyBezier $shape)
    {
        $this->shape = $this->normalized($shape);
    }

    protected function normalized(PolyBezier $shape)
    {
        $shape = clone $shape;
        $shape->translate(new ShieldPos(-$shape->start()->pos->x, $shape->start()->pos->y));
        $shape->scale(1/$shape->finish()->pos->x);
        return $shape->compile()->segments()[0];
    }


    function apply_line_segment(ShieldPos $p1, ShieldPos $p2, MutableShape $target, $height = null, $scale = 1)
    {
        $this->do_apply_line_segment($this->shape, $p1, $p2, $target, $height, $scale);
    }

    protected function do_apply_line_segment(CompiledBezierSegment $shape, ShieldPos $p1, ShieldPos $p2, MutableShape $target, $height = null, $scale = 1)
    {
        if ( $scale != 1 )
            $p2 = $p1->lerp($p2, $scale);

        if ( $height === null  )
            $height = $p1->distance($p2);

        $angle = atan2($p2->y - $p1->y, $p2->x - $p1->x);
        $normal_angle = $angle + M_PI/2;
        $normal = new ShieldPos(cos($normal_angle), sin($normal_angle));

        $count = floor(count($shape->sub_segments()) / $scale);

        for ( $i = 0;  $i < $count; $i++ )
        {
            $seg = $shape->sub_segments()[$i];
            if ( $seg->linear )
                $target->line_to($this->relpoint($seg->finish, $p1, $p2, $height, $normal));
            else
                $target->cubic_to(
                    $this->relpoint($seg->tan1, $p1, $p2, $height, $normal),
                    $this->relpoint($seg->tan2, $p1, $p2, $height, $normal),
                    $this->relpoint($seg->finish, $p1, $p2, $height, $normal)
                );
        }
    }

    private function relpoint(ShieldPos $p, ShieldPos $seg_a, ShieldPos $seg_b, $height, ShieldPos $normal)
    {
        $y = $height * $p->y;
        $p1 = $seg_a->lerp($seg_b, $p->x);
        return new ShieldPos($p1->x + $normal->x * $y, $p1->y + $normal->y * $y);
    }
}

class EdgeTypeRepeating extends EdgeTypeBezier
{
    const REVERSE_NORMAL = "normal";
    const REVERSE_FLIP = "flip";
    const REVERSE_PLAIN = "plain";

    function __construct(PolyBezier $shape, $can_be_halved, $reverse_behaviour, $scale_x, $scale_y)
    {
        parent::__construct($shape);
        $this->can_be_halved = $can_be_halved;
        $this->reverse_behaviour = $reverse_behaviour;
        $this->scale_x = $scale_x;
        $this->scale_y = $scale_y;
    }

    function apply_bezier_segment(CompiledBezierSegment $source, MutableShape $target, $feature_width, $feature_height)
    {
        $feature_width *= $this->scale_x;
        $feature_height *= $this->scale_y;

        if ( $source->flags() & BezierSegmentFlags::PLAIN )
            return $this->skip_segment($source, $target);

        if ( $source->flags() & BezierSegmentFlags::UNDERSIDE )
        {
            if ( $this->reverse_behaviour == self::REVERSE_FLIP )
                $feature_height = -$feature_height;
            else if ( $this->reverse_behaviour == self::REVERSE_PLAIN )
                return $this->skip_segment($source, $target);
        }

        if ( $this->reverse_behaviour == self::REVERSE_NORMAL && $source->flags() & BezierSegmentFlags::MIRROR )
        {
            $feature_height = -$feature_height;
        }

        $count = round($source->length() / $feature_width);

        if ( $count == 0 )
            $count = 1;

        $half = $this->can_be_halved && ($source->flags() & BezierSegmentFlags::SYMMETRICAL);
        if ( $half )
            $count -= 0.5;

        $size = $source->length() / $count;

        $p0 = $source->start();
        for ( $i = 1; $i <= $count; $i++ )
        {
            $p1 = $source->point_at_length($i * $size);
            $this->apply_line_segment($p0, $p1, $target, $feature_height);
            $p0 = $p1;
        }

        if ( $half )
        {
            $p1 = $source->finish();
            $this->apply_line_segment($p0, $p1, $target, $feature_height, 2);
            $target->line_to($p1);
        }
    }
}

class EdgeTypeNull extends EdgeType
{
    function apply_bezier_segment(CompiledBezierSegment $source, MutableShape $target, $feature_width, $feature_height)
    {
        $this->skip_segment($source, $target);
    }
}

class EdgeTypeFlory extends EdgeTypeBezier
{
    function __construct(PolyBezier $top, PolyBezier $bottom, PolyBezier $bottom_small)
    {
        parent::__construct($top);
        $this->bottom = $this->normalized($bottom);
        $this->size_mult = abs(($bottom->finish()->pos->x - $bottom->start()->pos->x) / ($top->finish()->pos->x - $top->start()->pos->x));
        $this->bottom_small = $this->normalized($bottom_small);
    }

    function apply_bezier_segment(CompiledBezierSegment $source, MutableShape $target, $feature_width, $feature_height)
    {
        $this->skip_segment($source, $target);
    }

    private function is_tight_corner(ShieldPos $p0, ShieldPos $p1, ShieldPos $pm)
    {
        $angle_in = $p0->angle_to($pm);
        if ( $angle_in < 0 )
            $angle_in += 2 * M_PI;

        $angle_out = $pm->angle_to($p1);
        if ( $angle_out < 0 )
            $angle_out += 2 * M_PI;

        $delta_angle = abs($angle_out-$angle_in);
        return $delta_angle >= M_PI * 0.2;
    }

    private function add_flory(ShieldPos $p0, ShieldPos $p1, ShieldPos $pm, MutableShape $target, $feature_height, $direction)
    {
        $shape = $this->shape;

        if ( $direction == -1 )
        {
            $shape = $this->is_tight_corner($p0, $p1, $pm) ? $shape = $this->bottom_small : $this->bottom;
        }

        $target->move_to($p0);
        $this->do_apply_line_segment($shape, $p0, $p1, $target, -$feature_height);
        $target->line_to($pm);
        $target->close();
    }

    private function add_flory_segment(CompiledBezierSegment $source, MutableShape $target, $feature_height, $direction, &$data)
    {
        $count = $data["count"];
        if ( $count == 0 || ($source->flags() & BezierSegmentFlags::PLAIN) )
        {
            $this->skip_segment($source, $target);
            return;
        }

        $size = $source->length() / $count;

        for ( $i = 1; $i < $count; $i++ )
        {
            $direction *= -1;

            $feature_width = $direction == 1 ? $feature_height : $feature_height * $this->size_mult;

            if ( isset($data["lines"][$i]) )
            {
                $seg_outer = $data["lines"][$i];
                $line_p0 = $seg_outer->point();
                $line_p1 = $seg_outer->polar_relative(-300, M_PI/2);
                $seg = $source->line_single_intersection($line_p0, $line_p1);
                if ( !$seg )
                    continue;

                $p0 = $seg->polar_relative($feature_width / 2, 0);
                $p1 = $seg->polar_relative($feature_width / 2, M_PI);
                $pm = $seg->point();

            }
            else
            {
                $seg = $source->segment_at_length($i * $size);
                $p0 = $seg->polar_relative($feature_width / 2, M_PI);
                $p1 = $seg->polar_relative($feature_width / 2, 0);
                $pm = $seg->point();

                $data["lines"][$i] = $seg;
            }

            $this->add_flory($p0, $p1, $pm, $target, $feature_width, $direction);
        }

        return $direction;
    }

    private function add_flory_bezier(CompiledPolyBezier $source, MutableShape $target, $feature_gap, $feature_height, &$data, &$tot_counts, $min_flories)
    {
        if ( $source->empty() || $source->segments()[0]->flags() & BezierSegmentFlags::PLAIN )
            return;


        $segments = $source->segments();
        $direction = 1;
        $underside = $segments[0]->flags() & BezierSegmentFlags::UNDERSIDE;

        if ( $underside )
        {
            $segments = $source->reversed()->segments();
            $direction = -1;
        }


        $prev_segment = $segments[$source->size()-1];

        if ( empty($data) )
        {
            foreach ( $segments as $seg )
            {
                $n = max($min_flories+1, round($seg->length() / $feature_gap));
                $tot_counts += $n;
                $data[] = ["count" => $n, "lines" => []];
            }
        }

        for ( $i = 0; $i < count($segments); $i++ )
        {
            $seg = $segments[$i];
            $pm = $seg->start();

            $tight = $this->is_tight_corner(
                $prev_segment->point_at_length($prev_segment->length() - $feature_height),
                $seg->point_at_length($feature_height),
                $pm
            );

            if ( $tight )
            {
                $old_direction = $direction;
                $direction = $underside ? -1 : 1;
            }

            $feature_width = $direction == 1 ? $feature_height : $feature_height * $this->size_mult;
            $p0 = $prev_segment->point_at_length($prev_segment->length() - $feature_width/M_SQRT2);
            $p1 = $seg->point_at_length($feature_width/M_SQRT2);
            if ( $underside )
                list($p0, $p1) = [$p1, $p0];
            $this->add_flory($p0, $p1, $pm, $target, $feature_width, $direction);

            if ( $tight )
            {
                $direction = $old_direction;
            }

            $direction = $this->add_flory_segment($seg, $target, $feature_height, $direction, $data[$i]);
            $direction *= -1;
            $prev_segment = $seg;
        }
    }

    function apply(CompiledShape $source, MutableShape $target, $feature_gap, $feature_height = null, $min_flories=1)
    {
        parent::apply($source, $target, $feature_gap, $feature_height);

        $data = [];
        $tot_counts = 0;
        foreach ( $source->sub_paths() as $path )
            $this->add_flory_bezier($path, $target, $feature_gap, $feature_height, $data, $tot_counts, $min_flories);
    }
}



class EdgeTypeFull extends EdgeTypeBezier
{
    function apply_bezier_segment(CompiledBezierSegment $source, MutableShape $target, $feature_width, $feature_height)
    {
        if ( $source->flags() & BezierSegmentFlags::STRAIGHT )
        {
            if ( $source->flags() & BezierSegmentFlags::UNDERSIDE )
                $feature_height *= -1;
            $p0 = $source->start();
            $p1 = $source->finish();
            $target->line_to($p0);
            $this->apply_line_segment($p0, $p1, $target, $feature_height);
        }
        else
        {
            $this->skip_segment($source, $target);
        }
    }
}

class EdgeTypeOnce extends EdgeTypeBezier
{

    function __construct(PolyBezier $shape, $symmetrical, $reverse_behaviour, $scale_x, $scale_y)
    {
        parent::__construct($shape);
        $this->symmetrical = $symmetrical;
        $this->reverse_behaviour = $reverse_behaviour;
        $this->scale_x = $scale_x;
        $this->scale_y = $scale_y;
    }

    function apply_bezier_segment(CompiledBezierSegment $source, MutableShape $target, $feature_width, $feature_height)
    {
        $feature_width *= $this->scale_x;
        $feature_height *= $this->scale_y;

        if ( $source->flags() & BezierSegmentFlags::PLAIN )
            return $this->skip_segment($source, $target);

        if ( $source->flags() & +BezierSegmentFlags::UNDERSIDE )
        {
            $feature_height = -$feature_height;
        }

        $feature_start = ($source->length() - $feature_width) / 2;
        $feature_end = $feature_start + $feature_width;

        $p0 = $source->point_at_length($feature_start);
        $target->line_to($p0);
        $p1 = $source->point_at_length($feature_end);
        $this->apply_line_segment($p0, $p1, $target, $feature_height);
        $target->line_to($source->finish());

    }
}
