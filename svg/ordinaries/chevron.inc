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


if ( existModifierWithKeyterm($node,'throughout') )
  // $ordinary['shape_spec'] = array ( 'NC' => 'V1X500Y0K1200l200M1000L1000m200J1200', 'CH' => 'V1X500Y300K1000l200M800L800m200J1000');
  $ordinary['body'] = array ( 'NC' => '<path fill-rule="evenodd" d="M500,0l600,1039 -100,173 -500,-866 -500,866 -100,-173 600,-1039z" />', 'CH' => '<path fill-rule="evenodd" d="M500,300l600,1039 -100,173 -500,-866 -500,866 -100,-173 600,-1039z" />');
elseif ( existModifierWithKeyterm($node,'fracted') )
  $ordinary['shape_spec'] = array ( 'NC' => 'V1X500Y200B100c282B800d200F600g282f100d100c282D600f200H800g282H100', 'CH' => 'V1X500Y300B100c282B800d200F600g282f100d100c282D600f200H800g282H100');
elseif ( existModifierWithKeyterm($node,'debruised') )
  $ordinary['shape_spec'] = array ( "NC" => 'X-65.6Y1065.6f200H800b200h200B800d200F800D800', 'CH' => 'X-65.6Y1165.6f200H800b200h200B800d200F800D800' );
elseif ( existModifierWithKeyterm($node,'couped') )
  $ordinary['shape_spec'] = array ( "NC" => 'V1X200Y800f200H624B624d200F424D424', "CH" => 'V1X200Y1000f200H624B624d200F424D424' );
