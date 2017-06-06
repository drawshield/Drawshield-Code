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

if ( $node->getAttribute('number') == '2')
  $ordinary['body'] = array ( 'NC' => '<path d="M-8,-8l308,508 -308,508 0,-1016z" /><path d="M1008,-8l-308,508 308,508 0,-1016z" />', 'CH' => '<path d="M-8,292l308,508 -308,508 0,-1016z" /><path d="M1008,292l-308,508 308,508 0,-1016z" />' );
else
  $ordinary['body'] = array ( 'NC' => '<path d="M-8,-8l308,508 -308,508 0,-1016z" />', 'CH' => '<path d="M-8,292l308,508 -308,508 0,-1016z" />' );

$ordinary['chargesWithin'] = array ('n','300,300,400,600');
