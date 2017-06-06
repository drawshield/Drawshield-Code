/* Copyright 2010 Karl R. Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */
var version = "0.9d";

var rep_called = false;
var used_colours = new String();
var used_fur = false;
var last_num = 1;

var templates = new Array (
  "{field} {1-6} {charge} {position} {orientation} {ucolour}",
  "{field} {1-6} {ccharge}",
  "{field} {1-9} {ccharge}",
  "{field} {arrangement}",
  "{field} {ordinary}",
  "{field} {ordinary} {1-6} {ccharge}",
  "{field} {ordinary} {single}",
  "{division} {1-9} {charge} {position} {orientation} {ucolour}",
  "{division} {1-9} {charge} {ucolour}",
  "{division} {ordinary}",
  "{division} {single}",
  "{division} {1-6} {ccharge}",
  "{division} {ordinary} {1-9} {ccharge}",
  "{division} {arrangement}",
  "{division} {ordinary} {1-6} {charge} {position} {orientation} {ucolour}"
);

var numbers = new Array (
  "zero", "a", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"
);

var positions = new Array (
 "in dexter side", "in sinister side", "in dexter chief", "in sinister chief",
  "in middle chief", "in dexter base", "in middle base", "in honour point", "in fess point", "in nombril"
);

var charges = new Array (
"annulet", "billet", "cartouche", "crescent", "delf", "fusil", "goutte", "lozenge", "mascle", "mullet", "mullet(s) of {3-9} {voided}",
"roundel", "rustre", "saltorel",
"Crosslet", "long cross(es)", "patriarchal cross(es)", "calvary cross(es) mounted on {3-6} degrees", "Tau cross(es)",
"maltese cross(es)", "cross(es) formy", "cross(es) pointed", "cross(es) botonny", "cross(es) cleche", "cross(es) cercele",
"cross(es) crosslet",
"cross(es) floretty", "cross(es) flory", "cross(es) fourche", "crosslet", "cross(es) moline", "cross(es) patonce",
"cross(es) paty floretty",
"cross(es) pointed", "cross(es) pomme", "cross(es) gammadion",
"fleur-de-lys()", "heart", "flame(s) of fire", "heart(s) crowned", "heart(s) flammant",
"Lion(s) passant", "Lion(s) rampant", "lion(s) statant", "lion(s) salient", "lion(s) sejant", "lion(s) sejant affronte",
"lion(s) sejant erect", "galley",
"lion(s) couchant", "Cornish Chough", "escallop",
"thistle", "cinquefoil", "rose", "sword", "trefoil",
"Addice", "axe", " cauldron", "chalice", "clarion", "gauntlet", "harp", "helm", "horn", "maunche", "mug", "pheon",
"scythe", "sheaf(s) of arrows", "table", "water bouget",
"hand", "arm", "tower", "arch(es)", "altar", "dove", "bee", "martlett", "stags head",
"chess rook", "castle", "ox", "fir tree", "salmon", "mill wheel", "cog wheel", "water wheel",
"estoile", "church bell", "hawk bell", "bomb", "saxon crown", "caltrap", "sun", "full moon",
    "galley",  "millwheel", "arm", "heart", "cricket", "dragonfly", "angel", "centaur",
    "cockatrice", "hydra", "salamander", "unicorn", "welsh dragon"
);

var fixed = new Array (
"bezant", "golpe", "hurt", "pellet", "plate", "pomme",  "torteau", "fountain", "rainbow"
);

var multis = new Array (
"sword(s) {colour} hilt(s) {colour} pommel(s) {colour}", "rose(s) {colour} seeded {colour} barbed {colour}",
"cornish chough(s) proper", "boars head(s) {colour} armed {colour}", "tiger(s) {colour} tongued {colour} armed {colour}",
"unicorn(s) {colour} tongued {colour} armed {colour}", "ram {colour} armed {colour} unguled {colour}",
"horse {colour} unguled {colour}", "boar {colour} armed {colour}", "bomb {colour} fired {colour}", "spear(s) proper"
);

var singles = new Array (
  "Virgin and child {colour}", "Paschal Lamb {colour}", "sun in his splendor {colour}", "tower triple towered"
);

  var colours = new Array (
  "azure",    "azure",   "azure",   "azure",
  "or",       "or",      "or",      "or",
  "vert",     "vert",    "vert",    "vert",
  "argent",   "argent",  "argent",  "argent",
  "purpure",  "purpure", "purpure", "purpure",
  "gules",    "gules",   "gules",   "gules",
  "sable",    "sable",   "sable",   "sable",
  "murrey",
  "tenne",
  "sanguine",
  "carnation",
  "brunatre",
  "cendree",
  "rose",
  "celestial azure"
  );

  // We only choose every third item, the other two are the colours to be added to the colours array so
  // they are not chosen
  var furs = new Array (
    "counter-ermine", "sable", "argent",
    "counter-potent",  "argent", "azure",
    "counter-vair", "argent", "azure",
    "ermine", "sable", "argent",
    "erminois", "sable", "or",
    "pean", "or", "argent",
    "potent",  "argent", "azure",
    "potent-counter-potent", "argent", "azure",
    "vair",  "argent", "azure",
    "vair-en-point",  "argent", "azure",
    "vair-in-pale", "argent", "azure"
  );

  var lines = new Array (
  "battled-embattled", "dancetty", "dovetailed", "embattled", "engrailed", "indented", "invected",
  "nebuly", "potenty", "raguly", "rayonny", "urdy", "wavy", "angled", "bevilled", "escartelly", "nowy",
  "arched", "double-arched", "twigged"
  );

  var treatments = new Array (
    "annuletty", "billetty",  "crusilly", "estoilly", "fleury",
    "fretty", "goutty", "grillage", "honeycombed", "lozengy", "maily",
    "masoned", "mullety", "papelonny", "checky", "scaly"
  );

