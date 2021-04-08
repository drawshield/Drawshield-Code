<?php

require_once("size.inc");

class TransformMatrix
{
    private $mat = [];

    function __construct()
    {
        $this->to_identity();
    }

    function to_identity()
    {
        $this->mat = [
            1., 0., 0., 0.,
            0., 1., 0., 0.,
            0., 0., 1., 0.,
            0., 0., 0., 1.,
        ];
    }

    function get($row, $col)
    {
        return $this->mat[$row * 4 + $col];
    }

    function set($row, $col, $value)
    {
        $this->mat[$row * 4 + $col] = $this->scalar($value);
    }

    function scalar($value)
    {
        return (float)$value;
    }

    function a() { return $this->get(0, 0); }
    function set_a($v) {  $this->set(0, 0, $v); }
    function b() { return $this->get(0, 1); }
    function set_b($v) {  $this->set(0, 1, $v); }
    function c() { return $this->get(1, 0); }
    function set_c($v) {  $this->set(1, 0, $v); }
    function d() { return $this->get(1, 1); }
    function set_d($v) {  $this->set(1, 1, $v); }
    function tx(){ return $this->get(3, 0); }
    function set_tx($v) { $this->set(3, 0, $v); }
    function ty(){ return $this->get(3, 1); }
    function set_ty($v) { $this->set(3, 1, $v); }

    function scale($x, $y = null)
    {
        if ( $y === null )
            $y = $x;

        $m = new TransformMatrix();
        $m->set_a($x);
        $m->set_d($y);
        $this->multiply($m);
        return $this;
    }

    function translate($x, $y = null)
    {
        if ( $y === null )
            $y = $x;

        $m = new TransformMatrix();
        $m->set_tx($x);
        $m->set_ty($y);
        $this->multiply($m);
        return $this;
    }

    function row($i)
    {
        return [$this->get($i, 0), $this->get($i, 1), $this->get($i, 2), $this->get($i, 3)];
    }

    function column($i)
    {
        return [$this->get(0, $i), $this->get(1, $i), $this->get(2, $i), $this->get(3, $i)];
    }

    static function dot_product($v1, $v2)
    {
        return array_sum(array_map(function($a, $b){return $a * $b;}, $v1, $v2));
    }

    function apply(ShieldPos $p): ShieldPos
    {
        $vector3 = [$p->x, $p->y, 0, 1];
        return new ShieldPos(
            self::dot_product($this->column(0), $vector3),
            self::dot_product($this->column(1), $vector3)
        );
    }

    function apply_box(ShieldRect $r) : ShieldRect
    {
        $points = [
            $this->apply(new ShieldPos($r->left(), $r->top())),
            $this->apply(new ShieldPos($r->right(), $r->top())),
            $this->apply(new ShieldPos($r->right(), $r->bottom())),
            $this->apply(new ShieldPos($r->left(), $r->bottom())),
        ];

        $min = array_shift($points);
        $max = clone $min;
        foreach ( $points as $p )
        {
            if ( $p->x < $min->x )
                $min->x = $p->x;
            if ( $p->x > $max->x )
                $max->x = $p->x;
            if ( $p->y < $min->y )
                $min->y = $p->y;
            if ( $p->y > $max->y )
                $max->y = $p->y;
        }
        return ShieldRect::from_corners($min->x, $min->y, $max->x, $max->y);
    }

    static function rotation($radians): TransformMatrix
    {
        $m = new TransformMatrix();
        $m->set_a( cos($radians));
        $m->set_b(-sin($radians));
        $m->set_c( sin($radians));
        $m->set_d( cos($radians));

        return $m;
    }

    function multiplied(TransformMatrix $other): TransformMatrix
    {
        $m = new TransformMatrix();
        for ( $row = 0; $row < 4; $row++ )
        {
            for ( $col = 0; $col < 4; $col++)
                $m->set($row, $col, self::dot_product($other->row($row), $this->column($col)));
        }
        return $m;
    }

    function multiply(TransformMatrix $other)
    {
        $m = $this->multiplied($other);
        $this->mat = $m->mat;
        return $this;
    }

    function rotate($radians)
    {
        $this->multiply(TransformMatrix::rotation($radians));
        return $this;
    }

    function skew($x_rad, $y_rad)
    {
        $m = new TransformMatrix();
        $m->set_c(tan($x_rad));
        $m->set_b(tan($y_rad));
        return $this->multiply($m);
    }

    private function add_svg_transform_item_split($name, $params)
    {
        $params = preg_split("/(,|\s)\s*/", trim($params));

        switch ( $name )
        {
            case "translate":
                $this->translate(
                    $params[0],
                    $params[1] ?? 0
                );
                break;
            case "scale":
                $this->scale($params[0], $params[1] ?? null);
                break;
            case "rotate":
                $ang = $params[0];
                $x = $y = 0;
                if ( count($params) > 2 )
                {
                    $x = $params[1];
                    $y = $params[2];
                    $this->translate(-$x, -$y);
                    $this->rotate(-deg2rad($ang));
                    $this->translate($x, $y);
                }
                else
                {
                    $this->rotate(-deg2rad($ang));
                }
                break;
            case "skewX":
                $this->skew(deg2rad($params[0]), 0);
                break;
            case "skewY":
                $this->skew(0, deg2rad($params[0]));
                break;
            case "matrix":
                $m = new TransformMatrix();
                $m->set_a($params[0]);
                $m->set_b($params[1]);
                $m->set_c($params[2]);
                $m->set_d($params[3]);
                $m->set_tx($params[4]);
                $m->set_ty($params[5]);
                $this->multiply($m);
                break;
        }
    }

    function add_svg_transform($transform)
    {
        $matches = [];
        preg_match_all('/([a-zA-Z]+)\s*\(([^\)]*)\)/', $transform, $matches);
        for ( $i = 0; $i < count($matches[0]); $i++ )
            $this->add_svg_transform_item_split($matches[1][$i], $matches[2][$i]);
    }

    static function from_svg_transform($transform): TransformMatrix
    {
        $m = new TransformMatrix;
        $m->add_svg_transform($transform);
        return $m;
    }
}
