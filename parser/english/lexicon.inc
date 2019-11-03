<?php /* Copyright 2014 Karl R. Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */
   
class languageDB extends lexicon {
  // These constants acts as keys into the pattern database
  // Some are also used to indicate the type of modifier
  const OVERALL = 'overall';
  const METAOVERALL = 'metaoverall';
  const SPLITS = 'splits';
  const ONFIELDOF = 'onfieldof';
  const SEMICOLON = 'semicolon';
  const COMMA = 'comma';
  const WORD_THE = 'wordthe';
  const WORD_OF = 'wordof';
  const WORD_ON = 'wordon';
  const WORD_IN = 'wordin';
  const COLOUR = 'colour';
  const BACKREF = 'backref';
  const TREATMENT = 'treatment';
  const FUR = 'fur';
  const SEMY = 'semy';
  const GROUP = 'group';
  const PROPER = 'proper';
  const ANDD = 'andd';
  const WITH = 'with';
  const QUARTERED = 'quartered';
  const QUARTERS = 'quarters';
  const COUNTERCHANGE = 'counterchange';
  const QUANTITY = 'amount';
  const NUMBER = 'number';
  const DIVISION_MODS = 'divmods';
  const DIVISION = 'division';
  const LINETYPE = 'linetype';
  const ORIENTATION = 'orientation';
  const ORDINARY = 'ordinary';
  const ORDINARY_PREFIX = 'ordprefix';
  const ORDINARY_MODS = 'ordmods';
  const COLOUR_MODS = 'compony';
  const CROWN_MODS = 'crownmods';
  const LINETYPE_PREFIX = 'linepre';
  const LINETYPE_POSTFIX = 'linepost';
  const COTTICE_MODS = 'cotticemods';
  const CHARGE_NOT_ORDINARY = 'chargeconf';
  const ORDINARY_OR_CHARGE = 'ordorchg';
  const ORDINARY_NOT_CHARGE = 'ordinaryconf';
  const CHARGE = 'charge';
  const BETWEEN = 'between';
  const COTTICE = 'cottice';
  const VOIDED = 'voided';
  const CHARGE_WITH = 'chargewith';
  const ARRANGEMENT = 'arrangement';
  const SPECIFIER = 'specifier';
  const LOCATION = 'location';
  const QUANTIFIER = 'quantifier';
  const BUNDLE = 'bundle';
  const CHARGE_PREFIX = 'chargepre';
  const CHARGE_ADJACENT = 'chargeadj';
  const CHARGE_FEATURES = 'chargefeat';
  const CHARGE_MODS = 'chargemod';
  const POSITION = 'position';
  const POINTS = 'points';
 // const SUPPLEMENTARY = 'supplementary';
  const SHOWN_EXTRA = 'shownextra';
  const SHOWN_MODS = 'shown';
  const DRAWN_MODS = 'drawn';
  const FONT = 'font';
  const QUARTERMARK = 'quartermark';
  const QUARTERMODS = 'quartermods';
  const LETTERS = 'letters';
  const INVERSE = 'inverse';
  const MANTLING = 'mantling';
  const ACHIEVEMENT_LOC = 'achievement_loc';
  const MOTTO = 'motto';
  const CREST = 'crest';
  const TORSE = 'torse';
  const HELMET = 'helmet';
  const ACHIEVEMENT = 'achievement';
  