var orientations = new Array (
  "fesswise", "palewise", "inverted", "reversed", "bendwise", "bendwise sinister"
);

var letters = new Array (
  "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N",
  "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
);

var arrangements = new Array (
"{3} {charge} in pale {ucolour}","{3} {charge} in pall {ucolour}",
"{3-6} {charge} in fess {ucolour}", "{2-6} {charge} in chief {ucolour}",
"{2-3} {charge} in base {ucolour}", "{2-4} {charge} in bend {ucolour}",
"{2-4} {charge} in bend sinsister {ucolour}", "{3} {charge} in pile {ucolour}",
"{5} {charge} in saltire {ucolour}", "{4} {charge} in cross {ucolour}", "{9} {charge} in orle {ucolour}",
"{2} {charge} addorsed {ucolour}", "{2} {charge} respecting each other {ucolour}",
"{4} {charge} {ucolour} in quadrangle"
);

  var divisions2 = new Array (
    "barry", "barry of 12", "barry of 20", "bendy", "bendy sinister", "chape", "chausse",
    "chevronny", "gyronny", "gyronny-of-six", "paly", "per chevron inverted",
    "per-bend", "per bend sinister", "per-chevron", "per-fess {line}",
    "per-pale {line}", "per-saltire", "pily", "quarterly", "fusily", "paly bendy", "pily bendy", "pily bendy sinister"
  );

  var divisions3 = new Array (
    "tierced in pale", "tierced-in-fess", "per pall", "per pall reversed", "per-pile",
    "tierced in bend", "tierced in bend sinister", "tierced in chevron"
  );

  var ordinaries = new Array (
    "base {voided}", "bend {line}", "bend sinister {line}", "bordure", "canton {voided}", "chevron {cotticed}", "chevron inverted",
    "chevron rompu", "chevron throughout", "chief {voided}", "chief triangular {voided}", "chief {line}",
    "cross {cotticed}", "fess {line}", "flaunch", "fret", "gore", "gyron", "inescutcheon", "square flaunch", "gusset",
    "orle", "pale {line}", "pall", "pile", "pile inverted", "quarter {voided}", "saltire {voided}",
    "tierce", "cross passant {voided}", "cross quarter pierced", "fess {cotticed}", "fess {line}", "fess {voided} {colour}",
    "cross double parted and fretty", "cross tripartite and fretty",
     "cross formy throughout", "tressure", "baton"
  );

  var diminutives4 = new Array (
    "chevronel", "chevronel(s) interlaced", "pile"
  );
  var diminutives8 = new Array (
    "bar", "bar(s) gemel", "palet", "bendlet", "scarpe"
  );


function randomVersion() {
  return version;
}

