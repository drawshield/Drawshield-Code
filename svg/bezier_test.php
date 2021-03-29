<?php

register_shutdown_function(function()
{
    $err = error_get_last();
    if ( !is_null($err) )
        print($err['message'] . " (" . $err['type'] . ") at " . $err['file'] . ":" . $err['line'] . "<br>\n");
});

require_once("bezier.inc");
require_once("shapes.inc");
require_once("edges/edge_type.inc");

$edge_path_embattled = new PolyBezier;
$edge_path_embattled
    ->add_point(new ShieldPos(0, 0))
    ->line_to(new ShieldPos(0, -5))
    ->line_to(new ShieldPos(10, -5))
    ->line_to(new ShieldPos(10, 5))
    ->line_to(new ShieldPos(20, 5))
    ->line_to(new ShieldPos(20, 0))
;
$edge_path_wavy = new PolyBezier;
$edge_path_wavy
    ->add_point(new ShieldPos(0, 0))
    ->cubic_to(
        new ShieldPos(0, -5),
        new ShieldPos(10, -5),
        new ShieldPos(10, 0)
    )
    ->cubic_to(
        new ShieldPos(10, 5),
        new ShieldPos(20, 5),
        new ShieldPos(20, 0)
    )
;
$edge_path_debug = new PolyBezier;
$edge_path_debug
    ->add_point(new ShieldPos(0, 0))
    ->line_to(new ShieldPos(10, 0))
    ->line_to(new ShieldPos(10, 10))
    ->line_to(new ShieldPos(10, 0))
;
$edge_path_flory = new PolyBezier;
$edge_path_flory
    ->add_point(new ShieldPos(0, 0))
    ->line_to(new ShieldPos(5, 10))
    ->line_to(new ShieldPos(10, 0))
;
// $edge = new EdgeType($edge_path_wavy);
// $edge = new EdgeTypeFlory($edge_path_flory);
$edge = EdgeType::load("angled");
$bez = SvgDParser::parse_d(getShape(new ShieldLayout(new ShieldSize(1, 1), "heater")));
$bez->reverse();
$bez
    ->move_to(new ShieldPos(200, 400))
    ->tag_segment(BezierSegmentFlags::NORMAL|BezierSegmentFlags::STRAIGHT)
    ->line_to(new ShieldPos(800, 400))
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->line_to(new ShieldPos(800, 600))
    ->tag_segment(BezierSegmentFlags::UNDERSIDE|BezierSegmentFlags::STRAIGHT)
    ->line_to(new ShieldPos(200, 600))
    ->tag_segment(BezierSegmentFlags::PLAIN)
    ->line_to(new ShieldPos(200, 400))
    ->close()
;
$bez->sub_paths()[0]->segment_tags[0] = BezierSegmentFlags::SYMMETRICAL;
// $bez->sub_paths()[0]->segment_tags[5] = BezierSegmentFlags::NORMAL;
$bez->sub_paths()[0]->segment_tags[10] = BezierSegmentFlags::SYMMETRICAL|BezierSegmentFlags::STRAIGHT;
/*
$bez1 = clone $bez;
$bez1->scale(0.5);
$bez1->translate(new ShieldPos(250, 250));
$bez1->reverse();
$bez->combine_from($bez1);
*/
$comp = $bez->compile();

$modified = new MutableShape();
$edge->apply($comp, $modified, 100, 100);



header("content-type: image/svg+xml");
?>
<svg version="1.1"
    xmlns="http://www.w3.org/2000/svg"
    width='3000'
    height='3000'>
<?php
echo "<path d='" . $bez->to_svg_d() . "' fillrule='evenodd' fill='red' transform='translate(100, 150) scale(.5, .5)' />";

echo "<g transform='translate(700, 150) scale(.5, .5)' >";
echo "<path d='" . $comp->to_svg_d() . "' fill='blue' />";
echo "<path d='" . $modified->to_svg_d() . "' fill='none' stroke='orange' stroke-width='4' />";
echo "</g>";

if ( $edge instanceof EdgeTypeBezier )
{
    $p1 = new ShieldPos(400, 50);
    $p2 = new ShieldPos(300, 120);
    $edge_test = new MutableShape();
    $edge_test->move_to($p1);
    $edge->apply_line_segment($p1, $p2, $edge_test);

    echo "<path d='" . $edge->shape->to_svg_d() . "' fill='none' stroke-width='0.01' stroke='orange' transform='translate(50, 50) scale(100, 100)' />";
    echo "<path d='M {$p1->x} {$p1->y} {$p2->x} {$p2->y}' fill='none' stroke='red' />";
    echo "<path d='" . $edge_test->to_svg_d() . "' fill='none' stroke='orange' />";
}

echo "\n</svg>\n";



