<?php

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

    private static function load_path(DomElement $element, $filename)
    {
        $path = SvgDParser::parse_d($element->getAttribute("d"));
        if ( $path->empty() || $path->sub_paths()[0]->empty() )
        {
            user_error("Empty path data in $filename", E_USER_WARNING);
            return null;
        }

        // Flip y coordinates before applying
        if ( self::get_meta_flag($element, "flip-y") )
            $path->scale(1, -1);

        // Reverse the order of the points
        if ( self::get_meta_flag($element, "reverse") )
            $path->reverse();

        return $path;
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
        $element = $xpath->evaluate("//*[local-name()='path'][1]")[0];

        if ( !$element || !$element->hasAttribute("d") )
        {
            user_error("No path data in $filename", E_USER_WARNING);
            return new EdgeTypeNull();
        }

        $path = self::load_path($element, $filename);
        if ( !$path )
            return new EdgeTypeNull();

        // Edge class ["full", "repeating"]
        $type = self::get_meta_flag($element, "type", "repeating");
        switch ( $type )
        {
            default:
                user_error("$type edge types are not supported yet", E_USER_WARNING);

            case "repeating":
                // What to do on reverse edges (see constants in EdgeTypeRepeating)
                $reverse_behaviour = self::get_meta_flag($element, "reverse-behaviour", EdgeTypeRepeating::REVERSE_NORMAL);

                // Pattern is symmetrical (can be cut in half if needed)
                $symmetrical = self::get_meta_flag($element, "symmetrical");

                // How much to stretch the pattern by along the path
                $scale_x = self::get_meta_flag($element, "scale-x", 1);
                // How much to stretch the pattern by along the normal
                $scale_y = self::get_meta_flag($element, "scale-y", 1);

                $edge = new EdgeTypeRepeating($path->sub_paths()[0], $symmetrical, $reverse_behaviour, $scale_x, $scale_y);
                break;

            case "full":
                $edge = new EdgeTypeFull($path->sub_paths()[0]);
                break;

            case "flory":
                $other_element = $xpath->evaluate("//*[local-name()='path'][2]")[0];
                if ( !$other_element || !$other_element->hasAttribute("d") )
                {
                    user_error("Missing second flory path in $filename", E_USER_WARNING);
                    return new EdgeTypeNull();
                }
                $other_path = self::load_path($other_element, $filename);
                if ( !$other_path )
                    return new EdgeTypeNull();
                $other_path = SvgDParser::parse_d($other_element->getAttribute("d"));
                $edge = new EdgeTypeFlory($path->sub_paths()[0], $other_path->sub_paths()[0]);
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
    function __construct(PolyBezier $top, PolyBezier $bottom)
    {
        parent::__construct($top);
        $this->bottom = $this->normalized($bottom);
    }

    private function plain(MutableShape $target, CubicBezier $bez)
    {
        $target->cubic_to($bez->tan1, $bez->tan2, $bez->finish);
    }

    function apply_bezier_segment(CompiledBezierSegment $source, MutableShape $target, $feature_width,
        $feature_height, $direction = 1, $last_segment_data = null)
    {
        if ( $source->flags() & BezierSegmentFlags::PLAIN )
        {
            $this->skip_segment($source, $target);
            return;
        }

        $count = round($source->length() / $feature_width);

        if ( $count == 0 )
        {
            $this->skip_segment($source, $target);
            return;
        }

        $size = $source->length() / $count;

        $seg_i = 0;

        for ( $i = 1; $i < $count; $i++ )
        {
            $direction *= -1;

            $l0 = $i * $size - $feature_height / 2;
            $l1 = $i * $size + $feature_height / 2;

            $seg0 = $source->segment_at_length($l0);
            $this->resolve_last($last_segment_data, $seg0, $source, $target);

            $p0 = $seg0->point();

            $seg1 = $source->segment_at_length($l1);
            $p1 = $seg1->point();

            $shape = $direction == 1 ? $this->shape : $this->bottom;
            $this->do_apply_line_segment($shape, $p0, $p1, $target, $direction*$feature_height);

            $last_segment_data = $seg1;
            $seg_i = $seg1->index+1;
        }

        return [$direction, $last_segment_data];
    }

    private function resolve_last(SegmentAtData $last_segment_data, SegmentAtData $seg0, CompiledBezierSegment $source, MutableShape $target)
    {
        if ( !$last_segment_data || $last_segment_data->index > $seg0->index )
            return;

        if ( $last_segment_data->index == $seg0->index )
        {
            $chunk = $seg0->segment->multi_split([$last_segment_data->t, $seg0->t])[1];
            $this->plain($target, $chunk);

            return;
        }

        $this->plain($target, $last_segment_data->split()[1]);
        $i = $last_segment_data->index + 1;

        if ( $i == 0 )
            return;

        for ( ; $i < $seg0->index; $i++ )
        {
            $this->plain($target, $source->sub_segments()[$i]);
        }

        if ( $seg0->t > 0 )
            $this->plain($target, $seg0->split()[0]);
    }

    function apply_bezier(CompiledPolyBezier $source, MutableShape $target, $feature_width, $feature_height)
    {


        $direction = 1;
        $prev_segment = $source->segments()[$source->size()-1];

        if ( ($prev_segment->flags() & BezierSegmentFlags::PLAIN) == 0 )
        {
            $p0 = $prev_segment->point_at_length($prev_segment->length() - $feature_height/M_SQRT2);
            $target->move_to($p0);
        }
        else
        {
            $target->move_to($source->start());
        }

        $last_segment_data = null;

        foreach ( $source->segments() as $seg )
        {
            if ( $last_segment_data )
            {
                if ( $last_segment_data->t < 1 )
                    $this->plain($target, $last_segment_data->split()[1]);

                for ( $i = $last_segment_data->index; $i < $prev_segment->size(); $i++ )
                    $this->plain($target, $prev_segment->sub_segments()[$i]);
            }

            if ( $seg->flags() & BezierSegmentFlags::PLAIN )
            {
                $this->skip_segment($seg, $target);
                $last_segment_data = null;
                $prev_segment = $seg;
                continue;
            }

            $l0 = $prev_segment->length() - $feature_height/M_SQRT2;
            $seg0 = $prev_segment->segment_at_length($l0);
            $p0 = $seg0->point();

            $l1 = $feature_height/M_SQRT2;
            $seg1 = $seg->segment_at_length($l1);
            $last_segment_data = $seg1;
            $p1 = $seg1->point();

            $this->apply_line_segment($p0, $p1, $target, $feature_height * $direction);
            $prev_segment = $seg;
            list($direction, $last_segment_data) = $this->apply_bezier_segment($seg, $target, $feature_width, $feature_height, $direction, $last_segment_data);
            $direction *= -1;
        }

        if ( $last_segment_data )
        {
            $seg = $source->segments()[$source->size()-1];
            if ( ($seg->flags() & BezierSegmentFlags::PLAIN) == 0 )
            {
                $l0 = $seg->length() - $feature_height/M_SQRT2;
                $seg0 = $seg->segment_at_length($l0);
                if ( $last_segment_data->parent === $seg0->parent )
                    $this->resolve_last($last_segment_data, $seg0, $seg, $target);
            }
        }

        $target->close();
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
