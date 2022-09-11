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

require_once(__dir__ . "/../bezier.inc");

abstract class ShieldShape
{

    static function from_layout($layout)
    {
        if ( $layout->section == "quarter" )
            return new RectShieldShape();
        else
            return ShieldShape::from_name($layout->shape);
    }

    static function from_name($name)
    {
        switch ( $name )
        {
            case "flag":
            case "chief":
            case "square":
            case "stamp":
            case "swatch":
                return new RectShieldShape();
            case "swiss":
                return new HeaterShieldShape(new ShieldPos(276.37,-146.6), new ShieldPos(500,-590.97), new ShieldPos(500,-931.29), 0, 70);
            case "spanish":
                return new HeaterShieldShape(new ShieldPos(272.06,0), new ShieldPos(492.86,-199.37), new ShieldPos(492.86,-445.07));
            case "sca":
                return new HeaterShieldShape(new ShieldPos(111.40432,-72.288697), new ShieldPos(440.8494,-378.93457), new ShieldPos(482.89269,-692.96463), 10);
//             case "english":
//                 return new HeaterShieldShape(new ShieldPos(13,-26.5), new ShieldPos(375.5,169), new ShieldPos(413,-3.3830906), 0);
            case "french":
                return new HeaterShieldShape(new ShieldPos(100,-20), new ShieldPos(493,-60), new ShieldPos(500,-122.7), 90);
            case "circle":
            case "oval":
                return new EllipseShieldShape();
            case "lozenge":
                return new LozengeShieldShape();
            case "heater":
            default:
                return new HeaterShieldShape(new ShieldPos(258, -60), new ShieldPos(501,-180), new ShieldPos(500, -378));
        }

    }

    function svg_path(ShieldLayout $layout, $inset, $reversed = false, MutableShape $builder = null)
    {
        if ( $builder == null )
            $builder = new MutableShape();

        $this->on_svg_path($layout, $inset, $reversed, $builder);

        return $builder;
    }

    abstract protected function on_svg_path(ShieldLayout $layout, $inset, $reversed, MutableShape $builder);

    function compony(ShieldLayout $layout, $inset, $width, MutableShape $builder = null)
    {
        if ( $builder == null )
            $builder = new MutableShape();

        $this->on_compony($layout, $inset, $width, $builder);

        return $builder;
    }

    protected static function radial_compony(ShieldLayout $layout, MutableShape $builder, $count = 16, $angle_start = 0, $angle_delta = 0, $direction = 1)
    {
        $cx = $layout->cross_point->x;
        $cy = $layout->cross_point->y;

        $length = 2000;
        $angle_step = M_PI * 2 / $count;
        if ( $angle_delta == 0 )
            $angle_delta = M_PI * 2;
        $builder->move_to($cx, $cy);

        for ( $i = 0; $angle_step * $i < $angle_delta; $i += 2 )
        {
            $builder->line_to(
                $cx + cos($direction * $angle_step * $i + $angle_start) * $length,
                $cy + sin($direction * $angle_step * $i + $angle_start) * $length
            );
            $builder->line_to(
                $cx + cos($direction * $angle_step * ($i+1) + $angle_start) * $length,
                $cy + sin($direction * $angle_step * ($i+1) + $angle_start) * $length
            );
            $builder->line_to($cx, $cy);
        }

        $builder->close();
    }

