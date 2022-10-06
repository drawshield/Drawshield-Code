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
   
    $this->patterns[languageDB::CHARGE_PREFIX] = array (
      // possibly duplicated in CHARGE_MODS, but sometimes occur first
      'covered', 'standing', 'open', 'heraldic', 'dexter', 'sinister',
      'double', 'branched', 'staved', 'single', 'modern', 
      'winged', 'natural', 'nude', 'fouled',
      array ( 'spread', 'displayed' ),
      array ( 'heads? of ?(a|an|the)', 'head' ),
      array ( 'claws? of ?(a|an|the)', 'claw' ),
      array ( 'legs? of ?(a|an|the)', 'leg' ),
      array ( 'closed?', 'closed' ),
      array ( '(dem[yi]|half)', 'demi' ),
      array ( 'like', 'like' ), // not sure whether this has any real meaning, but added as mod just in case
      array ( '(tuft|stalk|limb|bough|scrog|branche?)s? of ?(a|an|the)', 'portion' ),
      array ( 'sprigs? of ?(a|an|the)', 'sprig'),
      array ( 'double headed', 'double-headed' ),
      array ( 'oval shaped?', 'shape-oval'),
      array ( 'lozenge shaped?', 'shape-lozenge'),
      array ( 'square shaped?', 'shape-square'),
      array ( 'heater ?shaped?', 'shape-heater'),
      array ( 'equal armed', 'equal-armed'),
      array ( 'sinister (half|halves|parts?) ?of ?(a|an|the)', 'sinister-half'),
      array ( 'dexter (half|halves|parts?) ?of ?(a|an|the)', 'dexter-half'),
      array ( '(upper|top) (half|halves|parts?) ?of ?(a|an|the)', 'upper-half'),
      array ( '(lower|bottom) (half|halves|parts?) ?of ?(a|an|the)', 'lower-half'),
      array ( 'leather(s|ed)?', 'leathered'),
      );


$this->patterns[languageDB::TRADITION_PREFIX] = array (
    array ( 'swiss', 'swiss'),
    array ( 'french', 'french'),
    array ( 'english', 'english'),
    array ( 'italian', 'italian'),
    array ( 'polish', 'polish'),
    array ( 'spanish', 'spanish'),
    array ( 'german', 'german'),
    array ( 'african', 'african'),
    array ( 'persian', 'persian'),
    array ( 'hungarian', 'hungarian'),
);

$this->patterns[languageDB::TRADITION_DESCRIPTION] = array (
    'shaped?', 'styled?', 'tradition', 'origin'
);

