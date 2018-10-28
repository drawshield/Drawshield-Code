<?php
header('Content-Type: text/plain; charset=utf-8');

$errors = [];

/******************************************************************
**
**	Option Processing
**
*******************************************************************/

/*
**	Set up our default choices
*/
$options = [
    "tinc-common" => "on",
    "tinc-second" => "on",
    "tinc-modern" => "off",
    "tinc-furs" => "on",
    "tinc-treatments" => "on",

    "div-chance" => 50,
    "div-2part" => "on",
    "div-3part" => "on",
    "div-bars" => "on",
    "div-pattern" => "on",
    "div-edge" => "on",
    "div-counter" => "on",

    "ord-chance" => 50,
    "ord-common" => "on",
    "ord-multi" => "on",
    "ord-minor" => "on",
    "ord-rare" => "on",
    "ord-mods" => "on",

    "chg-chance" => 50,
    "chg-lion" => "on",
    "chg-cross" => "on",
    "chg-geom" => "on",
    "chg-bird" => "on",
    "loc-chance" => 10,

];

/*
**	Overwrite options with any supplied in the POST request
*/
$options = array_merge($options, $_POST, $_GET);

/******************************************************************
**
**	Define the lexicon (fixed items)
**
*******************************************************************/

$lexicon = [
/*
**	Tinctures
*/

    "tinc-common" => [ "azure", "or", "argent", "purpure", "gules", "sable", "vert" ],
    "tinc-second" => [ "murrey", "tenne", "sanguine", "carnation", "brunatre", "cendree", "rose", 
            "celestial azure", "bis", "senois", "orange" ],
    "tinc-modern" => [ 'iron', 'bronze', 'copper', 'lead', 'steel', 'white', 'buff', 'red-ochre',
            'yellow-ochre', 'crimson'],
    "fur" => [ "counter-ermine", "counter-potent", "counter-vair", "ermine", "erminois", "pean", 
    "potent", "potent-counter-potent", "vair", "vair-en-point", "vair-in-pale", "erminites" ],

    "treatment2" => [ "annuletty", "billetty",  "crusilly", "estoilly", "fleury",
            "fretty", "goutty", "grillage", "honeycombed", "lozengy", "maily",
            "masoned", "mullety", "papelonny", "checky", "scaly", "counter-billetty",
            "latticed", "plumetty", 
            ],
    "treatment1" => [ 'hurty', 'bezanty', 'platy', ],

/*
**	Divisons
*/

    "div-2part" => [ 
            "chevronny", "gyronny", "gyronny of {num-gyrons}", "per chevron inverted",
            "per-bend {edge-all}", "per bend sinister {edge-all}", "per-chevron {edge-simple}",
            "per-fess {edge-all}", "chape {edge-simple}", "chausse {edge-simple}",
            "per-pale {edge-all}", "per-saltire {edge-simple}", "pily", "quarterly {edge-simple}",  ],
    "div-3part" => [  "tierced in pale {edge-all}", "tierced-in-fess {edge-all}", "per pall", 
            "per pall reversed", "per-pile",
            "tierced in bend {edge-all}", "tierced in bend sinister {edge-all}", "tierced in chevron {edge-simple}" ],
    "div-bars" => [ "barry {edge-simple}", "barry of 12", "barry of 20", "bendy {edge-simple}",
            "bendy sinister {edge-simple}", "paly {edge-simple}", ],
    "div-pattern" => ["fusily", "paly bendy", "pily bendy", "pily bendy sinister", "fusily-bendy",
            "barry bendy", "barry pily", "barry indented the one in the other" ],
     "counter1" => [ "per fess {edge-all}", "per pale {edge-all}", "barry {edge-simple}", "paly {edge-simple}"],
    "counter2" => [ "per bend {edge-all}", "per chevron", "per saltire {edge-simple}", "per bend sinister {edge-all}" ],
    "num-gyrons" => ["6", "6", "6", "8", "10", "10", "12", "12", "12", "14", "16", "16", ],

    // edge types suitable for all shapes
    "edge-simple" => [ "bevilled", "engrailed", "escartelly", "angled", "arched", "invected", 
        "nowy", "wavy", "rayonny", "urdy",  ],
    // edge types that work best on straight edges only
    "edge-all" => [ "bevilled", "engrailed", "escartelly", "angled", "arched", "invected", 
        "nowy", "wavy", // same set as above, with the addition of:
        "battled embattled", "dancetty", "dancetty floretty", "double arched", "dovetailed",
        "embattled", "embattled arrondi", "embattled brettessy", "embattled counter-embattled",
        "embattled cupolaed", "embattled gabley", "embattled grady", "embattled in the form of mine dumps",
        /* "erable', */ "fir tree topped", "fir twigged", "hakulikoro", "indented", "indented pommetty",
        "liljakoro", "nebuly", "potenty", "raguly", "rayonny", "ristikoro", "thorny", 
        "trefle counter trefle", "urdy", "vallikoro", 'battled-ghibelline' ],

    "ord-common" => ["{ord-large1}", "{ord-large2}"],
    "ord-large1" => [ "bend", "fess", "pale", ],
    "ord-large2" => ["chevron", "chief",  "quarter {sinister}", "base", "saltire", "cross"],
    "ord-multi" => [ "{2-4} bars", "{2-4} barrulets", "{2-4} bendlets", "{2-4} chevronels {interlaced}",
                    "{2-4} palets", ],
    "ord-minor" => ["gore {sinister}", "canton {sinister}", "bordure", "flaunch {sinister}",
                    "graft {sinister}", "gusset {sinister}", "gyron {sinister}", "inescutcheon", 
                    "orle", "shakefork", "point dexter", "point sinister",
                    "chevron {chev-mod}", "cross parted and fretty", "cross tripartite and fretty".
                    "cross quarter pierced", "saltire parted and fretty", "fret couped",
                    "fillet cross", ],
    "ord-rare" => ["crancelin {sinister}", "canadian pale", "gorge", "trimount", "fillet", 
                    "fillet saltire", "bend {sinister} engouled by dragon heads",
                    "dance", "pallium", "lozenge throughout", "point in point", "enty in point",
                    "fess quadrate", "fess nowy"],
    "mod-ord1" => [ "quadrate", "nowy", "couped", "lozengy"],
    "mod-ord2" => [ "{edge-all}", "{edge-all}", "{edge-all}", 
                    "fimbriated {base-tincture}", "cotticed", "double cotticed",
                    "cotticed {base-tincture}", "voided", "compony {base-tincture}" ], // for large only
    "sinister" => [ '', '', "sinister"], // 50% chance
    "interlaced" => [ '', "interlaced"], // 50% chance
    "chev-mod" => [ "rompu", "fracted", "burst", "disjoint", "removed", "inarched", "couched {sinister}"],

    /*
    **	Lots and lots of charges, by group
    */

    // Crosses - Always do these singly
    "chg-cross1" => [ "Calvary Cross", "Celtic Cross", "couped Cross", "Latin Cross",
            "Egyptian Cross", "Greek Cross", "paternoster Cross", "patriarchal Cross",
            "portate Cross", "Russian Cross", "Cross of Saint James", "Tau Cross",
            "Maltese Cross", "Cross of four fusils", "Cross of nine fusils", "compass rose", "Cross Barby"],
    // do these in groups, and allow modifiers
    "chg-cross2" => ["Crosses Avellane", "Crosses Botonny", "Crosses Cercele", "Crosses Cleche",
            "Crosses Crosslet Crossed", "Crosses Crosslet", "Crosses Floretty",
            "Crosses Flory", "Crosses Formy", "Crosses Fourche", "Crosses Moline",
            "Crosses Patonce", "Crosses Paty Floretty", "Crosses Pointed", "Crosses Pomme",
            "Crosses Potent", "Crosses Sarcelly", "Crosslets", "fylfots",],
    "cross-mod" => [ "", "", "", "pierced", "fitchy", "voided"], // 50%
    "chg-cross" => ["a {chg-cross1} {base-tincture}", "{3-6} {chg-cross2} {cross-mod} {base-tincture}"],

    // Lions
    "chg-lion" => [ "a lion {lion-pose} {base-tincture} {lion-feature}", 
                "a lion {lion-pose} {base-tincture}", // preference for a single
                "2 lions {lion-pose} {arrange2} {base-tincture}",
                "3 lions {lion-pose} {arrange3} {base-tincture}",
                "4 lions {lion-pose} {arrange4} {base-tincture}",
                "{lion-head}"
            ],
    "lion-pose" => ["cadent", "couchant", "courant", "coward", "dormant",
        "passant guardant", "passant reguardant", "passant", "rampant guardant",
        "rampant reguardant", "rampant tail forked", "rampant tail nowed",
        "rampant", "salient", "sejant", "statant guardant", "statant",
        "", "", "", "", ""], // slight preference for no stated pose 
    "lion-feature" => ["armed {base-tincture}", "langued {base-tincture}",
        "armed and langued {base-tincture}"], // 50%
    "lion-head" => [ "a lion's head {arr-head} {base-tincture}",
                    "2 lion's heads {arrange2} {base-tincture}",
                    "{3-6} lion's heads {arr-head} {base-tincture}"],

    // simple shapes
    "chg-geom" => ["{3-6} {geom-fixed}", "a {geom-single} {base-tincture}",
            "{2-6} {geom-multi} {base-tincture}"],
    "geom-fixed" => ["bezants", "hurts", "oranges", "pellets", "plates",
            "pommes", "torteaux", "guzes", "golpes"],
    "geom-single" => ["fret", "hexagon {holed}", "lozenge {holed}", "penrose triangle",
            "square {holed}", "triangle {holed}"],
    "geom-multi" => ["billets", "cartouches", "crescents", "delfs", "fusils", "icicles",
            "mascles", "rustres", "saltorels", "annulets {concentric}", "mullets",
            "mullets of {4-10} {pierced}"],
    "concentric" => ["", "concentric"],
    "pierced" => ["", "pierced"],

    // Birds
    "bird-single" => [ "heron", "dove {dove-pose}", "swan {swan-pose}",
                "owl {displayed}", "peacock", "pair of wings", "crow {essorant}",
                "falcon", "alerion", "eagle", "eagle {eagle-pose}" ],
    "bird-multi" => [ "auks", "bats", "blackbirds", "choughs", "ducks", "seagulls {volant}",
                "martletts", "popinjays", "qauils", "ravens", "starlings"],
    "dove-pose" => ["", "", "essorant", "fondant", "volant"],
    "swan-pose" => ["", "", "essorant", "displayed", "nageant", "passant"],
    "eagle-pose" => ["closed", "displayed", "double headed", "natural", "rising"],
    "displayed" => ["", "displayed"],
    "essorant" => ["", "essorant"],
    "volant" => ["", "volant"],
    "chg-bird" => [ "a {bird-single} {base-tincture}",
                "{3-6} {bird-multi} {base-tincture}",
                "2 {bird-multi} {arrange2} {base-tincture}",
                "3 {bird-multi} {arrange3} {base-tincture}",
                "4 {bird-multi} {arrange4} {base-tincture}",],

    // Arrangements for various numbers
    "arrange2" => ["respecting each other", "addorsed", "in pale",
        "in-bend", "in bend sinister", "", ""],
    "arrange3" => ["1,2", "in pale", "in fess",
        "in bend", "in bend sinister", "", ""],
    "arrange4" => ["in a quadrangle", "in pale", "in fess",
        "in bend", "in bend sinister", "counter passant", "", ""],
    "arr-head" => ["affronte", "reguardant", "guardant", "", ""],
    "holed" => ["pierced", "voided", "", "",], // 50%

    // Some locations
    "location" => ["in first quarter", "in second quarter", "in third quarter", "in fourth quarter",
                "in abyss", "in {sinister} flank", "in nombril"],

];

