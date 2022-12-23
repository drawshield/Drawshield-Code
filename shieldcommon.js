/* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

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
            options += `&${misc}=yes`;

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


function displayMessages(svg, messageTarget="messageList", blazonTarget="resultstable", message_types=["warning", "alert", "licence", "blazon"]) {
    var messageText = '';
    var errorList = svg.getElementsByTagNameNS('*','message');
    for ( var i = 0; i < errorList.length; i++ ) {
        var errorItem = errorList[i];
        var category = errorItem.getAttribute('category');
        if ( message_types.indexOf(category) == -1 )
            continue;
        var context = errorItem.getAttribute('context');
        var lineno = errorItem.getAttribute('linerange');
        var message = errorItem.innerHTML;
        if (context != null) message += ' ' + context;
        if (lineno != null) message += ' near ' + lineno;
        switch (category) {
            case 'licence':
            case 'links':
            case 'legal':
            default:
                messageText += "<li>" + message + "</li>";
                break;
            case 'warning':
                messageText += "<li><span style='color:orange;'>WARNING</span> " + message + "</li>";
                break;
            case 'alert':
                messageText += "<li><span style='color:red'>" + message + "</span></li>";
                break;
        }
    }
    document.getElementById(messageTarget).innerHTML = "<ul>" + messageText + "</ul>";
    if ( blazonTarget )
        format_blazon_ml(svg.querySelector("blazon"), blazonTarget);
}

function format_blazon_ml(element, target_id)
{
    var target = document.getElementById(target_id);
    while ( target.hasChildNodes() )
        target.removeChild(target.firstChild);

    if ( !element )
        return;

    function add_text(target, text)
    {
        target.appendChild(document.createTextNode(text));
    }

    function add_text_element(target, tag, text)
    {
        add_text(target.appendChild(document.createElement(tag)), text);
    }

    function add_element(target, tag)
    {
        return target.appendChild(document.createElement(tag));
    }

    function format_impl(parent, element, indent)
    {
        var target = add_element(parent, "div");

        add_text_element(target, "strong", element.tagName);

        for ( var att of element.attributes )
        {
            var row = add_element(target, "span");
            add_text_element(row, "em", att.name);
            add_text(row, " = " + att.value);
        }

        for ( var child of element.childNodes )
        {
            if ( child instanceof Element )
            {
                // if ( child.tagName != "blazonML:input" )
                    format_impl(target, child, indent+1);
            }
            else if ( child instanceof Text )
            {
                var text = child.textContent.trim();
                if ( text != "" )
                {
                    add_text_element(target, "span", text);
                }
            }
        }
    }

    format_impl(target, element, 0);
}
