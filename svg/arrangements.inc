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

require_once('size.inc');
require_once('ordinaries/bend-helper.inc');


   // modify a (1000x1000) placement to fit within the given bounding box
   // TODO Deal with any arrangment that extends below 1000 (is this only in pale...?)
   function adjustPlacement($placement,$boundingBox, $scaleSize = true) {
     if ( $boundingBox == 'x' ) return $placement; // dummy entry to give multiple locations
     $newPlacement = '';
     $numberGroups = explode('/',$placement);
     $bbElements = explode(',',$boundingBox);
     if (count($bbElements) < 4) {
       error_log("bad BB - $boundingBox");
       return $placement;
     }
     $bbx = $bbElements[0];
     $bby = $bbElements[1];
     $bbw = $bbElements[2];
     $bbh = $bbElements[3];
     // list($bbx,$bby,$bbw,$bbh) = explode(',',$boundingBox); // replaced for error tracking
     $xScale = $bbw / 1000;
     $yScale = $bbh / 1000;
     foreach ( $numberGroups as $numberGroup ) {
       if ( strlen($numberGroup) < 2 ) { // i.e. if empty or just a single character like 'x'
         $newPlacement .= $numberGroup . '/';
         continue;
       } // else
       $numbers = explode(':',$numberGroup);
       foreach ( $numbers as $number ) {
         list ( $x,$y,$w,$h,$rot,$inv,$rev ) = explode ( ',', $number . ',,,,,,,');
         $newPlacement .= sprintf("%.2f",$bbx + ($x * $xScale)); // new x
         $newPlacement .= ',';
         if ( $y != '' ) $newPlacement .= sprintf("%.2f", $bby + ( $y * $yScale));
         $newPlacement .= ',';
         if ( $w != '' ) $newPlacement .= $scaleSize ? sprintf("%.2f", $w * $xScale) : $w;
         $newPlacement .= ',';
         if ( $h != '' ) $newPlacement .= $scaleSize ? sprintf("%.2f", $h * $yScale) : $h;
         if ("$rot$inv$rev" != "") {
            $newPlacement .= ",$rot";
            $newPlacement .= ",$inv";
            $newPlacement .= ",$rev";
         }
         $newPlacement .= ':';
       }
       // Strip off last ':', add a slash
       $newPlacement = rtrim($newPlacement,':') . '/';
     }
     // strip off last '/'
     $newPlacement = rtrim($newPlacement,'/');
     return $newPlacement;
   }

function adjustDistance($item, $placement) {
  foreach ( $item->childNodes as $child ) {
    if ( $child->nodeName == 'modifier' ) {
      switch ( $child->getAttribute('keyterm') ) {
        case 'further-apart':
          $placement = adjustPlacement($placement,'-100,-100,1200,1200', false);
          break;
        case 'much-further-apart':
          $placement = adjustPlacement($placement,'-200,-200,1400,1400', false);
          break;
        case 'very-much-further-apart':
          $placement = adjustPlacement($placement,'-300,-300,1600,1600', false);
          break;
        case 'closer-together':
           $placement = adjustPlacement($placement,'50,50,900,900', false);
           break;
        case 'much-closer-together':
           $placement = adjustPlacement($placement,'100,100,800,800', false);
           break;
        case 'very-much-closer-together':
           $placement = adjustPlacement($placement,'150,150,700,700', false);
           break;
      }
    }
  }
  return $placement;
}

/*

an armorial roll
with title "Please mark orle positions from 3? 4? to 10 on each shield"
consisting of
drawn in a heater shape cendree an orle argent a grid sable
caption "Heater"
then
drawn in a french shape cendree an orle argent a grid sable
caption "French"
then 
drawn in an oval shape cendree an orle argent a grid sable
caption "Oval"
then 
drawn in a lozenge shape cendree an orle argent a grid sable
caption "Lozenge"
then
drawn in a square shape cendree an orle argent a grid sable
caption "Square"
then 
drawn in an italian shape cendree an orle argent a grid sable
caption "Italian"
then 
drawn in a swiss shape cendree an orle argent a grid sable
caption "Swiss"
then 
drawn in a german shape cendree an orle argent a grid sable
caption "German"
then
drawn in a african shape cendree an orle argent a grid sable
caption "African"
then
drawn in a polish shape cendree an orle argent a grid sable
caption "Polish"
then
drawn in a spanish shape cendree an orle argent a grid sable
caption "Spanish"
then
drawn in an SCA shape cendree an orle argent a grid sable
caption "SCA"
then
drawn in a hungarian shape cendree an orle argent a grid sable
caption "Hungarian"
skip
then
drawn in a english shape cendree an orle argent a grid sable
caption "English"
*/

function inOrle($layout, $number) {
       global $messages;

       static $orles = array (
           'heater-5' => '////160,140,140,140:840,140:860,620:500,1070:130,620',
           'french-5' => '////160,140,140,140:840,140:860,560:500,990:130,560',
           'oval-5' => '////370,220,140,140:630,220:810,570:500,980:190,570',
           'lozenge-5' => '////320,300,140,140:680,300:720,700:500,800:280,700',
           'square-5' => '////220,140,140,140:780,140:860,560:500,860:140,560',
        'heater-11' => '///////////500,100,100,100:100,650:900,650:500,1050:100,100:900,100:100,400:900,400:200,900:800,900:300,100:700,100',
           'heater-12' => '///////////500,100,100,100:100,650:900,650:500,1050:100,100:900,100:100,400:900,400:200,900:800,900:300,100:700,100',
       );

       $shape = $layout->shape;
       $target = $shape . '-';
       $target .= $number;

       if (array_key_exists( $target, $orles)) {
           return $orles[$target];
       } else {
           $messages->addMessage('internal',"No placement for $number charges on $shape, sorry");
       }
       return 'x';
}

