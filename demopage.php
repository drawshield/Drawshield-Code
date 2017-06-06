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

  <body style="background:#AAAAAA;">
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
          <td rowspan="4" style="width: 459px; text-align:center">
            <textarea id="blazon" name="blazon" rows="6" cols="50" autocomplete="off" autocorrect="off" autocapitalize="off" spellcheck="false"></textarea>
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
            <input type="button" name="parsebutton" value="Parser" style="width: 90px"/>
          </td>
        </tr>
        <tr>
          <td style="width: 90px">
            <input type="button" name="referencesbutton" value="References" style="width: 90px"/></td>
        </tr>
        <tr>
          <td style="text-align:center;" colspan="2">
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
          <td style="width: 81px;">
            <input type="button" name="searchbutton"  id="searchbutton" value="Search" style="width: 90px;"/>
          </td>
        </tr>
      </table>
      <table style="border-collapse: collapse;">
        <tr>
          <th colspan="5" style="border-top:1px solid #000000;border-left:1px solid #000000;border-right:1px solid #000000;border-bottom:0px none;">Visual Appearance</th>
        </tr>
        <tr>
          <td style="text-align:center;width:100px;border-left:1px solid #000000;">Shiny &amp; New</td>
          <td style="text-align:center;width:100px;">Plain &amp; Flat</td>
          <td style="text-align:center;width:100px;">Painted Stone</td>
          <td style="text-align:center;width:100px;">Plastercast</td>
          <td style="text-align:center;width:100px;border-right:1px solid #000000;">Ink on Vellum</td>
        </tr>
        <tr>
          <td style="text-align:center;border-left:1px solid #000000;"><img src="thumbs/effects/shiny.png" height="80px" width="80px" onclick="document.getElementById('visual1').checked='checked';" /></td>
          <td style="text-align:center;"><img src="thumbs/effects/plain.png" height="80px" width="80px" onclick="document.getElementById('visual2').checked='checked';" /></td>
          <td style="text-align:center;"><img src="thumbs/effects/stonework.png" height="80px" width="80px" onclick="document.getElementById('visual3').checked='checked';" /></td>
          <td style="text-align:center;"><img src="thumbs/effects/plaster.png" height="80px" width="80px" onclick="document.getElementById('visual4').checked='checked';" /></td>
          <td style="text-align:center;border-right:1px solid #000000;"><img src="thumbs/effects/vellum.png" height="80px" width="80px" onclick="document.getElementById('visual5').checked='checked';" /></td>
        </tr>
        <tr>
          <td style="text-align:center;border-left:1px solid #000000;"><input id="visual1" type="radio" name="visual" value="shiny"  checked="checked" /></td>
          <td style="text-align:center;"><input id="visual2" type="radio" name="visual" value="none"/></td>
          <td style="text-align:center;"><input id="visual3" type="radio" name="visual" value="stonework" /></td>
          <td style="text-align:center;"><input id="visual4" type="radio" name="visual" value="plaster" /></td>
          <td style="text-align:center;border-right:1px solid #000000;"><input id="visual5" type="radio" name="visual" value="vellum" /></td>
        </tr>
        <tr>
          <th colspan="5" style="border-top:1px solid #000000;border-left:1px solid #000000;border-right:1px solid #000000;border-bottom:0px none;">Colour Scheme</th>
        </tr>
        <tr>
          <td style="text-align:center;border-left:1px solid #000000;">Drawshield</td>
          <td style="text-align:center;">Pastel</td>
          <td style="text-align:center;">Vibrant</td>
          <td style="text-align:center;">Copic</td>
          <td style="text-align:center;border-right:1px solid #000000;">Outlines</td>
        </tr>
        <tr>
          <td style="text-align:center;border-left:1px solid #000000;"><img src="thumbs/scheme/drawshield.png" height="80px" width="80px" /></td>
          <td style="text-align:center;"><img src="thumbs/scheme/pastel.png" height="80px" width="80px" /></td>
          <td style="text-align:center;"><img src="thumbs/scheme/vibrant.png" height="80px" width="80px" /></td>
          <td style="text-align:center;"><img src="thumbs/scheme/copic.png" height="80px" width="80px" /></td>
          <td style="text-align:center;border-right:1px solid #000000;"><img src="thumbs/scheme/outline.png" height="80px" width="80px" /></td>
        </tr>
        <tr>
          <td style="text-align:center;border-left:1px solid #000000;"><input type="radio" name="scheme" value="drawshield" checked="checked" /></td>
          <td style="text-align:center;"><input type="radio" name="scheme" value="pastel" /></td>
          <td style="text-align:center;"><input type="radio" name="scheme" value="vibrant" /></td>
          <td style="text-align:center;"><input type="radio" name="scheme" value="copic" /></td>
          <td style="text-align:center;border-right:1px solid #000000;"><input type="radio" name="scheme" value="nocolour" /></td>
        </tr>
        <tr>
          <th colspan="3" style="border-top:1px solid #000000;border-left:1px solid #000000;border-right:1px solid #000000;border-bottom:0px none;">&quot;Save to file&quot; Format</th>
          <th colspan="2" style="border-top:1px solid #000000;border-left:1px solid #000000;border-right:1px solid #000000;border-bottom:0px none;">Make Printable</th>
        </tr>
        <tr>
          <td style="text-align:center;border-left:1px solid #000000;"><img src="thumbs/other/dot-png.png" height="80px" width="80px" /></td>
          <td style="text-align:center;"><img src="thumbs/other/dot-svg.png" height="80px" width="80px" /></td>
          <td style="text-align:center;border-right:1px solid #000000;"><img src="thumbs/other/dot-jpg.png" height="80px" width="80px" /></td>
          <td style="text-align:center;border-right:1px solid #000000;" colspan="2"><img src="thumbs/other/printable.png" height="80px" width="80px" /></td></td>
        </tr>
        <tr>
          <td style="text-align:center;border-left:1px solid #000000;"><input type="radio" name="format" value="png" checked="checked" /></td>
          <td style="text-align:center;"><input type="radio" name="format" value="svg" /></td>
          <td style="text-align:center;border-right:1px solid #000000;"><input type="radio" name="format" value="jpg" /></td>
          <td style="text-align:center;border-right:1px solid #000000;" colspan="2"><input type="checkbox" id="printable" name="printable" value="on"/></td>
        </tr>
        <tr><td colspan="5" style="height:1px;border-bottom: 1px solid #000000;border-left: 1px solid #000000;border-right: 1px solid #000000;"></td></tr>
      </table>
    </form>
    <div id="resultstable"></div>
    <div class="do-not-print align-right"><form id="quickShield">
        <table style="width:260px;border-width:1px;border-style:solid;border-color:#050505;text-align:center">
          <tr>
            <td style="margin:auto;"><img id="quickImage" src="http://drawshield.net/include/shield/thumbs/other/yourshieldhere.png" width="250px" height="300px" alt="shield"/></td>
          </tr>
          <tr>
            <td style="margin:auto;"><textarea id="quickBlazon" name="quickBlazon" rows="6" cols="31"></textarea></td>
            </tr>
          <tr>
            <td style="margin:auto;"><input type="button" title="Update" style="width:200px" value="Draw This Shield!" onclick="quickshield('quickImage','quickBlazon');"/></td>
          </tr>
        </table>
    </form></div>
    <?php $basedir = basename(getcwd()); ?>
    <script type="text/javascript" src="XMLHttpRequest.js"></script>
    <script  type="text/javascript" src="shieldcommon.js"></script>
    <script  type="text/javascript" src="randomshield.js"></script>
   <script>   //<![CDATA[
      // Set button actions
      document.forms['myform'].createbutton.onclick = function () {randomifempty(); drawshield('drawshield.php?')};
      document.forms['myform'].textbutton.onclick = function() {randomifempty(); saveshield('drawshield.php?') };
      document.forms['myform'].searchbutton.onclick = function () {
        requestHTML( 'dbquery.php?term=' + encodeURIComponent(document.forms['myform'].searchterm.value),'resultstable');
      };
     document.forms['myform'].parsebutton.onclick = function () {
        requestHTML( 'drawshield.php?stage=parser&blazon=' + encodeURIComponent(document.forms['myform'].blazon.value),'resultstable');
      };
     document.forms['myform'].referencesbutton.onclick = function () {
        requestHTML( 'drawshield.php?stage=references&blazon=' + encodeURIComponent(document.forms['myform'].blazon.value),'resultstable');
      };
     function quickshield(target,blazon) {
       var thisimg = document.getElementById(target);
       var thisblazon = document.getElementById(blazon);
       thisimg.src = 'http://drawshield.net/include/shield/quickimage.php?size=250&outputformat=png&blazon=' + encodeURIComponent(thisblazon.value) + '&rand=' + Math.random() + '&dummy=file.png';
     }

    // Run automatically
    //window.onload=window.onload=setupshield(null,null,null);;
 //]]>
</script>
</body>
</html>
