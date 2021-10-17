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
    along with Foobar.  If not, see https://www.gnu.org/licenses/.
 */


if ( existModifierWithKeyterm($node,'couped') )
 $ordinary['shape_spec'] =  'X0Y100a300C1000e300G1000';
elseif ( existModifierWithKeyterm($node,'couped-chief') )
  $ordinary['shape_spec'] =  'X0Y100a300C1108e300G1108';
elseif ( existModifierWithKeyterm($node,'couped-base') )
  $ordinary['shape_spec'] =  'X0Y-8a300C1108e300G1108';
elseif ( existModifierWithKeyterm($node,'lozengy'))
  $ordinary['body'] = '<path d="M0,150l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150  M0,450l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150   M0,750l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150  M0,1050l75,-150 75,150 75,-150 75,150 -75,150 -75,-150 -75,150 -75,-150" />';
 elseif ( existModifierWithKeyterm($node, 'voided'))
   $ordinary['shape_spec'] =  'X333.33Y-38C1276e341g1276a341';
 else
   $ordinary['shape_spec'] =  'X333.33Y-8C1216e341g1216a341';

if ( existModifierWithKeyterm($node,'compony1') )
  $ordinary['body2'] = '<path d="M0,0h433v300h-433z M0,600h433v300h-433z M0,600h433v300h-433z " />';
elseif (existModifierWithKeyterm($node,'compony2') )
  $ordinary['body2'] = '<path d="M0,0h167v150h-167z M167,150h267v150h-267z M0,300h167v150h-167z M167,450h267v150h-267z M0,600h167v150h-167z M167,750h267v150h-267z M0,900h167v150h-167z M167,1050h267v150h-267z " />';

$ordinary['chargesOn'] = (string)ShieldRect::from_corners(75, 75, 333.33 - 75, $layout->cross_point->y * 2 - 75);