function inAnnulo($number, $box, $layout, $arc = "full")
{
    if ( $layout->size == $box->size )
    {
        $box->size->height = min($box->size->height, $layout->cross_point->y * 2);
        $box->pos = $layout->cross_point;
    }

    $cx = $box->pos->x;
    $cy = $box->pos->y;

    $radius = min($box->size->height, $box->size->width) / 2;

    $sizeLimit = 160;
    $turn = 180 / ($number -1);
    switch($arc) {
        case "upper":
            $angle = -90;
            break;
        case "lower":
            $angle = 90;
            break;
        case "sinister":
            $angle = 0;
            break;
        case "dexter":
            $angle = -180;
            break;
        default:
        case "full":
            $angle = 0;
            $turn = 360 / $number;
            $sizeLimit = 200;
            break;
    }
    // Make size dependent on circumference && number
    $size = intval(($radius * M_PI) / ($number * 0.9));
    $size = min($size, $sizeLimit);
    // if ($size > 150)
      //   $size = 200;

    $radius -= $size / 2;

    $placement = str_repeat('/',$number - 1);
    for ($i = 0; $i < $number; $i++) {
        $x = intval($cx + sin(deg2rad($angle)) * $radius);
        $y = intval($cy + cos(deg2rad($angle)) * ($radius * -1));
        $placement .= "$x,$y,$size,$size:";
        $angle += $turn;
    }
    return rtrim($placement,':');
}

function box_placement($box, $rot = 0, $inv = 0, $rev = 0)
{
    if ( $box == null )
        return "x";
    return "$box,$rot,$inv,$rev";
}

function array_to_placement_impl($array)
{
    if ( is_string($array) )
        return $array;
    if ( count($array) == 0 )
        return "x";
    return implode(":", $array);
}

function array_to_placement($array)
{
    return implode("/", array_map("array_to_placement_impl", $array));
}

function placement_combattant($boundingBox, $gap, $flip)
{
    $placement_arr = [];

    $width = (1 - $gap) / 2;
    $off_l = $width / 2;
    $off_r = 1 - $off_l;


    for ( $n = 2; $n <= 6; $n += 2 )
    {
        $placement_arr[] = []; // skip odds

        $per_side = $n / 2;
        $height = 1 / $per_side;
        $plac = [];
        $off_y = $height / 2;
        for ( $i = 0; $i < $per_side; $i++ )
        {
            $plac[] = box_placement($boundingBox->relative_box($off_l, $off_y + $height * $i, .5, $height), 0, 0, 1-$flip);
            $plac[] = box_placement($boundingBox->relative_box($off_r, $off_y + $height * $i, .5, $height), 0, 0, $flip);
        }
        $placement_arr[] = $plac;
    }

    return array_to_placement($placement_arr);
}

function placement_from_ordinary($ordinary_file, $boundingBox, $layout)
{
    $node = null;
    $ordinary = [];
    include "ordinaries/$ordinary_file.inc";
    return $ordinary['chargesOn'];
}

function placement_chevron(ShieldLayout $layout, $chev_width, $relative_size, $node)
{
    $placement = [];
    $plac_size = $chev_width * $relative_size;
    $plac_base = new ShieldRect(
        new ShieldPos($layout->cross_point->x, $layout->cross_point->y - $chev_width / M_SQRT2),
        new ShieldSize($plac_size, $plac_size)
    );

    $min_ext = min($layout->cross_point->x, $layout->size->height - $layout->cross_point->y);
    $off = $chev_width / M_SQRT2 * 1.1;

    for ( $n = 1; $n <= 8; $n++ )
    {
        $plac = [];
        if ( $n % 2 )
        {
            if ( $n > 1 )
                $plac = $placement[count($placement)-1];

            array_splice($plac, ($n-1)/2, 0, [box_placement($plac_base->translated(0, 1/5 * $chev_width / M_SQRT2), 0)]);

        }
        else
        {
            $delta = $min_ext / ($n/2+1);
            for ( $i = 0; $i < $n / 2; $i++ )
            {
                $d = $off + $delta * $i;
                $plac[] = box_placement($plac_base->translated(-$d, $d), 45);
                $plac[] = box_placement($plac_base->translated($d, $d), -45);
            }
        }

        $placement[] = $plac;
    }


    return array_to_placement($placement);
}

function placement_counter_passant($boundingBox)
{
    $placement_arr = [];

    for ( $n = 1; $n <= 8; $n++ )
    {
        $height = 1 / $n;
        $off = $height / 2;

        $plac = [];
        for ( $i = 0; $i < $n; $i++ )
        {
            $plac[] = box_placement($boundingBox->relative_box(.5, $off + $height * $i, 1, $height), 0, 0, $i % 2);
        }
        $placement_arr[] = $plac;
    }

    return array_to_placement($placement_arr);
}


function placement_pilewise($boundingBox)
{
    $cx = $boundingBox->pos->x;
    $cy = lerp($boundingBox->pos->y, $boundingBox->bottom(), 0.8);
    $r = $boundingBox->size->height / 2;

    $placement_arr = [[]];

    for ( $n = 2; $n <= 8; $n++ )
    {
        $angle_step = 64 / ($n - 1);
        $angle_start = -32;
        $size = $boundingBox->size->scaled(1/ ($n+1), 0.5);

        $plac = [];
        for ( $i = 0; $i < $n; $i++ )
        {
            $angle = $angle_start + $angle_step * $i;
            $plac[] = box_placement(
                new ShieldRect(
                    new ShieldPos(
                        $cx + $r * sin(deg2rad($angle)),
                        $cy - $r * cos(deg2rad($angle))
                    ),
                    $size
                ),
                $angle,
                1,
                0
            );

        }
        $placement_arr[] = $plac;
    }
    return array_to_placement($placement_arr);
}

function placement_quadruplet($boundingBox, $layout)
{
    $center = $layout->cross_point;


    $size = $boundingBox->size->scaled(.4);
    $halfX = $size->width / 2;
    $halfY = $size->height / 2;
    $quarterX = $halfX / 2;
    $quarterY = $halfY / 2;

    $cross = [
        new ShieldRect($center->translated(0, -$quarterY), $size),
        new ShieldRect($center->translated($quarterX, 0), $size),
        new ShieldRect($center->translated(0, $quarterY), $size),
        new ShieldRect($center->translated(-$quarterX, 0), $size),
    ];

    return array_to_placement([
        [],
        [],
        [],
        $cross,
        [],
    ]);
}