/******************************************************************
**
**	Variables to keep track of what has been used
**
*******************************************************************/

$phase = null;

$componentTinctures = [ 
    "counter-ermine" => [ "sable", "argent",],
    "counter-potent" => [  "argent", "azure",],
    "counter-vair" => [ "argent", "azure",],
    "ermine" => [ "sable", "argent",],
    "erminites" => [ "sable", "argent",],
    "erminois" => [ "sable", "or",],
    "pean" => [ "or", "argent",],
    "potent" => [  "argent", "azure",],
    "potent-counter-potent" => [ "argent", "azure",],
    "vair" => [  "argent", "azure",],
    "vair-en-point" => [  "argent", "azure",],
    "vair-in-pale" => [ "argent", "azure"],
    "platy" => [ "argent" ],
    "hurty" => [ "azure"],
    "bezanty" => [ "or" ]
];

$usedFieldTinctures = [];
$usedOrdinary = false;


/******************************************************************
**
**	Helper functions
**
*******************************************************************/

function byChance($chance) {
    return $chance > rand(1,99); // 0 = never, 100 = always 
}

function randomly($choices, $checkUsed = false) {
    if (!is_array($choices)) return $choices; // just in case we send a string - issue warning?
    $maxIndex = count($choices)  - 1;
    if ($maxIndex == 0) return $choices[0]; // Hobson's
    $limit = 20; // avoid infinite loops (just in case)
    do {
        $chosen = $choices[rand(0, $maxIndex)];
    } while (--$limit > 0 && $checkUsed && hasBeenUsed($chosen));
    return $chosen;
}

