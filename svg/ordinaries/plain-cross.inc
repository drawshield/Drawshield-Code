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


$ordinary['shape_spec'] = array ( 'NC' => 'X-100Y350A450G450a300C450A450c300E450C650e300G650E450g300',
                                  'CH' => 'X400Y200a200C400A500c200E500C500e200G500E500g200A500G400' );

$ordinary['chargesOn'] = chiefPresent() ? '500,700,160,160////250,700,160,160:750,700:500,450:500,700:500,950/x' :
  '500,500,200,200//200,500,220,220:800,500:500,250/200,500,200,200:500,220:800,500:500,800//200,500,200,200:500,200:800,500:500,500:500,800:500,1050';

$ordinary['chargesWithin'] = chiefPresent() ? 'x/200,450,180,160:800,450/x/200,450,180,160:800,450:200,940:800:840/s/s' : 'x/175,750,180,300:825,750/x/175,200,180,160:825,200:175,840:825:840/s/s';

if ( existModifierWithKeyterm($node, 'riveted')) {
  include '_rivet.inc';
  $rivetList = '';
  if ( chiefPresent() ) {
    for ($i = 10; $i < 1000; $i += 80) {
      if ($i < 400 or $i >= 600) {
        $rivetList .= '<g transform="translate(' . $i . ',615)">' . $rivet . '</g>';
        $rivetList .= '<g transform="translate(' . $i . ',765)">' . $rivet . '</g>';
      }
    }
    for ($i = 315; $i < 1200; $i += 80) {
      if ($i <= 600 or $i >= 790) {
        $rivetList .= '<g transform="translate(415,' . $i . ')">' . $rivet . '</g>';
        $rivetList .= '<g transform="translate(565,' . $i . ')">' . $rivet . '</g>';
      }
    }
  } else {
    for ($i = 10; $i < 1000; $i += 80) {
      if ($i < 350 or $i >= 650) {
        $rivetList .= '<g transform="translate(' . $i . ',365)">' . $rivet . '</g>';
        $rivetList .= '<g transform="translate(' . $i . ',615)">' . $rivet . '</g>';
      }
    }
    for ($i = 5; $i < 1200; $i += 80) {
      if ($i < 350 or $i >= 640) {
        $rivetList .= '<g transform="translate(365,' . $i . ')">' . $rivet . '</g>';
        $rivetList .= '<g transform="translate(615,' . $i . ')">' . $rivet . '</g>';
      }
    }
  }

  $ordinary['body3'] = $rivetList;
}
