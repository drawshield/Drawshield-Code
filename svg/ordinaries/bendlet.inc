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

include 'svg/ordinaries/bar.inc';

$ordinary['chargesWithin'] = chiefPresent() ? 'x/750,550,250,250:250,950,180,180/250,950,180,180:600,450,200,200:800,650/150,850,180,180:350,1050:600,450,200,200:800,650/600,450,140,140:800,450:800,650:200,900:350,1050:150,1050/s' :
  'x/750,250,250,250:250,750/750,250,250,250:200,700,200,200:400,900/200,650,200,200:400,850:600,150:800,350//600,200,140,140:800,200:800,400:200,600:400,800:200,800';