function placement_quadrangle($boundingBox, $layout)
{
    $center = $layout->cross_point;


    $size = $boundingBox->size->scaled(.4);
    $dx = $size->width / 2 + 50;
    $dy = $size->height / 2 + 50;

    $cross = [
        new ShieldRect($center->translated(-$dx, -$dy), $size),
        new ShieldRect($center->translated($dx, -$dy), $size),
        new ShieldRect($center->translated(-$dx, $dy), $size),
        new ShieldRect($center->translated($dx, $dy), $size),
    ];

    return array_to_placement([
        [],
        [],
        [],
        $cross,
        [],
    ]);
}

function adjustment_quarter($number, ShieldLayout $layout, $bigOrdinary)
{
    if ( $bigOrdinary )
    {
        $margin_out = 25;
        $margin_cross = 125;
    }
    else
    {
        $margin_out = $margin_cross = 50;
    }

    switch ( $number )
    {
        case 1:
            return ShieldRect::from_corners(
                $margin_out, $margin_out,
                $layout->cross_point->x - $margin_cross, $layout->cross_point->y - $margin_cross);
        case 2:
            return ShieldRect::from_corners(
                $layout->cross_point->x + $margin_cross, $margin_out,
                $layout->size->width - $margin_out, $layout->cross_point->y - $margin_cross);
        case 3:
            return ShieldRect::from_corners(
                $margin_out, $layout->cross_point->y + $margin_cross,
                $layout->cross_point->x - $margin_cross, $layout->size->height - $margin_out);
        case 4:
            return ShieldRect::from_corners(
                $layout->cross_point->x + $margin_cross, $layout->cross_point->y + $margin_cross,
                $layout->size->width - $margin_out, $layout->size->height - $margin_out);
    }

}