    protected static function rect_compony(ShieldRect $rect, MutableShape $builder, $width, $top = true, $bottom = true, $left = true, $right = true)
    {
        $count_x = 4;
        $count_y = 4;
        $step_x = $rect->size->width / $count_x;
        $step_y = $rect->size->height / $count_y;
        $width_diag = $width * M_SQRT2;

        if ( $top )
        {
            $builder->move_to($rect->left() + $width, $rect->top() + $width);
            $builder->line_to(-$width, -$width, true);
            for ( $i = 0; $i < $count_x - 1; $i++ )
            {
                $builder->line_to($step_x, 0, true);
                $builder->line_to(0, $width * ($i % 2 ? -1 : 1), true);
            }
//             $builder->move_to($rect->right() - $width, $rect->top() + $width);
//             $builder->line_to($width, -$width, true);
            $builder->close();
        }

        if ( $left )
        {
            $builder->line_to($rect->left() + $width, $rect->bottom() - $width);
            $builder->line_to(-$width, $width, true);
            for ( $i = 0; $i < $count_y -1; $i++ )
            {
                $builder->line_to(0, -$step_y, true);
                $builder->line_to($width * ($i % 2 ? -1 : 1), 0, true);
            }
            $builder->close();
        }

        if ( $bottom )
        {
            $builder->move_to($step_x + $rect->left(), $rect->bottom());
            for ( $i = 1; $i < $count_x; $i++ )
            {
                $builder->line_to(0, $width * ($i % 2 ? -1 : 1), true);
                $builder->line_to($step_x, 0, true);
            }
            $builder->line_to($rect->right() - $width, $rect->bottom() - $width);
            $builder->line_to($width, $width, true);
            $builder->close();
        }

        if ( $right )
        {
            $builder->move_to($rect->right(), $rect->bottom() - $step_y);
            for ( $i = 1; $i < $count_x; $i++ )
            {
                $builder->line_to($width * ($i % 2 ? -1 : 1), 0, true);
                $builder->line_to(0, -$step_y, true);
            }
            $builder->line_to($rect->right() - $width, $width + $rect->top());
            $builder->line_to($width, -$width, true);
            $builder->close();
        }
    }

    protected function on_compony(ShieldLayout $layout, $inset, $width, MutableShape $builder)
    {
        return self::radial_compony($layout, $builder);
    }

    function charge_placement(ShieldLayout $layout, $margin, $width)
    {
        $points = $this->on_charge_placement($layout, $margin, $width);
        if ( count($points) == 0 )
            return "x";
        $plac = [];
        $padding = $width / 5;
        $size = new ShieldSize($width-$padding, $width-$padding);
        foreach ( $points as $point )
            $plac[] = new ShieldRect($point, $size);
        return str_repeat("/", count($points)-1) . implode(":", $plac);
    }

    protected function on_charge_placement(ShieldLayout $layout, $margin, $width)
    {
        return [];
    }
}

class HeaterShieldShape extends ShieldShape
{

    function __construct(ShieldPos $tan1, ShieldPos $tan2, ShieldPos $dest, $margin_bottom = 0, $margin_top = 0)
    {
        // Cubic parameters from the tip, right side
        $this->tan1 = $tan1;
        $this->tan2 = $tan2;
        $this->dest = $dest;

        $this->margin_bottom = $margin_bottom;
        $this->margin_top = $margin_top;
    }

    protected function on_svg_path_full(ShieldLayout $layout, $inset, $reversed, MutableShape $builder)
    {
        $tip = new ShieldPos($layout->cross_point->x, $layout->size->height - $inset - $this->margin_bottom);
        $scale = ($layout->size->width - 2*$inset) / 1000;
        list($br_t1, $br_t2, $br) = $this->scaled_curve($tip, $scale, 1);
        list($bl_t1, $bl_t2, $bl) = $this->scaled_curve($tip, $scale, -1);

        $flags = BezierSegmentFlags::SYMMETRICAL;

        if ( $reversed )
            $flags |= BezierSegmentFlags::UNDERSIDE;

        $builder
            ->move_to($layout->size->width - $inset, $inset + $this->margin_top)
            ->tag_segment($flags)
            ->line_to($br->x, $br->y)
            ->cubic_to($br_t2->x, $br_t2->y, $br_t1->x, $br_t1->y, $tip->x, $tip->y)
            ->cubic_to($bl_t1->x, $bl_t1->y, $bl_t2->x, $bl_t2->y, $bl->x, $bl->y)
            ->line_to($inset, $inset + $this->margin_top)
            ->tag_segment($flags)
            ->line_to($layout->size->width - $inset, $inset + $this->margin_top)
            ->close()
        ;

        if ( $reversed )
            $builder->reverse_last();
    }

