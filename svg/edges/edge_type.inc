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

        $path = SvgDParser::parse_d($element->getAttribute("d"));
        if ( $path->empty() || $path->sub_paths()[0]->empty() )
        {
            user_error("Empty path data in $filename", E_USER_WARNING);
            return new EdgeTypeNull();
        }

        // Flip y coordinates before applying
        if ( self::get_meta_flag($element, "flip-y") )
            $path->scale(1, -1);

        // Reverse the order of the points
        if ( self::get_meta_flag($element, "reverse") )
            $path->reverse();

        // Edge class ["full", "repeating"]
        $type = self::get_meta_flag($element, "type", "repeating");
        if ( $type != "repeating" )
            user_error("$type edge types are not supported yet", E_USER_WARNING);


        // What to do on reverse edges (see constants in EdgeTypeRepeating)
        $reverse_behaviour = self::get_meta_flag($element, "reverse-behaviour", EdgeTypeRepeating::REVERSE_NORMAL);

        // Pattern is symmetrical (can be cut in half if needed)
        $symmetrical = self::get_meta_flag($element, "symmetrical");

        $edge = new EdgeTypeRepeating($path->sub_paths()[0], $symmetrical, $reverse_behaviour);
        $cache[$name] = $edge;
        return $edge;
    }
}

abstract class EdgeTypeBezier extends EdgeType
{
    function __construct(PolyBezier $shape)
    {
        $this->shape = clone $shape;
        $this->shape->translate(new ShieldPos(-$this->shape->start()->pos->x, $this->shape->start()->pos->y));
        $this->shape->scale(1/$this->shape->finish()->pos->x);
        $this->shape = $this->shape->compile()->segments()[0];
    }

    function apply_line_segment(ShieldPos $p1, ShieldPos $p2, MutableShape $target, $height = null, $scale = 1)
    {
        if ( $scale != 1 )
            $p2 = $p1->lerp($p2, $scale);

        if ( $height === null  )
            $height = $p1->distance($p2);

        $angle = atan2($p2->y - $p1->y, $p2->x - $p1->x);
        $normal_angle = $angle + M_PI/2;
        $normal = new ShieldPos(cos($normal_angle), sin($normal_angle));

        $count = floor(count($this->shape->sub_segments()) / $scale);

        for ( $i = 0;  $i < $count; $i++ )
        {
            $seg = $this->shape->sub_segments()[$i];
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

    function __construct(PolyBezier $shape, $can_be_halved, $reverse_behaviour)
    {
        parent::__construct($shape);
        $this->can_be_halved = $can_be_halved;
        $this->reverse_behaviour = $reverse_behaviour;
    }

    private function skip_segment(CompiledBezierSegment $source, MutableShape $target)
    {
        foreach ( $source->sub_segments() as $bez )
            $target->cubic_to($bez->tan1, $bez->tan2, $bez->finish);
    }

    function apply_bezier_segment(CompiledBezierSegment $source, MutableShape $target, $feature_width, $feature_height)
    {
        if ( $source->flags() & BezierSegmentFlags::PLAIN )
            return $this->skip_segment($source, $target);

        if ( $source->flags() & BezierSegmentFlags::REVERSE )
        {
            if ( $this->reverse_behaviour == self::REVERSE_FLIP )
                $feature_height = -$feature_height;
            else if ( $this->reverse_behaviour == self::REVERSE_PLAIN )
                return $this->skip_segment($source, $target);
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
        foreach ( $source->sub_segments() as $bez )
            $target->cubic_to($bez->tan1, $bez->tan2, $bez->finish);
    }
}


class EdgeTypeFlory extends EdgeTypeBezier
{
    function apply_bezier_segment(CompiledBezierSegment $source, MutableShape $target, $feature_width, $feature_height, $direction = 1)
    {
        if ( $source->flags() & BezierSegmentFlags::PLAIN )
        {
            foreach ( $source->sub_segments() as $bez )
                $target->cubic_to($bez->tan1, $bez->tan2, $bez->finish);

            return;
        }

        $count = round($source->length() / $feature_width);
        $size = $source->length() / $count;

        for ( $i = 1; $i < $count; $i++ )
        {
            $direction *= -1;
            $p0 = $source->point_at_length($i * $size - $feature_height / 2);
            $p1 = $source->point_at_length($i * $size + $feature_height / 2);
            $target->line_to($p0);
            $this->apply_line_segment($p0, $p1, $target, $feature_height * $direction);
        }

        return $direction;
    }

    function apply_bezier(CompiledPolyBezier $source, MutableShape $target, $feature_width, $feature_height)
    {
        $direction = 1;
        $prev_segment = $source->segments()[$source->size()-1];
        $p0 = $prev_segment->point_at_length($prev_segment->length() - $feature_height/M_SQRT2);
        $target->move_to($p0);

        foreach ( $source->segments() as $seg )
        {
            $p0 = $prev_segment->point_at_length($prev_segment->length() - $feature_height/M_SQRT2);
            $p1 = $seg->point_at_length($feature_height/M_SQRT2);
            $target->line_to($p0);
            $this->apply_line_segment($p0, $p1, $target, $feature_height * $direction);
            $prev_segment = $seg;
            $direction = $this->apply_bezier_segment($seg, $target, $feature_width, $feature_height, $direction);
            $direction *= -1;
        }

        $target->close();
    }
}
