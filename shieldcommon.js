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

// Javascript shield common bits


var xmlhttp;
var asText;
var useId;
var useHTMLId;
var shieldsize = 500;
var printable = false;
var shieldtarget = 'shieldimg';
var captiontarget = 'shieldcaption';
var tabletarget = 'resultstable';
var targetURL = '/include/shield/drawshield.php?';
var messageCallback;

function updateHTML() {
	if ( xmlhttp.readyState == 4 && xmlhttp.status == 200 ) {
    var item = document.getElementById(useHTMLId);
    while ( item.hasChildNodes() ) {
      item.removeChild(item.firstChild);
    }
    if ( xmlhttp.responseXML == null ) {
      errorText = document.createTextNode(xmlhttp.responseText);
      errorPara = document.createElement('p');
      errorPara.appendChild(errorText);
      item.insertBefore(errorPara,null);
    } else {
      item.innerHTML = xmlhttp.responseText;
    }
  }
}

function updateSVG() {
	if ( xmlhttp.readyState == 4 && xmlhttp.status == 200 ) {
    var shieldImg = document.getElementById(useId);
    while ( shieldImg.hasChildNodes() ) {
      shieldImg.removeChild(shieldImg.firstChild);
    }
    if ( xmlhttp.responseXML == null ) {
      errorText = document.createTextNode(xmlhttp.responseText);
      errorPara = document.createElement('p');
      errorPara.appendChild(errorText);
      shieldImg.insertBefore(errorPara,null);
    } else {
      if ( (navigator.userAgent.indexOf( "iPad" ) > 0) ||
                  (navigator.userAgent.indexOf( "iPod" ) > 0) ||
                  (navigator.userAgent.indexOf( "iPhone" ) > 0) 
                ) {
         var svg = document.importNode(xmlhttp.responseXML.firstChild, true);
       } else if (navigator.userAgent.indexOf('Edge') > 0){
         var svg = xmlhttp.responseXML.documentElement;
         svg = cloneAndFix(svg);
       } else {
         var svg = xmlhttp.responseXML.documentElement;
         svg = cloneToDoc(svg);
         //shieldImg.innerHTML = xmlhttp.responseText;
       }
       shieldImg.appendChild(svg);
    }
    asText = xmlhttp.responseText;
    if (messageCallback != null) {
        messageCallback(xmlhttp.responseXML);
    }
  }
}

function requestHTML(url,id) {
  if (!xmlhttp) xmlhttp = new XMLHttpRequest();
  if (!xmlhttp) return;
  useHTMLId = id;
  xmlhttp.open('GET', url, true);
  xmlhttp.onreadystatechange = updateHTML;
  xmlhttp.send(null);
}

function requestSVG(url,id,messageFunc) {
  if (!xmlhttp) xmlhttp = new XMLHttpRequest();
  if (!xmlhttp) return;
  useId = id;
  messageCallback = messageFunc;
  xmlhttp.open('GET', url, true);
  xmlhttp.onreadystatechange = updateSVG;
  xmlhttp.send(null);
}

function saveshield(url) {
    if ( typeof(url) !== 'undefined' && url !== null ) targetURL = url;
   blazonText = document.getElementById('blazon').value;
   window.location.replace( targetURL + 'asfile=1' + getOptions() + '&blazon=' + encodeURIComponent(blazonText));
}

