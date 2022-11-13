<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

     DrawShield is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
 */

class lexicon {
   public $patterns = array(); // of objects
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


  public function appendPattern($key, $pattern) {
    $this->patterns[$key][] = $pattern;
  }

  public function listKeys() {
    return array_keys($this->patterns);
  }

  public function removePatterns($key) {
    if (array_key_exists($key,$this->patterns))
      unset($this->patterns[$key]);
  }

    /**
     * \brief Returns an array of unique values for the patterns in the given key
     */
    function patternValues($key)
    {
        $all = [];
        foreach ( $this->patterns[$key] as $pat )
        {
            if ( !is_array($pat) )
                $all[] = $pat;
            else
                $all[] = $pat[1];
        }
        sort($all);
        return array_unique($all);
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


