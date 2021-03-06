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


$division['div2'] = createBody( 'O1Q1X500Y500c800e600g800A600   O1Q1X500Y500g600a600c600E600',
    getModifierByType($node,'linetype', false),60, $layout);
$division['div4'] = createBody( 'O1Q1X500Y{%50}c{%80}e600g{%80}A600',
  getModifierByType($node,'linetype', false),60, $layout);
$division['div3'] = createBody( 'O1Q1X500Y{%50}A600c{%80}e600g{%80}',
  getModifierByType($node,'linetype', false),60, $layout);
