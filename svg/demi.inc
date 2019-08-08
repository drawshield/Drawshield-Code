<?php 

class demiType {
  static $groups;
  static $charges;
  static $exceptions;

  public function __construct()
  {
    // Array is height then width
    self::$groups = array(
      'animal', 'game', 'livestock', 'wildlife', 'lion',
    );
    self::$charges = array(
      'billet'
    );
    self::$exceptions = array(

    );
  }
  
  public function getDemi ( $chargeGroup, $chargeName)
  {
    // Look for the most specific things first
    $search = $chargeGroup . '/' . $chargeName;
    if (in_array($search, self::$charges))
      return 'horizontal';
    elseif (in_array($chargeName, self::$exceptions))
      return 'vertical';
    elseif (in_array($chargeName, self::$charges))
      return  'horizontal';
    elseif (in_array($chargeGroup, self::$groups))
      return  'horizontal';

    return 'vertical'; // We didn't find a flex amount, but this is not an error
  }
}