function adjustment_from_position($position, $layout, $underlying_div, $bigOrdinary, $cadency, $item)
{
    global $messages;
    $height3rd = $layout->size->height / 3;
    $height6th = $height3rd / 2;
    $width3rd = $layout->size->width / 3;
    $width6th = $width3rd / 2;
    $width2th = $layout->size->width / 2;
    $height2th = $layout->size->height / 2;
    if ($cadency) {
        // make things smaller if they are just for marking cadency
        $width3rd = $width6th;
        $height3rd = $height6th;
    }
    $margin = max($height2th, $width2th) / 5;

    switch ( $position )
    {
        case 'ineach':
            // TODO more divisions!
            if ( !$underlying_div )
            {
                $messages->addMessage('warning','Need a division for "in each"');
                break;
            }
            switch ( $underlying_div )
            {
                case 'quarterly':
                case 'quarterly-per-fess':
                case 'quarterly-per-pale':
                    return [
                        adjustment_quarter(1, $layout, false),
                        adjustment_quarter(2, $layout, false),
                        adjustment_quarter(3, $layout, false),
                        adjustment_quarter(4, $layout, false),
                    ];
                    // TODO replace all of this as per the examples below to account for shield size
                case 'tierced-in-pale':
                    return ['50,100,233,1000', '383,100,233,1000', '716,100,233,1000'];
                case 'tierced-in-fess':
                    return ['100,100,800,200', '100,500,800,200', '100,900,800,200'];
                case 'tierced-in-bend':
                    return ['650,50,300,300', '350,350,300,300', '50,700,300,300'];
                case 'tierced-in-bend-sinister':
                    return ['50,50,300,300', '350,350,300,300', '650,700,300,300'];
                case 'tierced-in-chevron':
                    return ['400,0,200,200', '350,250,300,300', '350,850,300,300'];
                case 'per-pall':
                    return ['50,550,400,400', '350,50,300,300', '550,550,400,400'];
                case 'per-pile':
                    return ['50,700,250,250', '300,100,400,400', '700,700,250,250'];
                case 'per-pall-inverted':
                    return ['50,50,400,400', '550,50,400,400', '350,750,300,300'];
                case 'gyronny_of_8':
                    return ['50,250,200,200', '250,50,200,200', '550,50,200,200', '750,250,200,200', '750,550,200,200', '550,750,200,200', '250,750,200,200', '50,550,200,200'];
                default:
                    $messages->addMessage('warning','"in each" for ' . $underlying_div . 'not available' );
                    return [];
            }
            break;
        case 'ineach1st':
            if ( !$underlying_div ) {
                $messages->addMessage('warning','Need a division for "in each"');
                break;
            } // else
            switch ( $underlying_div ) {
                case 'quarterly':
                case 'quarterly-per-fess':
                case 'quarterly-per-pale':
                    return [
                        adjustment_quarter(1, $layout, false),
                        adjustment_quarter(4, $layout, false),
                    ];
                case 'gyronny_of_8':
                    return ['250,50,200,200', '750,250,200,200', '550,750,200,200', '50,550,200,200'];
            }
            break;
        case 'ineach2nd':
            if ( !$underlying_div ) {
                $messages->addMessage('warning','Need a division for "in each"');
                break;
            } // else
            switch ( $underlying_div ) {
                case 'quarterly':
                case 'quarterly-per-fess':
                case 'quarterly-per-pale':
                    return [
                        adjustment_quarter(2, $layout, false),
                        adjustment_quarter(3, $layout, false),
                    ];
                case 'gyronny_of_8':
                    return ['50,250,200,200', '550,50,200,200', '750,550,200,200', '250,750,200,200'];
            }
            break;
        case 'ineachQ':
            return [
                adjustment_quarter(1, $layout, $bigOrdinary),
                adjustment_quarter(2, $layout, $bigOrdinary),
                adjustment_quarter(3, $layout, $bigOrdinary),
                adjustment_quarter(4, $layout, $bigOrdinary),
            ];
        case 'ineachQ12':
            return [
                adjustment_quarter(1, $layout, $bigOrdinary),
                adjustment_quarter(2, $layout, $bigOrdinary),
            ];
        case 'ineachQ13':
            return [
                adjustment_quarter(1, $layout, $bigOrdinary),
                adjustment_quarter(3, $layout, $bigOrdinary),
            ];
        case 'ineachQ24':
            return [
                adjustment_quarter(2, $layout, $bigOrdinary),
                adjustment_quarter(4, $layout, $bigOrdinary),
            ];
        case 'ineachQ34':
            return [
                adjustment_quarter(3, $layout, $bigOrdinary),
                adjustment_quarter(4, $layout, $bigOrdinary),
            ];
        case 'inchief':
            if ( $layout->chief )
                return [$layout->chief->margin(50, 50)];
            break;
        case 'inbase':
            return [new ShieldRect(
                new ShieldPos($layout->size->width/2, $layout->size->height - 300),
                new ShieldSize(800, 250)
            )];
        case 'inflank':
            return [
                new ShieldRect(new ShieldPos(170, $layout->cross_point->y), new ShieldSize(300, 300)),
                new ShieldRect(new ShieldPos($layout->size->width - 170, $layout->cross_point->y), new ShieldSize(300, 300))
            ];
        case 'indexchief': return [
            new ShieldRect(
                new ShieldPos($width6th, $height6th),
                new ShieldSize($width3rd, $height3rd)
            ) ];
        case 'insinchief': return   [
            new ShieldRect(
                new ShieldPos($layout->size->width - $width6th, $height6th),
                new ShieldSize($width3rd, $height3rd)
            ) ];
        case 'inmidchief': return [ 
            new ShieldRect(
                new ShieldPos($width2th, $height6th),
                new ShieldSize($width3rd, $height3rd)
            ) ];
        case 'indexbase':
            return [
                new ShieldRect(
                    new ShieldPos($width6th, $height2th + $height3rd),
                    new ShieldSize($width3rd, $height3rd)
                ) ];
        case 'insinbase':
            return   [
                new ShieldRect(
                    new ShieldPos($layout->size->width - $width6th, $height2th + $height3rd),
                    new ShieldSize($width3rd, $height3rd)
                ) ];
        case 'inmidbase':
            return [
                new ShieldRect(
                    new ShieldPos($width2th, $height2th + $height3rd),
                    new ShieldSize($width3rd, $height3rd)
                ) ];
        case 'inhonpoint': return [
            new ShieldRect(
                new ShieldPos($width2th, $height3rd),
                new ShieldSize($width3rd, $height3rd)
            ) ];
        case 'inabyss':  // flow through
        case 'infesspoint': return [
            new ShieldRect(
                new ShieldPos($width2th, $height2th),
                new ShieldSize($width3rd, $height3rd)
            ) ];
        case 'innombril':return [
            new ShieldRect(
                new ShieldPos($width2th,2 * $height3rd),
                new ShieldSize($width3rd, $height3rd)
            ) ];
        case 'insinflank':
            return   [
                new ShieldRect(
                    new ShieldPos($layout->size->width - $width6th, $height2th),
                    new ShieldSize($width3rd, $height3rd)
                ) ];
        case 'indexflank':
            return [
                new ShieldRect(
                    new ShieldPos($width6th, $height2th),
                    new ShieldSize($width3rd, $height3rd)
                ) ];
        case 'insinside':
            return [
                new ShieldRect(
                    new ShieldPos($width2th * 1.5, $height2th),
                    new ShieldSize($width2th - $margin, $layout->size->height - $margin)
                ) ];
        case 'indexside':
            return [
                new ShieldRect(
                    new ShieldPos($width2th * 0.5, $height2th),
                    new ShieldSize($width2th - $margin, $layout->size->height - $margin)
                ) ];
        case 'inupperhalf':
            return [
                new ShieldRect(
                    new ShieldPos($width2th, $height2th / 2),
                    new ShieldSize($layout->size->width - $margin, $height2th - $margin)
                ) ];
        case 'inlowerhalf':
            return [
                new ShieldRect(
                    new ShieldPos($width2th, $height2th * 1.5),
                    new ShieldSize($layout->size->width - $margin, $height2th - $margin)
                ) ];
            // TODO All of the following should be like the above, so that it takes account of the actual
            // size of the shield
        case 'infirst':
            switch ( $underlying_div ) {
                case 'tierced-in-pale': return ['50,100,233,1000'];
                case 'tierced-in-fess': return ['100,100,800,200'];
                case 'tierced-in-bend': return ['650,50,300,300'];
                case 'tierced-in-bend-sinister': return ['50,50,300,300'];
                case 'tierced-in-chevron': return ['400,0,200,200'];
                case 'per-pile': return ['50,700,250,250'];
                case 'per-pall': return ['50,550,400,400'];
                case 'per-pall-inverted': return ['50,50,400,400'];
                // case 'per-saltire': return ['300,50,400,400'];
                case 'quarterly':
                case 'quarterly-per-pale':
                case 'quarterly-per-fess': return ['50,50,400,400'];
                default: $messages->addMessage('warning','no division for "in first"'); break;
            }
            break;
        case 'insecond':
            switch ( $underlying_div ) {
                case 'tierced-in-pale': return ['383,100,233,1000'];
                case 'tierced-in-fess': return ['100,500,800,200'];
                case 'tierced-in-bend':  // same as sinister
                case 'tierced-in-bend-sinister': return ['350,350,300,300'];
                case 'tierced-in-chevron':  return ['350,250,300,300'];
                case 'per-pall': return ['350,50,300,300'];
                case 'per-pile': return ['300,100,400,400'];
                case 'per-pall-inverted': return ['550,50,400,400'];
                case 'quarterly':
                case 'quarterly-per-pale':
                case 'quarterly-per-fess': return ['550,50,400,400'];
                default: $messages->addMessage('warning','no division for "in second"'); break;
            }
            break;
        case 'inthird':
            switch ( $underlying_div ) {
                case 'tierced-in-pale': return ['716,100,233,1000'];
                case 'tierced-in-fess': return ['100,900,800,200'];
                case 'tierced-in-bend': return ['50,700,300,300'];
                case 'tierced-in-bend-sinister': return ['650,700,300,300'];
                case 'tierced-in-chevron':  return ['350,850,300,300'];
                case 'per-pall': return ['550,550,400,400'];
                case 'per-pile': return ['700,700,250,250'];
                case 'per-pall-inverted': return ['350,750,300,300'];
                case 'quarterly':
                case 'quarterly-per-pale':
                case 'quarterly-per-fess': return ['50,550,400,400'];
                default: $messages->addMessage('warning','no division for "in third"'); break;
            }
            break;
        case 'inQ1':
            return [adjustment_quarter(1, $layout, $bigOrdinary)];
        case 'inQ2':
            return [adjustment_quarter(2, $layout, $bigOrdinary)];
        case 'inQ3':
            return [adjustment_quarter(3, $layout, $bigOrdinary)];
        case 'inQ4':
            return [adjustment_quarter(4, $layout, $bigOrdinary)];
        default:
            break;
    }
    return [];
}