function cloneAndFix(node,doc){
    var corrections = new Array (
            'attributeName',
            'attributeType',
            'baseFrequency',
            'baseProfile',
            'calcMode',
            'clipPathUnits',
            'contentScriptType',
            'contentStyleType',
            'diffuseConstant',
            'edgeMode',
            'externalResourcesRequired',
            'filterRes',
            'filterUnits',
            'glyphRef',
            'gradientTransform',
            'gradientUnits',
            'kernelMatrix',
            'kernelUnitLength',
            'keyPoints',
            'keySplines',
            'keyTimes',
            'lengthAdjust',
            'limitingConeAngle',
            'markerHeight',
            'markerUnits',
            'markerWidth',
            'maskContentUnits',
            'maskUnits',
            'numOctaves',
            'pathLength',
            'patternContentUnits',
            'patternTransform',
            'patternUnits',
            'pointsAtX',
            'pointsAtY',
            'pointsAtZ',
            'preserveAlpha',
            'preserveAspectRatio',
            'primitiveUnits',
            'refX',
            'refY',
            'repeatCount',
            'repeatDur',
            'requiredExtensions',
            'requiredFeatures',
            'specularConstant',
            'specularExponent',
            'spreadMethod',
            'startOffset',
            'stdDeviation',
            'stitchTiles',
            'surfaceScale',
            'systemLanguage',
            'tableValues',
            'targetX',
            'targetY',
            'textLength',
            'viewBox',
            'viewTarget',
            'xChannelSelector',
            'yChannelSelector',
            'zoomAndPan'
            );
  if (!doc) doc=document;
  var clone = doc.createElementNS(node.namespaceURI,node.nodeName);
  for (var i=0,len=node.attributes.length;i<len;++i){
    var a = node.attributes[i];
    if (/^xmlns\b/.test(a.nodeName)) continue; // IE can't create these
    var validName = a.localName.toLowerCase();
    for (var j=0,len2=corrections.length;j<len2;j++) {
        if (validName === corrections[j].toLowerCase()) {
            validName = corrections[j];
            break;
        }
    }
    clone.setAttributeNS(a.namespaceURI,validName,a.nodeValue);
  }
  for (var i=0,len=node.childNodes.length;i<len;++i){
    var c = node.childNodes[i];
    clone.insertBefore(
      c.nodeType===1 ? cloneAndFix(c,doc) : doc.createTextNode(c.nodeValue),
      null
    ); }
  return clone;
}

// Function provided by http://stackoverflow.com/users/405017/phrogz
function cloneToDoc(node,doc){
  if (!doc) doc=document;
  var clone = doc.createElementNS(node.namespaceURI,node.nodeName);
  for (var i=0,len=node.attributes.length;i<len;++i){
    var a = node.attributes[i];
    if (/^xmlns\b/.test(a.nodeName)) continue; // IE can't create these
    clone.setAttributeNS(a.namespaceURI,a.nodeName,a.nodeValue);
  }
  for (var i=0,len=node.childNodes.length;i<len;++i){
    var c = node.childNodes[i];
    clone.insertBefore(
      c.nodeType===1 ? cloneToDoc(c,doc) : doc.createTextNode(c.nodeValue),
      null
    ); }
  return clone;
}

function getOptions() {
    // Look for options being set
    var options = '';
    var radioValue = 'png';
    var radioButtons = document.getElementsByName('format');
    for ( var i = 0; i < radioButtons.length; i++ ) {
        var option = radioButtons[i];
        if ( option.checked ) {
            radioValue = option.value;
            break;
        }
    }
    options += '&saveformat=' + radioValue;

    radioValue = 'drawshield';
    radioButtons = document.getElementsByName('scheme');
    for ( i = 0; i < radioButtons.length; i++ ) {
        option = radioButtons[i];
        if ( option.checked ) {
            radioValue = option.value;
            break;
        }
    }
    options += '&palette=' + radioValue;

    radioValue = 'drawshield';
    radioButtons = document.getElementsByName('shape');
    for ( i = 0; i < radioButtons.length; i++ ) {
        option = radioButtons[i];
        if ( option.checked ) {
            radioValue = option.value;
            break;
        }
    }
    options += '&shape=' + radioValue;

    radioValue = 'shiny';
    radioButtons = document.getElementsByName('visual');
    for ( i = 0; i < radioButtons.length; i++ ) {
        option = radioButtons[i];
        if ( option.checked ) {
            radioValue = option.value;
            break;
        }
    }
    options += '&effect=' + radioValue;

    option = document.getElementById('printable');
    if ( option != null && option.checked == true) {
        options += '&printable=1';
        printable = true;
    }

    for ( var misc of ["webcols", "whcols", "tartancols"] )
        if ( document.getElementById(misc).checked )
            options += "&" + misc;

    var customPaletteArea = document.getElementById("customPalette");
    var paletteItems = customPaletteArea.value.split("\n").map(x => x.split("=")).map(x => x.map(a => a.trim()));
    for ( var [key, val] of paletteItems )
    {
        if ( key == "" || val == "" )
            continue;
        if ( key.search("/") == -1 )
            key = "heraldic/" + key;
        options += `&customPalette[${encodeURIComponent(key)}]=${encodeURIComponent(val)}`;
    }
    return options;
}