// TODO Deal with all the stuff in _quadrate8.inc
   $this->patterns[languageDB::CHARGE_FEATURES] = array ( 
     'crossed=',
 'fructed=',
 'beaked=',
 'hooped=',
 'plumed=',

     'seeded=',
 'barbed=',
 'gemmed=',
 'acorned=',
 'laced=',
 'necked=',

     'muzzled=',
 'allume=',
 'tusked=',
 'spotted=',
 'corded=',

     'chausse=',
 'crined=',
 'nooked=',
 'hooded=',
'spouting=',

     'habited=',
 'lined=',
 'maned=',
 'nerved=',
 'striped=',

     'penned=',
 'stringed=',
 'aulned=',
 'stemmed=',
 'book=',

     'belled=',
 'finned=',
 'nest=',
 'staff=',
 'seamed=',
 'waxed=',

     'acorns=',
 'towered=',
 'domed=',
 'tasselled=',
 'mortared=',

     'shelled=',
 'tied=',
 'headed=',
 // NOT heads
     'drooping=',
 'voided=',
 'beaded=',
 'ringed=',
 'masonry=',

      'leather(s|ed)?=leathered',
        'jessed?=jessed' ,
      'pole(s|d)?=pole',
        'portal(s|ed)?=portal',
      'cuff(s|ed)?=cuff',
      'in profile=in-profile',
      'turned up=turned-up',
      'tool(s|ed)?=tools',
        'with heads?=head',
      'knob(s|bed)?=knobbed',
      'apron(s|ed)?=apron',
      'belt(s|ed)?=belted',
      'trouser(s|ed)?=trousered',
      'hose(s|ed)?=trousered',
      'hat(s|ted)?=hatted',
      'boot(s|ed)?=booted',
      'housings?=caparisoned',
        'quill(s|ed)?=quilled',
        'furnished with quills?=quilled',
      'caparison(s|ed)?=caparisoned',
      'stirrup(s|ed)?=stirruped',
        'chain(s|ed)?=chains',
      'full faced=full-faced',
     'jessant ?de ?l[iy]s=jessant',
      'ribbon(s|ed)?=ribboned' ,
      'bows?=bow' ,
        'tip(s|ped)?=tipped' ,
      'hands?=hands' ,
      'buckle(s|d)? ?in ?the ?centre=buckled' ,
      'in the(ir?) shells?=shelled' ,
      'clasp(s|ed)=clasped' ,
      'thread(s|ed)=threaded' ,
      'garnish(ing|ed)=garnished' ,
      'vi[sz]ors?=visor' ,
      'spur(s|red)?=spurs' ,
      '(top|impost|cap|capital)s?=top' ,
      '(pedestal|base)s?=base' ,
      '(tyne|attir|antler)(s|ed)=attired' ,
      'banner(ed|s)?=banner' ,
       'flute[ds]=fluted' ,
       'fluke[ds]=fluked' ,
      's?haft(s|ed)?=shafted' ,
      '(flight|feather)ed=feathered' ,
       '(crested|combs|combed)?=combed' ,
       'jelloped=jelloped' ,
       '(queue|tail)(s|e?d)=tailed' ,
       '(banded|encircled) !(by|with|of)=encircled' ,
       'stocks?=stock' ,
       '(stave?|hilt)(s|ed)?=hilt' ,
       '(pom?mell?s?|pomm?ell?ed)=pomell' ,
       '(tongu|langu)(es|ed)=langued' ,
       'bladed?s?=blade' ,
       'h(andl|elv)ed?s?=handle' ,
       'leaves?d?=leaved' ,
       'leafed=leaved' ,
       '(b?e?legged|membered)=legged' ,
       '(u?o?ngull?ed|hoo[fv]ed?s?)=unguled' ,
       'trunks?e?d?=trunked' ,
       '(slipped|stalked|leaved)=slipped' ,
       '(legged|membered)=legged' ,
       '(winged|aisle)=winged' ,
       '(barbed|armed) ?at ?all ?points=armed' ,
       '?in ?full armou?r(ed)?=armoured' ,
       'point(s|ed) ?steeled=pointed' ,
       'heads? steeled=pointed' ,
       '(e|ai)guisc?e=pointed' ,
       '(horned|acorne)=horned' ,
       'horns=horned' ,
       '(fimbriated|edged?s?)=fimbriated' ,
       '(flower|bud)(s|d?ed)=flowers' ,
       'j[eo]llopped=wattled' ,
       'wattle(s|d)?=wattled' ,
       'hull(s|ed)?=hulled' ,
       'sail(s|ed)?=sails' ,
       'pennant(s|ed)?=pennants' ,
       'collar(s|ed)?=collared' ,
       'eyes ?of ?the ?tails?=eyes' ,
       '?vine stick=stick' ,
       '?fore feet fettered=feetfettered' ,
       'heads?=head' , // See Parker Corpus ADY, usually a mod but if both present, features
       'faces?=head' ,
       '(talon|claw)s?=claw' ,
       'ears?=ear' ,
       '([gj]ambe?|leg)s?=leg' ,
      'fired ?in ?# ?places=inflamed' ,
      '(flammant|bursting|exploding)=inflamed' ,
      '(en|in)flamed?=inflamed' ,
      'in ?a (flames?d?|fire)=inflamed' ,
      '(with|emitting) flames ?of ?fire=inflamed' ,
      'flames? issu(ing|ant) ?from ?each=inflamed' ,
      '(crack|hold)ing ?an? ?ore nuts?=holding-nut',
       'bellied=bellied' ,
       'pizzled=pizzled' ,
);
      $this->patterns[languageDB::BUNDLE] = array (
         '(sheaf|sheave|bundle|bunche?)s? ?of=bundle' ,
         'pairs? ?of=pair' ,
      );
   
   $this->patterns[languageDB::CHARGE_WITH] = array ( 
      array ( '(enrich|charg|deck)ed (by|with|of)', 'charged-with' ),
      array ( 'consisting of', 'charged-with'),
       array ( 'comprised of', 'charged-with'),
       array ( 'made up of', 'charged-with'),
      array ( '?(each|all|both) (enrich|charg|deck)ed (by|with|of)', 'charged-each' ),
      array ( 'thereon', 'charged-with' ),
      array ( 'charged (at|on) each ?(point|corner|limb) ?with', 'charged-each' ),
      array ( 'surmounted (by|with|of)', 'charged-with' ),
      array ( '(en)?circled (by|with|of)', 'encircled-by' ),
      array ( 'all within', 'encircled-by' ),
      array ( 'en(toured|vironed|vironee|veloped) (by|with|of)', 'encircled-by' ),
      array ( 'enfiled (by|with|of)', 'enfiled' ),
      array ( 'engoule (by|with|of)', 'engoule' ),
      array ( 'ensigned (by|with|of)', 'ensigned' ),
      array ( 'adorned (by|with|of)', 'adorned' ),
      array ( 'inscribed (by|with|of)', 'inscribed' ), // TODO consolidate some of these?
      array ( 'figured (by|with|of)', 'inscribed' ),
      array ( 'on ?the upper ?limbs?', 'on-upper-limb' ),  
      array ( 'on ?the lower ?limbs?', 'on-lower-limb' ),  
      array ( 'on ?the sides? ?limbs?', 'on-side-limb' ),
      array ( 'on ?the breasts?', 'on-breast' ),  
      array ( 'in the beak', 'in-beak' ),
      array ( 'issuing from', 'assurgent' ),
      array ( 'assurgent', 'assurgent' ),
      array ( 'suspending', 'suspending' ),
      array ( 'charged (at|on) # (point|corner|limb)s? ?with', 'charge-on-point' ),
      );

   $this->patterns[languageDB::HOLDING] = array (
      array ( 'holding a', 'in-hand-right' ),
      array ( 'held in (its|his|her|their|the) ?* (paw|hand)s? of', 'in-hand-right' ),
      array ( 'holding in (its|his|her|their|the) (paw|hand)s?', 'in-hand-right' ),
      array ( 'holding in ?(its|his|her|their|the) (jaw|mouth)s?', 'in-mouth' ),
      array ( 'holding in ?(her|his|its|their|the) (dexter|right) hand|paw', 'in-hand-right'),
      array ( 'holding in ?(her|his|its|their|the) (dexter|right) foot', 'in-foot-right'),
      array ( 'holding in ?(her|his|its|their|the) (sinister|left) hand|paw', 'in-hand-left'),
      array ( 'holding in ?(her|his|its|their|the) (sinister|left) foot', 'in-foot-left'),
      array ( 'holding in (both|all|each) ?(her|his|its|their) (hand|paw)(s?)', 'in-hand-both'),
      array ( 'flying', 'in-flag-point' ),
      );

  $this->patterns[languageDB::CROWN_MODS] = array (
    array ('crowne?d', 'crowned'),
      array ('crown', 'crown'),
    array ('wreathe?d', 'wreathed'),
    array ('(en)?gorged ?(with|by) ?an?', 'gorged'),
  // crown types for animal <type> crowned
  'antique', 'eastern', 'marquis', 'palisado',
     'saxon', 'vallary', 'viscount',
     array ('astral(ly)?', 'astral'),
     array ('baron(ially)?', 'baron'),
     array ('ducal(ly)?', 'ducal'),
     array ('mural(ly)?', 'mural'),
     array ('naval(ly)?', 'naval'),
     array ('royal(ly)?', 'royal'),
     array ('dukes?', 'duke'),
     array ('earls?', 'earl'),
     array ('grass', 'crown-of-grass'),
     array ('mullet', 'crown-of-mullets'),
     array ('myrtle', 'crown-of-myrtle'),
     array ('oak lea(f|ves?d?)', 'crown-of-oak-leaves'),
     array ('king of arms?', 'king-of-arms'),
  );
   

