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

// require_once ('../../lexicon.inc');

class languageDB extends Lexicon
{
    // These constants acts as keys into the pattern database
    // Some are also used to indicate the type of modifier
    const ROLL = 'roll';
    const NEXT = 'next';
    const CONSISTING = 'consisting';
    const SKIP = 'skip';
    const OVERALL = 'overall';
    const BOT_COMMAND = 'bot_command';
    const METAOVERALL = 'metaoverall';
    const SPLITS = 'splits';
    const ONFIELDOF = 'onfieldof';
    const SEMICOLON = 'semicolon';
    const COLON = 'colon';
    const DOUBLESEMI = 'doublesemi';
    const COMMA = 'comma';
    const PERIOD = 'period';
    const WORD_THE = 'wordthe';
    const WORD_OF = 'wordof';
    const WORD_ON = 'wordon';
    const WORD_AT = 'wordat';
    const WORD_TO = 'wordto';
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
    const OBJECTS = 'objects';
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
    const INARRANGEMENT = 'inarrangement';
    const ARRANGED = 'arranged';
    const SPECIFIER = 'specifier';
    const LOCATION = 'location';
    const QUANTIFIER = 'quantifier';
    const BUNDLE = 'bundle';
    const CHARGE_PREFIX = 'chargepre';
    const TRADITION_PREFIX = 'tradition-prefix';
    const TRADITION_DESCRIPTION = 'tradition-desc';
    const SHAPE_PREFIX = 'shape-prefix';
    const SHAPE_DESCRIPTION = 'shape-desc';
    const CHARGE_BETWEEN = 'chgbetween';
    const CHARGE_NEAR = 'chargenear';
    const CHARGE_FEATURES = 'chargefeat';
    const CHARGE_MODS = 'chargemod';
    const POSITION = 'position';
    const POINTS = 'points';
    const HALF = 'half';
    // const SUPPLEMENTARY = 'supplementary';
    const SHOWN_EXTRA = 'shownextra';
    const SHOWN_MODS = 'shown';
    const DRAWN_MODS = 'drawnmods';
    const DRAWN = 'drawn';
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
    const VERTICAL_POSITION = 'vert_pos';
    const MOTTO_TYPE = 'motto_type';
    const MOTTO_TEXT = 'string';
    const CREST = 'crest';
    const TORSE = 'torse';
    const CREST_HELM = 'cresthelm';
    const ACHIEVEMENT = 'achievement';
    const ORNAMENT = 'ornament';
    const ECCLESIASTIC = 'ecclesiastic';
    const EACH = 'each';
    const BLAZONED = 'blazoned';
    const VALUE = 'string';
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
    const PIECES = 'pieces';
    const CAPTION = 'caption';
    const CAPTIONPOS = 'captionpos';
    const BACKGROUND = 'background';
    const TITLE = 'title';
    const COMMANDS = 'commands';
 #   const DISPLACED = 'displaced';
 #
    const LEXEMEDIR = 'parser/english/lexemes';

    public function __construct()
    {
        global $options;

        if (!file_exists(self::LEXEMEDIR . "/lexemes.dat")) {
            $this->patterns = [];
            $lexDir = scandir(self::LEXEMEDIR);
            $category = "none";
            $group = '';
            foreach ($lexDir as $lexFile) {
                if (str_starts_with($lexFile, '.')) continue;
                if (!str_ends_with($lexFile, '.txt')) continue;
                $lexical_specifications = file(self::LEXEMEDIR . '/'
                    . $lexFile, FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
                foreach ($lexical_specifications as $lex_spec) {
                    $lex_spec = ltrim($lex_spec);
                    if ($lex_spec == '' || $lex_spec[0] == '#') continue;
                    if ($lex_spec[0] == '=') { // new category
                        if (str_contains($lex_spec, '/')) { // there is a group here
                            list($category, $group) = preg_split(':/:', $lex_spec);
                            $group .= '/';
                        } else {
                            $group = '';
                            $category = $lex_spec;
                        }
                        $category = substr($category, 1);
                        continue;
                    }
                    // else
                    list($searchTerm, $keyTerm) = preg_split('/=/', $lex_spec);
                    if ($keyTerm == '') $keyTerm = $searchTerm;
                    $new_entry = array($category, $group . $keyTerm);
                    $regexs = explode(" ", $searchTerm);
                    $this->patterns[] = array_merge($new_entry, $regexs);
                }
            } // only save the file if we are run locally to avoid race conditions when
            // run possibly in parallel by the web server
            if ($options['requestSource'] == 'shell') {
                file_put_contents(self::LEXEMEDIR . "/lexemes.dat", serialize($this->patterns));
            }
        } else {
            $this->patterns = unserialize(file_get_contents(self::LEXEMEDIR . "/lexemes.dat"));
        }
    }
}
