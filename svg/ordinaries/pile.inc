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
