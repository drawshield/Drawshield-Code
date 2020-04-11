<?php 

class extraInfoMap {


  
  static $groups = array();
  static $charges = array();

  public function __construct()
  {

    self::$groups = array(
      'animal' => array( 'demi' => 'horizontal' ),
      'game' => array( 'demi' => 'horizontal' ),
      'livestock' => array( 'demi' => 'horizontal' ),
      'wildlife' => array( 'demi' => 'horizontal' ),
      'lion' => array( 'demi' => 'horizontal' ),
      'sword' => array ('orient_to_ordinary' => -1),
      'axe' => array ('orient_to_ordinary' => -1),
      'flute' => array ('orient_to_ordinary' => -1),
      'knot' => array ('orient_to_ordinary' => 1),
      'plant' => array ('orient_to_ordinary' => 1),
      'projectile' => array ('orient_to_ordinary' => -1),
      'quadrate' => array( 'fimbriate' => true),
      'geometric' => array('fimbriate' => true),
      'mullet' => array('fimbriate' => true),
      'zodiac' => array('fimbriate' => true),
      'alchemical' => array('fimbriate' => true),
      'astronimical' => array('fimbriate' => true),
      'crown' => array('horizontal' => true),
      'fish' => array('horizontal' => true, 'orient_to_ordinary' => 1),
    );
    self::$charges = array(
            'flail' => array ('orient_to_ordinary' => -1),
            'pillar' => array ('orient_to_ordinary' => -1),
            'rocket' => array ('orient_to_ordinary' => -1),
            'bbox' => array ('orient_to_ordinary' => 1),
            'cartouche' => array ('orient_to_ordinary' => 1),
            'billet' => array ('orient_to_ordinary' => 1, 'demi' => 'horizontal'),
            'rocket-man' => array ('horizontal' => true, 'orient_to_ordinary' => 1),
            'trumpet' => array('horizontal' => true, 'orient_to_ordinary' => 1),
            'feather' => array('horizontal' => true, 'orient_to_ordinary' => 1),
            'chain' => array('horizontal' => true, 'orient_to_ordinary' => 1),
            'cross/long' => array('orient_to_ordinary' => -1),
            'sports/football-helmet' => array ('superposition' => '65,35,65'),
            'gonfannon' => array ('superposition' => '50,35,50'),
            'triangle' => array ('superposition' => '50,65,65'),
            'crescent' => array ('superposition' => '50,65,65'),
            'roundel' => array ('superposition' => '50,50,85'),
            'hurt' => array ('superposition' => '50,50,85'),
            'pellet' => array ('superposition' => '50,50,85'),
            'bezant' => array ('superposition' => '50,50,85'),
            'orange' => array ('superposition' => '50,50,85'),
            'plate' => array ('superposition' => '50,50,85'),
            'pomme' => array ('superposition' => '50,50,85'),
            'torteau' => array ('superposition' => '50,50,85'),
    );
  }
  
  public function getExtra ( $chargeGroup, $chargeName )
  {
    global $messages;

    // Look for the most specific things first
    $search = $chargeGroup . '/' . $chargeName;
    if (array_key_exists($search, self::$charges))
      return self::$charges[$search];
    elseif (array_key_exists($chargeName, self::$charges))
      return self::$charges[$chargeName];
    elseif (array_key_exists($chargeGroup, self::$groups))
      return self::$groups[$chargeGroup];
    return [];
  }
}