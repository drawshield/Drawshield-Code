<?php 

class properColour {
  
  static $brown;
  static $red;
  static $green;
  static $white;
  static $black;
  static $darkBrown;
  static $yellow;
  static $gray;
  static $grey;
  static $purple;
  static $blue;
  static $gold;
  static $silver;
  static $caucasian;
  static $orange;
  static $groups = array();
  static $charges = array();
  static $features = array();

  public function __construct()
  {

    self::$brown = rgb('brunatre');
    self::$red = rgb('gules');
    self::$green = rgb('vert');
    self::$white = rgb('argent');
    self::$silver = rgb('argent');
    self::$black = rgb('sable');
    self::$darkBrown = rgb('murrey');
    self::$yellow = rgb('or');
    self::$gold = rgb('or');
    self::$grey = rgb('cendree');
    self::$gray = rgb('cendree');
    self::$purple = rgb('purpure');
    self::$blue = rgb('azure');
    self::$caucasian = rgb('carnation');
    self::$orange = rgb('orange');
    // etc...
    self::$groups = array(
      'human' => self::$caucasian,
      'crown' => self::$yellow,
      'plant' => self::$green,
      'flower' => self::$green,
      'tree' => self::$green,
      'sword' => self::$white,
      'characters' => self::$black,
    );
    self::$charges = array(
      'skull' => self::$white,
      'badger' => self::$grey,
      'sun' => self::$yellow,
      'moon' => self::$white,
      'moor' => self::$black,
      'bat' => self::$black,
      'acorn' => self::$brown,
      'boar' => self::$brown,
      'chough' => self::$black,
      'crow' => self::$black,
      'raven' => self::$black,
      'daisy' => self::$white,
      'deer' => self::$brown,
      'ship' => self::$brown,
      'stag' => self::$brown,
      'falcon' => self::$brown,
      'rabbit' => self::$brown,
      'fox' => self::$red,
      'hammer' => self::$black,
      'hare' => self::$brown,
      'harp' => self::$brown,
      'moose' => self::$brown,
      'laurel-wreath' => self::$green,
      'dolphin' => self::$green,
      'dolphin-natural' => self::$gray,
      'rocket' => self::$gray,
      'dove' => self::$white,
      'clove' => self::$darkBrown,
      'elephant' => self::$gray,
      'antelope' => self::$brown,
      'leaf' => self::$green,
      'lavender' => self::$purple,
      'popinjay' => self::$green,
      'rose' => self::$red,
      'planet-earth' => self::$green,
      'tree' => self::$green,
      'pomegranate' => self::$green,
      'soccerball' => self::$white,
      'tennis-ball' => self::$yellow,
      'american-football' => self::$brown,
      'basketball' => self::$orange,
      'baseball-glove' => self::$brown,
      'baseball' => self::$white,
      'pelican' => self::$white,
      'mitre' => self::$yellow,
      'reddit-alien' => self::$white,
    );
    self::$features = array(
      'with-stone' => self::$gray,
      'slipped' => self::$green,
      'leaved' => self::$green,
      'barbed' => self::$green,
      'seeded' => self::$yellow,
      'tusked' => self::$white,
      'inflamed' => self::$yellow,
      'chough/legged' => self::$red,
      'chough/beaked' => self::$red,
      'dove/legged' => self::$red,
      'dove/beaked' => self::$red,
      'fructed' => self::$red,
      'dolphin/finned' => self::$red,
      'hammer/head' => self::$black,
      'hammer/handle' => self::$brown,
      'pickaxe/head' => self::$black,
      'shafted' => self::$brown,
      'trunked' => self::$brown,
      // 'stems' => self::$brown, // or is this    stemmed?
      'pomegranate/seeded' => self::$red,
      'birch/trunked' => self::$white,
      'hilt' => self::$gold,
      'pomell' => self::$gold,
      'blade' => self::$silver,
      'cornflower/slipped' => self::$blue,
      'poppy/slipped' => self::$red,
      'tulip/slipped' => self::$red,
      'daisy/slipped' => self::$white,
      'lily/slipped' => self::$white,
      'daisy/centre' => self::$yellow,
      'lily/stamen' => self::$yellow,
      'planet-earth/land' => self::$green,
      'planet-earth/water' => self::$blue,
      'rocket/inflamed' => self::$red,
      'reddit-alien/eyes' => self::$red,
      'vulning' => self::$red,
      'nest' => self::$brown,
    );
  }
  
  public function getProper ( $chargeGroup, $chargeName, $feature = null, $errorIfNotFound = true )
  {
    global $messages;

    // Look for the most specific things first
    if ($feature) {
      $search = $chargeName . '/' . $feature;
      if ( array_key_exists($search, self::$features))
        return self::$features[$search];
      elseif ( array_key_exists($feature, self::$features))
        return self::$features[$feature];
      // we must use feature, not just overall proper, but this is never an error
      else
        return false;
    }
    $search = $chargeGroup . '/' . $chargeName;
    if (array_key_exists($search, self::$charges))
      return self::$charges[$search];
    elseif (array_key_exists($chargeName, self::$charges))
      return self::$charges[$chargeName];
    elseif (array_key_exists($chargeGroup, self::$groups))
      return self::$groups[$chargeGroup];
    else {
      if ( $errorIfNotFound ) {
        $messages->addMessage('blazon', "No proper colour for $chargeName $feature");
        return self::$gray;
      }
    }
    return false; // We didn't find a colour, but this is not an error
  }
}