function get_placement(DOMElement $item, $layout) {
    global $xpath, $messages;

    $placement = ''; // This is what we are after, keep going until we find one
    $keyterm = $item->getAttribute('keyterm');
    if (is_null($keyterm) || strpos($keyterm,'/') === false) {
      return 'x';
    }
    list($group,$name) = explode('/', $keyterm);

    // Set up parameters for this charge
    $pos = getModifiersByTypeAsArray($item,'position'); // THIS IS AN array, go through each one, appending * bounding_box as many times as required.
    $arr = getModifierByType($item,'arrangement', true);
    $cadency = existModifierWithKeyterm($item,'difference');
    $entire = existModifierWithKeyterm($item,'entire');
    $number = $item->getAttribute('number');
    $overall = ($item->parentNode != null && $item->parentNode->parentNode != null && $item->parentNode->parentNode->nodeName == 'overall');
    // $apply_location = true;
    $forceCentre = (!($entire) && !count($pos) && ($layout->shape != 'pauldron') &&
        ($group == "quadrate" || $group == 'geometric' || $group == 'characters') && $number == 1);
    $div_node = $xpath->query("parent::objects/parent::simple/field/tincture/division", $item);
    if ( $div_node->length > 0 ) {
      /**
       * @var DOMElement $divisionNode
       */
      $divisionNode = $div_node->item(0);
      $underlying_div = $divisionNode->getAttribute('keyterm');
      if (existModifierWithKeyterm($divisionNode,'sinister')) $underlying_div .= '-sinister';
      if (existModifierWithKeyterm($divisionNode,'inverted')) $underlying_div .= '-inverted';
      if (existModifierWithKeyterm($divisionNode,'reversed')) $underlying_div .= '-reversed';
      if ($underlying_div == 'gyronny') {
        $ofnum = getModifierValueByKeyterm($divisionNode,'ofnum');
        $underlying_div .= '_of_' . (($ofnum == null) ? '8' : $ofnum);
      }
    } else {
      $underlying_div = null;
    }



    $margin = 0;
    // Figure out default bounding box size for this aspect ratio, chief presence && entire
    $center = new ShieldPos($layout->size->width / 2, $layout->size->height / 2);
    static $full_box_arrangements = ["inannulo", "inbend", "inbendsin", "inpalethrough", 'infessthrough', 'incross', 'incrosspoint'];
    if ( $entire || in_array($arr, $full_box_arrangements) )
    {
        $boundingBox = new ShieldRect($center, clone $layout->size);
    }
    else if ( $layout->charge_hint )
    {
        $boundingBox = $layout->charge_hint;
    }
    else
    {
        $margin = min($layout->size->width, $layout->size->height) / 10;

        if ( $layout->shape == "flag" )
            $margin = 50;

        $boundingBox = new ShieldRect(
            $center,
            new ShieldSize($layout->size->width - 2 * $margin, $layout->size->height - 2 * $margin)
        );
    }

    $adjustmentList = [];
    if ( count($pos) > 0 ) {
        // if ( !$apply_location ) {
            // $messages->addMessage('warning','"in" location conflicts with "on" or "between"');
        // } else {
            $bigOrdinary = $xpath->evaluate("boolean(../ordinary[@keyterm='fess'] or ../ordinary[@keyterm='pale'] or ../ordinary[@keyterm='plain-cross'])", $item);
            foreach ( array_keys($pos) as $position ) {
                $pos_adjust = adjustment_from_position($position, $layout, $underlying_div, $bigOrdinary, $cadency, $item);
                $adjustmentList = array_merge($adjustmentList, $pos_adjust);
            }
        // }
    }

    $placementList = [];
    if ( count($adjustmentList) == 0 )
        $adjustmentList = [$boundingBox];

    foreach ( $adjustmentList as $adjust ) {
        if ($adjust) {
            // at this point, if $adjustment is not null it contains the 4 coordinates of a bounding box on 
            // a 1000 x 1000 area. We need to map this bounding box onto the area we are actually given in
            // the current $boundingBox

            if ( $adjust instanceof ShieldRect )
            {
                $boundingBox = $adjust;
            }
            else
            {
                list($adjX, $adjY, $adjWidth, $adjHeight) = explode(',',$adjust);
                // Find the center of that adjustment bounding box as a ratio
                $adjCenterX = ($adjX + ($adjWidth / 2)) / 1000;
                $adjCenterY = ($adjY + ($adjHeight/ 2)) / 1000;
                // scale against 1000 x 1000 area
                $adjWidth /= 1000; $adjHeight /= 1000;
                $newCenter = new ShieldPos( $layout->size->width * $adjCenterX, $layout->size->height * $adjCenterY);
                $boundingBox = new ShieldRect ($newCenter, new ShieldSize(($layout->size->width * $adjWidth * 0.9),
                    ($layout->size->height * $adjHeight *0.9)));
            }
        }

        // Now try various placement regimes

        if ( $name == 'label' && $entire)
        {
            $placement = '500,150,1000,200';
            $boundingBox = 'x';
        }
        // strewn on a field (don't need any placement)
        else if ( $item->parentNode->nodeName == 'semyde' )
        {
            $placement = 'i';
        }
        else
        {
            // Are we above or below another charge?
            // If so, occupy only half the bounding box
            if ( $xpath->evaluate("boolean(parent::modifier[@keyterm='above']/parent::charge)",$item) )
            {
                $boundingBox->pos->y += $boundingBox->size->height / 4;
                $boundingBox->size->height /= 2;
                $forceCentre = false;
            }
            else if ( existModifierWithKeyterm($item,'above') )
            {
                $boundingBox->pos->y -= $boundingBox->size->height / 4;
                $boundingBox->size->height /= 2;
                $forceCentre = false;
            }

            // similarly for charge beside charge
            if ( $xpath->evaluate("boolean(parent::modifier[@keyterm='beside']/parent::charge)",$item) )
            {
                $boundingBox->pos->x += $boundingBox->size->width / 4;
                $boundingBox->size->width /= 2;
                $forceCentre = false;
            }
            else if ( existModifierWithKeyterm($item,'beside') )
            {
                $boundingBox->pos->x -= $boundingBox->size->width / 4;
                $boundingBox->size->width /= 2;
                $forceCentre = false;
            }
        }

        // Are we around another charge?
        if ( $placement == '' && $xpath->evaluate("boolean(parent::modifier[@keyterm='between']/parent::charge)",$item) ) {
          // If our parent charge is "on" an ordinary, we don't need to do anything
          if ( $xpath->evaluate("boolean(parent::modifier/parent::charge/parent::modifier[@keyterm='on'])",$item) ) {
            $placement = 'i';
            // similarly, if our parent charge has an explicit arrangement
          } elseif ( $xpath->evaluate("boolean(parent::modifier/parent::charge/modifier[@keyterm='arrangement'])",$item) ) {
            $placement = 'i';        
          } else { // Arrange on an open field
            if ( substr($item->parentNode->parentNode->getAttribute('keyterm'),0,8) == 'quadrate' ) {// Special placement for items around a quadrate cross
                $placement = '////300,380,160,160:700,380,160,160:300,750:700,750:500,980/';
            } else {
                if (existModifierWithKeyterm($item,'facing-out')) {
                    $placement = 'x/125,500,200,800:875,500,200,800,0,0,1/x/250,200,280,280:750,200,280,280,0,0,1:250,900,280,280,0,0,0:750,900,280,280,0,0,1';
                } elseif (existModifierWithKeyterm($item,'facing-in')) {
                    $placement = 'x/125,500,200,800,0,0,1:875,500,200,800,0,0,0/x/250,200,280,280,0,0,1:750,200,280,280,0,0,0:250,900,280,280,0,0,1:750,900,280,280,0,0,0';
                } else {
                    $placement = 'x/125,500,200,800:875,500/250,200,300,280:750,200,300,280:500,1000/250,200,280,280:750,200,280,280:250,900:750,900/250,200,240,240:750,200,240,240:500,200:250,900:750,900';
                }

            }
          }
          // If our parent charge has a position, then we need to apply it to ourselves also
          $pos = getModifiersByTypeAsArray($item->parentNode->parentNode,'position');
        }

        // explicit arrangements
        if ( $placement == '' && $arr ) {
          switch ( $arr ) {
            case 'inannulo':
                $placement = inAnnulo($number, $boundingBox, $layout);
                break;
            case 'inuppersemi':
                $placement = inAnnulo($number, $boundingBox, $layout, "upper");
                break;
            case 'inlowersemi':
                $placement = inAnnulo($number, $boundingBox, $layout, "lower");
                break;
            case 'indextersemi':
                $placement = inAnnulo($number, $boundingBox, $layout, "dexter");
                break;
            case 'insinistersemi':
                $placement = inAnnulo($number, $boundingBox, $layout, "sinister");
                break;
            case 'combattant':
                $placement = placement_combattant($boundingBox, 0, 0);
                break;
            case 'facing-in':
                $placement = placement_combattant($boundingBox, 0.1, 0);
                break;
            case 'facing-out':
                $placement = placement_combattant($boundingBox, 0, 1);
                break;
            case 'inbend':
                $placement = placement_in_bend($boundingBox, false, 0);
                break;
            case 'inbendsin':
                $placement = placement_in_bend($boundingBox, true, 0);
                break;
            case 'inbar':
                $placement = calcPlace($item,'v',$boundingBox);
                $item->appendChild(createModifierNode('chargemod','fesswise'));
                break;
            case 'inpale':
            case 'inpalethrough':
                $placement = calcPlace($item,'v',$boundingBox);
                break;
            case 'inchiefthrough':
                if ( $layout->chief )
                    $placement = calcPlace($item,'h', $layout->chief);
                break;
            case 'inpall':
                $placement = 'x/x/250,250,180,400,-45:750,250,,,45:500,750,,,0/s/s/s' ;
                break;
            case 'infess':
            case 'infessthrough':
                $placement = calcPlace($item,'h',$boundingBox);
                break;
            case 'inchevron':
                $placement = placement_chevron($layout, 200, 0.8, $item);
                break;
            case 'incross':
                $placement = placement_from_ordinary("plain-cross", $boundingBox, $layout);
                break;
            case 'insaltire':
                $placement_arr = explode("/", placement_from_ordinary("saltire", $boundingBox, $layout));
                $placement_arr[1] = box_placement($boundingBox->relative_box(0.5, 0.5, 0.75, 0.75), 45, 0, 1) . ":" .
                    box_placement($boundingBox->relative_box(0.5, 0.5, 0.75, 0.75), -45, 0, 0);
                $placement = implode("/", $placement_arr);
                break;
            case 'inpile':
                $placement = 'x/x/250,250,300,300:500,600:750,250/x/x/250,200,200,200:500:750:333.33,500:666.67:500,800';
                break;
            case 'inorle':
                $placement = inOrle($layout, $number);
                break;
            case 'counter-passant':
                $placement = placement_counter_passant($boundingBox);
                break;
            case 'pilewise':
                $placement = placement_pilewise($boundingBox);
                break;
            case 'quadrangle':
                $placement = placement_quadrangle($boundingBox, $layout);
                break;
            default:
                $placement = 'i';
                break; // internal error, no placement
          }
        }

            // If we have other charges around us (i.e. charge between charges), shrink a bit to make room
        if ( $xpath->evaluate("boolean(modifier[@keyterm='between']/charge)",$item ) && $item->getAttribute('type') != 'quadrate' ) {
            if ( $placement == '' )
                $placement = adjustplacement(calcPlace($item, 'w', $boundingBox), '150,150,700,700');
            else
                $placement = adjustplacement($placement, '150,150,700,700');
        }



        // On an underlying division (but ignore if in a specific location)
        if ( $placement == '' && count($pos)==0 && $underlying_div != null ) {
            if ($underlying_div=='per-bend' && $number==2) $placement = 'x/300,700,350,350:700,300/x/x/x/x';
            elseif ($underlying_div=='per-bend-sinister' && $number==2) $placement = 'x/300,300,350,350:700,700/x/x/x/x';
            elseif (substr($underlying_div,0,9)=='quarterly' && $number==4 ) $placement = 'x/x/x/250,250,300,300:750,250:250,750:750,750/x/x/';
            elseif ($underlying_div=='gyronny_of_8' && ($number==8 or $number==4) ) $placement = 'x/x/x/200,500,300,300:500,200:800,500:500,800/x/x/x/200,400,160,160:400,200:600,200:800,400:800,600:600,800:400,800:200,600';
            elseif ($underlying_div=='gyronny_of_6' && ($number==6 or $number==3) ) $placement = 'x/200,500,300,600:800,500/x/x/x/350,200,200,200:650,200:150,500:850,500:350,800:650,800';
            elseif ($underlying_div=='per-chevron' && $number==3 ) $placement = 'x/x/250,200,240,240:500,750,300,300:750,200,240,240';
            elseif ($underlying_div=='per-pall-reversed' && $number==3 ) $placement = 'x/x/250,300,300,300:500,900:750,300/x/x/x/';
            elseif ($underlying_div=='per-pall' && $number==3 ) $placement = 'x/x/250,700,300,300:500,200,300,300:750,700,300,300:/x/x/x/';
            elseif ($underlying_div=='per-saltire' && $number==3 ) $placement = 'x/200,500,240,240:800,500/200,500,240,240:500,850,300,300:800,500,240,240/200,500,240,240:500,850,300,300:800,500,240,240:500,200,240,240/x/x';
        }

        if ( $cadency ) { // unless there is a specific location, put in the center, but small
            $placement = calcPlace($item,'w',$boundingBox);
            $placement = adjustplacement($placement,'400,400,200,200');
        } elseif ( $overall && !($name == 'label' && $entire)) { // like cadency, but a bit bigger, fudge for label overall, already moved
            $placement = calcPlace($item,'w',$boundingBox);
            if ( $group == "quadrate" ) {
                $placement = adjustplacement($placement,'100,66,800,800');
            }
            else {
                $total_w = $layout->size->width;
                $total_h = $layout->size->height;
                $correct_h = $total_h / 1000;
                $x = $total_w / 2 - 200;
                $y = $total_h / 2 - 200;
                $w = $total_w * 0.4;
                $h = $w / $correct_h;
                $placement = adjustplacement($placement,"$x,$y,$w,$h");
            }
        } 

        // If we don't have a placement yet the default is just to arrange in wide rows
        if ( $placement == '' ) {
            // special case placements, put some charges in exact centre of cross
            if ( $forceCentre ) {
                // We re-center the bounding box on the cross point, but also need to make
                // sure that it still fits within the shield boundary (by checking the space
                // available between the top of the shield and the bounding box, leaving
                // a 20% margin) Issue 257 refers
                $availableHeight = ($layout->cross_point->y) * 0.8;
                // $boundingBox = $boundingBox->relative_box(0.9, 0.9, 0.9, 0.9);
                // $boundingBox->pos = clone $layout->cross_point;
                $boundingBox = new ShieldRect(clone $layout->cross_point, new ShieldSize($availableHeight * 2, $availableHeight * 2));
            }
            $placement = calcPlace($item,'w',$boundingBox);
        }
        $placement = adjustDistance($item, $placement);
        $placementList[] = $placement;
        $placement = '';
    }

    $retval = implode('*',$placementList);
    return $retval;
}