elseif ( existModifierWithKeyterm($node,'inarched') )
  $ordinary['body'] = array (
                "NC" => '<path fill-rule="evenodd" d="m 720,900 c 0,300 -100,500 -220,500 -120,0 -220,-200 -220,-500 0,-250 100,-460 220,-460 120,0 220,210 220,460 z m -80,-10 c 0,210 -60,310 -140,310 -80,0 -140,-100 -140,-310 0,-200 60,-350 140,-350 80,0 140,150 140,350 z" />
                      <path fill-rule="evenodd" d="M-65.6,1065.6l-141.4,-141.4l707,-707l707,707l-141.4,141.4l-565.6,-565.6l-565.6,565.6Z" />',
                'CH' => '<path fill-rule="evenodd" d="m 720,1100 c 0,300 -100,500 -220,500 -120,0 -220,-200 -220,-500 0,-250 100,-460 220,-460 120,0 220,210 220,460 z m -80,-10 c 0,210 -60,310 -140,310 -80,0 -140,-100 -140,-310 0,-200 60,-350 140,-350 80,0 140,150 140,350 z" />
                         <path fill-rule="evenodd" d="M-65.6,1215.6l-141.4,-141.4l707,-707l707,707l-141.4,141.4l-565.6,-565.6l-565.6,565.6Z" />' );
else  {
  switch ( $node->getAttribute('number') ) {
    case '2':
      $ordinary['shape_spec'] = array("NC" => 'V1X500Y400D800f200H1000B1000d200F800     V1X500Y800D800f200H1000B1000d200F800', "CH" => 'V1X500Y500D800f141H941B941d141F800  V1X500Y850D800f141H941B941d141F8');
      break;
    case '3':
      $ordinary['shape_spec'] = array("NC" => 'V1X500Y200D800f141H941B941d141F800  V1X500Y550D800f141H941B941d141F8   V1X500Y900D800f141H941B941d141F8', "CH" => 'V1X500Y500D800f141H941B941d141F800   V1X500Y800D800f141H941B941d141F   V1X500Y1100D800f141H941B941d141F');
      break;
    default: // just do one
      $ordinary['shape_spec'] = array("NC" => 'V1X500Y500D800f200H1000B1000d200F800', "CH" => 'V1X500Y650D800f200H1000B1000d200F800');
      break;
  }
  if ( existModifierWithKeyterm($node,'compony1') )
    $ordinary['body2'] = array ( 'NC' => '<path d="M 957,542 672,828 797,953 1080,668 z M 45,542 331,828 205,953 -80,668 z M 296,292 171,417 456,702 501,657 546,702 832,417 706,292 501,497 296,292 z" />',
                                  'CH' => '<path d="M 957,692 672,978 797,1100 1080,818 z M 45,692 331,978 205,1100 -80,818 z M 296,442 171,567 456,852 501,807 546,852 832,567 706,442 501,647 z" />' );
  elseif (existModifierWithKeyterm($node,'compony2') )
     $ordinary['body2'] = array ( 'NC' => '<path d="M 941,810 1070,684 1190,810 1070,935 941,810 M 815,684 690,810 815,935 941,810 z M 690,559 815,434 941,559 815,684 690,559 M 62.7,810 -62.7,684 -188,810 -62.7,935 z M 188,684 314,810 188,935 62.7,810 188,684 M 314,559 188,434 62.7,559 188,684 z M 439,434 502,496 564,434 690,559 502,747 314,559 439,434 m 0,0 L 314,308 502,120 690,308 564,434 502,371 z" />',
                                  'CH' => '<path d="M 941,960 1070,834 1190,960 1070,1090 941,960 M 815,834 690,960 815,1090 941,960 z M 690,709 815,584 941,709 815,834 690,709 M 62.7,960 -62.7,834 -188,960 -62.7,1090 z M 188,834 314,960 188,1090 62.7,960 188,834 M 314,709 188,584 62.7,709 188,834 z M 439,584 502,646 564,584 690,709 502,897 314,709 439,584 m 0,0 L 314,458 502,270 690,458 564,584 502,521 z" />');

  if ( division_to_modifier($node, 'quarterly') )
    $ordinary['body2'] = array ( 'NC' => 'V1X500Y100C259D848g259h848   V1X1100Y958c259f849G259B849',
                                  'CH' => 'V1X500Y250C259D848g259h848   V1X1100Y1108c259f849G259B849' );
}

$ordinary['chargesOn'] = chiefPresent() ? '500,550,100,100/250,775,100,100,45:750,775,,,-45/250,775,100,100,45:500,550,,,0:750,775,,,-45/200,825,100,100,45:400,625:600,625,,,-45:800,825/200,825,100,100,45:350,675:500,550,,,0:650,675,,,-45:800,825/150,875,100,100,45:250,775:350,675:650,675,,,-45:750,775:850,875' :
  '500,400,100,100/250,625,100,100,45:750,625,,,-45/250,625,100,100,45:500,400,,,0:750,625,,,-45/200,675,100,100,45:400,475:600,475,,,-45:800,675/200,675,100,100,45:350,525:500,400,,,0:650,525,,,-45:800,675/150,725,100,100,45:250,625:350,525:650,525,,,-45:750,625:850,725';

if ( existModifierWithKeyterm($node,'fracted'))
  $ordinary['chargeWithin'] = chiefPresent() ? 'x/200,500,300,300:800,500,300,300/200,500,340,340:500,1050,200,200:800,500,340,340/s/s/s' :
    'x/200,500,300,300:800,500,300,300/200,500,340,340:500,1050,200,200:800,500,340,340/200,500,200,240:500,140,200,160:500,1050,200,200:800,500,200,240/s/s';
else
  $ordinary['chargesWithin'] = chiefPresent() ? 'x/x/180,480,180,180:500,950,280,280:820,480,180,180/s/s' :
    'x/500,140,300,180:500,850,300,300/200,200,240,240:500,850,300,300:800,200,240,240/200,240,200,240:500,140,200,160:500,850,300,300:800,240,200,240/200,240,200,240:500,140,200,160:400,900,160,200:600,900:800,240,200,240/200,240,200,240:500,140,200,160:400,950,160,200:500,720:600,950:800,240,200,240';

// TODO See original (unfinished) code below, about aspect groups?
/*
 *
      case 'chevron':
        if (get_mod($ordinary,'throughout'))
          $placement = 'n';
        elseif (get_mod($ordinary,'rompu'))
          $placement = ($chief)? 'x/200,500,300,300:800,500,300,300/200,500,340,340:500,1050,200,200:800,500,340,340/s/s/s' :
                                 'x/200,500,300,300:800,500,300,300/200,500,340,340:500,1050,200,200:800,500,340,340/200,500,200,240:500,140,200,160:500,1050,200,200:800,500,200,240/s/s';
        else {
          if ( $aspect_group == 'wide' )
            $placement = ($chief) ? 'x/x/180,480,180,180:500,950,280,280:820,480,180,180/s/s' :
                                'x/500,140,300,180:500,850,300,300/200,200,240,240:500,850,300,300:800,200,240,240/200,240,200,240:500,140,200,160:500,850,300,300:800,240,200,240/200,240,200,240:500,140,200,160:400,900,160,200:600,900:800,240,200,240/200,240,200,240:500,140,200,160:400,950,160,200:500,720:600,950:800,240,200,240';
          else
            $placement = ($chief) ? 'x/x/180,480,180,180:500,950,280,280:820,480,180,180/s/s' : // TODO FINISH THESE (only 3 below is done)
                                'x/500,140,300,180:500,850,300,300/360,150,200,240:500,850,300,300:640,150,200,240/200,240,200,240:500,140,200,160:500,850,300,300:800,240,200,240/200,240,200,240:500,140,200,160:400,900,160,200:600,900:800,240,200,240/200,240,200,240:500,140,200,160:400,950,160,200:500,720:600,950:800,240,200,240';
        }
        break; *
 */

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  $ref = chiefPresent() ? 625 : 475;
  for ($i = 5; $i < 450; $i += 80) {
    $rivetList .= '<g transform="translate(' . $i . ',' . (245 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . $i . ',' . (455 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . (970 - $i) . ',' . (245 + $ref) . ')">' . $rivet . '</g>';
    $rivetList .= '<g transform="translate(' . (970 - $i) . ',' . (455 + $ref) . ')">' . $rivet . '</g>';
    $ref -= 80;
  }
  $ref = chiefPresent() ? 150 : 0;
  $rivetList .= '<g transform="translate(485,' . (245 + $ref) . ')">' . $rivet . '</g>';
  $rivetList .= '<g transform="translate(485,' . (455 + $ref) . ')">' . $rivet . '</g>';
  $ordinary['body3'] = $rivetList;
}