    protected function on_svg_path_sinister(ShieldLayout $layout, $inset, $reversed, MutableShape $builder, $margin_top)
    {
        $scale = ($layout->size->width - 2*$inset) / 500;
        $tip = new ShieldPos($inset, $layout->size->height - ($inset + $this->margin_bottom) * 1.5 );
        list($br_t1, $br_t2, $br) = $this->scaled_curve($tip, $scale, 1);

        if ( $br->y < $inset + $margin_top + 50 )
            $br = new  ShieldPos($layout->size->width - $inset, $inset + $margin_top);

        $flags = BezierSegmentFlags::SYMMETRICAL;

        if ( $reversed )
            $flags |= BezierSegmentFlags::UNDERSIDE;

        $builder
            ->move_to($inset, $inset + $margin_top)
            ->tag_segment($flags)
            ->line_to($layout->size->width - $inset, $inset + $margin_top)
            ->tag_segment($flags)
            ->line_to($br->x, $br->y)
            ->cubic_to($br_t2->x, $br_t2->y, $br_t1->x, $br_t1->y, $tip->x, $tip->y)
            ->tag_segment($flags)
            ->close()
        ;

        if ( $reversed )
            $builder->reverse_last();
    }

    protected function on_svg_path_dexter(ShieldLayout $layout, $inset, $reversed, MutableShape $builder, $margin_top)
    {
        $scale = ($layout->size->width - 2*$inset) / 500;
        $tip = new ShieldPos($layout->size->width - $inset, $layout->size->height - ($inset + $this->margin_bottom) * 1.5 );
        list($bl_t1, $bl_t2, $bl) = $this->scaled_curve($tip, $scale, -1);

        if ( $bl->y < $inset + $margin_top + 50 )
            $bl = new  ShieldPos($inset + $margin_top, $inset);

        $flags = BezierSegmentFlags::SYMMETRICAL;

        if ( $reversed )
            $flags |= BezierSegmentFlags::UNDERSIDE;

        $builder
            ->move_to($layout->size->width - $inset, $inset + $margin_top)
            ->tag_segment($flags)
            ->line_to($tip->x, $tip->y)
            ->tag_segment($flags)
            ->cubic_to($bl_t1->x, $bl_t1->y, $bl_t2->x, $bl_t2->y, $bl->x, $bl->y)
            ->line_to($inset, $inset + $margin_top)
            ->tag_segment($flags)
            ->line_to($layout->size->width - $inset, $inset + $margin_top)
            ->close()
        ;

        if ( $reversed )
            $builder->reverse_last();
    }

    protected function on_svg_path(ShieldLayout $layout, $inset, $reversed, MutableShape $builder)
    {
        switch ( $layout->section )
        {
            case "dexter":
                return $this->on_svg_path_dexter($layout, $inset, $reversed, $builder, $this->margin_top);
            case "sinister":
                return $this->on_svg_path_sinister($layout, $inset, $reversed, $builder, $this->margin_top);
            case "quarter-chief-dexter":
            case "quarter-chief-sinister":
                $ratio = $layout->size->height / $layout->size->width * 1.5;
                return RectShieldShape::rect_path($layout->rect_area()->margin(0, $this->margin_top * $ratio, 0, 0), $inset, $reversed, $builder);
            case "quarter-base-dexter":
                return $this->on_svg_path_dexter($layout, $inset, $reversed, $builder, 0);
            case "quarter-base-sinister":
                return $this->on_svg_path_sinister($layout, $inset, $reversed, $builder, 0);
            default:
                return $this->on_svg_path_full($layout, $inset, $reversed, $builder);
        }
    }

