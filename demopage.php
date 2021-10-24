<!DOCTYPE html
    PUBLIC
    "-//W3C//DTD XHTML 1.1 plus MathML 2.0 plus SVG 1.1//EN"
    "http://www.w3.org/2002/04/xhtml-math-svg/xhtml-math-svg.dtd">
<!-- Assumes shield directory is in /include, change below if not -->
<html xmlns="http://www.w3.org/1999/xhtml" lang="en">
  <head>
    <title>
      Drawshield Demonstration
    </title>
    <style>
    .style label {
        display: flex;
        flex-flow: column;
        align-items: center;
    }
    .style {
        display: grid;
        grid-template-columns: auto auto auto auto auto;
        border-left: 1px solid black;
        border-right: 1px solid black;
        padding: 1em;
    }
    .style:last-child {
        border-bottom: 1px solid black;
    }
    .style img {
        width: 80px;
        height: 80px;
    }
    .style-head {
        padding-top: 1em;
        font-weight: bold;
        text-align: center;
        border-top: 1px solid black;
        border-left: 1px solid black;
        border-right: 1px solid black;
    }
    .style-container {
        margin: 1em 0;
        width: 600px;
    }
    .custom-palette {
        display: block;
    }
    .custom-palette textarea {
        width: 100%;
        resize: horizontal;
    }
    #resultstable {
        white-space: pre;
        font-family: monospace;
    }
    #resultstable div {
        display: flex;
        flex-flow: column;
    }
    #resultstable > div div, #resultstable span {
        padding-left: 4ex;
        border-left: 1px solid rgba(0, 0, 0, 0.1);
    }
    #table-container {
        display: flex;
    }
    </style>
  </head>

<body style="background:#AAAAAA;">
    <h1>Draw a Shield</h1>
    <p>Enter a <strong>blazon</strong> into the box and click the <code>Create!</code> button.</p>
    <div id="table-container">
      <div>
      <table style="width:600px" summary="shield table">
        <tr>
          <td rowspan="6" style="width: 459px; text-align:center">
            <textarea id="blazon" name="blazon" rows="6" cols="50" autocomplete="off" autocorrect="off" autocapitalize="off" spellcheck="false"></textarea>
          </td>
          <td style="width: 81px">
            <input type="button" id="createbutton" value="Create!" style="width: 90px"/>
          </td>
        </tr>
        <tr>
          <td style="width: 90px">
            <input type="button" id="textbutton" value="Save As File" style="width: 90px"/>
          </td>
        </tr>
        <tr>
          <td style="width: 90px">
            <input type="button" id="parsebutton" value="Parser" style="width: 90px"/>
          </td>
        </tr>
        <tr>
          <td style="width: 90px">
            <input type="button" id="referencesbutton" value="References" style="width: 90px"/></td>
        </tr>
        <tr>
            <td style="width: 90px">
                <input type="button" id="random_blazon_button" value="Random" style="width: 90px"/>
            </td>
        </tr>
        <tr>
            <td style="width: 90px">
                <input type="button" id="show_ast_button" value="AST" style="width: 90px"/>
            </td>
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
            <td colspan="2">
                <div name="message" id="messageList" class="form-control"></div>
            </td>
        </tr>
        <tr>
          <td style="width: 459px;">
            <textarea id="searchterm" rows="2" cols="50"></textarea>
          </td>
          <td style="width: 81px;">
            <input type="button" name="searchbutton"  id="searchbutton" value="Search" style="width: 90px;"/>
          </td>
        </tr>
      </table><div></div></div>
      <div id="resultstable" style="display:none"></div>
    </div>
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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
 */

    $image_url = "https://drawshield.net/create/img";

    function table_items($type, $items)
    {
        global $image_url;
        $first = true;
        foreach ( $items as $item )
        {
            $slug = $item[1];
            echo "<label for='$type$slug'><span>{$item[0]}</span>";
            echo "<img src='$image_url/$slug.png' />";
            echo "<input id='$type$slug' type='radio' name='$type' value='$slug' ";
            if ( $first )
                echo "checked='checked'";
            echo " />";
            echo "</label>";
            $first = false;
        }
    }

    function table_check($title, $slug)
    {
        echo "<label for='$slug'><span>$title</span>";
        echo "<input id='$slug' type='checkbox' name='$slug' />";
        echo "</label>";
    }

    function table_cat($title, $type, $items)
    {
        echo "<div class='style-head'>$title</div>";
        echo "<div class='style'>";
        table_items($type, $items);
        echo "</div>";
    }

    echo "<div class='style-container'>";
    table_cat("Visual Appearance", "visual", [
        ["Plain &amp; Flat", "plain"],
        ["Shiny &amp; New", "shiny"],
        ["Painted Stone", "stonework"],
        ["Plastercast", "plaster"],
        ["Ink on Vellum", "vellum"],
        ["Ripples", "ripples"],
        ["Fabric", "fabric"],
        ["Inked", "inked"],
    ]);
    table_cat("Colour Scheme", "scheme", [
        ["Drawshield", "drawshield"],
        ["Wikipedia", "wikipedia"],
        ["Emoji", "emoji"],
        ["Wappenwiki", "wappenwiki"],
        ["CC3", "cc3"],
        ["Hatching", "hatching"],
        ["Outline", "outline"],
    ]);

    table_cat("Shape", "shape", [
        ["Heater", "heater"],
        ["French", "french"],
        ["Oval", "oval"],
        ["Lozenge", "lozenge"],
        ["Square", "square"],
        ["Italian", "italian"],
        ["Swiss", "swiss"],
        ["English", "english"],
        ["German", "german"],
        ["Polish", "polish"],
        ["Spanish", "spanish"],
        ["Circle", "circle"],
        ["SCA", "sca"],
        ["Swatch", "swatch"],
        ["Pauldron", "pauldron"],
        ["Stamp", "stamp"],
        ["Flag", "flag"],
    ]);

    echo "<div class='style-head'>Additional Tinctures</div>";
    echo "<div class='style'>";
    table_check("Named Web", "webcols");
    table_check("Warhammer", "whcols");
    table_check("Tartan", "tartancols");
    echo "</div>";

    echo "<div class='style-head'>Custom Palette</div>";
    echo "<div class='style custom-palette'>";

    require_once(__dir__ . "/svg/tinctures.inc");
    echo "<p>\n";
    echo "<input id='tincture_name' list='tincture_values' oninput='tincture_input(event);' autocomplete='off'/>\n";
    echo "<input id='tincture_color' type='color' />";
    echo "<button onclick='tincture_add()'>Add</button>";
    echo "<datalist id='tincture_values'>";
    $tinctures = load_tincures("drawshield", true, true, true, []);
    foreach ( $tinctures as $name => $val )
        echo "<option value='$name' data-color='$val'>$name</option>";
    echo "</datalist>";
    echo "</p>";

    echo "<textarea id='customPalette'></textarea>";
    echo "</div>";

    echo "</div>";
