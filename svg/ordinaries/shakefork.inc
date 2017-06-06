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


$ordinary['shape_spec'] = array ( 'NC' => 'X100Y100a100B424H424a100c100D465C424d100f100G424F465g100', 'CH' => 'X200Y400a71B324H324a71c71D356C324d71f71G324F356g71' );


$ordinary['chargesWithin'] = chiefPresent() ?  'x/220,1060,200,300:780,1060/220,1060,200,300:500,450,180,180:780,1060,200,300/220,900,200,160:780,900:220,840:780,1120/220,900,200,180:780,900:500,350,180,180:220,1120,200,160:780,1120/x' :
  'x/220,760,240,400:780,760/220,760,240,400:500,150,180,180:780,760,240,400/220,600,200,180:780,600:220,840:780,840/220,600,200,180:780,600:500,150,180,180:220,840,200,180:780,840/x';