    protected function scaled_curve($tip, $scale, $direction)
    {
        return [
            new ShieldPos($tip->x + $direction * $this->tan1->x * $scale, $tip->y + $this->tan1->y * $scale),
            new ShieldPos($tip->x + $direction * $this->tan2->x * $scale, $tip->y + $this->tan2->y * $scale),
            new ShieldPos($tip->x + $direction * $this->dest->x * $scale, $tip->y + $this->dest->y * $scale),
        ];
    }

    protected function on_compony(ShieldLayout $layout, $inset, $width, MutableShape $builder)
    {
        switch ( $layout->section )
        {
            case "dexter":
            case "quarter-base-dexter":
                self::rect_compony($layout->rect_area()->margin($inset), $builder, $width, true, false, false, false);
                $start = atan2(-$layout->cross_point->y, $layout->size->width - $layout->cross_point->x);
                $finish = atan2(-$layout->cross_point->y, -$layout->cross_point->x) + M_PI * 2;
                self::radial_compony($layout, $builder, 16, $start, $finish - $start);
                break;
            case "sinister":
            case "quarter-base-sinister":
                self::rect_compony($layout->rect_area()->margin($inset), $builder, $width, true, false, false, false);
                $start1 = atan2(-$layout->cross_point->y, $layout->size->width - $layout->cross_point->x);
                $finish = atan2(-$layout->cross_point->y, -$layout->cross_point->x) + M_PI * 2;
                self::radial_compony($layout, $builder, 16, $start1, $finish - $start1 - M_PI / 36);
                break;
            case "quarter-chief-dexter":
            case "quarter-chief-sinister":
                self::rect_compony($layout->rect_area()->margin($inset), $builder, $width);
                break;
            default:
                self::rect_compony($layout->rect_area()->margin($inset), $builder, $width, true, false, false, false);
                self::radial_compony($layout, $builder, 16, -M_PI/4, M_PI * 3 / 2);
                break;
        }
    }


    protected function on_charge_placement(ShieldLayout $layout, $margin, $width)
    {
        $w = $layout->size->width - 2 * $margin;
        $dy = ($layout->cross_point->y - $this->margin_top - $margin) / 2;
        $mid = $margin + $width / 2;
        $tip = new ShieldPos($layout->size->width / 2, $layout->size->height - $this->margin_bottom - $mid);
        $scale = ($layout->size->width - 2 * $mid) / 1000;
        $bottom = $layout->size->height - $this->margin_bottom - $mid + $this->dest->y * $scale;
        $n_bez = 2;
        $spots = [
            new ShieldPos($w * 1 / 4 + $margin, $mid + $this->margin_top),
            new ShieldPos($w * 2 / 4 + $margin, $mid + $this->margin_top),
            new ShieldPos($w * 3 / 4 + $margin, $mid + $this->margin_top),

            $tip,
        ];

        if ( $layout->cross_point->y < $bottom )
        {
            $spots[] = new ShieldPos($mid, $layout->cross_point->y );
            $spots[] = new ShieldPos($layout->size->width - $mid, $layout->cross_point->y );
        }
        else
            $n_bez++;

        if ( $layout->cross_point->y - $dy < $bottom )
        {
            $spots[] = new ShieldPos($mid, $layout->cross_point->y - $dy );
            $spots[] = new ShieldPos($layout->size->width - $mid, $layout->cross_point->y - $dy );
        }
        else
            $n_bez++;

        if ( $layout->cross_point->y + $dy < $bottom )
        {
            $spots[] = new ShieldPos($mid, $layout->cross_point->y + $dy );
            $spots[] = new ShieldPos($layout->size->width - $mid, $layout->cross_point->y + $dy );
        }
        else
            $n_bez++;

        $bez_l = $this->scaled_curve($tip, $scale, -1);
        $bez_r = $this->scaled_curve($tip, $scale, 1);
        $t_step = 1 / $n_bez;
        for ( $i = 1; $i <= $n_bez; $i++ )
        {
            $spots[] = CubicBezier::bezier_solve($t_step * $i, $tip, $bez_r[0], $bez_r[1], $bez_r[2]);
            $spots[] = CubicBezier::bezier_solve($t_step * $i, $tip, $bez_l[0], $bez_l[1], $bez_l[2]);
        }

        return $spots;
    }
}

