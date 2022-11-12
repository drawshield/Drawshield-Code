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


function unique($prefix = 'item') {
  static $uni_num = 0;

  $uni_num += 1;
  return "$prefix$uni_num";
}

  
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

  public function flush() {
    $this->storage = array();
    $this->storageKey = 0;
  }
   
}
  
class blazonML extends DOMDocument {
  protected $uid;
  
  // We introduce a level of indirection here, in case of future changes to the schema
  // and for consistency
  const E_BLAZON = 'blazon';
  const E_INPUT = 'input';
  const E_TOKEN = 'token';
  const E_SHIELD = 'shield';
  const E_ROLL = 'roll';
  const A_COLS = 'columns';
  const E_PLAIN = 'simple';
  const E_COMPLEX = 'complex';
  const E_INSTRUCTIONS = 'instructions';
  const E_SHAPE = 'shape';
  const E_CHIEF = 'chief';
  const E_PALETTE = 'palette';
  const E_COLOURSET = 'colourset';
  const E_EFFECT = 'effect';
  const E_DEBUG = 'debug';
  const E_ACHIEVEMENT = 'achievement';
  const E_CREST = 'crest';
  const E_TORSE = 'torse';
  const E_LIST = 'list';
  const E_MANTLING = 'mantling';
  const E_HELMET = 'helmet';
  const E_SUPPORTERS = 'supporters';
    const E_COMPARTMENT = 'compartment';
  const E_MOTTO = 'motto';
    const E_ORNAMENT = 'ornament';
  const A_TEXT = 'text';
  const E_LOCATION = 'location';
  const E_ASPECT = 'aspect';
  const A_INDEX = 'index';
  const A_CAPTION = 'caption';
  const A_TITLE = 'title';
  const A_TOKENS = 'tokens';
  const A_LINENUMBER = 'linenumber';
  const A_LINERANGE = 'linerange';
  const A_ID = 'ID';
  const A_IDREF = 'IDREF';
  const A_CONTEXT = 'context';
  const V_DIMIDIATED = 'dimidiated';
  const V_IMPALED = 'impaled';
  const V_QUARTERED = 'quartered';
  const E_MISSING = 'missing';
  const E_MESSAGE = 'message';
  const E_MESSAGES = 'messages';
  const E_OVERALL = 'overall';
  const E_TINCTURE = 'tincture';
  const A_ORIGIN = 'origin';
  const A_CATEGORY = 'category';
  const E_PROPER = 'proper';
    const E_FRINGE = 'fringe';
  const E_PENDING = 'pending';
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
  const V_IMPLIED = 'implied';
  
  public function __construct($version = '1.0', $encoding = null) {
    $this->uid = new unique();
    parent::__construct($version, $encoding);
  }
    public function makeIDNodeFromLexeme ( string $element, Lexeme $lexeme, array $params = null ) {
      $node = $this->makeNodeFromLexeme( $element, $lexeme, $params);
        $node->setAttribute(self::A_ID, $this->uid->getRef("N"));
        $node->setIdAttribute(self::A_ID, true);
        return $node;
    }

    public function makeNodeFromLexeme ( string $element, Lexeme $lexeme, array $params = null )
    {
        $node = parent::createElement($element);
        $node->setAttribute(self::A_KEYTERM, $lexeme->keyterm);
        $node->setAttribute(self::A_TOKENS, $lexeme->tokens);
        $node->setAttribute(self::A_LINENUMBER, $lexeme->startPos . '-' . $lexeme->endPos);
        if (!is_null($lexeme->value)) $node->setAttribute(self::A_PARAM, $lexeme->value);
        if (is_array($params)) {
            foreach ($params as $attribute => $value)
                if ($value)
                    $node->setAttribute($attribute, $value);
        }
        return $node;
    }

    public function makeNode ( $element, $params = null, $tokens = null, $lineNo = null ) {
        $node = parent::createElement($element);
        if ( is_array($params) ) {
            foreach ( $params as $attribute => $value )
                if ( $value )
                    $node->setAttribute($attribute,$value);
        }
        if ( $tokens ) $node->setAttribute(self::A_TOKENS, $tokens);
        if ( $lineNo ) $node->setAttribute(self::A_LINENUMBER, $lineNo);
        return $node;
    }

        public function makeIDNode ( $element, $params = null, $tokens = null, $lineNo = null ) {
      $node = self::makeNode($element, $params, $tokens, $lineNo);
    $node->setAttribute( self::A_ID, $this->uid->getRef("N"));
    $node->setIdAttribute(self::A_ID,true); 
    return $node;
  }

  public function makeTextNode($text) {
      return parent::createTextNode($text);
  }
  
  public function makeMod ( $type, $value = null, $keyterm = null, $tokens = null, $lineNo = null ) {
    $mod = self::makeNode(self::E_MODIFIER, array ( self::A_TYPE => $type, self::A_PARAM => $value, self::A_KEYTERM => $keyterm ), $tokens, $lineNo );
    return $mod;   
  }

  public function makeModFromLexeme($lexeme, $type, $value = null) {
      $overrides = array(self::A_TYPE => $type);
      if (!is_null($value)) {
          $overrides[self::A_PARAM] = $value;
      }
      return $this->makeNodeFromLexeme(self::E_MODIFIER, $lexeme, $overrides);
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

function transliterate($string) {
  return preg_replace(array('/è/','/é/','/ê/','/ç/','/à/','/á/','/â/','/È/','/É/','/Ê/','/Ç/','/À/','/Á/','/Â/'),
        				       array('e','e','e','c','a','a','a','e','e','e','c','a','a','a'), $string);
}

function calculateAR($ar) {
  if (strpos($ar, ':') > 0) {
    $arParts = explode(':', $ar);
    $numerator = intval($arParts[0]);
    $denominator = 0;
    if (count($arParts) > 1) {
      $denominator = intval($arParts[1]);
    }
    if ($denominator == 0) $denominator = 1;
    $ar = $numerator / $denominator;
  } else {
    $ar = floatval($ar);
  }
  if ($ar > 4) {
    $ar = 4;
  } elseif ($ar < 0.25) {
    $ar = 0.25;
  }
  return $ar;
}

function trace($filename, $linenumber) {
  $filename = basename($filename);
  return " class=\"$filename:$linenumber\" ";
}
