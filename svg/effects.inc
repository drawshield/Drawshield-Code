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


function getEffect($effect, $item, $width=1000, $height=1200) {
  global $messages;

  /*
   * 'viewbox' is as per SVG definition, allows for larger borders etc.
   * 'defs' gets added to the defs element, make sure it is valid there
   * 'above' gets added on top of the shield, but BEFORE it is clipped
   * 'filter' - for future expansion perhaps...
   * 'below' goes under the shield, AFTER it is clipped
   */
  static $effectData = array (
    'none' => array ('viewbox' => array(0, 0, 0, 0) ), // adjustments
      'fabric' => array (
       'viewbox' => array(0, 0, 0, 0),
          'credits' => 'Inkscape, Filters->Bumps->Linen Canvas ',
          'defs' => '<filter
       id="fabric"
       style="color-interpolation-filters:sRGB;">
      <feTurbulence
         id="feTurbulence2861"
         baseFrequency="0.4"
         numOctaves="5" />
      <feConvolveMatrix
         id="feConvolveMatrix2863"
         targetY="2"
         targetX="2"
         kernelMatrix="-4 0 0 0 -4 0 -2 0 -2 0 0 0 -10 0 0 0 -2 0 -2 0 -4 0 0 0 -4 "
         order="5 5" />
      <feConvolveMatrix
         id="feConvolveMatrix2865"
         targetY="1"
         targetX="1"
         kernelMatrix="2 0 2 0 -5 0 2 0 2 "
         order="3 3" />
      <feColorMatrix
         id="feColorMatrix2867"
         result="result1"
         type="saturate" />
      <feComposite
         id="feComposite2869"
         result="result2"
         k3="0.5"
         k2="0.5"
         k1="1.5"
         operator="arithmetic"
         in2="SourceGraphic" />
      <feBlend
         id="feBlend2871"
         result="result4"
         in2="result1"
         mode="normal" />
      <feColorMatrix
         id="feColorMatrix2873"
         type="luminanceToAlpha" />
      <feDiffuseLighting
         id="feDiffuseLighting2877"
         result="result3"
         diffuseConstant="1"
         surfaceScale="10">
        <feDistantLight
           id="feDistantLight2875"
           elevation="35"
           azimuth="180" />
      </feDiffuseLighting>
      <feComposite
         id="feComposite2879"
         result="result5"
         k3="0.5"
         k1="1.8"
         operator="arithmetic"
         in2="result4" />
      <feBlend
         id="feBlend2881"
         result="result6"
         in2="result3"
         mode="normal" />
      <feComposite
         id="feComposite2883"
         operator="in"
         in2="SourceGraphic" />
    </filter>',
          'filter' => ' filter="url(#fabric)" ',
      ),
    'inked' => array (
        'viewbox' => array(0,0,0,0),
        'credits' => 'Filter by Inkscape: old postcard + oil painting',
        'defs' => '
    <filter
       id="inkdrawn"
       style="color-interpolation-filters:sRGB;">
      <feFlood
         id="feFlood6288"
         result="result1"
         flood-opacity="0.1"
         flood-color="rgb(255,204,0)" />
      <feBlend
         id="feBlend6290"
         result="result6"
         mode="darken"
         in2="SourceGraphic" />
      <feComposite
         id="feComposite6292"
         k2="1"
         operator="arithmetic"
         result="result8"
         in2="result6" />
      <feColorMatrix
         id="feColorMatrix6294"
         in="result8"
         type="hueRotate"
         values="0"
         result="result5" />
      <feConvolveMatrix
         id="feConvolveMatrix6296"
         targetY="1"
         targetX="1"
         in="result5"
         divisor="1"
         kernelMatrix="0 -1 0 -1 5.05 -1 0 -1 0 "
         order="3 3"
         result="result1" />
      <feGaussianBlur
         id="feGaussianBlur6298"
         in="result1"
         stdDeviation="2"
         result="result2" />
      <feComponentTransfer
         id="feComponentTransfer6306"
         in="result2"
         result="result4">
        <feFuncR
           id="feFuncR6300"
           tableValues="0 0 0.2 0.2 0.4 0.4 0.6 0.6 0.8 0.8 1 1 1"
           type="table" />
        <feFuncG
           id="feFuncG6302"
           tableValues="0 0 0.2 0.2 0.4 0.4 0.6 0.6 0.8 0.8 1 1 1"
           type="table" />
        <feFuncB
           id="feFuncB6304"
           tableValues="0 0 0.2 0.2 0.4 0.4 0.6 0.6 0.8 0.8 1 1 1"
           type="table" />
      </feComponentTransfer>
      <feBlend
         id="feBlend6308"
         in="result4"
         mode="darken"
         result="result3"
         in2="result1" />
      <feColorMatrix
         id="feColorMatrix6310"
         result="result7"
         values="1"
         type="saturate" />
      <feComposite
         id="feComposite6312"
         result="fbSourceGraphic"
         operator="in"
         in2="SourceGraphic" />
      <feColorMatrix
         id="feColorMatrix7079"
         values="0 0 0 -1 0 0 0 0 -1 0 0 0 0 -1 0 0 0 0 1 0"
         in="fbSourceGraphic"
         result="fbSourceGraphicAlpha" />
      <feMorphology
         result="result0"
         in="fbSourceGraphic"
         radius="2"
         id="feMorphology7081" />
      <feTurbulence
         result="result91"
         baseFrequency="0.078"
         numOctaves="1"
         id="feTurbulence7083" />
      <feDisplacementMap
         yChannelSelector="G"
         xChannelSelector="R"
         scale="6.3"
         in="result0"
         id="feDisplacementMap7085"
         in2="result91" />
    </filter>',
          'filter' => ' filter="url(#inkdrawn)" ',
    ),
    'stonework' => array (
      'viewbox' => array(0, 0, 0, 0),
      'credits' => 'Filters by Inkscape: emboss + roughen inside + rough paper',
      'defs' => '<filter
       id="stonework"
       height="1.3"
       width="1.3"
       y="-0.15"
       x="-0.15"
       color-interpolation-filters="sRGB">
      <feColorMatrix
         id="feColorMatrix3589"
         result="result2"
         type="luminanceToAlpha" />
      <feDiffuseLighting
         id="feDiffuseLighting3591"
         diffuseConstant="0.6"
         result="result1"
         surfaceScale="-10">
        <feDistantLight
           id="feDistantLight3593"
           azimuth="225"
           elevation="35" />
      </feDiffuseLighting>
      <feComposite
         id="feComposite3595"
         in2="SourceGraphic"
         result="result3"
         k3="0.5"
         k1="1.5"
         in="result1"
         operator="arithmetic" />
      <feComposite
         id="feComposite3597"
         in2="result3"
         result="result5"
         operator="in"
         in="result3" />
      <feBlend
         id="feBlend3599"
         in2="result1"
         result="result6"
         mode="normal"
         in="result5" />
      <feComposite
         id="feComposite3601"
         in2="result6"
         result="result7"
         operator="arithmetic"
         k2="1" />
      <feComposite
         id="feComposite3603"
         in2="SourceGraphic"
         in="result7"
         operator="in"
         result="fbSourceGraphic" />
      <feColorMatrix
         result="fbSourceGraphicAlpha"
         in="fbSourceGraphic"
         values="0 0 0 -1 0 0 0 0 -1 0 0 0 0 -1 0 0 0 0 1 0"
         id="feColorMatrix3605" />
      <feTurbulence
         id="feTurbulence3607"
         numOctaves="5"
         seed="0"
         type="fractalNoise"
         baseFrequency="0.08"
         in="fbSourceGraphic" />
      <feGaussianBlur
         id="feGaussianBlur3609"
         stdDeviation="0.5"
         result="result91" />
      <feDisplacementMap
         id="feDisplacementMap3611"
         in2="result91"
         scale="20"
         yChannelSelector="G"
         xChannelSelector="R"
         in="fbSourceGraphic"
         result="result1" />
      <feComposite
         id="feComposite3613"
         in2="fbSourceGraphic"
         in="result1"
         operator="atop"
         result="fbSourceGraphic" />
      <feColorMatrix
         result="fbSourceGraphicAlpha"
         in="fbSourceGraphic"
         values="0 0 0 -1 0 0 0 0 -1 0 0 0 0 -1 0 0 0 0 1 0"
         id="feColorMatrix3615" />
      <feTurbulence
         id="feTurbulence3617"
         type="fractalNoise"
         baseFrequency="0.04"
         numOctaves="5"
         seed="0"
         result="result4"
         in="fbSourceGraphic" />
      <feDisplacementMap
         id="feDisplacementMap3619"
         in2="result4"
         in="fbSourceGraphic"
         yChannelSelector="G"
         xChannelSelector="R"
         scale="10"
         result="result3" />
      <feDiffuseLighting
         id="feDiffuseLighting3621"
         lighting-color="rgb(233,230,215)"
         diffuseConstant="1"
         surfaceScale="2"
         result="result1"
         in="result4">
        <feDistantLight
           id="feDistantLight3623"
           azimuth="235"
           elevation="40" />
      </feDiffuseLighting>
      <feComposite
         id="feComposite3625"
         in2="result1"
         operator="in"
         in="result3"
         result="result2" />
      <feComposite
         id="feComposite3627"
         in2="result1"
         result="result5"
         operator="arithmetic"
         k1="1.7" />
      <feBlend
         id="feBlend3629"
         in2="result3"
         in="result5"
         mode="normal" />
    </filter>',
      'filter' => ' filter="url(#stonework)" ',
    ),
    'plaster' => array (
      'viewbox' => array(-20, -20, 40, 20),
      'credits' => '',
      'defs' => '<filter id="plaster" >
      <feTurbulence
         id="feTurbulence3100"
         numOctaves="5"
         baseFrequency="0.2"
         type="fractalNoise" result="speckles"/>
           <feColorMatrix type="luminanceToAlpha" in="SourceGraphic"/>
<feDiffuseLighting diffuseConstant="1" surfaceScale="10" result="diffuse3">
<feDistantLight elevation="28" azimuth="90" /></feDiffuseLighting>
<feComposite operator="in" in2="SourceGraphic" result="plaster"/>
<feOffset in="plaster" dx="7" dy="7" result="offset"/>
<feBlend mode="normal" in="plaster" in2="offset" />
         <feBlend mode="lighten" in2="speckles" />
</filter>',
      'filter' => ' filter="url(#plaster)" ',
    ),
    'shiny' => array (
      'viewbox' => array(0, 0, 30, 30),
      'defs' => '
      <filter id="shiny" x="0" y="0" width="150%" height="150%">
      <feGaussianBlur in = "SourceAlpha" stdDeviation = "4" result = "blur1"/>
        <feSpecularLighting result = "specOut" in = "blur1" specularExponent = "20" lighting-color = "#696969">
            <fePointLight x = "300" y = "300" z = "500"/>
        </feSpecularLighting>
        <feComposite result="highlight" in = "SourceGraphic" in2 = "specOut" operator = "arithmetic" k1 = "0" k2 = "1" k3 = "1" k4 = "0"/>
      <feOffset result="offOut" in="SourceGraphic" dx="14" dy="14" />
      <feColorMatrix result = "matrixOut" in = "offOut" type = "matrix" values = "0.2 0 0 0 0 0 0.2 0 0 0 0 0 0.2 0 0 0 0 0 1 0"/>
      <feGaussianBlur result="blurOut" in="matrixOut" stdDeviation="10" />
      <feBlend in="highlight" in2="blurOut" mode="normal"/>
    </filter>',
      'filter' => ' filter="url(#shiny)" ',
    ),
    'ripples' => array (
      'viewbox' => array(0, 0, 30, 30),
      'credits' => 'http://www.inkscapeforum.com/memberlist.php?mode=viewprofile&u=3691',
      'defs' => '
      <filter
      id="ripples">
     <feTurbulence
        id="feTurbulence3052"
        type="fractalNoise"
        baseFrequency="0.005"
        numOctaves="2"
        result="result1"
        seed="0" />
     <feDisplacementMap
        id="feDisplacementMap3054"
        in2="result1"
        scale="50"
        in="SourceGraphic"
        xChannelSelector="B"
        yChannelSelector="R" />
   </filter>',
      'filter' => ' filter="url(#ripples)" ',
    ),
    'vellum' => array (
    'viewbox' => array(-20, -20, 40, 20),
      'defs' => '<filter id="vellum"  color-interpolation-filters="sRGB">
<feFlood flood-opacity="1" flood-color="#d3b06c"/>
         <feBlend mode="normal" in2="SourceGraphic" result="background" />

<feTurbulence numOctaves="5" seed="0" type="fractalNoise" baseFrequency="0.08" />
<feGaussianBlur stdDeviation="0.5" result="result91" />
<feDisplacementMap scale="20" yChannelSelector="G" xChannelSelector="R" in="SourceGraphic" result="result1" in2="result91" />
<feComposite in="result1" in2="background" operator="atop" result="roughen" />

      <feTurbulence
         id="feTurbulence3100"
         numOctaves="5"
         baseFrequency="0.2"
         type="fractalNoise" result="speckles"/>
               <feTurbulence
         id="feTurbulence93100"
         numOctaves="3"
         baseFrequency="0.03 0.009"
         type="turbulence" result="waves"/>

<feBlend mode="normal" in="speckles" in2="waves"/>


      <feColorMatrix
         id="feColorMatrix3102"
         result="result0"
         values="1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 3 -1 " />
      <feFlood
         id="feFlood3104"
         flood-opacity="1"
         flood-color="#cccccc"
         result="result1" />
      <feBlend
         id="feBlend3106"
         in2="roughen"
         mode="normal"
         in="result1"
         result="result2" />
      <feComposite
         id="feComposite3108"
         in2="result0"
         operator="out"
         result="result3" />



         <feBlend mode="multiply" in="result3" in2="roughen" /></filter>
         ',

      'filter' => ' filter="url(#vellum)" ',
  ),
  );

  // Some synonyms
  switch ($effect) {
     case 'waving':
     case 'ripple':
     case 'rippling':
      $effect = 'ripples';
      break;
    case "plain": 
    case "flat": 
      $effect = 'none';
      break;
    case "stone":
    case "stony":
    case "stonelike":
      $effect = 'stonework';
      break;
     case "plasterwork":
      $effect = 'plaster';
      break;
     case "leather":
      $effect = 'vellum';
      break;
     case "metal":
      $effect = 'shiny';
      break;
  }
  if ( ! array_key_exists($effect,$effectData) )
    $effect = 'none';
  $data = $effectData[$effect];
  if ( array_key_exists('credits', $data))
    $messages->addMessage('licence',$data['credits']);
  if ( isset($data[$item])) {
    $retval = $data[$item];
    if ($item == 'viewbox') {
      $retval = $data[$item][0] . ',' . $data[$item][1] . ',' . ($width + $data[$item][2]) . ',' . ($height + $data[$item][2]);
    } 
    return $retval;
  } else {
    return null;
  }
}
