<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see https://www.gnu.org/licenses/. */

class metadataMap {


  
  static $groups = array();
  static $charges = array();

  public function __construct()
  {

    self::$groups = array(
      'alchemical' => array('fimbriate' => true, 'fixStroke' => true),
      'animal' => array( 'demi' => 'horizontal', 'hflex' => 1.5, 'wflex' => 1.5 ),
      'architecture' => array( 'demi' => 'horizontal', 'hflex' => 1.5, 'wflex' => 1.5 ),
      'astronomical' => array('fimbriate' => true, 'fixStroke' => true),
      'axe' => array ('orient_to_ordinary' => -1, 'hflex' => 1.4, 'wflex' => 1.22 ),
      'crown' => array("situation" => "fesswise", 'hflex' => 1.5, 'wflex' => 1.5 ),
      'fish' => array("situation" => "fesswise", 'orient_to_ordinary' => 1),
      'flute' => array ('orient_to_ordinary' => -1),
      'game' => array( 'demi' => 'horizontal' ),
      'geometric' => array('fimbriate' => true, 'hflex' => 2, 'wflex' => 2, 'fixStroke' => true),
      'knot' => array ('orient_to_ordinary' => 1, 'hflex' => 1.4, 'wflex' => 1.4 ),
      'lion' => array( 'demi' => 'horizontal', 'hflex' => 2, 'wflex' => 2 ),
        'dragon' => array( 'demi' => 'horizontal', 'hflex' => 0, 'wflex' => 0 ),
      'livestock' => array( 'demi' => 'horizontal' ),
      'mullet' => array('fimbriate' => true),
      'mythical' => array( 'demi' => 'horizontal', 'hflex' => 2, 'wflex' => 2 ),
      'plant' => array ('orient_to_ordinary' => 1, 'hflex' => 1.3, 'wflex' => 1.3 ),
      'projectile' => array ('orient_to_ordinary' => -1),
      'quadrate' => array( 'fimbriate' => true, 'fixStroke' => true),
      'sword' => array ('orient_to_ordinary' => -1, 'hflex' => 1.1, 'wflex' => 1.2 ),
      'wildlife' => array( 'demi' => 'horizontal', 'hflex' => 1.2, 'wflex' => 1.2 ),
      'zodiac' => array('fimbriate' => true, 'fixStroke' => true),
    );
    self::$charges = array(
            'bbox' => array ('orient_to_ordinary' => -1, 'hflex' => 999, 'wflex' => 999 ),
            'book' => array ( 'hflex' => 1.4, 'wflex' => 1.2 ),
            'penguin' => array ('demi' => 'horizontal'),
            'bezant' => array ('superposition' => '50,50,85', 'hflex' => 1, 'wflex' => 1),
        'crown-of-thorns' => array ('superposition' => '50,50,45', 'hflex' => 1, 'wflex' => 1),
        'billet' => array ('orient_to_ordinary' => 1, 'demi' => 'horizontal', 'hflex' => 2, 'wflex' => 1.5),
            'cartouche' => array ('orient_to_ordinary' => 1, 'hflex' => 2, 'wflex' => 1.5),
            'chain' => array("situation" => "fesswise", 'orient_to_ordinary' => 1),
            'crescent' => array ('superposition' => '50,65,65', 'hflex' => 1.6, 'wflex' => 1.6),
            'cross/long' => array('orient_to_ordinary' => -1),
            'feather' => array("situation" => "fesswise", 'orient_to_ordinary' => 1),
            'flail' => array ('orient_to_ordinary' => -1),
            'fusil' => array ( 'hflex' => 1.5, 'wflex' => 1 ),
        'emblem/cross-of-navarre' => array ( 'hflex' => 1.3, 'wflex' => 1.3 ),
        'gonfannon' => array ('superposition' => '50,35,50'),
            'hurt' => array ('superposition' => '50,50,85', 'hflex' => 1, 'wflex' => 1),
            'lozenge' => array ( 'hflex' => 1.6, 'wflex' => 1.6 ),
            'skull' => array ( 'fimbriate' => true ),
            'orange' => array ('superposition' => '50,50,85', 'hflex' => 1, 'wflex' => 1),
            'pellet' => array ('superposition' => '50,50,85', 'hflex' => 1, 'wflex' => 1),
            'pillar' => array ('orient_to_ordinary' => -1),
            'plate' => array ('superposition' => '50,50,85', 'hflex' => 1, 'wflex' => 1),
            'pomme' => array ('superposition' => '50,50,85', 'hflex' => 1, 'wflex' => 1),
            'rainbow' => array ( 'hflex' => 999, 'wflex' => 999 ),
            'rocket' => array ('orient_to_ordinary' => -1),
            'rocket-man' => array ("situation" => "fesswise", 'orient_to_ordinary' => 1),
            'roundel' => array ('superposition' => '50,50,85', 'hflex' => 1, 'wflex' => 1),
            'sports/football-helmet' => array ('superposition' => '65,35,65'),
            'natural/grassy-mound' => array ('superposition' => '50,45,95'),
            'torteau' => array ('superposition' => '50,50,85', 'hflex' => 1, 'wflex' => 1),
            'triangle' => array ('superposition' => '50,65,65'),
            'trumpet' => array("situation" => "fesswise", 'orient_to_ordinary' => 1),
            'buckle' => array("situation" => "fesswise", 'orient_to_ordinary' => 1),
        'bird/wing' => array('superposition' => '50,70,70' ),
        'livestock/egg' => array('fimbriate' => true, 'hflex' => 0, 'wflex' => 0),
        'animal/paw-print' => array('fimbriate' => true, 'hflex' => 0, 'wflex' => 0),
        'games/meeple' => array('fimbriate' => true, 'hflex' => 0, 'wflex' => 0),
        'natural/teardrop' => array('fimbriate' => true, 'hflex' => 0, 'wflex' => 0),
        'square' => array('hflex' => 0, 'wflex' => 0),
        'ham' => array("situation" => "fesswise", 'orient_to_ordinary' => 1),
        'plant/seeblatt' => array('hflex' => 0, 'wflex' => 0),
        'snake' => array("situation" => "fesswise"),
    );
  }
  
  /*
   * We build up the extras array from the most general to the most specific, allowing values to
   * be overwritten by more specific elements
   */
  
  public function getMeta ( $chargeGroup, $chargeName )
  {
        $metas = [];

        if (array_key_exists($chargeGroup, self::$groups))
            $metas = self::$groups[$chargeGroup]; // Don't need to merge as this is the first

        if (array_key_exists($chargeName, self::$charges))
            $metas = array_merge($metas, self::$charges[$chargeName]);

        $search = $chargeGroup . '/' . $chargeName;
        if (array_key_exists($search, self::$charges))
            $metas = array_merge($metas, self::$charges[$search]);

        return $metas;
  }
}
