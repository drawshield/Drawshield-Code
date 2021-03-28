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

    static function load($name) : EdgeType
    {
        static $cache = [];

        if ( isset($cache[$name]) )
            return $cache[$name];

        $filename = __dir__ . "/$name.svg";
        if ( !file_exists($filename) )
            return new EdgeTypeNull();

        $dom = new DOMDocument();
        $dom->load($filename);
        $xpath = new DOMXPath($dom);
        $d = $xpath->evaluate("string(//path[1]/@d)");
        if ( !$d )
            return new EdgeTypeNull();

        $edge = new EdgeTypeRepeating(SvgDParser::parse_d($d)->sub_paths()[0]);
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

    function apply_line_segment(ShieldPos $p1, ShieldPos $p2, MutableShape $target, $height = null)
    {
        if ( $height === null  )
            $height = $p1->distance($p2);
        $angle = atan2($p2->y - $p1->y, $p2->x - $p1->x);
        $normal_angle = $angle + M_PI/2;
        $normal = new ShieldPos(cos($normal_angle), sin($normal_angle));

        foreach ( $this->shape->sub_segments() as $seg )
        {
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
    function apply_bezier_segment(CompiledBezierSegment $source, MutableShape $target, $feature_width, $feature_height)
    {
        if ( $source->flags() & BezierSegmentFlags::PLAIN )
        {
            foreach ( $source->sub_segments() as $bez )
                $target->cubic_to($bez->tan1, $bez->tan2, $bez->finish);

            return;
        }

        if ( $source->flags() & BezierSegmentFlags::REVERSE )
            $feature_height = -$feature_height;

        $count = round($source->length() / $feature_width);
        $size = $source->length() / $count;

        $p0 = $source->start();
        for ( $i = 1; $i <= $count; $i++ )
        {
            $p1 = $source->point_at_length($i * $size);
            $this->apply_line_segment($p0, $p1, $target, $feature_height);
            $p0 = $p1;
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