function calcPlace( $node, $aspect, $bb = '100,100,800,900'  ) {
    global $messages;
    
  $wide_rows = array (
    '0',          // 0
    '1',          // 1
    '2',          // 2
    '2,1',        // 3
    '3,1',        // 4
    '3,2',        // 5
    '3,2,1',      // 6
    '3,3,1',      // 7
    '3,3,2',      // 8
    '3,3,3',      // 9
    '4,3,2,1',    // 10
    '4,3,3,1',    // 11
    '4,3,3,2',    // 12
    '4,3,3,3',    // 13
    '5,4,3,2',    // 14
    '5,4,3,2,1',  // 15
    '5,4,3,3,1',  // 16
    '5,4,3,3,2',  // 17
    '6,5,4,2,1',  // 18
    '6,5,4,3,1',  // 19
    '6,5,4,3,2',  // 20
  );
  $narrow_rows = array (
    '0',
    '1',
    '1,1',
    '1,1,1',
    '2,2',
    '2,2,1',
    '2,2,2',
    '3,2,2',
    '3,3,2',
    '3,3,3',
    '4,3,3',
    '4,4,3',
    '4,4,4',      // 12
    '4,4,3,2',    // 13
    '4,4,3,2,1',  // 14
    '4,4,4,3',    // 15
    '4,4,4,4',    // 16
    '4,4,4,4,1',  // 17
    '4,4,4,4,2',  // 18
    '4,4,4,4,3',  // 19
    '4,4,4,4,4',  // 20
  );
  $big_rows = array ( // over 20, all the same
    '5,4,4,4,4',      // 21
    '5,5,4,4,4',      // 22
    '5,5,5,4,4',      // 23
    '5,5,5,5,4',      // 24
    '5,5,5,5,5',      // 25
    '6,5,5,5,5',      // 26
    '6,6,5,5,5',      // 27
    '6,6,6,5,5',      // 28
    '6,6,6,6,5',      // 29
    '6,6,6,6,6',      // 30
    '6,5,5,5,5,5',    // 31
    '6,6,5,5,5,5',    // 32
    '6,6,6,5,5,5',    // 33
    '6,6,6,6,5,5',    // 34
    '6,6,6,6,6,5',    // 35
    '6,6,6,6,6,6',    // 36
    '7,6,6,6,6,6',    // 37
    '7,7,6,6,6,6',    // 38
    '7,7,7,6,6,6',    // 39
    '7,7,7,7,6,6',    // 40
    '7,7,7,7,7,6',    // 41
    '7,7,7,7,7,7',    // 42
    '7,6,6,6,6,6,6',  // 43
    '7,7,6,6,6,6,6',  // 44
    '7,7,7,6,6,6,6',  // 45
    '7,7,7,7,6,6,6',  // 46
    '7,7,7,7,7,6,6',  // 47
    '7,7,7,7,7,7,6',  // 48
    '7,7,7,7,7,7,7',  // 49
    '8,7,7,7,7,7,7',  // 50
    '8,8,7,7,7,7,7',  // 51
    '8,8,8,7,7,7,7',  // 52
    '8,8,8,8,7,7,7',  // 53
    '8,8,8,8,8,7,7',  // 54
    '8,8,8,8,8,8,7',  // 55
    '8,8,8,8,8,8,8',  // 56
    '8,7,7,7,7,7,7,7',// 57
    '8,8,7,7,7,7,7,7',// 58
    '8,8,8,7,7,7,7,7',// 59
    '8,8,8,8,7,7,7,7',// 60
    '8,8,8,8,8,7,7,7',// 61
    '8,8,8,8,8,8,7,7',// 62
    '8,8,8,8,8,8,8,7',// 63
    '8,8,8,8,8,8,8,8',// 64


  );

  if ( $bb instanceof ShieldRect )
  {
    $bb_x = $bb->left();
    $bb_y = $bb->top();
    $bb_w = $bb->size->width;
    $bb_h = $bb->size->height;
  }
  else if ( is_string($bb) )
  {
    list($bb_x,$bb_y,$bb_w,$bb_h) = explode(',',$bb);
  }
  else if (is_null($bb) || strpos( $bb, ',') === false)
  {
      error_log("Bad bounding box - $bb");
      $bb_x = $bb_y = 100;
      $bb_h = 800;
      $bb_w = 900;
  }

  $number = getNumberOfCharges($node);
 // $number = $node->getAttribute('number');
  $conjoined = existModifierWithKeyterm($node,'conjoined');
  $facing_in = existModifierWithKeyterm($node,'facing-in') || existModifierWithKeyterm($node,'combattant');
  $facing_out = existModifierWithKeyterm($node,'facing-out');
  if ( $number > 64 ) return $number;
  elseif ( $number < 1 ) $number = 1;
  if ( $rows = getModifierValueByKeyterm($node,'arrange-by-rows') )
    $row_list = explode(',', $rows);
  else {
    switch ( $aspect ) {
      case 'h': // single horizontal row
        $row_list = array($number);
        break;
      case 'v': // single vertical row
        $row_list = array_fill(0,$number,'1');
        break;
      case 'w': // wide box
        if ($number > 20)
          $row_list = explode(',',$big_rows[$number - 21]);
        else
          $row_list = explode(',',$wide_rows[$number]);
        break;
      case 'n': // narrow box
        if ($number > 20)
          $row_list = explode(',',$big_rows[$number - 21]);
        else
          $row_list = explode(',',$narrow_rows[$number]);
        break;
    }
  }

  if ( $number < 1 ) $number = 1; // Fixes bug elsewhere?
  $placement = str_repeat('x/', $number-1);
  $space_pct = 0.2;
  $max_row = max($row_list);
  $num_rows = count($row_list);

  if ( $conjoined == 'true' ) {
    // Calculate the charge bounding box (same size for all)
    $cbb_height = $bb_h / $num_rows;
    $cbb_width = $bb_w / $max_row;
    for ( $i = 0; $i < $num_rows; $i++ ) {
      $centre_y = $bb_y + ($cbb_height * $i) + ($cbb_height / 2);
      for ( $j = 0; $j < $row_list[$i]; $j++ ) {
        if ( $row_list[$i] == $max_row && $max_row != 1 ) {
          $centre_x = $bb_x + ($cbb_width * $j) + ($cbb_width/2);
        } else {
          $centre_x = $bb_x + ((1000 - $bb_x - $bb_x - ($cbb_width * $row_list[$i]))/2) + ($cbb_width * $j) + ($cbb_width/2);
        }
        $placement .= sprintf ( "%.2f,%.2f,%.2f,%.2f:",$centre_x,$centre_y,$cbb_width,$cbb_height);
      }
    }
  // } elseif ($number == 1) {
  //  $placement = sprintf ( "%.2f,%.2f,%.2f,%.2f:",($bb_x + ($bb_w*0.5)), ($bb_y + ($bb_h*0.5)), ($bb_w*0.8), ($bb_h*0.8));
  } else {
    // Calculate the charge bounding box (same size for all)
    $vert_gap = ($num_rows == 1) ? 0 : ($bb_h * $space_pct) / ($num_rows-1);
    $cbb_height = ($num_rows == 1) ? $bb_h : ($bb_h * (1-$space_pct)) / $num_rows;
    $cbb_width = ($max_row <= 1 ) ? $bb_w : ($bb_w * (1-$space_pct)) / $max_row;
    $horiz_gap = ($bb_w * $space_pct) / max(1,($max_row - 1));
    for ( $i = 0; $i < $num_rows; $i++ ) {
      $centre_y = $bb_y + (($cbb_height + $vert_gap) * $i) + ($cbb_height / 2);
      $offset = ($bb_w - ($cbb_width * $row_list[$i]) - ($horiz_gap * ($row_list[$i]-1)))/2;
      for ( $j = 0; $j < $row_list[$i]; $j++ ) {
        if ( $row_list[$i] != 1 ) {
          $centre_x = $bb_x + $offset + (($cbb_width + $horiz_gap) * $j) + ($cbb_width/2);
        } else {
          $centre_x = $bb_x + ($bb_w/2);
        }
        $placement .= sprintf ( "%.2f,%.2f,%.2f,%.2f",$centre_x,$centre_y,$cbb_width,$cbb_height);
        if ( $facing_in ) {
           if (($j % 2) == 0 ) { // flip alternate charges if facing in ("combattant")
                $placement .= ",0,0,1";
            } else {
                $placement .= ",0,0,0";
            }
        }
        if ( $facing_out ) {
           if (($j % 2) == 1 ) { // flip alternate charges if facing in ("combattant")
                $placement .= ",0,0,1";
            } else {
                $placement .= ",0,0,0";
            }
        }
        $placement .= ":";
      }
    }
  }
  return rtrim($placement,':');
}

