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

var version = "0.9a";
var size=280;

var shieldData = {};
shieldData["~field~"] = [ "Choose the type of field",
      '~plain~', "A single colour or pattern", 'plain.png',
      '~divided~', "Divided into two or more parts", 'divided.png'
      ];
shieldData["~plain~"] = [ "Now choose a type of colour",
      '~colour~', "A plain colour", 'plain.png',
      '~treatment~',  "A field treatment", 'treatment.png',
      '~fur~',   "A fur", 'fur.png'
      ];
shieldData["~fur~"] = [ "Choose one of the furs",
      'ermine', "Ermine", 'ermine.png',
      'potent', "Potent", 'potent.png',
      'pean', "Pean", 'pean.png',
      'vair', "Vair", 'vair.png',
      'counter-ermine', "Counter-ermine", 'ermines.png',
      'vair-en-point', "Vair-en-point", 'vair-en-point.png',
      'counter-potent', "Counter-potent", 'counter-potent.png',
      'erminois', "Erminois", 'erminois.png'
      ];
shieldData["~colour~"] = [ "Choose a plain colour",
      'or', "Or", 'or.png',
      'argent', "Argent", 'argent.png',
      'azure', "Azure", 'azure.png',
      'gules', "Gules", 'gules.png',
      'purpure', "Purpure", 'purpure.png',
      'sable', "Sable", 'sable.png',
      'vert', "Vert", 'vert.png',
      'brunatre', "Brunatre", 'brunatre.png',
      'carnation', "Carnation", 'carnation.png',
      'murrey', "Murrey", 'murrey.png',
      'orange', "Orange", 'orange.png',
      'tenne', "Tenne", 'tenne.png'
      ];
shieldData["~treatment~"] = [ "Choose the type of treatment",
      'masoned ~colour~ ~and~ ~colour~', "Masoned", 'masoned.png',
      'honeycombed ~colour~ ~and~ ~colour~', "Honeycombed", 'honeycombed.png',
      'fretty ~colour~ ~and~ ~colour~', "Fretty", 'fretty.png',
      'crusilly ~colour~ ~and~ ~colour~', "Crusilly", 'crusilly.png',
      'annuletty ~colour~ ~and~ ~colour~', "Anuletty", 'annuletty.png',
      'fleury ~colour~ ~and~ ~colour~', "Fleury", 'fleury.png',
      'scaly ~colour~ ~and~ ~colour~', "Scaly", 'scaly.png',
      'lozengy ~colour~ ~and~ ~colour~', "Lozengy", 'lozengy.png'
      ];
shieldData["~and~"] = [ "Now choose another, different colour",
      'and', 'Click to continue', 'next.png',
      ];
shieldData["~ordchg~"] = [ "Do you want to add something else?",
      '~ordinary~', "An Ordinary", 'ordinary.png',
      '~diminutives~', "Some Smaller Ordinaries", 'diminutive.png',
      '~numcharges~', "Some Charges", 'charge.png',
      'on ~ordinary~ ~numcharges~', "Some charges on an ordinary", 'chgonord.png',
      'A ~charge~ ~chargeinposition~', "One charge in a set position", 'indexterchief.png',
      ];
shieldData["~chargeinposition~"] = [ "Where do you want to place the charge?",
      'in dexter chief', "In dexter chief", 'indexterchief.png',
      'in middle chief', "In middle chief", 'inmiddlechief.png',
      'in sinister chief', "In sinister chief", 'insinisterchief.png',
      'in dexter base', "In dexter base", 'indexterbase.png',
      'in middle base', "In middle base", 'inmiddlebase.png',
      'in sinister base', "In sinister base", 'insinisterbase.png',
      'in fess point', "In fess point", 'infesspoint.png',
      'in nombril', "In nombril", 'innombril.png',
      ];
shieldData["~divided~"] = [ "How do you want to divide the shield? (You will colour the black area first)",
      'per pale ~linemod~ ~plain~ and ~plain~', "Per Pale", 'perpale.png',
      'per bend ~plain~ and ~plain~', "Per Bend", 'perbend.png',
      'quarterly ~plain~ and ~plain~', "Quartely", 'quarterly.png',
      'tierced in pale ~plain~, ~plain~ and ~plain~', "Tierced in pale", 'tierced-in-pale.png',
      'per pall ~plain~, ~plain~ and ~plain~', "Per pall", 'perpall.png',
      'per chevron ~plain~ and ~plain~', "Per Chevron", 'perchevron.png',
      'barry ~plain~ and ~plain~', "Barry", 'barry.png',
      'per fess ~linemod~ ~plain~ and ~plain~', "Per Fess", 'perfess.png',
      'paly ~plain~ and ~plain~', "Paly", 'paly.png',
      'per pile ~plain~ and ~plain~', "Per pile", 'perpile.png',
      'per bend sinister ~plain~ and ~plain~', "Per Bend Sinister", 'perbendsin.png',
      'gyronny ~plain~ and ~plain~', "Gyronny", 'gyronny.png',
      ];
