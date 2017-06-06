<?php
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 11/09/14
 * Time: 18:30
 */

$colours = array (
  'bezant' => 'or',
  'golpe' => 'purpure',
  'guze' => 'sanguine',
  'hurt' => 'azure',
  'orange' => 'tenne',
  'pellet' => 'sable',
  'plate' => 'argent',
  'pomme' => 'vert',
  'torteau' => 'gules',
);

foreach ( $node->childNodes as $child ) {
  if ( $child->nodeName == 'tincture') {
    $child->setAttribute('origin', 'implied');
  }
}

if ( $chargeKey == 'bezant' ) {
  $charge['svgCode'] = '<svg height="450" width="424">
    <path d="m367.9,168   c-16.96,107.6 -112.4,208.1 -213.1,224.4 -100.7,16.32 -168.6,-57.67 -151.7,-165.3 16.96,-107.6 112.4,-208.1 213.1,-224.4 100.7,-16.32 168.6,57.67 151.7,165.3   z " style="fill:' . rgb('or') . ';stroke-width:3px;"/>
    <path d="M342.2,44.95 413.2,115.9   C470.1,304.8 270.8,500.1 112.9,437.2   L37.95,362.2   C257.8,486.1 433.7,165.4 342.2,44.95   z " style="fill:' . rgb('or') . ';stroke-width:3px;" />
    <path d="M342.2,44.95 413.2,115.9   C470.1,304.8 270.8,500.1 112.9,437.2   L37.95,362.2   C257.8,486.1 433.7,165.4 342.2,44.95   z " style="opacity:0.5;fill:#999999;"/>
  </svg>';
} else
  $charge['svgCode'] = '<svg height="500" width="500"><circle style="fill:' . rgb($colours[$chargeKey]) . ';" cx="250" cy="250" r="250" /></svg>';