function number($lower, $upper) {
    // return a random number between lower and upper inclusive, but prefer lower
    $multiplier = $upper - $lower + 1;
    $choices = [];
    for ($i = $lower; $i <= $upper; $i++) {
        for ($j = 0; $j < $multiplier; $j++) {
            $choices[] = "$i";
        }
        $multiplier--;
    }
    return randomly($choices);
}

function markAsUsed($tincture) {
    global $usedFieldTinctures, $componentTinctures;

    if (array_key_exists($tincture, $componentTinctures)) {
        $usedFieldTinctures = array_merge($usedFieldTinctures, $componentTinctures[$tincture]);
    }
    $usedFieldTinctures[] = $tincture;
}

function hasBeenUsed($tincture) {
    global $usedFieldTinctures, $componentTinctures;

    $used = in_array($tincture,$usedFieldTinctures,true);
    if (!$used) { // also check component parts of proposed item
        if (array_key_exists($tincture, $componentTinctures)) {
            foreach ($componentTinctures[$tincture] as $component) {
                if (hasBeenUsed($component)) {
                    $used = true;
                    break;
                }
            }
        }
    }
    return $used;
}

function punctuate($string) {
    static $prefix = '';
    $output = '';
    $indent = '';
    $words = preg_split('/[ \t]+/', $string);
    $numwords = count($words);
    for( $i = 0; $i < $numwords; $i++) {
        switch ($words[$i]) {
            case '#':
                $output .= "\n$prefix";
                break;
            case '>':
                $prefix .= '    ';
                break;
            case '<':
                $prefix = substr($prefix,0,-4);
                break;
            case "2":
                $output .= "two";
                break;
            case "3":
                $output .= "three";
                break;
            case "4":
                $output .= "four";
                break;
            case "5":
                $output .= "five";
                break;
            case "6":
                $output .= "six";
                break;
            case 'a':
                if (strpos(' aeiou', $words[$i +1][0])) {
                    $output .= "an";
                } else {
                    $output .= "a";
                }
                break;
            default:
                $output .= $words[$i];
        }
        $output .= " ";
    }
    return rtrim(strtoupper($output[0]) . substr($output,1)) . ".\n";
}