function pluralise(words) {
  if ( last_num > 1 ) { // plural
    if ( words.search(/\(/) != -1 ) {
      // remove just the brackets
      words = words.replace(/[\(\)]/g,"");
    } else { // add an 's'
      words = words.concat("s");
    }
  } else { // singular
    words = words.replace(/\(.*?\)/g,"");
  }
  return words;
}

function isDigit(c) {
  return ((c >= "0") && (c <= "9"));
}

function do_rep(word){
  rep_called = true;
  word = word.slice(1,-1);
  if ( isDigit(word.charAt(0)) ) {
    if ( isDigit(word.charAt(2)) ) {
      from = parseInt(word.charAt(0));
      to = parseInt(word.charAt(2));
      last_num = parseInt((Math.random()*(to - from + 1))+from);
      return numbers[last_num];
    } else { // only one digit found
      last_num = parseInt(word.charAt(0));
      return numbers[last_num];
    }
  } // else
  switch (word) {
    case "ucolour":
      var uc = '';
      do {
        uc = colours[parseInt(Math.random()*colours.length)];
      } while ( used_colours.match(":" + uc + ":") != null );
      used_colours = used_colours.concat(":" + uc + ":");
      return uc;
    case "colour":
      return colours[parseInt(Math.random()*colours.length)];
      break;
    case "single":
    	return " the " + singles[parseInt(Math.random()*singles.length)];
    	break;
    case "voided":
      if ( Math.random() > 0.8 ) {
        return "voided";
      } else {
        return "";
      }
    case "cotticed":
      if ( Math.random() > 0.9 ) {
        return "cotticed";
      } else if ( Math.random() > 0.9 ) {
        return "double cotticed";
      } else {
        return "";
      }
    case "fur":
      rand = parseInt(Math.random()*(furs.length/3));
      used_colours = used_colours.concat(":" + furs[(rand*3)+1] + ":" + furs[(rand*3)+2] + ":");
      return furs[rand*3];
      break;
    case "ccharge":
      num = Math.random();
      if ( num > 0.4 ) {
        return pluralise(charges[parseInt(Math.random()*charges.length)]) + " {ucolour}";
      } else if ( num > 0.2 ) {
        return pluralise(fixed[parseInt(Math.random()*fixed.length)]);
      } else if ( num > 0.1 ) {
        return pluralise(multis[parseInt(Math.random()*multis.length)]);
      } else {
        return pluralise("letter") + " " + letters[parseInt(Math.random()*letters.length)] + " {ucolour}";
      }
      break;
    case "charge":
      return pluralise(charges[parseInt(Math.random()*charges.length)]);
      break;
    case "position":
      if ( Math.random() > 0.5 ) {
        return positions[parseInt(Math.random()*positions.length)];
      } else {
        return "";
      }
      break;
    case "arrangement":
      return arrangements[parseInt(Math.random()*arrangements.length)];
      break;
    case "treatment":
      return treatments[parseInt(Math.random()*treatments.length)];
      break;
    case "line":
      if ( Math.random() > 0.5 ) {
        return lines[parseInt(Math.random()*lines.length)];
      } else {
        return "";
      }
      break;
    case "orientation":
      if ( Math.random() > 0.7 ) {
        return orientations[parseInt(Math.random()*orientations.length)];
      } else {
        return "";
      }
      break;
    case "diminutive4":
        return pluralise(diminutives4[parseInt(Math.random()*diminutives4.length)]);
      break;
    case "diminutive8":
        return pluralise(diminutives8[parseInt(Math.random()*diminutives8.length)]);
      break;
    case "sordinary": // This is used in the quiz
      if ( Math.random() > 0.3 ) {
        return "a " + ordinaries[parseInt(Math.random()*ordinaries.length)];
      } else {
        return "{1-4} {diminutive4}";
      }
      break;
    case "ordinary":
      col = "{fur}";
      fim = "";
      voided = "";
      if ( used_fur || Math.random() > 0.3 ) {
        col = "{ucolour}";
      }
      if ( Math.random() > 0.8 ) {
        voided = " voided";
      }
      if ( Math.random() > 0.8 ) {
        fim = " fimbriated {colour}";
      }
      if ( Math.random() > 0.2 ) {
        return "a " + ordinaries[parseInt(Math.random()*ordinaries.length)] + " " + col + voided + fim;
      } else if ( Math.random() < 0.5 ) {
        return "{1-4} {diminutive4} {ucolour}";
      } else {
        return "{2-8} {diminutive8} {ucolour}";
      }
      break;
    case "field":
      switch (parseInt(Math.random()*10)) {
        case 1: case 2: case 3: case 9: return "{ucolour}"; break;
        case 4: case 5: case 6: return "{treatment} {ucolour} and {ucolour}"; break;
        default:
          used_fur = true;
          return "{fur}";
      }
      break;
    case "divisions2":
        return divisions2[parseInt(Math.random()*divisions2.length)];
        break;
    case "divisions3":
        return divisions3[parseInt(Math.random()*divisions3.length)];
        break;
    case "division":
      switch (parseInt(Math.random()*10)) {
        case 1:
        case 9:
        case 8: col1 = "{ucolour}"; col2 = "{ucolour}"; col3 = "{ucolour}"; break;
        case 2: col1 = "{treatment} {ucolour} and {ucolour}"; col2 = "{ucolour}"; col3 = "{ucolour}"; break;
        case 3: col1 = "{treatment} {ucolour} and {ucolour}"; col2 = "{ucolour}"; col3 = "{fur}"; break;
        case 4: col1 = "{ucolour}"; col2 = "{fur}"; col3 = "{ucolour}"; break;
        case 5: col1 = "{treatment} {ucolour} and {ucolour}"; col2 = "{fur}"; col3 = "{ucolour}"; break;
        case 6: col1 = "{fur}"; col2 = "{ucolour}"; col3 = "{treatment} {ucolour} and {ucolour}"; break;
        case 7: col1 = "{ucolour}"; col2 = "{ucolour}"; col3 = "{fur}"; break;
        default: col1 = "{ucolour}"; col2 = "{treatment} {ucolour} and {ucolour}"; col3 = "{fur}"; break;
      }
      if ( Math.random() > 0.3 ) {
        return "{divisions2} " + col1 + " and " + col2;
      } else {
        return "{divisions3} " + col1 + ", " + col2 + " and " + col3;
      }
      break;
    default:
      return "(" + word + "?)";
  }
}

function expandBlazon( blazon ) {
  var reg = new RegExp("\{[\-a-zA-Z0-9]+\}","gm");
  do {
    rep_called = false;
    blazon = blazon.replace(reg,do_rep);
  } while (rep_called);
  return blazon;
}

function randomShield() {
  used_colours = "";
  var blazon = new String(templates[parseInt(Math.random()*templates.length)]);
  return expandBlazon(blazon);
}
