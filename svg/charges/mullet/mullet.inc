<?php
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 12/08/14
 * Time: 18:50
 */

$num_of = getModifierValueByKeyterm($node, 'of-num', true);

if ( !existModifierWithKeyterm($node, "interlaced") )
{
    if ( $num_of  < 4 )
        $num_of = 5;

    $size = 400;
    $cx = $size / 2;
    $cy = $size / 2;
    $stroke = 3;
    $r1 = $size / 2 - $stroke;
    $r_scale = min(($num_of - 5) / (64-5) * .2 + .5, 0.7);
    if ( $num_of == 4 )
        $r_scale = 0.4;
    $r2 = $r1 * $r_scale;
    $d = "M $cx " . ($cy - $r1) . " L ";
    $angle = M_PI * 2 / $num_of;

    for ( $i = 0; $i < $num_of; $i++ )
    {
        $x1 = $cx + sin($angle * $i) * $r1;
        $y1 = $cy - cos($angle * $i) * $r1;

        $x2 = $cx + sin($angle * ($i+0.5)) * $r2;
        $y2 = $cy - cos($angle * ($i+0.5)) * $r2;

        $d .= "$x1 $y1 $x2 $y2 ";
    }

    $d .= "z";

    if ( existModifierWithKeyterm($node, "pierced") )
    {
        $pr = $r2 * $r_scale * 0.9;
        $hx = $cx + $pr;
        $d .= " M $hx,$cy a $pr,$pr 0 0 1 -$pr,$pr $pr,$pr 0 0 1 -$pr,-$pr $pr,$pr 0 0 1 $pr,-$pr $pr,$pr 0 0 1 $pr,$pr z";
    }

    $charge['file'] = "";
    $charge['svgCode'] = <<<SVG
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<svg
    xmlns:dc="http://purl.org/dc/elements/1.1/"
    xmlns:cc="http://creativecommons.org/ns#"
    xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
    xmlns:svg="http://www.w3.org/2000/svg"
    xmlns="http://www.w3.org/2000/svg"
    width="$size"
    height="$size"
    version="1.1"
>
  <metadata>
    <rdf:RDF>
      <cc:Work rdf:about="">
        <dc:format>image/svg+xml</dc:format>
        <dc:type rdf:resource="http://purl.org/dc/dcmitype/StillImage"/>
        <cc:license rdf:resource=""/>
        <dc:title/>
        <dc:creator>
          <cc:Agent>
            <dc:title>(Drawshield internal)</dc:title>
          </cc:Agent>
        </dc:creator>
      </cc:Work>
    </rdf:RDF>
  </metadata>
<g style="fill:#ffff00;fill-opacity:1;fill-rule:evenodd;stroke:#000000;stroke-width:$stroke;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-opacity:1;stroke-dasharray:none" id="body">
    <path d="$d" />
</g>
</svg>
SVG;
}
else
{
    if ( $num_of < 4 )
        $num_of = 5;

    $suffix = '-of' . $num_of . '-voided-interlaced';

    $file = getBestMatchFilename(__dir__, 'mullet', rtrim($suffix,'-'));
    if ($file != '')
        $charge['file'] = $file;
}