function cleanup($string) { // remove expansions to get core value
    return trim(preg_replace('/{.+?}/', '', $string));
}

/*
**	Expansions
*/

function expand($tokenString) {
    global $options, $lexicon, $errors;
    global $usedOrdinary;
    global $phase;

    if (strpos($tokenString,'{') === false) return $tokenString;
    $tokens = preg_split("/([ ,;\"'\\.]+)/", $tokenString, -1, PREG_SPLIT_DELIM_CAPTURE );
    $newString = '';
    foreach ($tokens as $token) {
        $replacement = null;
        if ($token[0] == '{') {
            $tokenValue = substr($token,1,strlen($token)-2);
            if (ctype_digit($tokenValue[0])) {
                list($lower,$upper) = explode('-',$tokenValue);
                $replacement = number($lower,$upper);
            } elseif (substr($tokenValue,0,3) == 'ord') {
                // record the ordinary used
                $replacement = randomly($lexicon[$tokenValue]);
                $usedOrdinary = cleanup($replacement);
            } else {
                switch($tokenValue) {
                     // check these haven't been used before
                    case 'treatment1': 
                    // treatment2 can be re-used as it will be diff. colours
                    case 'fur':
                    case 'tinc-common':
                    case 'tinc-modern':
                    case 'tinc-second':
                        $replacement = randomly($lexicon[$tokenValue], true); // check re-use 
                        markAsUsed($replacement);
                        break;
                    case 'edge-all': 
                    case 'edge-simple':
                        // whatever we find, need to set $replacement so that {token} is gone
                        if ($phase == 'field' && $options["div-edge"] != "on") return '';
                        $replacement = (byChance($options['edge-chance'])) ? 
                                randomly($lexicon[$tokenValue]) : '';
                        break;
                    case 'treatment': // prefer two colours as more of them
                            $replacement = (bychance(90)) ? 
                                    "{treatment2} {base-tincture} and {base-tincture}" :
                                    "{treatment1} {base-tincture}";
                        break;
                    default:
                        if (array_key_exists($tokenValue, $lexicon)) {
                            $replacement = randomly($lexicon[$tokenValue]);
                        } else {
                            $errors[] = "unknown key value - $token";
                            $replacement = ''; // remove error expansion
                        }
                }
            }
            $replacement = expand($replacement);
        }
        $newString .= $replacement ?? $token;
    }
    return $newString;
}

