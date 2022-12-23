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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */

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
  static $pink;
  static $gold;
  static $silver;
  static $caucasian;
  static $orange;
  static $rose;
  static $groups = array();
  static $charges = array();
  static $features = array();

  public function __construct()
  {
      global $options;

      $hatching = $options['palette'] == 'hatching';

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
    self::$pink = rgb('carnation');
    self::$orange = rgb('orange');
    self::$rose = rgb('rose');
    // etc...
    self::$groups = array(
      'bear' => self::$brown,
      'human' => self::$caucasian,
      'crown' => self::$yellow,
      'plant' => self::$green,
      'flower' => self::$green,
      'tree-fruit' => self::$green,
      'sword' => self::$white,
      'characters' => self::$black,
      'playing-card' => false, // use whatever you find
      'badges' => self::$red, // dummy entry
    );
    self::$charges = array(
        'group' => self::$gray,  // dummy value, group is not visible
        'rainbow' => self::$gray,  // dummy value, rainbow has rainbow colours!
        'peacock' => $hatching ? self::$blue : '#0020c2',
        'chaplet' => self::$green,
        'grassy-mound' => self::$green,
        'butterfly' => self::$blue,
      'flames' => self::$yellow,
      'polar-bear' => self::$white, // overrides bear group
      'camelopard' => self::$orange,
      'squirrel' => self::$brown,
      'beaver' => self::$brown,
      'auk' => self::$white,
      'parachute' => self::$white,
      'talbot' => self::$white,
      'skull' => self::$white,
      'molar' => self::$white,
      'bone' => self::$white,
      'snowflake' => self::$white,
      'crystal' => self::$white,
      'badger' => self::$grey,
      'sun' => self::$yellow,
      'sun-rays' => self::$yellow,
      'moon' => self::$white,
      'moor' => self::$black,
      'bat' => self::$black,
      'acorn' => self::$brown,
      'boar' => self::$brown,
      'chough' => self::$black,
      'crow' => self::$black,
      'pig' => self::$pink,
        'tophat' => self::$black,
        'cap-of-maintenance' => self::$red,
      'raven' => self::$black,
      'daisy' => self::$white,
      'deer' => self::$brown,
      'condor' => self::$gray,
      'ship' => self::$brown,
      'stag' => self::$brown,
      'falcon' => self::$brown,
      'rabbit' => self::$brown,
      'log' => self::$brown,
      'fox' => self::$red,
        'welsh-dragon' => self::$red,
      'barber-pole' => self::$red,
      'hammer' => self::$black,
      'hare' => self::$brown,
      'harp' => self::$brown,
      'moose' => self::$brown,
      'laurel-wreath' => self::$green,
      'dolphin' => self::$green,
      'dolphin-natural' => self::$gray,
      'fallen-knight-embowed' => self::$gray,
      'rocket' => self::$gray,
      'aubergine' => self::$purple,
      'dove' => self::$white,
      'clove' => self::$darkBrown,
      'elephant' => self::$gray,
      'antelope' => self::$brown,
      'leaf' => self::$green,
      'lavender' => self::$purple,
      'popinjay' => self::$green,
      'lily-pad' => self::$green,
      'cactus' => self::$green,
      'pumpkin' => self::$orange,
      'rose' => self::$red,
      'planet-earth' => self::$green,
      'tree' => self::$green,
      'pomegranate' => self::$green,
      'soccerball' => self::$white,
      'stork' => self::$white,
      'tennis-ball' => self::$yellow,
      'american-football' => self::$brown,
      'basketball' => self::$orange,
      'baseball-glove' => self::$brown,
        'cock' => self::$brown,
      'baseball' => self::$white,
      'pelican' => self::$white,
      'mitre' => self::$yellow,
      'boss' => self::$gray,
      'rose-tudor' => self::$red,
      'rose-york' => self::$white,
      'rose-lancaster' => self::$red,
      'rose-natural' => self::$rose,
      'zebra' => self::$white,
      'banana-stem' => self::$yellow,
      'rubber-duck' => self::$yellow,
      'magpie' => self::$white,
      'starling' => self::$black,
      'fleece' => self::$yellow,
      'chrysanthemum' => self::$yellow,
    );
    self::$features = array(
        'condor/necked' => self::$caucasian,
        'condor/legged' => self::$caucasian,
        'condor/beaked' => self::$yellow,
        'jelloped' => self::$red,
        'cock/legged' => self::$yellow,
        'cock/beaked' => self::$yellow,
        'condor/armed' => self::$gray,
        'chaplet/slipped' => self::$red,
        'chaplet/leaved' => self::$green,
      'conker/fructed' => self::$brown,
      'with-stone' => self::$gray,
      'slipped' => self::$green,
      'leaved' => self::$green,
      'barbed' => self::$green,
      'seeded' => self::$yellow,
      'tusked' => self::$white,
      'dove/legged' => self::$white,
      'turned-up' => self::$white,
      'inflamed' => self::$yellow,
      'dove/beaked' => self::$yellow,
      'chough/legged' => self::$red,
      'chough/beaked' => self::$red,
      'rubber-duck/beaked' => self::$red,
      'starling/legged' => self::$red,
      'starling/beaked' => self::$red,
      'fructed' => self::$red,
      'dolphin/finned' => self::$red,
      'hands' => self::$caucasian,
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
      'zebra/striped' => self::$black,
      'zebra/unguled' => self::$black,
        'unguled' => self::$grey,
      'vulning' => self::$red,
      'nest' => self::$brown,
      'rose-lancaster/seeded' => self::$white,
      'rose-tudor/inner' => self::$white,
      'banana-stem/stemmed' => self::$green,
      'banana-stem/flowers' => self::$red,
      'cactus/flowers' => self::$red,
      'magpie/winged' => self::$black,
        'vine/stick' => self::$black,
      'magpie/legged' => self::$black,
      'clan-ross/seeded' => self::$red,
    );
  }
  
  public function getProper ( $chargeGroup, $chargeName, $feature = null, $errorIfNotFound = true )
  {
    global $messages;

    if ( $chargeGroup == "custom" )
    {
        $color = SmartChargeGroup::instance()->charge($chargeName)->proper($feature);
        if ( $color === null )
        {
            if ( $errorIfNotFound )
            {
                $messages->addMessage('blazon', "No proper colour for $chargeName $feature");
                return self::$gray;
            }
            return false;
        }
        return $color;
    }

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
