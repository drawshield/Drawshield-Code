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

require_once(__dir__ . "/../lexicon.inc");
require_once(__dir__ . "/../../svg/custom_charges.inc");


class languageDB extends lexicon
{
    // These constants acts as keys into the pattern database
    // Some are also used to indicate the type of modifier
    const OVERALL = 'overall';
    const BOT_COMMAND = 'bot_command';
    const METAOVERALL = 'metaoverall';
    const SPLITS = 'splits';
    const ONFIELDOF = 'onfieldof';
    const SEMICOLON = 'semicolon';
    const COLON = 'colon';
    const DOUBLESEMI = 'doublesemi';
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
    const COLOUR_MODS = 'colourmods';
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
    const HOLDING = 'holding';
    const ARRANGEMENT = 'arrangement';
    const SPECIFIER = 'specifier';
    const LOCATION = 'location';
    const QUANTIFIER = 'quantifier';
    const BUNDLE = 'bundle';
    const CHARGE_PREFIX = 'chargepre';
    const TRADITION_PREFIX = 'tradition-prefix';
    const TRADITION_DESCRIPTION = 'tradition-desc';
    const CHARGE_BETWEEN = 'chgbetween';
    const CHARGE_NEAR = 'chargenear';
    const CHARGE_FEATURES = 'chargefeat';
    const CHARGE_MODS = 'chargemod';
    const POSITION = 'position';
    const POINTS = 'points';
    // const SUPPLEMENTARY = 'supplementary';
    const SHOWN_EXTRA = 'shownextra';
    const SHOWN_MODS = 'shown';
    const DRAWN_MODS = 'drawn';
    const FONT = 'font';
    const LAYERING = "layers";
    const QUARTERMARK = 'quartermark';
    const QUARTERMODS = 'quartermods';
    const QUARTERS_WORDS = 'quarters_word';
    const QUARTERS_ORDINAL = 'quarters_ordinal';
    const QUARTERS_ROMAN = 'quarters_roman';
    const LETTERS = 'letters';
    const INVERSE = 'inverse';
    const MANTLING = 'mantling';
    const MANTLING_MODS = 'mantling_mods';
    const MANTLES = 'mantles';
    const COMPARTMENT = 'compartment';
    const COMP_TYPE = 'comptype';
    const SUPPORTERS = 'supporters';
    const ACHIEVEMENT_LOC = 'achievement_loc';
    const MOTTO = 'motto';
    const MOTTO_POSITION = 'motto_pos';
    const MOTTO_TYPE = 'motto_type';
    const MOTTO_TEXT = 'motto_tet';
    const CREST = 'crest';
    const TORSE = 'torse';
    const HELMET = 'helmet';
    const ACHIEVEMENT = 'achievement';
    const ORNAMENT = 'ornament';
    const ECCLESIASTIC = 'ecclesiastic';
    const EACH = 'each';
    const BLAZONED = 'blazoned';
    const VALUE = 'value';
    const TARTAN = 'tartan';
    const WEBCOLS = 'webcols';
    const WHCOLS = 'whcols';
    const TARTANCOLS = 'tartancols';
    const HERALDIC = 'heraldic';
    const USING = 'using';
    const FRINGED = 'fringed';
    const SHIELD = 'shield';
    const CHGLIST = 'chglist';
    const LINING = 'lining';
    const TRADITION = 'tradition';
    const SIGNIFIER = 'signifier';
    const ALONGSIDE = 'alongside';
 #   const DISPLACED = 'displaced';

