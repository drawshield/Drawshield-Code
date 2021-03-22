<?php

abstract class ShieldShape
{
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
            case "heather":
            default:
                return new HeatherShieldShape(new ShieldPos(258, -60), new ShieldPos(501,-180), new ShieldPos(500, -378));
        }

    }

    function svg_path(ShieldLayout $layout, $inset, $region = "", $reversed = false, SvgPathBuilder $builder = null)
    {
        if ( $builder == null )
            $builder = new SvgPathBuilder();

        $this->on_svg_path($layout, $inset, $region, $reversed, $builder);

        return $builder;
    }

    abstract protected function on_svg_path(ShieldLayout $layout, $inset, $region, $reversed, SvgPathBuilder $builder);
}

class HeatherShieldShape extends ShieldShape
{

    function __construct(ShieldPos $tan1, ShieldPos $tan2, ShieldPos $dest)
    {
        // Cubic parameters from the tip, right side
        $this->tan1 = $tan1;
        $this->tan2 = $tan2;
        $this->dest = $dest;
    }

    protected function on_svg_path_full(ShieldLayout $layout, $inset, $reversed, SvgPathBuilder $builder)
    {
        $tip = new ShieldPos($layout->center_point->x, $layout->size->height - $inset);
        $scale = ($layout->size->width - 2*$inset) / 1000;
        list($br_t1, $br_t2, $br) = $this->scaled_curve($tip, $scale, 1);
        list($bl_t1, $bl_t2, $bl) = $this->scaled_curve($tip, $scale, -1);


        if ( $reversed )
        {
            $builder
                ->move($layout->size->width - $inset, $inset)
                ->line($inset, $inset)
                ->line($bl->x, $bl->y)
                ->cubic($bl_t2->x, $bl_t2->y, $bl_t1->x, $bl_t1->y, $tip->x, $tip->y)
                ->cubic($br_t1->x, $br_t1->y, $br_t2->x, $br_t2->y, $br->x, $br->y)
                ->close()
            ;
        }
        else
        {
            $builder
                ->move($inset, $inset)
                ->line($layout->size->width - $inset, $inset)
                ->line($br->x, $br->y)
                ->cubic($br_t2->x, $br_t2->y, $br_t1->x, $br_t1->y, $tip->x, $tip->y)
                ->cubic($bl_t1->x, $bl_t1->y, $bl_t2->x, $bl_t2->y, $bl->x, $bl->y)
                ->close()
            ;
        }
    }

    protected function on_svg_path_sinister(ShieldLayout $layout, $inset, $reversed, SvgPathBuilder $builder)
    {
        $scale = ($layout->size->width - 2*$inset) / 500;
        $tip = new ShieldPos($inset, $layout->size->height - $inset * 1.5);
        list($br_t1, $br_t2, $br) = $this->scaled_curve($tip, $scale, 1);

        if ( $reversed )
        {
            $builder
                ->move($layout->size->width - $inset, $inset)
                ->line($inset, $inset)
                ->line($tip->x, $tip->y)
                ->cubic($br_t1->x, $br_t1->y, $br_t2->x, $br_t2->y, $br->x, $br->y)
                ->close()
            ;
        }
        else
        {
            $builder
                ->move($inset, $inset)
                ->line($layout->size->width - $inset, $inset)
                ->line($br->x, $br->y)
                ->cubic($br_t2->x, $br_t2->y, $br_t1->x, $br_t1->y, $tip->x, $tip->y)
                ->close()
            ;
        }
    }

    protected function on_svg_path_dexter(ShieldLayout $layout, $inset, $reversed, SvgPathBuilder $builder)
    {
        $scale = ($layout->size->width - 2*$inset) / 500;
        $tip = new ShieldPos($layout->size->width - $inset, $layout->size->height - $inset * 1.5);
        list($bl_t1, $bl_t2, $bl) = $this->scaled_curve($tip, $scale, -1);

        if ( $reversed )
        {
            $builder
                ->move($layout->size->width - $inset, $inset)
                ->line($inset, $inset)
                ->line($bl->x, $bl->y)
                ->cubic($bl_t2->x, $bl_t2->y, $bl_t1->x, $bl_t1->y, $tip->x, $tip->y)
                ->close()
            ;
        }
        else
        {
            $builder
                ->move($inset, $inset)
                ->line($layout->size->width - $inset, $inset)
                ->line($tip->x, $tip->y)
                ->cubic($bl_t1->x, $bl_t1->y, $bl_t2->x, $bl_t2->y, $bl->x, $bl->y)
                ->close()
            ;
        }
    }

    protected function on_svg_path(ShieldLayout $layout, $inset, $region, $reversed, SvgPathBuilder $builder)
    {
        if ( strpos($region, "dexter") !== false )
            $this->on_svg_path_dexter($layout, $inset, $reversed, $builder);
        else if ( strpos($region, "sinister") !== false )
            $this->on_svg_path_sinister($layout, $inset, $reversed, $builder);
        else
            $this->on_svg_path_full($layout, $inset, $reversed, $builder);
    }

    protected function scaled_curve($tip, $scale, $direction)
    {
        return [
            new ShieldPos($tip->x + $direction * $this->tan1->x * $scale, $tip->y + $this->tan1->y * $scale),
            new ShieldPos($tip->x + $direction * $this->tan2->x * $scale, $tip->y + $this->tan2->y * $scale),
            new ShieldPos($tip->x + $direction * $this->dest->x * $scale, $tip->y + $this->dest->y * $scale)
        ];
    }
}

class RectShieldShape extends ShieldShape
{

    protected function on_svg_path(ShieldLayout $layout, $inset, $region, $reversed, SvgPathBuilder $builder)
    {
        if ( $reversed )
        {
            $builder
                ->move($layout->size->width - $inset, $inset)
                ->line($inset, $inset)
                ->line($inset, $layout->size->height - $inset)
                ->line($layout->size->width - $inset, $layout->size->height - $inset)
                ->close()
            ;
        }
        else
        {
            $builder
                ->move($inset, $inset)
                ->line($layout->size->width - $inset, $inset)
                ->line($layout->size->width - $inset, $layout->size->height - $inset)
                ->line($inset, $layout->size->height - $inset)
                ->close()
            ;
        }
    }
}