?>
    <table style="border-collapse: collapse;">
        <tr>
            <th colspan="3" style="border-top:1px solid #000000;border-left:1px solid #000000;border-right:1px solid #000000;border-bottom:0px none;">&quot;Save to file&quot; Format</th>
            <th colspan="2" style="border-top:1px solid #000000;border-left:1px solid #000000;border-right:1px solid #000000;border-bottom:0px none;">Make Printable</th>
        </tr>
        <tr>
            <td style="text-align:center;border-left:1px solid #000000;">PNG</td>
            <td style="text-align:center;">SVG</td>
            <td style="text-align:center;border-right:1px solid #000000;">JPG</td>
            <td style="text-align:center;border-right:1px solid #000000;" colspan="2">Printable</td></td>
        </tr>
        <tr>
            <td style="text-align:center;border-left:1px solid #000000;"><input type="radio" name="format" value="png" /></td>
            <td style="text-align:center;"><input type="radio" name="format" value="svg" checked="checked" /></td>
            <td style="text-align:center;border-right:1px solid #000000;"><input type="radio" name="format" value="jpg" /></td>
            <td style="text-align:center;border-right:1px solid #000000;" colspan="2"><input type="checkbox" id="printable" name="printable" value="on"/></td>
        </tr>
        <tr><td colspan="5" style="height:1px;border-bottom: 1px solid #000000;border-left: 1px solid #000000;border-right: 1px solid #000000;"></td></tr>
    </table>

    <script type="text/javascript" src="shieldcommon.js"></script>
    <script>   //<![CDATA[
        function tincture_input(ev)
        {
            var data_list = document.getElementById("tincture_values");
            var option = data_list.querySelector(`[value='${ev.target.value}']`);
            if ( option )
                document.getElementById("tincture_color").value = option.dataset["color"];
        }

        function tincture_add()
        {
            var name_input = document.getElementById("tincture_name");
            var color_input = document.getElementById("tincture_color");
            document.getElementById("customPalette").value +=
                `${name_input.value}=${color_input.value}\n`
            ;
            name_input.value = "";
            color_input.value = "";
        }

        function request_blazon_ml(url, id)
        {
            var xmlhttp = new XMLHttpRequest();
            if (!xmlhttp)
                return;
            xmlhttp.open('GET', url, true);
            xmlhttp.onreadystatechange = function()
            {
                if ( xmlhttp.readyState == 4 && xmlhttp.status == 200 )
                    format_blazon_ml(xmlhttp.responseXML.documentElement, id);
            };
            xmlhttp.send(null);
        }

        // Set button actions
        document.getElementById("createbutton").onclick = function () {
            drawshield('drawshield.php?', displayMessages);
        };
        document.getElementById("textbutton").onclick = function() {
            saveshield('drawshield.php?');
        };
        document.getElementById("searchbutton").onclick = function () {
            document.getElementById("resultstable").style.display = "block";
            requestHTML( 'dbquery.php?term=' + encodeURIComponent(document.getElementById("searchterm").value),'resultstable');
        };
        document.getElementById("parsebutton").onclick = function () {
            document.getElementById("resultstable").style.display = "block";
            request_blazon_ml( 'drawshield.php?stage=parser&blazon=' + encodeURIComponent(document.getElementById("blazon").value),'resultstable');
        };
        document.getElementById("referencesbutton").onclick = function () {
            document.getElementById("resultstable").style.display = "block";
            request_blazon_ml( 'drawshield.php?stage=references&blazon=' + encodeURIComponent(document.getElementById("blazon").value),'resultstable');
        };
        document.getElementById("random_blazon_button").onclick = function () {
            randomShieldCallback(function(blazon){
                document.getElementById("blazon").value = blazon;
                drawshield('drawshield.php?', displayMessages);
            });
        }
        document.getElementById("show_ast_button").onclick = function () {
            var target = document.getElementById("resultstable");
            if ( target.style.display == "none" )
                target.style.display = "block";
            else
                target.style.display = "none";
        };
    //]]>
    </script>
</body>
</html>