function randomShieldCallback(callback)
{
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.open('GET', "randomblazon.php", true);
    xmlhttp.onreadystatechange = function() {
        if ( xmlhttp.readyState == 4 && xmlhttp.status == 200 )
            callback(xmlhttp.responseText.replace("created by", "// created by"));
    };
    xmlhttp.send(null);
}

function drawshield(url, messageFunc) {
    if ( typeof(url) !== 'undefined' && url != null ) targetURL = url;
    // Isolate blazon
   shieldCaption = document.getElementById(captiontarget);
   blazonText = document.getElementById('blazon').value;
   eol1 = blazonText.indexOf('#');
   eol2 = blazonText.indexOf('/');
   if ( eol1 == -1 ) { eol = eol2; }
   else if ( eol2 == -1 ) { eol = eol1; }
   else { eol = Math.min(eol1, eol2); }

    if ( eol != -1 ) { captionText = blazonText.slice(0,eol); }
    else { captionText = blazonText; }
    shieldCaption.firstChild.nodeValue= captionText ;
    shieldCaption.setAttribute("href","http://drawshield.net/create/?blazon=" + encodeURIComponent(captionText));
    // Add a random number to force resend (avoids caching)
    myOpts = getOptions(); // set printable as side effect
    if ( printable )
      window.open(targetURL + myOpts + '&size=1000' + '&blazon=' + encodeURIComponent(blazonText),'_blank');
    else
      requestSVG(targetURL + getOptions() + '&size=' + shieldsize + '&rand=' + Math.random()
	   + '&blazon=' + encodeURIComponent(blazonText),shieldtarget, messageFunc);
}

function dbquery() {
  searchTerm = document.getElementById('searchterm').value;
  requestHTML('/include/shield/dbquery.php?term=' + encodeURIComponent(searchTerm),tabletarget);
}

// Arguments are:
// target - name of the div element that holds the shield image
// size - horizontal size of the shield, in pixels, if null = 500
// initial - the initial blazon to use
// caption - name of the paragraph element that contains the shield caption

function setupshield(target, size, initial, caption) {
    initBlazon = "";
    if (typeof(target) !== 'undefined' && target != null) shieldtarget = target;
    if (typeof(caption) !== 'undefined' && caption != null) captiontarget = caption;
    shieldCaption = document.getElementById(captiontarget);
    if (typeof(initial) !== 'undefined' && initial != null) {
        initBlazon = '&blazon=' + encodeURIComponent(initial);
        document.getElementById('blazon').value = initial;
        shieldCaption.setAttribute("href", "http://drawshield.net/create/?blazon=" + encodeURIComponent(initial));
    } else
        initial = "Your shield here";
    if (typeof(size) !== 'undefined' && size > 0) shieldsize = size;
    shieldCaption.firstChild.nodeValue = initial;
    requestSVG('/include/shield/drawshield.php?&highlight=1&size=' + shieldsize + initBlazon, shieldtarget);
}


function displayMessages(svg) {
    var messageText = '';
    var remarksHTML = '';
    var creditHTML = '';
    var linksHTML = '';
    var errorList = svg.getElementsByTagNameNS('*','message');
    for ( var i = 0; i < errorList.length; i++ ) {
        var errorItem = errorList[i];
        var category = errorItem.getAttribute('category');
        var context = errorItem.getAttribute('context');
        var lineno = errorItem.getAttribute('linerange');
        var message = errorItem.innerHTML;
        if (context != null) message += ' ' + context;
        if (lineno != null) message += ' near ' + lineno;
        switch (category) {
            case 'licence':
                creditHTML += "<li>" + message + "</li>";
                break;
            case 'links':
                linksHTML += "<li>" + message + "</li>";
                break;
            case'warning':
                remarksHTML += "<li><span style='color:orange;'>WARNING</span> " + message + "</li>";
            break;
            case'legal':
                remarksHTML += "<li>" + message + "</li>";
                break;
            case'alert':
                remarksHTML += "<li><span style='color:red'>" + message + "</span></li>";
                break;
            default:
                messageText += message + ' ';
        }
    }
    if ( messageText.length > 0 ) {
        var messageTarget = 'messageList';
        document.getElementById(messageTarget).innerHTML = messageText;
    }
}