    public function __construct()
    {
        $this->language = 'english';

        // Each top level element of the patterns array is a named sub-array
        // Each named sub-array represents a particular word category
        // The sub arrays contain a list of mappings between regular expressions
        // and a "key term". (If there is no key term given, the regex itself
        // is used). The grammar requests a search for a particular word
        // category, and if found, the specific key term is returned.

        $this->patterns = array(
            self::BOT_COMMAND => array ( '!draw', '!test', '!todo' ),
            self::CHGLIST => array ("and also", "& also", "along with"),
            self::VALUE => array("*"),
            self::WORD_THE => array('the'),
            self::SIGNIFIER => array('the', 'a', 'an'),
            self::OVERALL => array('overall', 'sur le tout', 'all with(in)?', "over all", "in pretence"),
            self::METAOVERALL => array(
                array ('over(all)? ?all ?the ?# (quarters|quarterings|halves|parts|sections)', 'metaoverall'),
                array ('o(n|ver) the whole', 'metaoverall')
            ),
      /*      self::DISPLACED => array (
                'alternate', 'displaced',
            ), */
            self::ALONGSIDE => array (
                array ("joined (with|to)", "joined"),
                array ("next to", "alongside"),
                array ("alongside", "alongside"),
            ),
            self::MANTLING => array(
                'mantling', 'lambrequin' ),
            self::MANTLES => array (
                array('ribbons', 'ribbons'),  // note default
                array('cloak', 'cloak'),
                array('lea(f|ves)', 'leaves'),
                    array('royal', 'royal'),
                array('cloak ermine', 'cloak-ermine'),
                array('ermine cloak', 'cloak-ermine'),
                array('cloak counter ermine', 'cloak-counter-ermine'),
                array('counter ermine cloak', 'cloak-counter-ermine'),
                array('cloak pean', 'cloak-pean'),
                array('pean cloak', 'cloak-pean'),
                array('cloak erminois', 'cloak-erminois'),
                array('erminois cloak', 'cloak-erminois'),
                    array('grandee of spains?', 'grandee-of-spain'),
                    array('peer of frances?', 'peer-of-france'),
                    array('napole[oa]n(ic)?', 'napoleanic'),
                    array('princes? of the holy roman empire', 'prince-hre'),
                    array('holy roman empire princes?', 'prince-hre'),
                ),
            self::LINING => array( 'doubled', 'lined', 'and', '&'),
            self::FRINGED => array ('?a fringed?'),
            self::SUPPORTERS => array ('supporters?'),
            self::ACHIEVEMENT => array('achievement', 'hatchment' ),
            self::ORNAMENT => array('ornaments?'),
            self::CREST => array(
                array('?(a|the) crest ?of', 'crest'), // room for other crest types?
                array('?with no crest', 'no-crest'),
            ),
            self::TRADITION => array (
                array ('?roman catholic', 'catholic'),
                array ('anglican', 'anglican'),
                // more to add
            ),
            self::ECCLESIASTIC => array(
                array('cardinal archbishop', 'cardinal-archbishop'),
                array('archbishop cardinal ', 'cardinal-archbishop'),
                array('cardinal bishop', 'cardinal-bishop'),
                array('bishop cardinal ', 'cardinal-bishop'),
                array('priest', 'priest'),
                array('monsignor', 'monsignor'),
                array('monsignor supernumerary', 'monsignor-supernumerary'),
                array('canon', 'canon'),
                array('bishop', 'bishop'),
                array('archbishop', 'archbishop'),
                array('abbot', 'abbot'),
                array('territorial abbot', 'territorial-abbot'),
                array('chaplain ?of ?his ?holiness', 'chaplain'),
                array('prelate ?of ?his ?holiness', 'prelate'),
                array('(vicar|dean)', 'vicar'),
                array('(patriarch|primate)', 'patriarch'),
            ),
            self::MANTLING_MODS => array(
                array ('crowne?d?', 'crowned'),
                array ('pavill?ione?d?', 'pavilion'),
            ),
            self::TORSE => array(
                array('?(a|the) (torse|bourlet)', 'torse'),
            ),
            self::COMPARTMENT => array(
                array('?(a|the) compartment ?of ?(a|the)', 'compartment'),
            ),
            self::COMP_TYPE => array(
                array('shelf', 'shelf'),
                array('moun(t|d)', 'mound'),
                array('rocks?', 'rocks'),
                array('stones?', 'rocks'),
                array('pedestal', 'pedestal'),
                array('walls?', 'wall'),
                array('bricks?', 'wall'),
                array('castle', 'castle'),
            ),
            self::SHIELD => array(
                array('?the (shield|escutcheon)', 'shield'),
                array('?the (interior|contents?)', 'interior'),
            ),
            self::ACHIEVEMENT_LOC => array(
                array('?to ?the sinister', 'sinister'),
                array('?to ?the dexter', 'dexter'),
                array('?(in|of|to) ?the (middle|centre|center|rear)', 'centre'),
                array('behind', 'centre'),
            ),
            self::HELMET => array(
                array('?(the|a|an) royal helm(et)', 'royal'),
                array('?(the|a|an) monarchs? helm(et)', 'royal'),
                array('?(the|a|an) (knight|baronet)s? helm(et)', 'baronet'),
                array('?(the|a|an) ?(esquire|gentlem[ea]n)s? helm(et)', 'esquire'),  // DEFAULT
                array('?(the|a|an) (peer|lord)s? helm(et)', 'peer'),
            ),
            self::MOTTO => array (
                array('(motto|devi[sc]e)', 'motto'),
            ),
            self::MOTTO_TEXT => array (
                array ('*', self::MOTTO_TEXT),
            ),
            self::MOTTO_POSITION => array (
                array ('above', 'above'),
                array ('(at|on) ?the top', 'above'),
                array ('below', 'below'),
                array ('(at|on) ?the bottom', 'below'),
            ),
            self::MOTTO_TYPE => array (
              array ('ribbons?', 'ribbon' ),
                array ('scrolls?', 'scroll' ),
            ),
            self::BLAZONED => array('(em)blazoned ?(with|of)', 'with ?(a|the) blazon ?of'),
            self::SPLITS => array(
                array('impaled with', 'impaled'),
                array('impaling', 'impaled'),
                array('dimidiated with', 'dimidiated'),
            ),
            self::ONFIELDOF => array(
                array('?(on|upon) a court ?of', 'court'),
                array('?(on|upon) a field ?of', 'field'),
                array('?the field', 'field'),
            ),
            self::LAYERING => array('over', 'under'), // recognised, not implemented
            self::WITH => array(
                array('(with|upon)', 'with'),
                array ('bearing', 'with')
            ),
            self::BETWEEN => array('between', 'inter', 'inside', 'within', 'surrounded by', 'accompagne', 'cantonned by'),
            self::QUARTERED => array(
                array('quartered', 'quartered'), // Always a marshalling
                array('grandquartered', 'quartered'), // Always a marshalling
                array('quarter(ed|ly) per saltire', 'quartered-per-saltire'), // Always a marshalling
                array('(grand)?quarterly', 'quarterly'), // Could be a marshalling or a division
                array('?part(y|ed) per fesse?', 'per-fess',),
                array('tierced? (in|per) fesse?', 'per-fess',),
                array('parted fesse?wise', 'per-fess',),
                array('?part(y|ed) per pale', 'per-pale',),
                array('parted palewise', 'per-pale',),
                array('tierced? (in|per) pale', 'per-pale',),
            ),
            self::QUARTERMARK => array('\\)'),
            self::LETTERS => array(
                array('a', 1),
                array('b', 2),
                array('c', 3),
                array('d', 4),
                array('e', 5),
                array('f', 6),
                array('g', 7),
                array('h', 8),
                array('i', 9),
                array('j', 10),
                array('k', 11),
                array('l', 12),
                array('m', 13),
                array('n', 14),
                array('o', 15),
                array('p', 16),
                array('q', 17),
                array('r', 18),
                array('s', 19),
                array('t', 20),
                array('u', 21),
                array('v', 22),
                array('w', 23),
                array('x', 24),
                array('y', 25),
                array('z', 26),
                array('aa', 27),
                array('ab', 28),
                array('ac', 29),
                array('ad', 30),
                array('ae', 31),
                array('af', 32),
                array('ag', 33),
                array('ah', 34),
                array('ai', 35),
                array('aj', 36),
                array('ak', 37),
                array('al', 38),
                array('am', 39),
                array('an', 40),
                array('ao', 41),
                array('ap', 42),
                array('aq', 43),
                array('ar', 44),
                array('as', 45),
                array('at', 46),
                array('au', 47),
                array('av', 48),
                array('aw', 49),
                array('al', 50),
                array('ay', 51),
                array('az', 52),
                array('ba', 53),
                array('bb', 54),
                array('bc', 55),
                array('bd', 56),
                array('be', 57),
                array('bf', 58),
                array('bg', 59),
                array('bh', 60),
                array('bi', 61),
                array('bj', 62),
                array('bk', 63),
                array('bl', 64),
            ),
           self::QUARTERS_WORDS => array(
                array('first', 1),
                array('second', 2),
                array('third', 3),
                array('fourth', 4),
                array('fifth', 5),
                array('sixth', 6),
                array('seventh', 7),
                array('eighth', 8),
                array('ninth', 9),
                array('tenth', 10),
                array('eleventh', 11),
                array('twelth', 12),
                array('thirteenth', 13),
                array('fourteenth', 14),
                array('fifteenth', 15),
                array('sixtenth', 16),
                array('seventeenth', 17),
                array('eigtheenth', 18),
                array('nineteenth', 19),
                array('twentieth', 20),
                array('twenty first', 21),
                array('twenty second', 22),
                array('twenty third', 23),
                array('twenty fourth', 24),
                array('twenty fifth', 25),
                array('twenty sixth', 26),
                array('twenty seventh', 27),
                array('twenty eigth', 28),
                array('twenty ninth', 29),
                array('thirtieth', 30),
                array('thirty first', 31),
                array('thirty second', 32),
                array('thirty third', 33),
                array('thirty fourth', 34),
                array('thirty fifth', 35),
                array('thirty sixth', 36),
                array('thirty seventh', 37),
                array('thirty eigth', 38),
                array('thirty ninth', 39),
                array('fortieth', 40),
                array('forty first', 41),
                array('forty second', 42),
                array('forty third', 43),
                array('forty fourth', 44),
                array('forty fifth', 45),
                array('forty sixth', 46),
                array('forty seventh', 47),
                array('forty eigth', 48),
                array('forty ninth', 49),
                array('fiftieth', 50),
                array('fifty first', 51),
                array('fifty second', 52),
                array('fifty third', 53),
                array('fifty fourth', 54),
                array('fifty fifth', 55),
                array('fifty sixth', 56),
                array('fifty seventh', 57),
                array('fifty eigth', 58),
                array('fifty ninth', 59),
                array('sixtieth', 60),
                array('sixty first', 61),
                array('sixty second', 62),
                array('sixty third', 63),
                array('sixty fourth', 64),
            ),
            self::QUARTERS_ORDINAL => array(
                array('1st', 1),
                array('2nd', 2),
                array('3rd', 3),
                array('4th', 4),
                array('5th', 5),
                array('6th', 6),
                array('7th', 7),
                array('8th', 8),
                array('9th', 9),
                array('10th', 10),
                array('11th', 11),
                array('12th', 12),
                array('13th', 13),
                array('14th', 14),
                array('15th', 15),
                array('16th', 16),
                array('17th', 17),
                array('18th', 18),
                array('19th', 19),
                array('20th', 20),
                array('21st', 21),
                array('22nd', 22),
                array('23rd', 23),
                array('24th', 24),
                array('25th', 25),
                array('26th', 26),
                array('27th', 27),
                array('28th', 28),
                array('29th', 29),
                array('30th', 30),
                array('31st', 31),
                array('32nd', 32),
                array('33rd', 33),
                array('34th', 34),
                array('35th', 35),
                array('36th', 36),
                array('37th', 37),
                array('38th', 38),
                array('39th', 39),
                array('40th', 40),
                array('41st', 41),
                array('41st', 41),
                array('42nd', 42),
                array('43rd', 43),
                array('44th', 44),
                array('45th', 45),
                array('46th', 46),
                array('47th', 47),
                array('48th', 48),
                array('49th', 49),
                array('50th', 50),
                array('51st', 51),
                array('52nd', 52),
                array('53rd', 53),
                array('54th', 54),
                array('55th', 55),
                array('56th', 56),
                array('57th', 57),
                array('58th', 58),
                array('59th', 59),
                array('60th', 60),
                array('61st', 61),
                array('62nd', 62),
                array('63rd', 63),
                array('64th', 64),
            ),
            self::QUARTERS_ROMAN => array(
                array('i', 1),
                array('ii', 2),
                array('iii', 3),
                array('(iv|iiii)', 4),
                array('v', 5),
                array('vi', 6),
                array('vii', 7),
                array('viii', 8),
                array('ix', 9),
                array('x', 10),
                array('xi', 11),
                array('xii', 12),
                array('xiii', 13),
                array('xiv', 14),
                array('xv', 15),
                array('xvi', 16),
                array('xvii', 17),
                array('xviii', 18),
                array('xix', 19),
                array('xx', 20),
                array('xxi', 21),
                array('xxii', 22),
                array('xxiii', 23),
                array('(xxiv|xxiiii)', 24),
                array('xxv', 25),
                array('xxvi', 26),
                array('xxvii', 27),
                array('xxviii', 28),
                array('xxix', 29),
                array('xxx', 30),
                array('xxxi', 31),
                array('xxxii', 32),
                array('xxxiii', 33),
                array('(xxxiv|xxxiiii)', 34),
                array('xxxv', 35),
                array('xxxvi', 36),
                array('xxxvii', 37),
                array('xxxviii', 38),
                array('xxxix', 39),
                array('xl', 40),
                array('xli', 41),
                array('xlii', 42),
                array('xliii', 43),
                array('(xliv|xliiii)', 44),
                array('xlv', 45),
                array('xlvi', 46),
                array('xlvii', 47),
                array('xlviii', 48),
                array('xlix', 49),
                array('l', 50),
                array('li', 51),
                array('lii', 52),
                array('liii', 53),
                array('(liv|liiii)', 54),
                array('lv', 55),
                array('lvi', 56),
                array('lvii', 57),
                array('lviii', 58),
                array('lix', 59),
                array('lx', 60),
                array('lxi', 61),
                array('lxii', 62),
                array('lxiii', 63),
                array('(lxiv|lxiiii)', 64),
            ),
            self::COLON => array (':'),
            self::SEMICOLON => array(':', ';', '\\.'), // need to escape '.' as this is regex special character
            self::DOUBLESEMI => array(';;'),
            self::COMMA => array(',', '\\.'), // allow periods in more places
            self::WORD_OF => array('of'),
            self::WORD_ON => array('on'),
            self::WORD_IN => array('in ?the'),
            self::POINTS => array('points', 'rows'),
            self::ANDD => array(
                array('(and|&|also)', 'and'),
                array('(and|&) also', 'and'),
            ),
            self::HERALDIC => array (
                array ('heraldic', 'heraldic'),
                array ('herald(s|dry)?', 'heraldic'),
            ),
            self::COLOUR => array(
                // basic tinctures (with abbreviations)
                array('or', 'or'),
                array('az(ure)?', 'azure'),
                array('vert|sinople', 'vert'),
                array('gui?(les)?', 'gules'),
                array('arg(ent)?', 'argent'),
                array('sa(ble)?', 'sable'),
                array('purp(le|ure)', 'purpure'),
                // additional tinctures
                'murrey', 'sanguine', 'carnation', 'brunatre', 'rose',
                'buff',
                array('cei?ndree', 'cendree'),
                array('bis(que)?', 'bis'),
                array('celestial azure', 'celestial-azure'),
                array('bleu celeste', 'celestial-azure'),
                array('horizon blue', 'celestial-azure'),
                array('red ochre', 'red-ochre'),
                array('yellow ochre', 'yellow-ochre'),
                array('senois', 'senois'),
                array('(tanned|tenne|tawny)', 'tenne'),
                'orange',
                // "de" names (for gouttes particularly)
                array('d or', 'or'),
                array('dor', 'or'),
                array('d eau', 'argent'),
                array('deau', 'argent'),
                array('de larmes', 'azure'),
                array('de sangu?e?', 'gules'),
                array('de vin', 'purpure'),
                array('de poix', 'sable'),
                array('d huile', 'vert'),
                array('dhuile', 'vert'),
                array('d olive', 'vert'),
                array('dolive', 'vert'),
                // obsolete names
                /* Start Comment obsolete names
                 'amethyst', 'diamond', 'dragons head', 'dragons tail', 'emerald',
                'jacinth', 'luna', 'mars', 'pearl', 'ruby', 'sol', 'topaz', 'venus',
                End comment here */
                // English names
                array('iron', 'iron'),
                array('bronze', 'bronze'),
                array('copper', 'copper'),
                array('crimson', 'crimson'),
                array('lead', 'lead'),
                array('steel ^legion', 'steel'),
                array('gold', 'or'),
                array('blue', 'azure'),
                array('green', 'vert'),
                array('red', 'gules'),
                array('silver', 'argent'),
                array('black', 'sable'),
                array('brown', 'brunatre'),
                array('gr[ae]y', 'gray'),
                array('pink', 'rose'),
                array('white', 'white'),
            ),
            self::BACKREF => array(
                array('(like|of|as) the same', 'ofsame'),
                array('(like|of|as) the last', 'oflast'),
                array('(like|of|as) the field', 'offield'),
                array('(like|of|as) the (first|1st)', 'offirst'),
                array('(like|of|as) the (second|2nd)', 'ofsecond'),
                array('(like|of|as) the (third|3rd)', 'ofthird'),
                array('(like|of|as) the (fourth|4th)', 'offourth'),
            ),
            self::TARTAN => array(
                array('tartan', 'tartan',),
                array('tartan no repeat', 'tartan-no-repeat',),
            ),
            self::TREATMENT => array(
                array('glen check[sy]?', 'glen-check',),
                array('prince of wales check[sy]?', 'glen-check',),
                array('hounds tooth', 'houndstooth',),
                array('houndstooth', 'houndstooth',),
                array('gingham', 'gingham',),
                array('rosace', 'rosace',),
                array('shepherds? check[sy]?', 'shepherd-check',),
                array('buffalo check[sy]?', 'buffalo-check',),
                array('graph check[sy]?', 'graph-check',),
                array('ann?ulett?y', 'annuletty',),
                array('rou?ndell?y', 'roundelly',),
                array('bezant(y|e)', 'bezanty', true),
                array('billett?y', 'billetty',),
                array('(checky|cheque?y|checkered)', 'checky'),
                array('?billett?y counter billett?y', 'counter-billetty',),
                array('crusill?[ye]', 'crusilly',),
                array('estoill?[ye]', 'estoilly',),
                array('fretty', 'fretty',),
                array('(treille|latticed)', 'latticed',), // TODO draw this, see boutell
                array('plumetty', 'plumetty',),
                array('go?utty?e?e?', 'goutty',),
                array('grillage', 'grillage',),
                array('honeycombed', 'honeycombed',),
                array('ermined', 'ermined',),
                array('estencell?y', 'estencely',),
                array('hurty', 'hurty', true),
                array('(lozengy|masculy)', 'lozengy',),
                array('maily', 'maily',),
                array('(masonn?ed?|muraille)', 'masoned',),
                array('mullett?y', 'mulletty',),
                array('pap(e|i)l?lonn?(y|e)', 'papelonny',),
                array('platy', 'platy', true),
                array('scaly', 'scaly',),
                array('escaille', 'scaly',),
                array('sem(ee?|y) de l(i|y)s', 'seme-de-lys',),
                array('fl(o|eu)ry', 'seme-de-lys',),
                array('vair(e|y)', 'vairy',),
                array('potent(e|y)', 'potenty',),
                array('ai?[qg]ui?lated', 'aquilated'), // TODO draw this
                array('ferrated', 'ferrated'), // TODO and this
                //array ( 'trefle', 'trefle' ), // TODO and this
                array('verd(e|oy)', 'verdy'), // TODO and this
                array('barruly wavy', 'barruly-wavy'), // TODO and this
                array('pied ?at ?random', 'pied'),
                array('piebald', 'pied'),
                array('grad(ed|y)', 'grady'),
                array('grad(ed|y) in fesse?', 'grady-in-fess'),
                array('grad(ed|y) in pale', 'grady-in-pale'),
                array('grad(ed|y) in bend', 'grady-in-bend'),
                array('grad(ed|y) in bend sinister', 'grady-in-bendsin'),
                array('grad(ed|y) in sinister bend', 'grady-in-bendsin'),
                array('grad(ed|y) in annull?o', 'grady-in-annulo'),
                // Following DO NOT HAVE HERALDIC PRECEDENT (but are useful anyway)
                array('guzy', 'guzy', true),
                array('pommy', 'pommy', true),
                array('torty', 'torty', true),
            ),
            self::INVERSE => array('reversed', 'inverted',
                array('contourny', 'reversed',),),
            self::FUR => array(
                'ermine', 'erminois', 'erminites', 'pean', 'potent', 'vair',
                //array ( 'or ermined sa(ble)?', 'erminois',           ),
                // array ( 'sa(ble)? ermined or', 'pean',            ),
                array('vair [ei]n pale', 'vair-in-pale',),
                array('vair [ei]n point', 'vair-en-point',),
                array('ermines', 'counter-ermine',),
                array('counter ermine', 'counter-ermine',),
                // array ( 'sa(ble)? ermined arg(ent)?', 'counter-ermine',  ),
                array('counter vair', 'counter-vair',),
                array('vair ancient?', 'vair-ancient',),
                array('counter potent', 'counter-potent',),
                array('potent counter potent', 'potent-counter-potent',),
                array('tapiss?ee? ?of ?wheat', 'tapisse',),
                // Sports pitches
                array('soccer', 'soccer-field'),
                array('?american football', 'american-football-field'),
                array('tennis', 'tennis-court'),
                array('basketball', 'basket-ball'),
                array('basket ball', 'basket-ball'),
            ),
            self::SEMY => array('(semee?|semy) (de|with|of)', '(aspersed|strewn) with', '(poudre|powdered) with'),
            self::GROUP => array('(both|all|the|his|her|its)',),
            self::PROPER => array('(proper|ppr)'),
            self::COUNTERCHANGE => array('interchanged?', 'transmuted', 'counter cha[nr]ged?', 'countercha[nr]ged?', 'countercoloured', 'de l un en l autre'),
            self::DIVISION => array(
                array('pily', 'pily',),
                array('quarterly', 'quarterly',),
                array('?part(y|ed) per cross', 'quarterly',),
                array('quarterly ?, per fesse?', 'quarterly-per-fess',),
                array('quarterly ?, per pale', 'quarterly-per-pale',),
                array('paly bendy', 'paly-bendy',),
                array('bendy paly', 'paly-bendy',),
                array('pily bendy', 'pily-bendy',),
                array('bendy pily', 'pily-bendy',),
                array('b[ua]rr?(y|uly) ?sans ?nombre', 'barry',),
                array('paly', 'paly',),
                array('?per chape', 'chape',),
                array('?per chape ploye', 'chape-ploye',),
                array('chevronn?(ell)?y', 'chevronny',),
                array('?per chausse', 'chausse',),
                array('quarterly quartered', 'gyronny',),
                array('?part(y|ed) ?per g(i|y)ronny', 'gyronny',),
                array('bendy', 'bendy',),
                array('?part(y|ed) per fesse?', 'per-fess',),
                array('parted fesse?wise', 'per-fess',),
                array('?part(y|ed) per pale', 'per-pale',),
                array('?part(y|ed) per chief', 'per-chief',),
                array('parted palewise', 'per-pale',),
                array('?part(y|ed) per pile', 'per-pile',),
                array('parted pilewise', 'per-pile',),
                array('?part(y|ed) per pall', 'per-pall',),
                array('?part(y|ed) per pall arrondi', 'per-pall-arrondi',),
                array('tierced? (in|per) pairle', 'per-pall',),
                array('tierced? (in|per) pairle arrondi', 'per-pall-arrondi',),
                array('?part(y|ed) per chevron', 'per-chevron',),
                array('parted chevronwise', 'per-chevron',),
                array('?part(y|ed) per bend', 'per-bend',),
                array('parted bendwise', 'per-bend',),
                array('?part(y|ed) per saltire', 'per-saltire',),
                array('parted saltirewise', 'per-saltire',),
                array('tierced? (in|per) pale', 'tierced-in-pale',),
                array('tierced? (in|per) fesse?', 'tierced-in-fess',),
                array('tierced? (in|per) bend', 'tierced-in-bend',),
                array('tierced? (in|per) chevron', 'tierced-in-chevron',),
                array('fusill?y', 'fusily',),
                array('fusill?y bendy ?dexter', 'fusily-bendy',),
                array('fusill?y in bend', 'fusily-bendy',),
                // array ( '(lozengy|masculy)',     'lozengy',            ),
                array('barry bendy ?lozengy', 'barry-bendy',),
                array('bendy lozengy', 'barry-bendy',),
                array('barry pily', 'barry-pily',),
                array('barry indented ?the (one|1) in the other', 'barry-indented-oio',),
            ),
            self::DIVISION_MODS => array(
                array('of # ?pieces?', 'ofnum'),
                array('rivett?ed', 'riveted'),
                array('enhanced', 'enhanced'),
                array('abased', 'abased'),
            ),
            self::QUANTITY => array(
                array('(a|an|the|some)', "1"),
            ),
            self::NUMBER => array(
                '1', '2', '3', '4', '5', '6', '7', '8', '9',
                '10', '11', '12', '13', '14', '15', '16', '17', '18', '19',
                '20', '21', '22', '23', '24', '25', '26', '27', '28', '29',
                '30', '31', '32', '33', '34', '35', '36', '37', '38', '39',
                '40', '41', '42', '43', '44', '45', '46', '47', '48', '49',
                '50', '51', '52', '53', '54', '55', '56', '57', '58', '59',
                '60', '61', '62', '63', '64',
                array('one', "1"),
                array('two', "2"),
                array('three', "3"),
                array('four', "4"),
                array('five', "5"),
                array('six', "6"),
                array('seven', "7"),
                array('eight', "8"),
                array('nine', "9"),
                array('ten', "10"),
                array('eleven', "11"),
                array('twelve', "12"),
                array('thirteen', "13"),
                array('fourteen', "14"),
                array('fifteen', "15"),
                array('sixteen', "16"),
                array('seventeen', "17"),
                array('eighteen', "18"),
                array('nineteen', "19"),
                array('twenty', "20"),
                array('twenty one', "21"),
                array('twenty two', "22"),
                array('twenty three', "23"),
                array('twenty four', "24"),
                array('twenty five', "25"),
                array('twenty six', "26"),
                array('twenty seven', "27"),
                array('twenty eight', "28"),
                array('twenty nine', "29"),
                array('thirty', "30"),
                array('thirty one', "31"),
                array('thirty two', "32"),
                array('thirty three', "33"),
                array('thirty four', "34"),
                array('thirty five', "35"),
                array('thirty six', "36"),
                array('thirty seven', "37"),
                array('thirty eight', "38"),
                array('thirty nine', "39"),
                array('forty', "40"),
                array('forty one', "41"),
                array('forty two', "42"),
                array('forty three', "43"),
                array('forty four', "44"),
                array('forty five', "45"),
                array('forty six', "46"),
                array('forty seven', "47"),
                array('forty eight', "48"),
                array('forty nine', "49"),
                array('fifty', "50"),
                array('fifty one', "51"),
                array('fifty two', "52"),
                array('fifty three', "53"),
                array('fifty four', "54"),
                array('fifty five', "55"),
                array('fifty six', "56"),
                array('fifty seven', "57"),
                array('fifty eight', "58"),
                array('fifty nine', "59"),
                array('sixty', "60"),
                array('sixty one', "61"),
                array('sixty two', "62"),
                array('sixty three', "63"),
                array('sixty four', "64"),
                array('(as|so) many', 'asmany'),
                array('(6[5-9]|[7-9][0-9]|[1-9][0-9][0-9]+)', 'bignum'),
            ),
            self::ORIENTATION => array(
                'sinister', 'reversed', 'dexter', 'inverted'
            ),
            self::ORDINARY => array(
                'another', // special term, means "voided of" (should only be used after "on" or "charged with"
                array('bars?', 'bar'),
                array('barrulets?', 'barrulet'),
                array('crancelin', 'crancelin'),
                array('bend archy coronett[ey]', 'crancelin'),
                array('bendlets?', 'bendlet'),
                array('e?sca[rf]pe?s?', 'scarpe'),
                array('chevronels?', 'chevronel'),
                array('couple closes?', 'couple-close'),
                array('close couples?', 'couple-close'),
                array('closets?', 'closet'),
                array('(baston|riband)s?', 'riband'),
                array('[ie]ndorses?', 'endorse'),
                array('humets?', 'humet'),
                array('pall?ets?', 'palet'),
                array('piles? ?of ?(3|three) ?points ?issuing ?from ?the ?chief', 'pile'),
                array('mountains? ?of (3|three) (peaks|points)', 'mountain-of-3-peaks'),
                array('mountains?', 'mountain'),
                array('base', 'base'),
                array('bends?', 'bend'),
                array('chief', 'chief'),
                array('chief triangular', 'chief-triangular'),
                array('triangular chief ', 'chief-triangular'),
                array('fesse?', 'fess'),
                array('(grand|great) (staff|stave)', 'grand-stave'),
                array('fesse? between (2|two) chevrons?', 'fess-between-chevrons'),
                array('dance', 'dance'),
                array('ford', 'ford'),
                array('gores?', 'gore'),
                array('gore sinister', 'gore-sinister'),
                array('sinister gore', 'gore-sinister'),
                array('pallium', 'pallium'),
                array('pale', 'pale'),
                array('boss', 'boss'),
                // array ( '(file|lam?bel)', 'label'),
                array('point', 'point'), // TODO draw this - see meaning 2 parker/point
                array('plaine? point', 'base'), // (plain) point and base are synonyms
                array('ecclesiastical pale', 'pale-ecclesiastical'),
                array('canadian pale', 'pale-canadian'),
                array('quarter', 'quarter'),
                array('tierce', 'tierce'),
                array('(gorge|whirlpool|gurges)', 'gorge'),
                array('grid', 'grid'),
                array('trimount', 'trimount'),
                array('moun[td]', 'mount'),
                array('dexter (bas?ton|battoon|batune) ?couped', 'baton-dexter'),
                array('(bas?ton|battoon|batune) dexter ?couped', 'baton-dexter'),
                array('(bas?ton|battoon|batune) ?sinister ?couped', 'baton'),
                array('bordure', 'bordure'),
                array('cantons?', 'canton'),
                array('cantells?', 'cantell'),
                array('square cantons?', 'square-canton'), // only meaningful for flags
                array('double trea?s?sure', 'double-tressure'),
                array('fillet', 'fillet'),
                array('fillet saltire', 'fillet-saltire'),
                array('fillet bordure', 'fillet-bordure'),
                array('flau?nche?s?', 'flaunch'),
                array('flau?nche? sinister', 'flaunch-sinister'),
                array('sinister flau?nche?', 'flaunch-sinister'),
                array('(voider|flank|flanque|flasque)s?', 'flaunch'),
                array('grafts?', 'graft'),
                array('gussets?', 'gusset'),
                array('gyrons?', 'gyron'),
                array('based e?s?quie?r?res?', 'gyron'),
                array('inescut?cheon', 'inescutcheon'),
                array('orle', 'orle'),
                array('pall', 'pall'),
//            array( '(shake|hay)fork', 'shakefork' ),
                array('pale furche', 'shakefork'),
                array('square flau?nche?s?', 'square-flaunch'),
                array('square (flanks?|flanques?|flasques?)', 'square-flaunch'),
                array('?royal trea?s?sure', 'tressure'),
                array('lozenge throughout', 'vetu'),
                array('lozenge in point', 'vetu'),
                array('vetu', 'vetu'),
                array('ent[ey] ?[ie]n ?point', 'enty'),
                array('nordic cross', 'cross-nordic'),
                array('point ?in (point|pointed|enty)', 'point-in-point'),
                array('point dexter', 'point-dexter'),
                array('point sinister', 'point-sinister'),
            ),
            self::ORDINARY_PREFIX => array( // can be before (and after) ordinary
                'sinister', 'reversed', 'dexter', 'inverted',
                array('(hause|enhanced)', 'enhanced'),
                array('(abased|abaisse)', 'abased',),
                array('(rompu|downset)', 'rompu'),
                array('removed', 'removed'),
                array('(debruised|fracted)', 'fracted'),
                array('double (debruised|fracted)', 'double-fracted'),
                array('(eclate|burst|split)', 'burst'),
                array('(disjoint|disjointed|brisse)', 'disjointed'),
                array('(debrui|depres|oppres)sed (with|by|of)', 'depressed'),
                array('false', 'voided'),
                array('pointed', 'pointed'),
            ),
            self::COLOUR_MODS => array(
                array('(fimbri?ated|edged)', 'fimbriated'),
            ),
            self::LINETYPE_PREFIX => array(
                array('the (dexter|first|1st|upper|higher) ?(one|1)', 'barmod-up'),
                array('the (sinister|last|lower|lowest) ?(one|1)', 'barmod-down'),
                array('the (inner|inside) ?ones?', 'barmod-in'),
                array('the (outer|outside) ?ones?', 'barmod-out'),
            ),
            self::LINETYPE_POSTFIX => array(
                array('(at|on) ?the top', 'top-only'),
                array('(at|on) ?the bottom', 'bottom-only'),
            ),
            self::ORDINARY_MODS => array( // only come after ordinary
                array('(hause|enhanced)', 'enhanced'),
                array('(abased|abaisse)', 'abased',),
                array('(abased|abaisse)', 'abased',),
                array('de(bas|hanc)ed', 'abased',),
                array('(compony|gobon[ey]|gobonated)', 'compony1'),
                array('counter (compony|gobon[ey]|gobonated)', 'compony2'),
                'closetted', 'embowed', 
                array('?nowy quadrate', 'quadrate'),
                array('nowy lozengy', 'nowy-lozengy'),
                array('rivett?ed', 'riveted'),
                array('[ie]narched', 'inarched'),
                array('(couped|humetty|alese)', 'couped'),
                array('(couped|humetty|alese) ?in sinister', 'couped-sinister'),
                array('(couped|humetty|alese) ?in dexter', 'couped-dexter'),
                array('(couped|humetty|alese) ?in chief', 'couped-chief'),
                array('(couped|humetty|alese) ?in base', 'couped-base'),
                array('fl(o|eu)ry counter fl(o|eu)ry', 'flory-counter-flory'),
                array('fl(o|eu)ry counterfl(o|eu)ry', 'flory-counter-flory'),
                array('pointed in sinister', 'pointed-sinister'),
                array('pointed in dexter', 'pointed-dexter'),
                array('pointed in chief', 'pointed-chief'),
                array('pointed in base', 'pointed-base'),
                array('(bra[zc]ed|interlaced)', 'braced'),
                array('(gemm?ell?(s|ed)?)', 'gemel'),
                array('double dancetty', 'fracted'),
                array('bott?onn?(ee|y)', 'botonny'),
                array('engouled ?(into|by) ?dragons? ?heads?', 'engouled'),
                array('of # (point|peak)s', 'points'), // only if label is a ordinary
                // array ( '(de)?bruised|fracted|disjoint ?at ?the ?top', 'fracted' ),
                array('en(aluron|urney|toire|toyre)', 'entire'),
                'lozengy', 'triangular', 'throughout', 'of pretence', 'ploye',
            ),
            self::COTTICE_MODS => array(
                array('cott?i(s|ss|c)ed', '1'),
                array('close coupled', '1'),
                array('(endorsed|addorsed)', '1',),
                array('cost', '1',), // TODO should only really be 0.5 (i.e. single cottice above)
                array('doubl[ey] cott?i(s|ss|c)ed', '2',),
                array('(triple|treble|triply) cott?i(s|ss|c)ed', '3',),
            ),
            self::COTTICE => array(array('cott?i(s|ss|c)es?', 'cottice')),
            self::VOIDED => array('voided'),
            self::LINETYPE => array(
                'indented', 'nebuly', 'nowy', 'plain', 'potenty',
                'raguly', 'dovetailed', 'engrailed', 'escartelly', 'erable',
                'thorny', 'ristikoro', 'hakulikoro', 'vallikoro',
                'liljakoro',
                array('meander(ed|ing)?', 'meander'),
                array('angled ?lower', 'angled'),
                array('angled (opposite|upper)', 'angled-upper'),
                array('(em)?battled ghibell?ine', 'battled-ghibelline'),
                array('(em)?battled ?in ?the ?form ?of mine dumps', 'battled-mine-dumps'),
                array('(battled|double) (e?m?battled|crenn?ell?e?y?)', 'battled-embattled'),  // two levels of battlements, on (opposite|upper) surface only
                array('(em)?battled grady', 'battled-embattled'),
                array('(em)?battled brett?ess?e?y?', 'battled-brettesse'),                // both sides, same pattern
                array('(e?m?battled|crenn?ell?e?y?) counter (e?m?battled|crenn?ell?e?y?)', 'battled-counter'), // both sides, opposite pattern
                array('?super (e?m?battled|crenn?ell?e?y?)', 'embattled'),        // top only
                array('(e?m?battled|crenn?ell?e?y?) arrondi', 'embattled-arrondi'),           // top only, round crenelles
                array('invec[kt]ed', 'invected'),
                array('(en)?arched', 'arched'),
                array('bevill?ed (opposite|upper)', 'bevilled-upper'),
                array('bevill?y (opposite|upper)', 'bevilled-upper'),
                array('bevill?ed ?lower', 'bevilled'),
                array('bevill?y ?lower', 'bevilled'),
                array('dau?ncett(e|y) fl(o|eu)r(y|etty)', 'dancetty-floretty'),
                array('dau?ncett(e|y) ?the peaks ?ensigned ?with ?a fleur-de-lys', 'dancetty-floretty'),
                array('dau?ncett[ye]', 'dancetty'),
                array('double (en)?arched', 'double-arched'),
                array('(en)?arched double', 'double-arched'),
                array('(rayonated|radiant)', 'rayonny'),
                array('rayonn(e|y)', 'rayonny'),
                array('(wavy|undy|serpentine)', 'wavy'),
                array('(urdy|urde|champaine|champion)', 'urdy'),
                array('?fir twiggy?e?d?', 'twigged'),
                array('sapinagy', 'twigged'),
                array('?(embattled) cupolae?d', 'cupolaed'),
                array('?(embattled) gable?d?y?', 'gabled'),
                array('indented pommette?y?', 'indented-pommetty'),
                array('fir tree topped', 'fir-tree-topped'),
                array('trefl[ey] counter trefl[ey]', 'trefle'),
            ),
            self::ORDINARY_OR_CHARGE => array( // can distinguish by number (perhaps!)
                array('?(plain|holy) cross(es)? ?passant', 'cross/plain-cross'),
                array('chevrons?', 'geometric/chevron'),
                array('(shake|hay)forks?', 'geometric/shakefork'),
                array('chevron(el)?s? couched', 'geometric/chevron-couched'),
                array('cross (formee|patt?ee?)', 'quadrate/cross-formy'),
                array('cross (formy|patt?y)', 'quadrate/cross-formy'),
                array('frets?', 'geometric/fret'),
                array('salta?ires?', 'cross/saltire'),
            ),
            self::ORDINARY_NOT_CHARGE => array( // ordinaries that can be confused with charges
                array('salta?ire ?double parted ?and fretty', 'saltire-2fretty'),
                array('cross ?double parted ?and fretty', 'cross-2fretty'),
                array('cross tripartite ?and fretty', 'cross-3fretty'),
                array('cross quarter pierced?', 'cross-pierced'),
                array('cross quarterpierced?', 'cross-pierced'),
                // array( 'cross formy throughout', 'cross-formy' ),
                array('fillet cross', 'fillet-cross'),
                array('cross fillet', 'fillet-cross'),
                array('frets? couped', 'fret-couped'),
            ),
            self::ARRANGEMENT => array(
                array('in ann?ull?o', 'inannulo',),
                array('in ?the pale', 'inpale',),
                array('in ?the pale throughout', 'inpalethrough',),
                array('in ?the pall', 'inpall',),
                array('in ?the fesse? ^point', 'infess',),
                array('in ?the fesse? throughout', 'infessthrough',),
                array('in ?the bend sinister', 'inbendsin',),
                array('in ?the bend ?dexter', 'inbend',),
                array('in ?the chevron', 'inchevron',),
                array('in ?the pile', 'inpile',),
                array('in ?an orle', 'inorle',),
                array('in ?the orle', 'inorle',),
                array('in ?the salti(er|re)', 'insaltire',),
                array('salti(er|re)w(ays|ise)', 'insaltire',),
                array('salti(er|re) w(ays|ise)', 'insaltire',),
                array('in ?the cross', 'incross',),
                array('?in counter passant', 'counter-passant'),
                array('pilewise', 'pilewise',),
                array('in bar', 'inbar',),
                array('bar(wise|ways)', 'inbar',),
                array('in ?the chief throughout', 'inchiefthrough',),
                array('in ?a quadrangle', 'quadrangle',),
                array('chevronwise', 'chevronwise',),
                array('in point', 'pilewise',),
                array('in triangle', 'triangle',),
                array('respecting each other', 'facing-in'),
                array('respectant', 'facing-in'),
                array('counter rampant', 'facing-in'),
                array('(affrontt?ant|aspectant)', 'facing-in'),
                array('confront(e|ing|y)', 'combattant'),
                array('combatt?ant', 'combattant'),
                array('(addorsed|adosse|endorsed)', 'facing-out'),
            ),
            self::SPECIFIER => array(
                array('(first|1st)', 'first'),
                array('(second|2nd)', 'second'),
                array('(third|3rd)', 'third'),
                array('(fourth|4th)', 'fourth'),
                array('?each dexter', 'dexter'),
                array('?each sinister', 'sinister'),
                array('(each|every|all|both) ?of ?the', 'each'),
                array('middle', 'middle'),
                array('cent(re|er)', 'middle'),
                array('upper', 'upper'),
                array('lower', 'lower'),
            ),
            self::EACH => array(
                array('(each|every|all|both) ?of ?the', 'each'),
            ),
            self::LOCATION => array(
                array('chief ^throughout ?point', 'chief'),
                array('base ?point', 'base'),
                array('honou?r point', 'honpoint'),
                array('fesse? point', 'fesspoint'),
                array('cross point', 'fesspoint',),
                array('navel point', 'nombril'),
                array('nombril', 'nombril'),
                array('(flanks?|flaunche?s?)', 'flank'),
                array('(abisme|abyss)', 'abyss'),
                array('(side|half)', 'side'),
            ),
            self::QUANTIFIER => array(
                array('quarters?', 'quarter'),
                array('(half|section|part)', 'all'),
            ),
            self::CHARGE_BETWEEN => array(
                array('(between|betwixt|inter|btw) ?them', 'between'),
                //  array ( 'within ?(them|it)', 'within' ),
            ),
            self::CHARGE_NEAR => array(
                array('above ?(them|it)', 'above'),
                array('(stand|rest)ing on', 'above'),
                array('below ?(them|it)', 'below'),
                array('(hang|depend)ing from', 'below'),
                array('beside', 'beside'),
                array('suspended (by|from|of)', 'below'),
                array('resting (each|its|his|her) claws? (up)?on', 'above'),
                array('sustaining', 'below'),
            ),
            self::SHOWN_EXTRA => array(
                array('to ?the dexter', 'to-dexter'),
                array('to ?the sinister', 'to-sinister'),
                array('likewise', 'likewise'),
                array('?in the same (way|fashion|manner)', 'likewise'),
            ),
            self::SHOWN_MODS => array(
                array('(shown|shewn|moved) ?somewhat', 'shown'),
                array('somewhat', 'shown'),
                array('(from|to|of|than) ?(the|is) (normal|usual) ?(position|location|place)s?', 'than-normal'),
                array('very much', 'very-much'),
                array('a great deal', 'very-much'),
                array('much', 'much'),
                array('a lot', 'much'),
                array('a little', 'slightly'),
                array('slightly', 'slightly'),
                array('(larger|bigger)', 'larger'),
                array('smaller', 'smaller'),
                array('(rotated|turned)', 'rotated-dexter'),
                array('(rotated|turned) ?to(ward)?s? ?the dexter', 'rotated-dexter'),
                array('(rotated|turned) (anti|counter)clockwise', 'rotated-dexter'),
                array('(rotated|turned) (anti|counter) clockwise', 'rotated-dexter'),
                array('(rotated|turned) ?to(ward)?s? ?the sinister', 'rotated-sinister'),
                array('(rotated|turned) clockwise', 'rotated-sinister'),
                array('(rotated|turned) one quarter turn (anti|counter)clockwise', 'rotated-quarter-dexter'),
                array('(rotated|turned) one quarter turn (anti|counter) clockwise', 'rotated-quarter-dexter'),
                array('(rotated|turned) one quarter turn ?to(ward)?s? ?the dexter', 'rotated-quarter-dexter'),
                array('(rotated|turned) one quarter turn clockwise', 'rotated-quarter-sinister'),
                array('(rotated|turned) one quarter turn ?to(ward)?s? ?the sinister', 'rotated-quarter-sinister'),
                array('(rotated|turned) one third turn (anti|counter)clockwise', 'rotated-third-dexter'),
                array('(rotated|turned) one third turn (anti|counter) clockwise', 'rotated-third-dexter'),
                array('(rotated|turned) one third turn ?to(ward)?s? ?the dexter', 'rotated-third-dexter'),
                array('(rotated|turned) one third turn clockwise', 'rotated-third-sinister'),
                array('(rotated|turned) one third turn ?to(ward)?s? ?the sinister', 'rotated-third-sinister'),
                array('(rotated|turned) one sixth turn (anti|counter)clockwise', 'rotated-sixth-dexter'),
                array('(rotated|turned) one sixth turn (anti|counter) clockwise', 'rotated-sixth-dexter'),
                array('(rotated|turned) one sixth turn ?to(ward)?s? ?the dexter', 'rotated-sixth-dexter'),
                array('(rotated|turned) one sixth turn clockwise', 'rotated-sixth-sinister'),
                array('(rotated|turned) one sixth turn ?to(ward)?s? ?the sinister', 'rotated-sixth-sinister'),
                array('(rotated|turned) one eighth turn (anti|counter)clockwise', 'rotated-eighth-dexter'),
                array('(rotated|turned) one eighth turn (anti|counter) clockwise', 'rotated-eighth-dexter'),
                array('(rotated|turned) one eighth turn ?to(ward)?s? ?the dexter', 'rotated-eighth-dexter'),
                array('(rotated|turned) one eighth turn clockwise', 'rotated-eighth-sinister'),
                array('(rotated|turned) one eighth turn ?to(ward)?s? ?the sinister', 'rotated-eighth-sinister'),
                array('(raised|lifted|higher)', 'raised'),
                array('(lowere?d?|dropped)', 'lowered'),
                array('further apart', 'further-apart'),
                array('closer? ?together', 'closer-together'),
                array('wide(r|nd)','wider'),
                array('narrowe[rd]','narrower'),
                array('taller','taller'),
                array('shorte(r|nd)','shorter'),
                array('stroke[sd]','stroked'),
                array('fancy','fancy'),
                array('in ?a fancy style','fancy'),
                array('embellished','fancy'),
            ),
            self::USING => array ( 'with', 'in', 'using', 'on'),
            self::DRAWN_MODS => array(
                array('drawn', 'drawn'),
                array('?(a|an|the) * (palette|tinctures)', 'palette'),
                array('?the * colours?', 'colours'),
                array('?(a|an|the) * shape', 'shape'),
                array('?(a|an|the) shape ?of ?an? *', 'shape'),
                array('?(a|an|the) * (appearance|style|effect)', 'style'),
                array('?(a|an|the) aspect ?ratio ?of *', 'aspect'),
                array('?(a|an|the) * aspect ?ratio', 'aspect'),
                array('?(a|an|the) proportions? ?of *', 'aspect'),
                array('?(a|an|the) * proportions?', 'aspect'),
                array('?a grid(ded)?', 'grid'),
            ),
            self::QUARTERMODS => array(
                array('per cross', 'per-cross'),
                array('per saltire', 'per-saltire'),
                array('per saltire', 'per-saltire'),
            ),
            self::FONT => array(
                array('in (a|the) * (font|face|style|script)', 'font'),
            ),
        );
        include "charge.inc";
        include "chargemod.inc";
        include "web-colours.inc";
        include "warhammer-colours.inc";
        include "tartan-colours.inc";
        SmartChargeGroup::instance()->add_patterns($this);
    }
}