class RectShieldShape extends ShieldShape
{
    static function rect_path(ShieldRect $area, $inset, $reversed, MutableShape $builder)
    {
        $flags = BezierSegmentFlags::SYMMETRICAL;

        if ( $reversed )
            $flags |= BezierSegmentFlags::UNDERSIDE;

        $builder
            ->move_to($area->left() + $inset, $area->top() + $inset)
            ->tag_segment($flags)
            ->line_to($area->right() - $inset, $area->top() + $inset)
            ->tag_segment($flags)
            ->line_to($area->right() - $inset, $area->bottom() - $inset)
            ->tag_segment($flags)
            ->line_to($area->left() + $inset, $area->bottom() - $inset)
            ->tag_segment($flags)
            ->close()
        ;

        if ( $reversed )
            $builder->reverse_last();
    }

    protected function on_svg_path(ShieldLayout $layout, $inset, $reversed, MutableShape $builder)
    {
        self::rect_path($layout->rect_area(), $inset, $reversed, $builder);
    }

    protected function on_compony(ShieldLayout $layout, $inset, $width, MutableShape $builder)
    {
        self::rect_compony($layout->rect_area()->margin($inset), $builder, $width);
    }

    protected function on_charge_placement(ShieldLayout $layout, $margin, $width)
    {
        $w = $layout->size->width - 2 * $margin;
        $h = $layout->size->height - 2 * $margin;
        $mid = $margin + $width / 2;
        return [
            new ShieldPos($w * 1 / 4 + $margin, $mid),
            new ShieldPos($w * 2 / 4 + $margin, $mid),
            new ShieldPos($w * 3 / 4 + $margin, $mid),

            new ShieldPos($layout->size->width / 2, $layout->size->height - $mid),

            new ShieldPos($mid, $h * 2 / 4 + $margin),
            new ShieldPos($layout->size->width - $mid, $h * 2 / 4 + $margin),

            new ShieldPos($mid, $h * 1 / 4 + $margin),
            new ShieldPos($layout->size->width - $mid, $h * 1 / 4 + $margin),

            new ShieldPos($mid, $h * 3 / 4 + $margin),
            new ShieldPos($layout->size->width - $mid, $h * 3 / 4 + $margin),

            new ShieldPos($w * 1 / 4 + $margin, $layout->size->height - $mid),
            new ShieldPos($w * 3 / 4 + $margin, $layout->size->height - $mid),
        ];
    }
}


class EllipseShieldShape extends ShieldShape
{
    protected function on_svg_path_dexter(ShieldLayout $layout, $inset, $reversed, MutableShape $builder)
    {
        $flags = BezierSegmentFlags::NORMAL;

        if ( $reversed )
            $flags |= BezierSegmentFlags::UNDERSIDE;

        $left = $inset;
        $right = $layout->size->width - $inset;

        $top = $inset;
        $bottom = $layout->size->height - $inset;

        $rx = ($right - $left);
        $ry = ($bottom - $top) / 2;

        $handle_x = $rx * 0.55;
        $handle_y = $ry * 0.55;

        $mid_y = ($top + $bottom) / 2;


        $builder->move_to($right, $bottom)
            ->tag_segment($flags)
            ->cubic_to(
                $right - $handle_x, $bottom,
                $left, $mid_y + $handle_y,
                $left, $mid_y
            )
            ->cubic_to(
                $left, $mid_y - $handle_y,
                $right - $handle_x, $top,
                $right, $top
            )
            ->tag_segment($flags)
            ->close()
        ;

        if ( $reversed )
            $builder->reverse_last();
    }

