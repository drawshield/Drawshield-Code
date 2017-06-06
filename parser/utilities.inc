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
  
interface checkpoint {
  public function save();
  public function restore($state);
  public function discard($state);
}

class persistentStore implements checkpoint {
  protected $storage = array();
  private $storageKey = 0;
  
  public function save() {
    // Not used here - use store($data) instead
  }
  
  public function store($data) {
    $myKey = $this->storageKey++;
    $this->storage[$myKey] = $data;
    return $myKey;
  }
  
  public function restore($state) {
    if ( isset($this->storage[$state]) ) {
      $data = $this->storage[$state];
      unset($this->storage[$state]);
    } else {
      $data = null;
    }
    return $data;
  }
  
  public function discard($state) {
    unset ($this->storage[$state]);
  }
   
}
  
class blazonML extends DOMDocument {
  protected $uid;
  
  // We introduce a level of indirection here, in case of future changes to the schema
  const E_BLAZON = 'blazon';
  const E_SHIELD = 'shield';
  const E_PLAIN = 'simple';
  const E_COMPLEX = 'complex';
  const A_INDEX = 'index';
  const A_TOKENS = 'tokens';
  const A_ID = 'ID';
  const A_IDREF = 'IDREF';
  const V_DIMIDIATED = 'dimidiated';
  const V_IMPALED = 'impaled';
  const V_QUARTERED = 'quartered';
  const E_MISSING = 'missing';
  const E_OVERALL = 'overall';
  const E_TINCTURE = 'tincture';
  const A_ORIGIN = 'origin';
  const E_PROPER = 'proper';
  const A_NAME = 'name';
  const E_FUR = 'fur';
  const E_TREATMENT = 'treatment';
  const E_SEMYDE = 'semyde';
  const E_FIELD = 'field';
  const E_COUNTERCHANGED = 'counterchanged';
  const E_COLOUR = 'colour';
  const E_DIVISION = 'division';
  const E_OBJECTS = 'objects';
  const E_ORDINARY = 'ordinary';
  const E_CHARGE = 'charge';
  const A_TYPE = 'type';
  const A_NUMBER = 'number';
  const E_MODIFIER = 'modifier';
  const A_PARAM = 'value';
  const E_FEATURE = 'feature';
  const A_LINETYPE = 'linetype';
  const V_FEATURE = 'feature';
  const A_KEYTERM = 'keyterm';
  
  public function __construct($version = '1.0', $encoding = null) {
    $this->uid = new unique();
    parent::__construct($version, $encoding);
  }
  
  public function makeNode ( $element, $params = null, $tokens = null ) {
    $node = parent::createElement($element);
    $node->setAttribute( self::A_ID, $this->uid->getRef("N"));
    $node->setIdAttribute(self::A_ID,true); 
    if ( is_array($params) ) {
      foreach ( $params as $attribute => $value )
        if ( $value )
          $node->setAttribute($attribute,$value);
    }
    if ( $tokens ) $node->setAttribute(self::A_TOKENS, $tokens);
    return $node;       
  }
  
  public function makeMod ( $type, $value = null, $keyterm = null, $tokens = null ) {
    $mod = self::makeNode(self::E_MODIFIER, array ( self::A_TYPE => $type, self::A_PARAM => $value, self::A_KEYTERM => $keyterm ), $tokens );
    return $mod;   
  }
}

class unique {
  protected $counter = 0;
  protected $myInstance;
  protected static $instance = 0;

  public function __construct(){
    $this->myInstance = ++self::$instance;
  }
  
  public function getRef($prefix = '') {
    return $prefix . $this->myInstance . '-' . $this->counter++;
  }
}
   
?>
