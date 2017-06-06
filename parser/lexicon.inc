<?php /* Copyright 2014 Karl R. Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

class lexicon {
   protected $patterns = array(); // of objects
   protected $language;
    
   public function getPatterns($key) {
    //if ( !array_key_exists ( $key, $this->patterns ) and file_exists("$this->language/$key.inc") )
    //  include "$this->language/$key.inc";
    if ( array_key_exists ( $key, $this->patterns )) {
      return ( $this->patterns[$key] );
    } else {
      return null;
    }
  }
  
  public function addPatterns($key,$newPatterns) {
    $this->patterns[$key] = $newPatterns;
  }

  public function removePatterns($key) {
    if (array_key_exists($key,$this->patterns))
      unset($this->patterns[$key]);
  }

/*  
  public function listKeyterms() {
    $keyterms = array();
    
    foreach ( $this->patterns as $patternSet ) {
      foreach ( $patternSet as $pattern ) {
        if ( is_array($pattern) ) 
          $keyterm = $pattern[1];
        else
          $keyterm = $pattern;
        if ( !array_search($keyterm,$keyterms) )
          $keyterms[] = $keyterm;
      }
    }
    return $keyterms;
  } */
  
};