    protected function on_svg_path_sinister(ShieldLayout $layout, $inset, $reversed, MutableShape $builder)
    {
        $flags = BezierSegmentFlags::NORMAL;

        if ( $reversed )
            $flags |= BezierSegmentFlags::UNDERSIDE;

        $left = $inset;
        $right = $layout->size->width - $inset;

        $top = $inset;
        $bottom = $layout->size->height - $inset;

        $rx = ($right - $left);
        $ry = ($bottom - $top) / 2;

        $handle_x = $rx * 0.55;
        $handle_y = $ry * 0.55;

        $mid_y = ($top + $bottom) / 2;


        $builder->move_to($left, $bottom)
            ->tag_segment($flags)
            ->cubic_to(
                $left + $handle_x, $bottom,
                $right, $mid_y + $handle_y,
                $right, $mid_y
            )
            ->cubic_to(
                $right, $mid_y - $handle_y,
                $left + $handle_x, $top,
                $left, $top
            )
            ->tag_segment($flags)
            ->close()
        ;

        if ( !$reversed )
            $builder->reverse_last();
    }

    protected function on_svg_path_full(ShieldLayout $layout, $inset, $reversed, MutableShape $builder)
    {
        $left = $inset;
        $right = $layout->size->width - $inset;

        $top = $inset;
        $bottom = $layout->size->height - $inset;

        $rx = ($right - $left) / 2;
        $ry = ($bottom - $top) / 2;

        $handle_x = $rx * 0.55;
        $handle_y = $ry * 0.55;

        $mid_x = ($right + $left) / 2;
        $mid_y = ($top + $bottom) / 2;

        $flags = BezierSegmentFlags::NORMAL;

        if ( $reversed )
            $flags |= BezierSegmentFlags::UNDERSIDE;

        $builder->move_to($right, $mid_y)
            ->tag_segment($flags)
            ->cubic_to(
                $right, $mid_y + $handle_y,
                $mid_x + $handle_x, $bottom,
                $mid_x, $bottom
            )
            ->tag_segment($flags)
            ->cubic_to(
                $mid_x - $handle_x, $bottom,
                $left, $mid_y + $handle_y,
                $left, $mid_y
            )
            ->tag_segment($flags)
            ->cubic_to(
                $left, $mid_y - $handle_y,
                $mid_x - $handle_x, $top,
                $mid_x, $top
            )
            ->tag_segment($flags)
            ->cubic_to(
                $mid_x + $handle_x, $top,
                $right, $mid_y - $handle_y,
                $right, $mid_y
            )
            ->close()
        ;

        if ( $reversed )
            $builder->reverse_last();
    }

    protected function on_svg_path_quarter(ShieldLayout $layout, $inset, $reversed, MutableShape $builder, $quarter)
    {
        $flags = BezierSegmentFlags::NORMAL;

        if ( $reversed )
            $flags |= BezierSegmentFlags::UNDERSIDE;

        $left = $inset;
        $right = $layout->size->width - $inset;

        $top = $inset;
        $bottom = $layout->size->height - $inset;

        $rx = ($right - $left);
        $ry = ($bottom - $top);


        $handle_x = $rx * 0.55;
        $handle_y = $ry * 0.55;

        switch ( $quarter )
        {
            case 1:
                $ymult = 1;
                $xmult = 1;
                $reversed = !$reversed;
                break;
            case 2:
                $ymult = 1;
                $xmult = -1;
                break;
            case 3:
                $ymult = -1;
                $xmult = 1;
                break;
            case 4:
                $ymult = -1;
                $xmult = -1;
                $reversed = !$reversed;
                break;
        }

        if ( $xmult == -1 )
            list($left, $right) = [$right, $left];

        if ( $ymult == -1 )
            list($top, $bottom) = [$bottom, $top];


        $builder->move_to($right, $top)
            ->tag_segment($flags)
            ->cubic_to(
                $right - $xmult * $handle_x, $top,
                $left, $bottom - $ymult * $handle_y,
                $left, $bottom
            )
            ->tag_segment($flags)
            ->horizontal($right)
            ->tag_segment($flags)
            ->close()
        ;

        if ( $reversed )
            $builder->reverse_last();
    }

