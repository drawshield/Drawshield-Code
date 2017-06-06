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


$ordinary['body'] = array ( 'NC' => '<path d="M200,200h600v600a400,200,0,0,1 -300,200a400,200,0,0,1 -300,-200v-600z" />',
                          'CH' => '<path d="M200,500h600v300a400,200,0,0,1 -300,200a400,200,0,0,1 -300,-200v-300" />');

$ordinary['chargesOn'] = array('w', chiefPresent() ? '300,600,400,250' : '300,300,400,500');