/******************************************************************
**
**	Phase 0 - Preparation, based on options
**
*******************************************************************/

/*
**	Pre-determine the big decisions (as they sometimes interact)
*/

$showDivision = byChance($options['div-chance']);
$showOrdinary = byChance($options['ord-chance']);
$showCharge = byChance($options['chg-chance']);

/*
**	Set up list of chosen textures
*/
// idiot check - prevent all tinctures being off
if ($options['tinc-common'] == 'off' && $options['tinc-second'] == 'off' 
                    && $options['tinc-modern'] == 'off')
    $options['tinc-common'] = 'on';

if ($options['tinc-common'] == 'on') {
    $lexicon["base-tincture"][] = "{tinc-common}"; 
    if ($showCharge || $showOrdinary || $showDivision)
    {    
        $lexicon["base-tincture"][] = "{tinc-common}"; // Have a preference for common tinctures
        $lexicon["base-tincture"][] = "{tinc-common}"; // when chosing colours for ordinaries etc.
    }
}
if ($options['tinc-second'] == "on") 
    $lexicon["base-tincture"][] = "{tinc-second}";
if ($options['tinc-modern'] == "on") 
    $lexicon["base-tincture"][] = "{tinc-modern}";
// fieldTincture are used for plain or divided fields only
$lexicon["field-tincture"] = $lexicon["base-tincture"];
if ($options['tinc-furs'] == "on") $lexicon["field-tincture"][] = "{fur}";
if ($options['tinc-treatments'] == "on") $lexicon["field-tincture"][] = "{treatment}";

/*
**	Set up lists of available charges
*/

if ($showCharge) {
    $lexicon['charge-x1'] = [];
    $leixcon['charge-xn'] = [];
    $lexicon['charge-all'] = [];
    if ($options["chg-cross"] == "on") {
        $lexicon['charge-x1'][] = "a {chg-cross1} {base-tincture}";
        $lexicon['charge-all'] = "{chg-cross}";
        $lexicon['charge-xn'][] = "{chg-cross2} {cross-mod} {base-tincture}";
    }
    if ($options["chg-lion"] == "on") {
        $lexicon['charge-x1'][] = "a lion {lion-pose} {base-tincture} {lion-feature}";
        $lexicon['charge-x1'][] = "a lion's head {arr-head} {base-tincture}";
        $lexicon['charge-all'] = "{chg-lion}";
        $lexicon['charge-xn'][] = "lions {lion-pose} {base-tincture} {lion-feature}";
        $lexicon['charge-xn'][] = "lion's heads {arr-head} {base-tincture}";
    }
    if ($options["chg-geom"] == "on") {
        $lexicon['charge-x1'][] = "a {geom-single} {base-tincture}";
        $lexicon['charge-all'] = "{chg-geom}";
        $lexicon['charge-xn'][] = "{geom-multi} {base-tincture}";
        $lexicon['charge-xn'][] = "{geom-fixed}";
    }
    if ($options["chg-bird"] == "on") {
        $lexicon['charge-x1'][] = "a {bird-single} {base-tincture}";
        $lexicon['charge-all'] = "{chg-bird}";
        $lexicon['charge-xn'][] = "{bird-multi} {base-tincture}";
    }
}

/******************************************************************
**
**	Phase 1 - the field
**
*******************************************************************/

$phase = 'field';