    protected function on_svg_path(ShieldLayout $layout, $inset, $reversed, MutableShape $builder)
    {
        switch ( $layout->section )
        {
            case "dexter":
                return $this->on_svg_path_dexter($layout, $inset, $reversed, $builder);
            case "sinister":
                return $this->on_svg_path_sinister($layout, $inset, $reversed, $builder);
            case "quarter-chief-dexter":
                return $this->on_svg_path_quarter($layout, $inset, $reversed, $builder, 1);
            case "quarter-chief-sinister":
                return $this->on_svg_path_quarter($layout, $inset, $reversed, $builder, 2);
            case "quarter-base-dexter":
                return $this->on_svg_path_quarter($layout, $inset, $reversed, $builder, 3);
            case "quarter-base-sinister":
                return $this->on_svg_path_quarter($layout, $inset, $reversed, $builder, 4);
            default:
                return $this->on_svg_path_full($layout, $inset, $reversed, $builder);
        }
    }
}


class LozengeShieldShape extends ShieldShape
{
    protected function on_svg_path(ShieldLayout $layout, $inset, $reversed, MutableShape $builder)
    {
        switch ( $layout->section )
        {
            case "dexter":
                $points = [
                    new ShieldPos($layout->size->width - $inset, 2 * $inset),
                    new ShieldPos($inset, $layout->size->height / 2),
                    new ShieldPos($layout->size->width - $inset, $layout->size->height - 2 * $inset),
                ];
                break;
            case "sinister":
                $points = [
                    new ShieldPos($inset, 2 * $inset),
                    new ShieldPos($layout->size->width - $inset, $layout->size->height / 2),
                    new ShieldPos($inset, $layout->size->height - 2 * $inset),
                ];
                break;
            case "quarter-chief-dexter":
                $points = [
                    new ShieldPos($layout->size->width - $inset, $inset * 2),
                    new ShieldPos($inset * 2, $layout->size->height - $inset),
                    new ShieldPos($layout->size->width - $inset, $layout->size->height - $inset),
                ];
                break;
            case "quarter-chief-sinister":
                $points = [
                    new ShieldPos($inset, $inset * 2),
                    new ShieldPos($layout->size->width - $inset * 2, $layout->size->height - $inset),
                    new ShieldPos($inset, $layout->size->height - $inset),
                ];
                break;
            case "quarter-base-dexter":
                $points = [
                    new ShieldPos($layout->size->width - $inset, $layout->size->height - $inset * 2),
                    new ShieldPos($inset * 2, $inset),
                    new ShieldPos($layout->size->width - $inset, $inset),
                ];
                break;
            case "quarter-base-sinister":
                $points = [
                    new ShieldPos($inset, $layout->size->height - $inset * 2),
                    new ShieldPos($layout->size->width - $inset * 2, $inset),
                    new ShieldPos($inset, $inset),
                ];
                break;
            default:
                $points = [
                    new ShieldPos($layout->size->width / 2, $inset),
                    new ShieldPos($layout->size->width - $inset, $layout->size->height / 2),
                    new ShieldPos($layout->size->width / 2, $layout->size->height - $inset),
                    new ShieldPos($inset, $layout->size->height / 2),
                ];
                break;
        }


        $flags = BezierSegmentFlags::SYMMETRICAL;

        if ( $reversed )
            $flags |= BezierSegmentFlags::UNDERSIDE;

        $builder->move_to(array_shift($points));

        foreach ( $points as $p )
            $builder->tag_segment($flags)->line_to($p);

        $builder->tag_segment($flags);
        $builder->close();

        if ( $reversed )
            $builder->reverse_last();
    }
}
