<?php /* Copyright 2010 Karl R. Wilcox 

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

$num = $node->getAttribute('number');
if ( $num < 1 ) $num = 1;
if ( existModifierWithKeyterm($node, 'depressed')) {
  $ordinary['body'] = '<path d="M350,-100 500,900 650,-100z" />';
} else {
  switch ( $num ) {
    case 1:
      $ordinary['body'] = array ( 'NC' => '<path d="M50,-8l0,8 450,779 450,-779 0,-8 -900,0z" />', 'CH' => '<path d="M50,292l0,8 450,779 450,-779 0,-8 -900,0z" />' );
      break;
    case 2:
      $ordinary['body'] = '<path d="M 464.67,1086.86 L 345.47,-38.44 L -24.24,-38.44 L 464.67,1086.86 z" />
      <path d="M 535.75,1086.86 L 654.95,-38.44 L 1024.67,-38.44 L 535.75,1086.86 z" />';
      break;
    case 3:
      $ordinary['body'] = '<path d="M 359.61,1086.86 L 256.57,-38.44 L -24.24,-38.44 L 359.61,1086.86 z" />
      <path d="M 635.75,1086.86 L 738.79,-38.44 L 1019.61,-38.44 L 635.75,1086.86 z" />
      <path d="M 321.22,-38.44 L 496.99,1086.86 L 670.74,-38.44 L 321.22,-38.44 z" />';
      break;
    case 4:
    default:
      $ordinary['body'] = '<path d="M 422.24,1086.86 L 191.92,-38.44 L -24.24,-38.44 L 422.24,1086.86 z" />
      <path d="M 262.51,-38.44 L 474.64,1086.86 L 460.50,-38.44 L 262.51,-38.44 z" />
      <path d="M 574.16,1086.86 L 804.47,-38.44 L 1020.64,-38.44 L 574.16,1086.86 z" />
      <path d="M 733.88,-38.44 L 521.75,1086.86 L 535.89,-38.44 L 733.88,-38.44 z" />';
      break;
  }
}

$ordinary['chargesOn'] = array ('w', chiefPresent() ? '300,400,400,300' : '300,100,400,300');

$ordinary['chargesWithin'] = chiefPresent() ? 'x/150,900,180,180:850,900/x/130,800,160,160:870,800:200,1040:800,1040/x/x' : 'x/150,700,180,180:850,700/x/130,600,160,160:870,600:200,840:800,840/x/x';