// Now, what types of field can we chose from?
$fieldTypes = [ "{field-tincture}" ]; // The default
if ($showDivision) {
    // idiot check - prevent everything being off
    if ($options['div-2part'] == 'off' && $options['div-3part'] == 'off' 
        && $options['div-bars'] == 'off' && $options['div-pattern'] == 'off')
    $options['div-2part'] = 'on';

    $fieldTypes = [];
    if ($options['div-2part'] == "on") 
            $fieldTypes =  [
                "{div-2part} {base-tincture} and {field-tincture}", // prefer 3x as much as div-3part
                "{div-2part} {field-tincture} and {base-tincture}",
                "{div-2part} {field-tincture} and {base-tincture}",
            ] ;
    if ($options['div-3part'] == "on")
            $fieldTypes[] = "{div-3part} {field-tincture}, {base-tincture} and {field-tincture}" ;
    if ($options['div-bars'] == "on")
            $fieldTypes[] = "{div-bars} {field-tincture} and {base-tincture}" ;
    if ($options['div-pattern'] == "on")
            $fieldTypes[] = "{div-pattern} {field-tincture} and {base-tincture}" ;
    if ($options['div-counter'] == "on")
            $fieldTypes[] = "{counter1} {base-tincture} and {base-tincture} {counter2} counterchanged" ;
}

$blazon = expand(randomly($fieldTypes));

/******************************************************************
**
**	Phase 2 - Ordinaries
**
*******************************************************************/

$phase = "ordinary";
if ($showOrdinary) {
        // idiot check - prevent everything being off
    if ($options['ord-common'] == 'off' && $options['ord-multi'] == 'off' 
            && $options['ord-minor'] == 'off' && $options['ord-rare'] == 'off')
        $options['ord-common'] = 'on';

    $ordinaryTypes = [];
    if ($options["ord-common"] == "on") {
        $ordinaryTypes[] = "a {ord-common} {field-tincture}";
        $ordinaryTypes[] = "a {ord-common} {field-tincture}";
        $ordinaryTypes[] = "a {ord-common} {field-tincture}"; // preference for big ordinaries
        if ($options["ord-mods"] == "on") {
            $ordinaryTypes[] = "a {ord-large1} {field-tincture} {mod-ord1}";
            $ordinaryTypes[] = "a {ord-large1} {field-tincture} {mod-ord2}";
            $ordinaryTypes[] = "a {ord-large1} {field-tincture} {mod-ord2}";
            $ordinaryTypes[] = "a {ord-large2} {field-tincture} {mod-ord2}";
        }    
    }
    if ($options["ord-multi"] == "on") {
        $ordinaryTypes[] = "{ord-multi} {base-tincture}";
        $ordinaryTypes[] = "{ord-multi} {base-tincture}"; // quite like these too...
    }
    if ($options["ord-minor"] == "on") // these are less common
        $ordinaryTypes[] = "a {ord-minor} {base-tincture}";
    if ($options["ord-rare"] == "on")
        $ordinaryTypes[] = "a {ord-rare} {base-tincture}";
    $ordinaryText .= expand(randomly($ordinaryTypes));

    if ($showCharge) {
        switch ($usedOrdinary) {
            case 'quarter':
            case 'pile':
            case 'inescutcheon':
            case 'base':
                $ordinaryText = expand("on > # $ordinaryText < # {charge-x1} ");
                $showCharge = false;
                break;
            case 'fess':
            case 'bend':
            case 'chief':
            case 'pile':
            case 'chevron':
                $ordinaryText = expand("on > # $ordinaryText < # {2-4} {charge-xn}");
                $showCharge = false;
                break;
            case 'cross':
            case 'saltire':
                $ordinaryText = expand("on > # $ordinaryText < # 4 {charge-xn}");
                $showCharge = false;
                break;
            default:
                break; // just draw a charge in the normal place
        }
    }

    $blazon .= " > # $ordinaryText";
}

/******************************************************************
**
**	Charge phase
**
*******************************************************************/

if ($showCharge) {
    $blazon .= expand(" > # {charge-all}");
    if (bychance($options['loc-chance']))
        $blazon .= expand(" # {location}");
}


/******************************************************************
**
**	Output phase
**
*******************************************************************/

$blazon = punctuate($blazon) . "\n// created by Drawshield.net/random";

//header('Content-Type: text/plain; charset=utf-8');
echo $blazon;
if (count($errors)) {
    echo "-- Errors Reported:\n";
    foreach ($errors as $error) {
        echo "$error\n";
    }
}
if ($options['dump']) {
    var_dump(${$options['dump']});
}