  public function __construct() {
    global $options;

    $this->language = 'english';
     
    // Each top level element of the patterns array is a named sub-array
    // Each named sub-array represents a particular word category 
    // The sub arrays contain a list of mappings between regular expressions
    // and a "key term". (If there is no key term given, the regex itself
    // is used). The grammar requests a search for a particular word
    // category, and if found, the specific key term is returned.
 
    $this->patterns = array(
      self::WORD_THE => array ( 'the' ),
      self::OVERALL => array ( 'overall', 'sur le tout', 'all with(in)?', "over all", "in pretence" ),
      self::METAOVERALL => array ( 'over(all)? ?all ?the ?# (quarters|quarterings|halves|parts|sections)', ),
      self::MANTLING => array (
        array ('mantling ?of ?ribbons', 'ribbons'),  // note default
        array ('lambrequin ?of ?ribbons', 'ribbons'),
        array ( 'cloak ?mantling', 'cloak'),
        array ( 'mantling ?of ?a cloak', 'cloak'),
      ),
      self::ACHIEVEMENT => array ('achievement', 'hatchment'),
      self::CREST => array (
        array ('?(a|the) crest ?of', 'crest' ), // room for other crest types?
      ),
      self::TORSE => array (
        array ('?(a|the) (torse|bourlet)', 'torse' ),
      ),
      self::ACHIEVEMENT_LOC => array (
        array ('?to ?the sinister', 'sinister'),
        array ('?to ?the dexter', 'dexter'),
        array ('?(of|to) ?the centre', 'centre'),
      ),
      self::HELMET => array (
        array ('?(the|a|an) royal helmet', 'royal'),
        array ('?(the|a|an) monarchs? helmet', 'royal'),
        array ('?(the|a|an) (knight|baronet)s? helmet', 'baronet'),
        array ('?(the|a|an) ?(esquire|gentlem[ea]n)s? helmet', 'esquire'),  // DEFAULT
        array ('?(the|a|an) (peer|lord)s? helmet', 'peer'),
      ),
      self::MOTTO => array ('motto *', 'motto'),
      // self::SUPPLEMENTARY => array (
      //   array ('crest', 'crest'),
      //   array ('supporters', 'supporters'),
      //   array ('motto', 'motto'),
      //   array ('devi[sc]e', 'motto'),
      // ),
      self::SPLITS => array (
        array ( 'impaled with', 'impaled' ),
        array ( 'dimidiated with', 'dimidiated' ),
      ),
      self::ONFIELDOF => array ( '?(on|upon) a (field|court) ?of' ),
      self::WITH => array ( array ('(with|upon)', 'with' ) ),
      self::BETWEEN => array ( 'between', 'inter', 'inside', 'within', 'surrounded by', 'accompagne', 'cantonned by' ),
      self::QUARTERED => array (
        array ( '(grand)?quartered', 'quartered' ), // Always a marshalling
        array ( '(grand)?quarterly', 'quarterly' ), // Could be a marshalling or a division
      ),
      self::QUARTERMARK => array ( '\\)' ),
      self::LETTERS => array (
        array ('a', 1),
        array ('b', 2),
        array ('c', 3),
        array ('d', 4),
        array ('e', 5),
        array ('f', 6),
        array ('g', 7),
        array ('h', 8),
        array ('i', 9),
        array ('j', 10),
        array ('k', 11),
        array ('l', 12),
        array ('m', 13),
        array ('n', 14),
        array ('o', 15),
        array ('p', 16),
        array ('q', 17),
        array ('r', 18),
        array ('s', 19),
        array ('t', 20),
        array ('u', 21),
        array ('v', 22),
        array ('w', 23),
        array ('x', 24),
        array ('y', 25),
        array ('z', 26),
        array ('aa', 27),
        array ('bb', 28),
        array ('cc', 29),
        array ('dd', 30),
        array ('ee', 31),
        array ('ff', 32),
        array ('gg', 33),
        array ('hh', 34),
        array ('ii', 35),
        array ('jj', 36),
        array ('kk', 37),
        array ('ll', 38),
        array ('mm', 39),
        array ('nn', 40),
        array ('oo', 41),
        array ('pp', 42),
        array ('qq', 43),
        array ('rr', 44),
        array ('ss', 45),
        array ('tt', 46),
        array ('uu', 47),
        array ('vv', 48),
        array ('ww', 49),
        array ('xl', 50),
        array ('yy', 51),
        array ('zz', 52),
        array ('aaa', 53),
        array ('bbb', 54),
        array ('ccc', 55),
        array ('ddd', 56),
        array ('eee', 57),
        array ('fff', 58),
        array ('ggg', 59),
        array ('hhh', 60),
        array ('iii', 61),
        array ('jjj', 62),
        array ('kkk', 63),
        array ('lll', 64),
      ),
      self::QUARTERS => array (
        array ( '(first|1st|i)', 1 ),
        array ( '(second|2nd|ii)', 2 ),
        array ( '(third|3rd|iii)', 3 ),
        array ( '(fourth|4th|iv|iiii)', 4 ),
        array ( '(fifth|5th|v)', 5 ),
        array ( '(sixth|6th|vi)', 6 ),
        array ( '(seventh|7th|vii)', 7 ),                                                 
        array ( '(eighth|8th|viii)', 8 ),
        array ( '(ninth|9th|ix)', 9 ),
        array ( '(tenth|10th|x)', 10 ),
        array ( '(eleventh|11th|xi)', 11 ),
        array ( '(twelth|12th|xii)', 12 ),
        array ( '(thirteenth|13th|xiii)', 13 ),
        array ( '(fourteenth|14th|xiv)', 14 ),
        array ( '(fifteenth|15th|xv)', 15 ),
        array ( '(sixtenth|16th|xvi)', 16 ),
        array ( '(seventeenth|17th|xvii)', 17 ),
        array ( '(eigtheenth|18th|xviii)', 18 ),
        array ( '(nineteenth|19th|xix)', 19 ),
        array ( '(twentieth|20th|xx)', 20 ),
        array ( 'twenty first', 21 ),
        array ( '(21st|xxi)', 21 ),
        array ( 'twenty second', 22 ),
        array ( '(22nd|xxii)', 22 ),
        array ( 'twenty third', 23 ),
        array ( '(23rd|xxiii)', 23 ),
        array ( 'twenty fourth', 24 ),
        array ( '(24th|xxiv|xxiiii)', 24 ),
        array ( 'twenty fifth', 25 ),
        array ( '(25th|xxv)', 25 ),
        array ( 'twenty sixth', 26 ),
        array ( '(26th|xxvi)', 26 ),
        array ( 'twenty seventh', 27 ),
        array ( '(27th|xxvii)', 27 ),
        array ( 'twenty eigth', 28 ),
        array ( '(28th|xxviii)', 28 ),
        array ( 'twenty ninth', 29 ),
        array ( '(29th|xxix)', 29 ),
        array ( '(thirtieth|30th|xxx)', 30 ),
        array ( 'thirty first', 31 ),
        array ( '(31st|xxxi)', 31 ),
        array ( 'thirty second', 32 ),
        array ( '(32nd|xxxii)', 32 ),
        array ( 'thirty third', 33 ),
        array ( '(33rd|xxxiii)', 33 ),
        array ( 'thirty fourth', 34 ),
        array ( '(34th|xxxiv|xxxiiii)', 34 ),
        array ( 'thirty fifth', 35 ),
        array ( '(35th|xxxv)', 35 ),
        array ( 'thirty sixth', 36 ),
        array ( '(36th|xxxvi)', 36 ),
        array ( 'thirty seventh', 37 ),
        array ( '(37th|xxxvii)', 37 ),
        array ( 'thirty eigth', 38 ),
        array ( '(38th|xxxviii)', 38 ),
        array ( 'thirty ninth', 39 ),
        array ( '(39th|xxxix)', 39 ),
        array ( '(fortieth|40th|xl)', 40 ),
        array ( 'forty first', 41 ),
        array ( '(41st|xli)', 41 ),
        array ( 'forty second', 42 ),
        array ( '(42nd|xlii)', 42 ),
        array ( 'forty third', 43 ),
        array ( '(43rd|xliii)', 43 ),
        array ( 'forty fourth', 44 ),
        array ( '(44th|xliv|xliiii)', 44 ),
        array ( 'forty fifth', 45 ),
        array ( '(45th|xlv)', 45 ),
        array ( 'forty sixth', 46 ),
        array ( '(46th|xlvi)', 46 ),
        array ( 'forty seventh', 47 ),
        array ( '(47th|xlvii)', 47 ),
        array ( 'forty eigth', 48 ),
        array ( '(48th|xlviii)', 48 ),
        array ( 'forty ninth', 49 ),
        array ( '(49th|xlix)', 49 ),
        array ( '(fiftieth|50th|l)', 50 ),
        array ( 'fifty first', 51 ),
        array ( '(51st|li)', 51 ),
        array ( 'fifty second', 52 ),
        array ( '(52nd|lii)', 52 ),
        array ( 'fifty third', 53 ),
        array ( '(53rd|liii)', 53 ),
        array ( 'fifty fourth', 54 ),
        array ( '(54th|liv|liiii)', 54 ),
        array ( 'fifty fifth', 55 ),
        array ( '(55th|lv)', 55 ),
        array ( 'fifty sixth', 56 ),
        array ( '(56th|lvi)', 56 ),
        array ( 'fifty seventh', 57 ),
        array ( '(57th|lvii)', 57 ),
        array ( 'fifty eigth', 58 ),
        array ( '(58th|lviii)', 58 ),
        array ( 'fifty ninth', 59 ),
        array ( '(59th|lix)', 59 ),
        array ( '(sixtieth|60th|lx)', 60 ),
        array ( 'sixty first', 61 ),
        array ( '(61st|lxi)', 61 ),
        array ( 'sixty second', 62 ),
        array ( '(62nd|lxii)', 62 ),
        array ( 'sixty third', 63 ),
        array ( '(63rd|lxiii)', 63 ),
        array ( 'sixty fourth', 64 ),
        array ( '(64th|lxiv|lxiiii)', 64 ),
      ),
      self::SEMICOLON => array ( ':', ';', '\\.' ), // need to escape '.' as this is regex special character
      self::COMMA => array ( ',' ),
      self::WORD_OF => array ( 'of' ),
      self::WORD_ON => array ( 'on' ),
      self::WORD_IN => array ( 'in ?the' ),
        self::POINTS => array ( 'points' ),
      self::ANDD => array ( 
        array ( '(and|&|also)', 'and'), 
        array ( '(and|&) also', 'and'),
        ),
      self::COLOUR => array (
          // basic tinctures (with abbreviations)
          array ( 'or',  'or'    ),
          array ( 'az(ure)?',  'azure'   ),
          array ( 'vert|sinople', 'vert' ),
          array ( 'gui?(les)?', 'gules'    ),
          array ( 'arg(ent)?', 'argent'  ),
          array ( 'sa(ble)?', 'sable' ),
          array ( 'purp(le|ure)', 'purpure' ),
          // additional tinctures
          'murrey',  'sanguine',  'carnation', 'brunatre',  'rose', 
          'buff' ,
          array (  'cei?ndree', 'cendree' ),
          array ( 'bis(que)?', 'bis' ),
          array ( 'celestial azure', 'celestial-azure'  ),
          array ( 'bleu celeste', 'celestial-azure'  ),
          array ( 'horizon blue', 'celestial-azure'  ),
          array ( 'red ochre', 'red-ochre'  ),
          array ( 'yellow ochre', 'yellow-ochre'  ),
          array ( 'senois', 'senois' ),
          array ( '(tanned|tenne|tawny)', 'tenne' ),
          'orange',
          // "de" names (for gouttes particularly)
          array ( 'd or', 'or'  ),
          array ( 'd eau', 'argent'  ),
          array ( 'de larmes', 'azure'  ),
          array ( 'de sangu?e?', 'gules' ),
          array ( 'de vin', 'gules'   ),
          array ( 'de poix', 'sable'  ),
          array ( 'd huile',  'vert' ),
          array ( 'd olive',  'vert' ),
          // obsolete names
          /* Start Comment obsolete names
           'amethyst', 'diamond', 'dragons head', 'dragons tail', 'emerald',
          'jacinth', 'luna', 'mars', 'pearl', 'ruby', 'sol', 'topaz', 'venus',
          End comment here */
          // English names
          array ( 'iron', 'iron'),
          array ( 'bronze', 'bronze'),
          array ( 'copper', 'copper'),
          array ( 'crimson', 'crimson'),
          array ( 'lead', 'lead'),
          array ( 'steel', 'steel'),
        array ( 'gold',  'or'    ),
        array ( 'blue',  'azure'   ),
        array ( 'green', 'vert' ),
        array ( 'red', 'gules'    ),
        array ( 'silver', 'argent'  ),
        array ( 'black', 'sable' ),
        array ( 'brown', 'brunatre' ),
        array ( 'gr[ae]y', 'gray' ),
        array ( 'pink', 'rose' ),
        array ( 'white', 'white' ),
        ),
        self::BACKREF => array (
            array ( '(like|of|as) the same', 'ofsame'),
            array ( '(like|of|as) the last', 'oflast'),
            array ( '(like|of|as) the field', 'offield'),
            array ( '(like|of|as) the (first|1st)', 'offirst'),
            array ( '(like|of|as) the (second|2nd)', 'ofsecond'),
            array ( '(like|of|as) the (third|3rd)', 'ofthird'),
            array ( '(like|of|as) the (fourth|4th)', 'offourth'),
        ),
        self::TREATMENT => array (
            array ( 'ann?ulett?y',    'annuletty',    ),
            array ( 'rou?ndell?y',    'roundelly',    ),
            array ( 'bezant(y|e)',  'bezanty', true ),
            array ( 'billett?y',     'billetty',     ),
            array ( '(checky|cheque?y|checkered)', 'checky' ),
            array ( '?billett?y counter billett?y', 'counter-billetty',),
            array ( 'crusill?[ye]',     'crusilly',     ),
            array ( 'estoill?[ye]',     'estoilly',    ),
            array ( 'fretty',       'fretty',       ),
            array ( '(treille|latticed)',       'latticed',       ), // TODO draw this, see boutell
            array ( 'plumetty',       'plumetty',       ),
            array ( 'go?utty?e?e?',      'goutty',     ),
            array ( 'grillage',     'grillage',     ),
            array ( 'honeycombed',  'honeycombed',  ),
            array ( 'ermined',  'ermined',  ),
            array ( 'estencell?y',  'estencely',  ),
            array ( 'hurty',        'hurty',       true ),
            array ( '(lozengy|masculy)',     'lozengy',            ),
            array ( 'maily',        'maily',        ),
            array ( '(masonn?ed?|muraille)',      'masoned',      ),
            array ( 'mullett?y',    'mulletty',   ),
            array ( 'pap(e|i)l?lonn?(y|e)', 'papelonny', ),
            array ( 'platy',        'platy',       true ),
            array ( 'scaly',        'scaly',        ),
            array ( 'sem(e|y) de l(i|y)s', 'seme-de-lys', ),
            array ( 'fl(o|eu)ry', 'seme-de-lys', ),
            array ( 'vair(e|y)',    'vairy',  ),
            array ( 'potent(e|y)',    'potenty',  ),
            array ( 'ai?[qg]ui?lated', 'aquilated' ), // TODO draw this
            array ( 'ferrated', 'ferrated' ), // TODO and this
            //array ( 'trefle', 'trefle' ), // TODO and this
            array ( 'verd(e|oy)', 'verdy' ), // TODO and this
            array ( 'barruly wavy', 'barruly-wavy' ), // TODO and this
            array ( 'pied ?at ?random', 'pied' ),
            array ( 'piebald', 'pied' ),
            array ( 'grad(ed|y)', 'grady'),
            array ( 'grad(ed|y) in fesse?', 'grady-in-fess'),
            array ( 'grad(ed|y) in pale', 'grady-in-pale'),
            array ( 'grad(ed|y) in bend', 'grady-in-bend'),
            array ( 'grad(ed|y) in bend sinister', 'grady-in-bendsin'),
            array ( 'grad(ed|y) in sinister bend', 'grady-in-bendsin'),
            array ( 'grad(ed|y) in annull?o', 'grady-in-annulo'),
            // Following DO NOT HAVE HERALDIC PRECEDENT (but are useful anyway)
            array ( 'guzy',        'guzy',       true ),
            array ( 'pommy',        'pommy',       true ),
            array ( 'torty',        'torty',       true ),
          ),
        self::INVERSE => array ('reversed', 'inverted'),
        self::FUR => array (
          'ermine',   'erminois', 'erminites', 'pean', 'potent', 'vair',
          //array ( 'or ermined sa(ble)?', 'erminois',           ),                      
          // array ( 'sa(ble)? ermined or', 'pean',            ),
          array ( 'vair [ei]n pale', 'vair-in-pale',    ),
          array ( 'vair [ei]n point', 'vair-en-point',   ),
          array ( 'ermines', 'counter-ermine',  ),
          array ( 'counter ermine', 'counter-ermine',  ),
          // array ( 'sa(ble)? ermined arg(ent)?', 'counter-ermine',  ),
          array ( 'counter vair', 'counter-vair',    ),
          array ( 'counter potent', 'counter-potent',  ),
          array ( 'potent counter potent', 'potent-counter-potent', ),
          array ( 'tapiss?ee? ?of ?wheat',    'tapisse',    ),
          // Sports pitches
            array ('soccer', 'soccer-field'),
            array ( '?american football', 'american-football-field'),
            array ( 'tennis', 'tennis-court'),
            array ( 'basketball', 'basket-ball'),
            array ( 'basket ball', 'basket-ball'),
        ),
        self::SEMY => array ( '(seme|semy) (de|with|of)', '(aspersed|strewn) with', '(poudre|powdered) with' ),
        self::GROUP => array ( '(both|all|the|his|her|its)',  ),
        self::PROPER => array ( '(proper|ppr)' ),
        self::COUNTERCHANGE => array ( 'interchanged?', 'transmuted', 'counter cha[nr]ged?', 'countercha[nr]ged?', 'countercoloured', 'de l un en l autre' ),
        self::DIVISION => array (
          array ( 'pily',                  'pily',               ),
          array ( 'quarterly',             'quarterly',          ),
          array ( '?part(y|ed) per cross',      'quarterly',          ),
          array ( 'quarterly ?, per fesse?',    'quarterly-per-fess', ),
          array ( 'quarterly ?, per pale',    'quarterly-per-pale', ),
          array ( 'paly bendy',            'paly-bendy',         ),
          array ( 'bendy paly',            'paly-bendy',         ),
          array ( 'pily bendy',            'pily-bendy',         ),
          array ( 'bendy pily',            'pily-bendy',         ),
          array ( 'b[ua]rr?(y|uly) ?sans ?nombre', 'barry',      ),
          array ( 'paly',                  'paly',               ),
          array ( 'chape',                 'chape',              ),
          array ( 'chevronn?(ell)?y',      'chevronny',          ),
          array ( 'chausse',               'chausse',            ),
          array ( 'quarterly quartered',   'gyronny',		 ),
          array ( '?part(y|ed) ?per g(i|y)ronny',    'gyronny',            ),
          array ( 'bendy',                 'bendy',              ),
          array ( '?part(y|ed) per fesse?' ,    'per-fess',           ),
          array ( 'parted fesse?wise' ,      'per-fess',           ),
          array ( '?part(y|ed) per pale' ,      'per-pale',           ),
          array ( 'parted palewise' ,      'per-pale',           ),
          array ( '?part(y|ed) per pile' ,      'per-pile',           ),
          array ( 'parted pilewise' ,      'per-pile',           ),
          array ( '?part(y|ed) per pall' ,      'per-pall',           ),
          array ( '?part(y|ed) per pall arrondi' ,      'per-pall-arrondi',           ),
          array ( 'tierced? (in|per) pairle' ,    'per-pall',           ),
          array ( 'tierced? (in|per) pairle arrondi' ,    'per-pall-arrondi',           ),
          array ( '?part(y|ed) per chevron' ,   'per-chevron',        ),
          array ( 'parted chevronwise' ,   'per-chevron',        ),
          array ( '?part(y|ed) per bend',       'per-bend',           ),
          array ( 'parted bendwise',       'per-bend',           ),
          array ( '?part(y|ed) per saltire' ,   'per-saltire',        ),
          array ( 'parted saltirewise' ,   'per-saltire',        ),
          array ( 'tierced? (in|per) pale' ,      'tierced-in-pale',    ),
          array ( 'tierced? (in|per) fesse?' ,      'tierced-in-fess',    ),
          array ( 'tierced? (in|per) bend' ,      'tierced-in-bend',    ),
          array ( 'tierced? (in|per) chevron',    'tierced-in-chevron', ),
          array ( 'fusill?y',              'fusily',             ),
          array ( 'fusill?y bendy ?dexter','fusily-bendy',       ),                                     
          array ( 'fusill?y in bend',      'fusily-bendy',       ),
          // array ( '(lozengy|masculy)',     'lozengy',            ),
          array ( 'barry bendy ?lozengy' , 'barry-bendy',        ),
          array ( 'bendy lozengy' , 'barry-bendy',        ),
          array ( 'barry pily' ,           'barry-pily',         ),
          array ( 'barry indented ?the (one|1) in the other', 'barry-indented-oio',  ),
       ),   
       self::DIVISION_MODS => array ( 
         array ( 'of # ?pieces?', 'ofnum' ),
         array ( 'rivett?ed', 'riveted' ),
         ),
       self::QUANTITY => array (
          array ( '(a|an|the|some)', "1" ),
          ),
       self::NUMBER => array (
          '1', '2', '3', '4', '5', '6', '7', '8', '9',
          '10', '11', '12', '13', '14', '15', '16', '17', '18', '19',
          '20', '21', '22', '23', '24', '25', '26', '27', '28', '29',
          '30', '31', '32', '33', '34', '35', '36', '37', '38', '39',
          '40', '41', '42', '43', '44', '45', '46', '47', '48', '49',
          '50', '51', '52', '53', '54', '55', '56', '57', '58', '59',
          '60', '61', '62', '63', '64',
          array ( 'one', "1" ),
          array ( 'two', "2" ),
          array ( 'three', "3" ),
          array ( 'four', "4" ),
          array ( 'five', "5" ),
          array ( 'six', "6"  ),
          array ( 'seven', "7" ),
          array ( 'eight', "8" ),
          array ( 'nine', "9" ),
          array ( 'ten', "10" ),
          array ( 'eleven', "11" ),
          array ( 'twelve', "12" ),
          array ( 'thirteen', "13" ),
          array ( 'fourteen', "14" ),
          array ( 'fifteen', "15" ),
          array ( 'sixteen', "16" ),
          array ( 'seventeen', "17" ),
          array ( 'eighteen', "18" ),
          array ( 'nineteen', "19" ),
          array ( 'twenty', "20" ),
          array ( 'twenty one', "21" ),
          array ( 'twenty two', "22" ),
          array ( 'twenty three', "23" ),
          array ( 'twenty four', "24" ),
          array ( 'twenty five', "25" ),
          array ( 'twenty six', "26" ),
          array ( 'twenty seven', "27" ),
          array ( 'twenty eight', "28" ),
          array ( 'twenty nine', "29" ),
          array ( 'thirty', "30" ),
          array ( 'thirty one', "31" ),
          array ( 'thirty two', "32" ),
          array ( 'thirty three', "33" ),
          array ( 'thirty four', "34" ),
          array ( 'thirty five', "35" ),
          array ( 'thirty six', "36" ),
          array ( 'thirty seven', "37" ),
          array ( 'thirty eight', "38" ),
          array ( 'thirty nine', "39" ),
          array ( 'forty', "40" ),
          array ( 'forty one', "41" ),
          array ( 'forty two', "42" ),
          array ( 'forty three', "43" ),
          array ( 'forty four', "44" ),
          array ( 'forty five', "45" ),
          array ( 'forty six', "46" ),
          array ( 'forty seven', "47" ),
          array ( 'forty eight', "48" ),
          array ( 'forty nine', "49" ),
          array ( 'fifty', "50" ),
          array ( 'fifty one', "51" ),
          array ( 'fifty two', "52" ),
          array ( 'fifty three', "53" ),
          array ( 'fifty four', "54" ),
          array ( 'fifty five', "55" ),
          array ( 'fifty six', "56" ),
          array ( 'fifty seven', "57" ),
          array ( 'fifty eight', "58" ),
          array ( 'fifty nine', "59" ),
          array ( 'sixty', "60" ),
          array ( 'sixty one', "61" ),
          array ( 'sixty two', "62" ),
          array ( 'sixty three', "63" ),
          array ( 'sixty four', "64" ),
          array ( '(as|so) many', 'asmany' ),
          ),
        self::ORIENTATION => array ( 
           'sinister', 'reversed','dexter', 'inverted', 'enhanced', 
           array ( '(abased|abaisse)', 'abased', ),
          ),
        self::ORDINARY => array (
            'another', // special term, means "voided of" (should only be used after "on" or "charged with"
            array( 'bars?', 'bar' ),
            array( 'barrulets?', 'barrulet' ),
            array( 'crancelin', 'crancelin' ),
            array( 'bend archy coronett[ey]', 'crancelin' ),
            array( 'bendlets?', 'bendlet' ),
            array( 'e?sca[rf]pe?s?', 'scarpe' ),
            array( 'chevronels?', 'chevronel' ),
            array( 'couple closes?', 'couple-close' ), 
            array( 'close couples?', 'couple-close' ), 
            array( 'closets?', 'closet' ),
            array( '(baston|riband)', 'riband' ), 
            array( '[ie]ndorses?', 'endorse' ),
            array( 'humets?', 'humet' ),
            array( 'pall?ets?', 'palet' ),
            array( 'piles? ?of ?(3|three) ?points ?issuing ?from ?the ?chief', 'pile' ),
            array( 'mountains?', 'mountain' ),
            array( 'base', 'base' ),
            array( 'bends?', 'bend' ),
            array( 'chief', 'chief' ),
            array( 'fesse?', 'fess' ),
            array( 'fesse? between (2|two) chevrons?', 'fess-between-chevrons' ),
            array( 'dance', 'dance' ),
            array( 'ford', 'ford' ),
            array( 'gores?', 'gore' ),
            array( 'gore sinister', 'gore-sinister' ),
            array( 'sinister gore', 'gore-sinister' ),
            array( 'pallium', 'pallium' ),
            array( 'pale', 'pale' ),
            // array ( '(file|lam?bel)', 'label'),
            array( 'point', 'point' ), // TODO draw this - see meaning 2 parker/point
            array( 'plaine? point', 'base' ), // (plain) point and base are synonyms
            array( 'ecclesiastical pale', 'pale-ecclesiastical' ),
            array( 'canadian pale', 'pale-canadian' ),
            array( 'quarter', 'quarter' ),
            array( 'tierce', 'tierce' ),
            array( '(gorge|whirlpool|gurges)', 'gorge' ),
            array( 'grid', 'grid' ),
            array( 'trimount', 'trimount' ),
            array( 'moun[td] of (3|three) peaks', 'trimount' ),
            array( 'dexter (bas?ton|battoon|batune) ?couped', 'baton-dexter' ),
            array( '(bas?ton|battoon|batune) dexter ?couped', 'baton-dexter' ),
            array( '(bas?ton|battoon|batune) ?sinister ?couped', 'baton' ),
            array( 'bordure', 'bordure' ),
            array( 'cantons?', 'canton' ),
            array( 'square cantons?', 'square-canton' ), // only meaningful for flags
            array( 'double trea?s?sure', 'double-tressure' ),
            array( 'fillet', 'fillet' ),
            array( 'fillet saltire', 'fillet-saltire' ),
            array( 'flau?nche?s?', 'flaunch' ),
            array( 'flau?nche? sinister', 'flaunch-sinister' ),
            array( 'sinister flau?nche?', 'flaunch-sinister' ),
            array( '(voider|flank|flanque|flasque)s?', 'flaunch' ),
            array( 'grafts?', 'graft' ),
            array( 'gussets?', 'gusset' ),
            array( 'gyrons?', 'gyron' ),
            array( '?based e?s?quie?r?res?', 'gyron' ),
            array( 'inescut?cheon', 'inescutcheon' ),
            array( 'orle', 'orle' ),
            array( 'pall', 'pall' ),
//            array( '(shake|hay)fork', 'shakefork' ),
            array( 'pale furche', 'shakefork' ),
            array( 'square flau?nche?s?', 'square-flaunch' ),
            array( 'square (flanks?|flanques?|flasques?)', 'square-flaunch' ),
            array( '?royal trea?s?sure', 'tressure' ),
            array( 'lozenge throughout', 'vetu' ),
            array( 'lozenge in point', 'vetu' ),
            array( 'vetu', 'vetu' ),
            array( 'ent[ey] ?[ie]n ?point', 'enty'),
            array( 'nordic cross', 'cross-nordic'),
            array( 'point ?in (point|pointed|enty)', 'point-in-point'),
            array( 'point dexter', 'point-dexter'),
            array( 'point sinister', 'point-sinister'),
          ),
        self::ORDINARY_PREFIX => array ( // can be before (and after) ordinary
            'sinister', 'reversed', 'dexter', 'inverted', 
            array ( '(hause|enhanced)', 'enhanced' ),     
            array ( '(abased|abaisse)', 'abased', ),
            array ( '(rompu|downset)', 'rompu' ),
            array ( 'removed', 'removed' ),
            array ( '(debruised|fracted)', 'fracted' ),
            array ( '(eclate|burst|split)', 'burst' ),
            array ( '(disjoint|disjointed|brisse)', 'disjointed' ),
            array ( '(debrui|depres|oppres)sed (with|by|of)', 'depressed' ),
            array ( 'false', 'voided' ),
          ),
        self::COLOUR_MODS => array (
           array ( '(compony|gobone|gobonated)', 'compony1' ),
           array ( 'counter (compony|gobone|gobonated)', 'compony2' ),
           array ( '(fimbriated|edged)', 'fimbriated'  ),
           ),
        self::LINETYPE_PREFIX => array (
           array ( 'the (dexter|first|1st|upper|higher) ?(one|1)', 'barmod-up' ),
           array ( 'the (sinister|last|lower|lowest) ?(one|1)','barmod-down' ),  
           array ( 'the (inner|inside) ?ones?',  'barmod-in' ),
           array ( 'the (outer|outside) ?ones?', 'barmod-out' ),
           ),  
        self::LINETYPE_POSTFIX => array (
          array ( '(at|on) ?the top', 'top-only' ),
          array ( '(at|on) ?the bottom', 'bottom-only' ),
           ),
        self::ORDINARY_MODS => array ( // only come after ordinary
          'closetted', 'embowed',
           array ( '?nowy quadrate', 'quadrate' ),
           array ( 'nowy lozengy', 'nowy-lozengy' ),
           array ( 'rivett?ed', 'riveted'),
           array ( '[ie]narched', 'inarched' ),
           array ( '(couped|humetty|alese)', 'couped' ),
           array ( '(couped|humetty|alese) ?in sinister', 'couped-sinister' ),
           array ( '(couped|humetty|alese) ?in dexter', 'couped-dexter' ),
           array ( '(couped|humetty|alese) ?in chief', 'couped-chief' ),
           array ( '(couped|humetty|alese) ?in base', 'couped-base' ), 
           array ( 'pointed', 'pointed' ),                   
           array ( 'fl(o|eu)ry counter fl(o|eu)ry', 'flory-counter-flory' ),
           array ( 'pointed ?in sinister', 'pointed-sinister' ),
           array ( 'pointed ?in dexter', 'pointed-dexter' ),
           array ( 'pointed ?in chief', 'pointed-chief' ),
           array ( 'pointed ?in base', 'pointed-base' ), 
           array ( '(bra[zc]ed|interlaced)', 'braced' ),
           array ( '(gemm?ell?(s|ed)?)', 'gemel' ),
           array ( 'double dancetty', 'fracted' ),
           array ( 'bott?onn?(ee|y)', 'botonny' ), 
           array ( 'engouled ?(into|by) ?dragons? ?heads?', 'engouled'),
           array ( 'of # points', 'points' ), // only if label is a ordinary
           // array ( '(de)?bruised|fracted|disjoint ?at ?the ?top', 'fracted' ),
           array ( 'en(aluron|urney|toire|toyre)', 'entire' ),
           'lozengy', 'triangular', 'throughout', 'of pretence', 'voided', 'ploye',
           ),
        self::COTTICE_MODS => array ( 
          array ( 'cott?i(s|ss|c)ed', '1'),
           array ( 'close coupled', '1'),
           array ( '(endorsed|addorsed|cost)', '1',  ),
           array ( 'doubl[ey] cott?i(s|ss|c)ed', '2',  ),
           array ( '(triple|treble|triply) cott?i(s|ss|c)ed', '3',  ),
           ),
        self::COTTICE => array ( array ( 'cott?i(s|ss|c)es?', 'cottice') ),
        self::VOIDED => array ( 'voided' ),
        self::LINETYPE => array ( 
          'indented', 'nebuly', 'nowy', 'plain', 'potenty',
          'raguly', 'dovetailed', 'engrailed', 'escartelly', 'erable',
          'thorny', 'ristikoro', 'hakulikoro', 'vallikoro',
          'liljakoro',
          array ( 'meander(ed|ing)?', 'meander' ), 
          array ( 'angled ?lower', 'angled' ),
          array ( 'angled (opposite|upper)', 'angled-upper' ),
          array ('(em)?battled ghibell?ine', 'battled-ghibelline' ),
          array ('(em)?battled ?in ?the ?form ?of mine dumps', 'battled-mine-dumps' ),
          array ( '(battled|double) (e?m?battled|crenn?ell?e?y?)', 'battled-embattled' ),  // two levels of battlements, on (opposite|upper) surface only
          array ( '(em)?battled grady', 'battled-embattled'  ),
          array ( '(em)?battled brett?ess?e?y?', 'battled-brettesse'  ),                // both sides, same pattern
          array ( '(e?m?battled|crenn?ell?e?y?) counter (e?m?battled|crenn?ell?e?y?)', 'battled-counter'  ), // both sides, opposite pattern
          array ( '?super (e?m?battled|crenn?ell?e?y?)','embattled'  ),        // top only
          array ( '(e?m?battled|crenn?ell?e?y?) arrondi', 'embattled-arrondi' ),           // top only, round crenelles
          array ( 'invec[kt]ed', 'invected' ),
          array ( '(en)?arched', 'arched' ),
          array ( 'bevill?ed (opposite|upper)', 'bevilled-upper' ),
          array ( 'bevill?y (opposite|upper)', 'bevilled-upper' ),
          array ( 'bevill?ed ?lower', 'bevilled' ),
          array ( 'bevill?y ?lower', 'bevilled' ),
          array ( 'dau?ncett(e|y) fl(o|eu)r(y|etty)', 'dancetty-floretty' ),
          array ( 'dau?ncett(e|y) ?the peaks ?ensigned ?with ?a fleur-de-lys', 'dancetty-floretty' ),
          array ( 'dau?ncetty', 'dancetty' ),
          array ( 'double (en)?arched', 'double-arched' ),
          array ( '(en)?arched double', 'double-arched' ),
          array ( '(rayonated|radiant)', 'rayonny' ),
          array ( 'rayonn(e|y)', 'rayonny' ),
          array ( '(wavy|undy|serpentine)', 'wavy' ),
          array ( '(urdy|urde|champaine|champion)','urdy' ),
          array ( '?fir twiggy?e?d?', 'twigged' ),
          array ( 'sapinagy', 'twigged' ),
          array ( '?(embattled) cupolae?d', 'cupolaed' ),
          array ( '?(embattled) gable?d?y?', 'gabled' ),
          array ( 'indented pommette?y?', 'indented-pommetty' ),
          array ( 'fir tree topped', 'fir-tree-topped' ),
          array ( 'trefl[ey] counter trefl[ey]', 'trefle'),
          ),
      self::ORDINARY_OR_CHARGE => array ( // can distinguish by number (perhaps!)
          array( '?(plain|holy) cross(es)? ?passant', 'cross/plain-cross' ),
          array( 'chevrons?', 'geometric/chevron' ),
          array( '(shake|hay)forks?', 'geometric/shakefork' ),
          array( 'chevron(el)?s? couched', 'geometric/chevron-couched' ),
        array( 'cross (formee|patt?ee?)', 'quadrate/cross-formy' ),
        array( 'cross (formy|patt?y)', 'quadrate/cross-formy' ),
        array( 'frets?', 'geometric/fret' ),
          array( 'salta?ires?', 'cross/saltire' ),
        ),
      self::ORDINARY_NOT_CHARGE => array ( // ordinaries that can be confused with charges
          array( 'salta?ire ?double parted ?and fretty', 'saltire-2fretty' ), 
          array( 'cross ?double parted ?and fretty', 'cross-2fretty' ), 
          array( 'cross tripartite ?and fretty', 'cross-3fretty' ), 
          array( 'cross quarter pierced?', 'cross-pierced' ),
          array( 'cross quarterpierced?', 'cross-pierced' ),
          // array( 'cross formy throughout', 'cross-formy' ),
          array( 'fillet cross', 'fillet-cross' ),
          array( 'cross fillet', 'fillet-cross' ), 
          array( 'frets? couped', 'fret-couped' ), 
        ),
      self::ARRANGEMENT => array (
          array ( 'in ann?ull?o',               'inannulo',        ),
          array ( 'in ?the pale',               'inpale',        ),
          array ( 'in ?the pale throughout',    'inpalethrough', ),
          array ( 'in ?the pall' ,              'inpall',        ),
          array ( 'in ?the fesse? !point' ,     'infess',        ),
          array ( 'in ?the fesse? throughout' , 'infessthrough', ),
          array ( 'in ?the bend sinister' ,     'inbendsin',     ),
          array ( 'in ?the bend ?dexter' ,      'inbend',        ),
          array ( 'in ?the chevron' ,           'inchevron',     ),
          array ( 'in ?the pile'  ,             'inpile',        ),
          array ( 'in ?an orle'  ,              'inorle',        ),
          array ( 'in ?the orle'   ,            'inorle',        ),
          array ( 'in ?the salti(er|re)' ,           'insaltire',     ),
          array ( 'salti(er|re)w(ays|ise)' ,           'insaltire',     ),
          array ( 'salti(er|re) w(ays|ise)' ,           'insaltire',     ),
          array ( 'in ?the cross'  ,            'incross',       ),
          array ( '?in counter passant' ,     'counter-passant'  ),
          array ( 'pilewise'      ,             'pilewise',      ),
          array ( 'in bar'  ,                   'inbar',         ),
          array ( 'bar(wise|ways)'  ,           'inbar',         ),
          array ( 'in ?the chief throughout' ,  'inchiefthrough',),
          array ( 'in ?a quadrangle'   ,        'quadrangle',    ),
          array ( 'chevronwise'   ,        'chevronwise',    ),
          array ( 'in point'   ,        'pilewise',    ),
          array ( 'in triangle'   ,        'triangle',    ),
          array ( 'respecting each other',  'facing-in'  ),
          array ( 'respectant',  'facing-in'  ),
          array ( 'counter rampant',  'facing-in'  ),
          array ( '(affrontt?ant|combatt?ant|aspectant)',  'facing-in'  ),
          array ( 'confront(e|ing|y)',  'facing-in'  ),
          array ( '(addorsed|adosse|endorsed)',  'facing-out' ),
        ),
      self::SPECIFIER => array (
          array ( '(first|1st)', 'first' ),
          array ( '(second|2nd)', 'second' ),
          array ( '(third|3rd)', 'third' ),
          array ( '(fourth|4th)', 'fourth' ),
          array ( '?each dexter', 'dexter' ),
          array ( '?each sinister', 'sinister' ),
          array ( '(each|every|all|both) ?of ?the', 'each' ),
          array ( 'middle', 'middle' ),
          array ( 'cent(re|er)', 'middle' ),
          array ( 'upper', 'upper' ),
          array ( 'lower', 'lower' ),
          ),
      self::LOCATION => array (
         array ( 'chief ?point', 'chief' ),
         array ( 'base ?point', 'base' ),
         array ( 'honou?r point', 'honpoint' ),
         array ( 'fesse? point', 'fesspoint' ),
         array ( 'navel point', 'nombril' ),
         array ( 'nombril', 'nombril' ),
         array ( '(flanks?|flaunche?s?)', 'flank' ),
         array ( '(abisme|abyss)', 'abyss' ),
         array ( '(side|half)', 'side' ),
          ),
        self::QUANTIFIER => array (
          array ( 'quarters?', 'quarter' ),
          array ( '(half|section|part)', 'all' ),
          ),
        self::CHARGE_ADJACENT => array (
          array ( '(between|betwixt|inter) ?them', 'between' ),
        //  array ( 'within ?(them|it)', 'within' ),
          array ( 'above ?(them|it)',  'above' ),
          array ( 'below ?(them|it)',  'below' ),  
          array ( 'with ?(them|it)',   'below' ),
          ),
        self::SHOWN_EXTRA => array (
          array ( 'to ?the dexter', 'to-dexter'),
          array ( 'to ?the sinister', 'to-sinister'),
          array ( 'likewise', 'likewise'),
          array ( '?in the same (way|fashion|manner)', 'likewise'),
          ),
        self::SHOWN_MODS => array (
          array ( '(shown|shewn|moved) ?somewhat', 'shown'),
          array ( 'somewhat', 'shown'),
          array ( '?(from|to|of|than) ?(the|is) (normal|usual) ?(position|location|place)s?', 'than-normal'),
          array ( 'very much', 'very-much'),
          array ( 'a great deal', 'very-much'),
          array ( 'much', 'much'),
          array ( 'a lot', 'much'),
          array ( 'a little', 'slightly'),
          array ( 'slightly', 'slightly'),
          array ( '(larger|bigger)', 'larger' ),
          array ( 'smaller', 'smaller' ),
          array ( '(above|raised|lifted|higher)', 'raised'),
          array ( '(below|lowere?d?|dropped)', 'lowered'),
          array ( 'further apart', 'further-apart'),
          array ( 'closer? together', 'closer-together'),
          ),
        self::DRAWN_MODS => array (
          array ( 'drawn', 'drawn' ),
          array ( '(with|in|using) ?(a|an|the) * (palette|tinctures|colou?rs)', 'scheme'),
          array ( '(with|in|using) ?(a|an|the) * shape', 'shape'),
          array ( '(with|in|using) ?(a|an|the) * (appearance|style|effect)', 'style'),
          array ( '(with|in|using) ?(a|an|the) aspect ?ratio ?of *', 'aspect'),
          array ( '(with|in|using) ?(a|an|the) * aspect ?ratio', 'aspect'),
          array ( '(with|in|using) ?(a|an|the) proportions? ?of *', 'aspect'),
          array ( '(with|in|using) ?(a|an|the) * proportions?', 'aspect')
      ),
        self::QUARTERMODS => array (
          array ( 'per cross', 'per-cross'),
          array ( 'per saltire', 'per-saltire'),
          array ( 'per saltire', 'per-saltire'),
        ),
        self::FONT => array ( 
          array ( 'in (a|the) * (font|face|style)', 'font' ),
          ),
      );
      include "charge.inc";
      include "chargemod.inc";
      if ($options['shape'] == 'flag') {
        include "flag-colours.inc";
        $this->patterns[self::COLOUR] = array_merge($this->patterns[self::COLOUR],$flagColours);

      }
    }
  }
