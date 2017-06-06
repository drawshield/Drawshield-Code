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


$ordinary['body'] =  array (
            'NC' => '<path d="M200,0 A 450,450 0,0,1 400,400 A 450,450 0,0,1 0,200L0,800 A 450,450, 0,0,1 400,600 A 400,500 0,0,1 100,1200 L 900,1200 A 400,500 0,0,1 600,600 A 450,450 0,0,1 1000,800 L 1000,200 A 450,450 0,0,1 600,400 A 450,450 0,0,1 800,0 Z" />',
            'CH' => '<path d="M200,200 A 450,450 0,0,1 400,600 A 450,450 0,0,1 0,400L0,1000 A 450,450, 0,0,1 400,800 A 400,500 0,0,1 100,1400 L 900,1400 A 400,500 0,0,1 600,800 A 450,450 0,0,1 1000,1000 L 1000,400 A 450,450 0,0,1 600,600 A 450,450 0,0,1 800,200 Z" />');

$ordinary['chargesOn'] = chiefPresent() ? 'x///100,700,120,120:900,700:500,400:500,1100/s/s' : 'x///100,500,120,120:900,500:500,100:500,900/s/s';

$ordinary['chargesWithin'] = chiefPresent() ? 'x/250,950,180,180:750,950/x/250,450,180,180:750,450:250,950:750,950/x/x' : 'x/250,750,180,180:750,750/x/250,250,180,180:750,250:250,750:750,750/x/x';

