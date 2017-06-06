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
var testVersion = "v0.9";

var testTemplates = new Array (
  "[field] {1} [charge] {ucolour}",
  "[division] [ordinary]",
  "[divisions3] {ucolour}, [field] and {ucolour}",
  "{ucolour} [arrangement]",
  "[field] [ordinary]",
  "[treatment] {ucolour} and {ucolour}",
  "[field] {2-6} [ccharge]",
  "[fur]",
  "[ucolour] {sordinary} [ucolour]",
  "[field] [sordinary] {ucolour}",
  "{divisions2} [ucolour] and [ucolour]",
  "[divisions2] {ucolour} and {fur}",
  "{ucolour} {1} {ccharge} [position]"
);

function getBlazons(maxChoice) {
  var brac1 = new RegExp("\\[","g");
  var brac2 = new RegExp("]","g");
  var choices = new Array();
  var i = 0;
  var blazon = new String(testTemplates[parseInt(Math.random()*testTemplates.length)]);
  used_colours = "";

  firstPass = expandBlazon(blazon);
  prev_colours = used_colours;
  firstPass = firstPass.replace( brac1, '{' );
  firstPass = firstPass.replace( brac2, '}' );

  do {
    used_colours = prev_colours;
    choice = firstPass;
    choice = expandBlazon(choice);
    same = false;
    for ( j = 0; j < i; j++ ) {
      if ( choice == choices[j] ) {
        same = true;
      }
    }
    if ( !same ) {
      choices[i] = choice;
      i += 1;
    }
  } while ( i < maxChoice );

  return choices;
}

var right = 0;
function init_quiz() {
  form = document.forms['myform'];
  questions = document.getElementById('questions');
  choices = getBlazons(3);
  right = parseInt(Math.random()*3);
  document.getElementById('Q0').firstChild.nodeValue = choices[0];
  document.getElementById('Q1').firstChild.nodeValue= choices[1];
  document.getElementById('Q2').firstChild.nodeValue= choices[2];
  requestSVG('/include/shield/drawshield.php?nolog=1&size=300&blazon=' + encodeURIComponent(choices[right]),'shieldimg');
}

function newquestion() {
  document.getElementById('I0').src = "/include/shield/thumbs/question.png";
  document.getElementById('I1').src = "/include/shield/thumbs/question.png";
  document.getElementById('I2').src = "/include/shield/thumbs/question.png";
  init_quiz();
};

function checkanswer(answer) {
  document.getElementById('I' + answer).src = (answer == right ) ? "/include/shield/thumbs/right.png" : "/include/shield/thumbs/wrong.png";
};


window.onload=init_quiz();