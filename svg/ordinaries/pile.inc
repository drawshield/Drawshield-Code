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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
 */

$num = $node->getAttribute('number');
if ( $num < 1 ) $num = 1;

if ( existModifierWithKeyterm($node,'throughout') )
    $bottom = $layout->size->height;
else
    $bottom = $layout->size->height * 0.9;

if ( existModifierWithKeyterm($node, 'depressed')) {
  $ordinary['body'] = '<path d="M350,-100 500,900 650,-100z" />';
} elseif ( existModifierWithKeyterm($node, 'wavy')) {
  $ordinary['body'] = '<path d="M-70.81451159 -148.2279915C-16.8506249 -136.3001177 -0.6017534701 -104.87627206 -22.06654284 -53.95683568C-43.5258224 -3.02674223711 -27.27695131 28.39710581 26.68142591 40.31431602C80.64531569 52.24218467 96.8961994 83.66603237 75.43140625 134.5854636C53.97213081 185.5155646 70.22100499 216.9374038 124.179375 228.8546068C178.1432689 240.7824806 194.3921369 272.2063283 172.9273386 323.1257585C151.4680611 374.0558595 167.7189479 405.4797072 221.677321 417.3969102C275.6412046 429.324784 291.8900829 460.7486214 270.4252949 511.6680619C248.9660174 562.5981526 265.2148854 594.0220003 319.1732585 605.9392136C373.1371524 617.8670771 389.3880289 649.2909454 367.9232409 700.2103756C346.4639737 751.1404663 362.7128314 782.564314 416.6712148 794.4815273C470.6350984 806.4093908 486.8839664 837.8332385 465.4191784 888.752679C444.3156629 938.8384272 459.7096 970.0445123 511.5438659 982.4001966C511.5438659 982.4001966 511.5418574 983.0238307 511.5418574 983.0238307C511.9894439 982.9249713 512.4131035 982.8135562 512.8555091 982.7120085C513.2979147 982.8135562 513.721564 982.9249713 514.1691608 983.0238307C514.1691608 983.0238307 514.1691608 982.4001966 514.1691608 982.4001966C566.0019023 970.044193 581.3950978 938.8378195 560.2918398 888.752679C538.8270415 837.8332385 555.0759198 806.4093908 609.0398034 794.4815273C662.9981868 782.564314 679.2470445 751.1404663 657.7877773 700.2103756C636.322979 649.2909454 652.5738658 617.8670771 706.5377494 605.9392136C760.4961328 594.0220003 776.7450008 562.5981526 755.2857233 511.6680619C733.8209353 460.7486214 750.0698136 429.324784 804.0336869 417.3969102C857.9920703 405.4797072 874.2429468 374.0558595 852.7836693 323.1257585C831.3188813 272.2063283 847.5677493 240.7824806 901.5316432 228.8546068C955.490006 216.9374038 971.7388843 185.5155646 950.2796068 134.5854636C928.8148188 83.66603237 945.0656953 52.24218467 999.0295892 40.31431602C1052.987952 28.39710581 1069.236717 -3.02674223711 1047.777594 -53.95683568C1026.3127545 -104.87627206 1042.561674 -136.3001177 1096.525537 -148.2279915C1096.525537 -148.2279915 514.9283738 -143.7905043 514.1168574 -145.946696C514.1168574 -145.946696 -70.81451159 -148.2279915 -70.81451159 -148.2279915C-70.81451159 -148.2279915 -70.81451159 -148.2279915 -70.81451159 -148.2279915" />';
} elseif (existModifierWithKeyterm($node,'embowed')) {
  $ordinary['body'] = '<path d="m -150,-100 c 300,0 650,800 650,800 0,0 350,-800 650,-800 z" />';
} else {
    $builder = new MutableShape();

  switch ( $num ) {
    case 1:
        $builder->combine_from(SvgDParser::parse_d("M 50,-8 V 0 L 500,$bottom 950,0 V -8 Z"));
      break;
    case 2:
        $builder->combine_from(SvgDParser::parse_d("M 464.67,$bottom L 345.47,-38.44 L -24.24,-38.44 L 464.67,$bottom z"));
        $builder->combine_from(SvgDParser::parse_d("M 535.75,$bottom L 654.95,-38.44 L 1024.67,-38.44 L 535.75,$bottom z"));
      break;
    case 3:
      $builder->combine_from(SvgDParser::parse_d("M 359.61,$bottom L 256.57,-38.44 L -24.24,-38.44 L 359.61,$bottom z"));
      $builder->combine_from(SvgDParser::parse_d("M 635.75,$bottom L 738.79,-38.44 L 1019.61,-38.44 L 635.75,$bottom z"));
      $builder->combine_from(SvgDParser::parse_d("M 321.22,-38.44 L 496.99,$bottom L 670.74,-38.44 L 321.22,-38.44 z"));
      break;
    case 4:
    default:
      $builder->combine_from(SvgDParser::parse_d("M 422.24,$bottom L 191.92,-38.44 L -24.24,-38.44 L 422.24,$bottom z"));
      $builder->combine_from(SvgDParser::parse_d("M 262.51,-38.44 L 474.64,$bottom L 460.50,-38.44 L 262.51,-38.44 z"));
      $builder->combine_from(SvgDParser::parse_d("M 574.16,$bottom L 804.47,-38.44 L 1020.64,-38.44 L 574.16,$bottom z"));
      $builder->combine_from(SvgDParser::parse_d("M 733.88,-38.44 L 521.75,$bottom L 535.89,-38.44 L 733.88,-38.44 z"));
      break;
  }
  $ordinary['shape_spec'] = $builder;
}

$ordinary['chargesOn'] = ["w", new ShieldRect(
    new ShieldPos($layout->size->width / 2, $bottom / 3),
    new ShieldSize($bottom * 0.6, $bottom * 0.6)
)];

$ordinary['chargesWithin'] = 'x/150,700,180,180:850,700/x/130,600,160,160:870,600:200,840:800,840/x/x';
