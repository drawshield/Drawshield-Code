<?php 

class flexAmount {
  static $groups;
  static $charges;

  public function __construct()
  {
    // Array is height then width
    self::$groups = array(
      'lion' => array(2,2),
      'animal' => array(1.5,1.5),
      'axe' => array(1.4,1.2),
      'bird' => array(1.2,1.2),
      'game' => array(1.2,1.2),
      'wildlife' => array(1.2,1.2),
      'plant' => array(1.3,1.3),
      'sword' => array(1.1,1.2),
      'knot' => array(1.4,1.4),
      'crown' => array(2,1.5),
    );
    self::$charges = array(
      'book' => array(1.4,1.2),
      'billet' => array(2,1.5),
      'cartouche' => array(2,1.5),
      'crescent' => array(1.6,1.6),
      'lozenge' => array(1.6,1.6),
      'rainbow' => array(999,999),
      'bbox' => array(999,999),
    );
  }
  
  public function getFlex ( $chargeGroup, $chargeName)
  {
    // Look for the most specific things first
    $search = $chargeGroup . '/' . $chargeName;
    if (array_key_exists($search, self::$charges))
      return self::$charges[$search];
    elseif (array_key_exists($chargeName, self::$charges))
      return self::$charges[$chargeName];
    elseif (array_key_exists($chargeGroup, self::$groups))
      return self::$groups[$chargeGroup];

    return array(0,0); // We didn't find a flex amount, but this is not an error
  }
}