shieldData["~ordinary~"] = [ "Choose an ordinary to add to the shield",
      'a bend ~ordlinemod~ ~plain~', "A Bend", 'bend.png',
      'a fess ~ordlinemod~ ~plain~', "A Fess", 'fess.png',
      'a pale ~plain~', "A Pale", 'pale.png',
      'a chief ~linemod~ ~plain~', "A Chief", 'chief.png',
      'a chevron ~plain~', "A Chevron", 'chevron.png',
      'a cross ~plain~', "A Cross", 'cross.png',
      'a bordure ~plain~', "A Bordure", 'bordure.png',
      'a canton ~plain~', "A Canton", 'canton.png',
      ];
shieldData["~ordlinemod~"] = [ "How should the edges of the ordinary be drawn?",
      '', "Plain", 'perfess.png',
      'angled', "Angled", 'angled.png',
      'bevilled', "Bevilled", 'bevilled.png',
      'escartelly', "Escartelly", 'escartelly.png',
      'indented', "Indented", 'indented.png',
      ];
shieldData["~linemod~"] = [ "How should the lines or edges be drawn?",
      '', "Plain", 'perfess.png',
      'angled', "Angled", 'angled.png',
      'bevilled', "Bevilled", 'bevilled.png',
      'escartelly', "Escartelly", 'escartelly.png',
      'indented', "Indented", 'indented.png',
      'engrailed', "Engrailed", 'engrailed.png',
      'embattled', "Embattled", 'embattled.png',
      'nebuly', "Nebuly", 'nebuly.png',
      ];
shieldData["~numcharges~"] = [ "How many charges do you want to add?",
      'a ~charge~', "One", 'one.png',
      'two ~charge~', "Two", 'two.png',
      'three ~charge~', "Three", 'three.png',
      'four ~charge~', "Four", 'four.png',
      'five ~charge~', "Five", 'five.png',
      'six ~charge~', "Six", 'six.png',
      ];
shieldData["~diminutives~"] = [ "How many small ordinaries (known as diminutives) would you like to add?",
      'a ~diminutive~ ~colour~', "One", 'one.png',
      'two ~diminutive~ ~colour~', "Two", 'two.png',
      'three ~diminutive~ ~colour~', "Three", 'three.png',
      'four ~diminutive~ ~colour~', "Four", 'four.png',
      ];
shieldData["~diminutive~"] = [ "Which diminutive would you like to add?",
      'bar', "Bar", "bar.png",
      'palet', "Palet", "palet.png",
      'bendlet', "Bendlet", "bendlet.png",
      'chevronel', "Chevronel", "chevronel.png",
      'chevronel interlaced', "Chevronel Interlaced", "chevronel-interlaced.png",
      'piles', "Piles", "piles.png",
      ];
shieldData["~charge~"] = [ "Which charge would you like to add? (These are just a few examples of the many charges available).",
      'lion passant ~colour~', "Lion Passant", 'lion-passant.png',
      'mullet ~colour~', "Mullet", 'mullet.png',
      'bezant', "Bezant", 'bezant.png',
      'gauntlet ~colour~', "Gauntlet", 'gauntlet.png',
      'maltese cross ~colour~', "Maltese Cross", 'maltese-cross.png',
      'rose ~colour~ seeded argent barbed gules', "Rose", "rose.png",
      'antique crown ~colour~', "Crown", 'crown.png',
      'cornish chough proper', "Cornish Chough", 'chough.png',
      ];

function testVersion() {
  return version;
}

var bStack;
var qStack;

function init_build() {
    var form = document.forms['myform'];
    form.blazon.value="~field~";
    questions = document.getElementById("questions");
    questions.replaceChild( nextQuestion ( form.blazon.value ), questions.firstChild );
//  document.getElementById('caption').firstChild.nodeValue = "Go back one step";
    bStack = [];
    qStack = [];
    requestSVG('/include/shield/drawshield.php?&highlight=0&size=' + size + '&blazon=Argent%20the%20word%20shield;%20in%20chief%20the%20word%20Your;%20the%20word%20here%20in%20base%20sable','shieldimg');
}

