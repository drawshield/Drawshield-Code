<?php
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 02/09/14
 * Time: 19:07
 */

function getEffect($item) {
  global $options, $messages;

  /*
   * 'viewbox' is as per SVG definition, allows for larger borders etc.
   * 'defs' gets added to the defs element, make sure it is valid there
   * 'above' gets added on top of the shield, but BEFORE it is clipped
   * 'filter' - for future expansion perhaps...
   * 'below' goes under the shield, AFTER it is clipped
   */
  static $effectData = array (
    'none' => array ('viewbox' => '0 0 1000 1200', ),
    'stonework' => array (
      'viewbox' => '0 0 1000 1200',
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
      'viewbox' => '-20 -20 1040 1220',
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
      'viewbox' => '0 0 1030 1230',
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
    'vellum' => array (
    'viewbox' => '-20 -20 1040 1220',
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

  $effect = $options['effect'];
  if ( ! array_key_exists($effect,$effectData) )
    $effect = 'none';
  $data = $effectData[$effect];
  if ( array_key_exists('credits', $data))
    $messages->addMessage('licence',$data['credits']);
  if ( isset($data[$item]))
    return $data[$item];
  else
    return null;
}