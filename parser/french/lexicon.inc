<?php 

class languageDB extends Lexicon
{
    // These constants acts as keys into the pattern database
    // Some are also used to indicate the type of modifier
    const COULEUR='couleur';
    const FOURRURE='fourrure';
    const NATUREL='naturel';
    const QUANTITE='quantite';
    const NOMBRE='nombre';
    const ORDINAIRE='ordinaire';
    const MOT_ET='et';
    const MOT_DE='de';
    const TRAITEMENT='traitement';
    const SEME='seme';
    const CHARGE='charge';
    const LIGNE='ligne';
    // You don't need to define these in the lexicon, the tokeniser sets them
    const SEMICOLON = 'semicolon';
    const COMMA = 'comma';
    const COLON = 'colon';
    const DOUBLESEMI = 'doublesemi';
    const PERIOD = 'period';
    const ONFIELDOF = 'onfieldof';
    
    const LEXEMEDIR = 'parser/french/lexemes';

    public function __construct()
    {
        global $options;

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
                if ($lex_spec == '' || $lex_spec[0] == '#' || !str_contains($lex_spec,'=')) continue;
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
        }
    }
}