function finished() {
    document.getElementById('blazon').removeAttribute('disabled');
//  document.forms['myform'].prevbutton.value="Update!";
//  document.forms['myform'].prevbutton.onclick = update;
//  document.getElementById('caption').firstChild.nodeValue = "Draw new shield";
    var questions = document.getElementById('questions');
    var para = document.createElement('p');
    var words = document.createTextNode('Congratulations! You have now completed your ' +
    'design! To make any more changes or chose different options ');
    para.appendChild(words);
    var link = document.createElement('a');
    link.setAttribute('href',"http://drawshield.net/create/?blazon=" + encodeURIComponent(form.blazon.value));
    words = document.createTextNode('click this link ');
    link.appendChild(words);
    para.appendChild(link);
    words = document.createTextNode('to transfer your blazon to the "Create" page", or click the "Refresh" button to start over. ' +
    ' This program only has a small subset of blazonry terms, but I hope it has helped you understand how blazons are constructed. ' +
        'Take a look at the help page and the suggested reading to find out more of what can be done.');
    para.appendChild(words);
    questions.replaceChild(para,questions.firstChild);
}

function goback() {
  if ( bStack.length > 0 ) {
    document.forms['myform'].blazon.value = bStack.pop();
    var questions = document.getElementById('questions');
    questions.replaceChild(qStack.pop(),questions.firstChild);
  }
}

function update() {
//  document.forms['myform'].prevbutton.value="Back";
//  document.forms['myform'].prevbutton.onclick = goback;
  form = document.forms['myform'];
  requestSVG('/include/shield/drawshield.php?nolog=1&size=' + size+ '&blazon=' + encodeURIComponent(form.blazon.value),'shieldimg');
}

function do_reset() {
  document.getElementById('blazon').disabled='disabled';
  init_build();
}

function do_replace(answer) {
  form = document.forms['myform'];
  if ( !form.blazon.hasAttribute('disabled') ) return;
  bStack.push(form.blazon.value);
  shieldImg = document.getElementById('shieldimg');
  questions = document.getElementById('questions');
  qStack.push( questions.firstChild );
  if ( form.blazon.value.indexOf("~") == -1 ) {
    form.blazon.value += ' ' + answer;
  } else {
    form.blazon.value = form.blazon.value.replace( /~.*?~/, answer );
  }
  questions.replaceChild(nextQuestion ( form.blazon.value ), questions.firstChild);
  if ( form.blazon.value.indexOf("~") == -1 ) {
    requestSVG('/include/shield/drawshield.php?nolog=1&size=' + size+ '&blazon=' + encodeURIComponent(form.blazon.value),'shieldimg');
    window.location = "#top";
  }
}

function nextQuestion( blazon ) {

  target = blazon.match(/~.*?~/);
  if ( target == null ) {
    sData = shieldData["~ordchg~"];
    } else {
    sData = shieldData[target];
  }
  retDOM = document.createElement('div');
  temp = document.createElement('h2');
  temp.appendChild(document.createTextNode(sData[0]));
  retDOM.appendChild(temp);
  tr = 0;
  table = document.createElement('table');
  table.setAttribute('border','2');
  table.setAttribute('style','z-index:100;position:relative;top:0px;');
  trow = null;
  for (var i = 1; i < sData.length; i +=3 ) {
    if ( tr == 0 ) {
      if ( trow != null ) { table.appendChild(trow); }
      trow = document.createElement('tr');
    }
    tr += 1;
    td = document.createElement('td');
    td.setAttribute('onclick','do_replace("' + sData[i] + '")');
    td.setAttribute('style','background:#EBEBE4;');
    img = document.createElement('img');
    img.setAttribute('class','align-center');
    img.setAttribute('alt','choice');
    img.setAttribute('width','120');
    img.setAttribute('height','120');
    if ( sData[i+2] != '' ) {
      img.setAttribute('src','/include/shield/thumbs/' + sData[i+2]);
    } else {
      img.setAttribute('src','/include/shield/thumbs/none.png');
    }
    td.appendChild(img);
    temp = document.createElement('p');
    temp.setAttribute('class','align-center');
    temp.appendChild(document.createTextNode(sData[i+1]));
    td.appendChild(temp);
    trow.appendChild(td);
    if ( tr == 4 ) {
      tr = 0;
    }
  }
  if ( trow.hasChildNodes() ) { table.appendChild(trow); }
  retDOM.appendChild(table);
  return retDOM;
}
window.onload=init_build();
