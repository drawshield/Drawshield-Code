<!DOCTYPE
    html
    PUBLIC
    "-//W3C//DTD XHTML 1.1 plus MathML 2.0 plus SVG 1.1//EN"
    "http://www.w3.org/2002/04/xhtml-math-svg/xhtml-math-svg.dtd">
<!-- Assumes shield directory is in /include, change below if not -->
<html xmlns="http://www.w3.org/1999/xhtml" lang="en">
  <head>
    <title>
      Drawshield Demonstration
    </title>
  </head>

  <body>
    <h1>
      Draw a Shield
    </h1>
    <p>
      Enter a
      <strong>
        blazon
      </strong>
      into the box and click the
      <code>
        Create!
      </code>
      button. 
    </p>
    <form action="POST" id="myform">
      <table style="width:600px" summary="shield table">
        <tr>
          <td rowspan="3" style="width: 459px; text-align:center">
            <textarea name="blazon" rows="5" cols="50"></textarea>
          </td>
          <td style="width: 81px">
            <input type="button" name="createbutton" value="Create!" style="width: 90px"/>
          </td>
        </tr>
        <tr>
          <td style="width: 90px">
            <input type="button" name="textbutton" value="Save As File" style="width: 90px"/>
          </td>
        </tr>
        <tr>
          <td style="width: 90px">
            <select name="format" size="1">
              <option value="svg" selected="selected">
                SVG
              </option>
              <option value="blazonml">
                BlazonML
              </option>
              <option value="prepro">
                Prepro
              </option>
              <option value="svgtext">
                SVG(text)
              </option>
            </select>
          </td>
        </tr>
        <tr>
          <td style="text-align:center" colspan="2">
            <div id="shieldimg">
            </div>
            <p id="shieldcaption">
              Your shield here
            </p>
          </td>
        </tr>
        <tr>
          <td style="width: 459px;">
            <textarea name="searchterm" rows="2" cols="50"></textarea>
          </td>
          <td style="width: 81px">
            <input type="button" name="searchbutton"  id="searchbutton" value="Search" style="width: 90px"/>
          </td>
        </tr>
        <tr>
          <td style="width: 459px;">
          <p>Key:</p><textarea name="keyname"  id="keyname" rows="2" cols="50"></textarea>
          </td>
          <td style="width: 81px">
            <input type="button" name="insertbutton" name="insertbutton" value="Insert" style="width: 90px" rowspan="5"/>
          </td>
        </tr>
        <tr>
          <td><p>Blazon:</p><textarea name="blazon2"  id="blazon2" rows="2" cols="50"></textarea></td>
        </tr>
        <tr>
          <td><p>Desc:</p><textarea name="description"  id="description" rows="2" cols="50"></textarea></td>
        </tr>
        <tr>
          <td><p>Source:</p><textarea name="source"  id="source" rows="2" cols="50"></textarea></td>
        </tr>
        <tr>
          <td><p>Notes:</p><textarea name="notes"  id="notes" rows="2" cols="50"></textarea></td>
        </tr>
      </table>
    </form>
    <div id="resultstable"></div>
    <?php $basedir = basename(getcwd()); ?>
    <script type="text/javascript" src="XMLHttpRequest.js"></script>
    <script type="text/javascript">
      //<![CDATA[
      var xmlhttp = new XMLHttpRequest();
      var incDir = '/include/';
      var asText;
      var useId;
      function showerrors(evt) {
       var errorbox = document.getElementById("errorbox");
       if ( errorbox.getAttribute("visibility") == "hidden" ) {
         errorbox.setAttribute("visibility","visible");
       } else {
         errorbox.setAttribute("visibility","hidden");
       }
      }
      function updateSVG() { // Simple AJAX updater
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
              newNode = document.importNode(xmlhttp.responseXML.firstChild, true);
              shieldImg.appendChild(newNode);
            } else {
              shieldImg.innerHTML = xmlhttp.responseText;
            }
          }
        }
      }
      function requestSVG(url,id) {
        useId = id;
        xmlhttp.open('GET', url, true);
        xmlhttp.onreadystatechange = updateSVG;
        xmlhttp.send(null);
      }
      // Set button actions
      document.forms['myform'].createbutton.onclick = function () {
        shieldCaption = document.getElementById('shieldcaption');
        shieldCaption.firstChild.nodeValue= document.forms['myform'].blazon.value;
        choice = document.forms['myform'].format.options.selectedIndex;
        format = document.forms['myform'].format.options[choice].value;
        requestSVG( incDir + 'shield/drawshield.php?nolog=1&format=' + format + '&blazon=' + encodeURIComponent(document.forms['myform'].blazon.value),'shieldimg');
      };
      document.forms['myform'].textbutton.onclick = function () {
        window.location.replace( incDir + 'shield/drawshield.php?asfile=1&nolog=1&blazon=' + encodeURIComponent(document.forms['myform'].blazon.value));
      };
      document.forms['myform'].insertbutton.onclick = function () {
        requestSVG( incDir + '<?php echo $basedir; ?>/dbinsert.php?keyname=' + encodeURIComponent(document.forms['myform'].keyname.value) +
                                  '&blazon=' + encodeURIComponent(document.forms['myform'].blazon2.value) +
                                  '&source=' + encodeURIComponent(document.forms['myform'].source.value) +
                                  '&description=' + encodeURIComponent(document.forms['myform'].description.value) +
                                  '&notes=' + encodeURIComponent(document.forms['myform'].notes.value),'resultstable');
      };
      document.forms['myform'].searchbutton.onclick = function () {
        requestSVG( incDir + '<?php echo $basedir; ?>/dbquery.php?term=' + encodeURIComponent(document.forms['myform'].searchterm.value),'resultstable');
      };
      // Run automatically
      window.onload=requestSVG( incDir + 'shield/drawshield.php?nolog=1&blazon=' + encodeURIComponent( 'Argent the word shield; in chief the word Your; in base the word here sable'),'shieldimg');
 //]]>
</script>
</body>
</html>
