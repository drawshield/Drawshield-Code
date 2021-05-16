<?php

require_once("svg/svg_feature_marker.inc");


function extract_colors(SvgFeatureMarker $marker, DOMDocument $document, $path)
{
    $colors = $marker->extract_colors_document($document);

    $palette = $marker->palette();

    foreach ( $colors as $color )
    {
        if ( !isset($palette[$color]) )
            $palette[$color] = "";
    }


    echo "<h1>Mapping:</h1>";
    echo "<p>use 'main' for the main tincutre, 'outline' for outlines, and feature names for the rest</p>";
    echo "<form method='get'><input type='hidden' value='$path' name='path' /><div class='palette'>";
    $idn = 0;
    foreach ( $palette as $color => $feature )
    {
        $id = "color_$idn";
        $idn++;
        echo "<p><label title='$color' for='$id' style='background:$color'></label><input name='color_$color' value='$feature' id='$id'/></p>";
    }

    echo '</div><button type="submit">Apply</button></form>';
}

function show_final(SvgFeatureMarker $marker, DOMDocument $document, $path)
{
    $marker->convert_document($document, true);
    echo "<h1>Processed:</h1>";
    echo "<div id='output'>" . $document->saveXml() . "</div>";

    $base = basename($path);
    echo "<button onclick='svg_element_save(document.getElementById(\"output\").querySelector(\"svg\"), \"$base\");'>Download</button>";

    echo "<h1>Proper:</h1>";
    echo "<pre>";
    $charge = basename($path, ".svg");
    $found = [];
    foreach ( $marker->palette() as $color => $feature )
    {
        if ( $feature == "outline" || in_array($feature, $found) )
            continue;

        $found[] = $feature;

        if ( $feature == "main" )
            echo "'$charge' => '$color',\n";
        else
            echo "'$charge/$feature' => '$color',\n";

    }
    echo "</pre>";

}


$path = $_GET["path"] ?? "";
$abs_path = __dir__ . "/svg/charges/" . $path;

?>
<!DOCTYPE html>
<html>
<head>
    <style>
        body {
            background: #AAAAAA;
        }
        #contents {
            max-width: 1024px;
            margin: 0 auto;
        }
        .palette {
            display: flex;
            flex-flow: row wrap;
        }
        .palette label {
            display: inline-block;
            width: 80px;
            height: 80px;
            margin: 1ex;
            vertical-align: middle;
        }
        .preview {
            max-width: 100%;
        }
        h1 {
            font-size: larger;
        }
    </style>
    <script>
        function save_url(url, filename)
        {
            var link = document.createElement("a");
            link.href = url;
            link.download = filename;
            link.style.display = "none";
            document.body.appendChild(link);
            link.click();
            document.body.removeChild(link);
        }

        function save_file(file)
        {
            var url = URL.createObjectURL(file);
            save_url(url, file.name);
            URL.revokeObjectURL(url);
        }

        function svg_element_to_file(svgdom, filename)
        {
            var serializer = new XMLSerializer();
            var content = serializer.serializeToString(svgdom);
            return new File([content], filename, {type:"image/svg+xml"});
        }

        function svg_element_save(svgdom, filename)
        {
            var file = svg_element_to_file(svgdom, filename);
            save_file(file);
        }
    </script>
</head>
<body>
<div id="contents">

<h1>File:</h1>
<form>
    <label for='path'>Charge file (eg: <tt>dragon/segreant.svg</tt>)</label>
    <input id='path' type="text" value="<?php echo $path; ?>" style="width: 100%;" name="path" />
    <button type="submit">Load</button>
</form>

<?php

if ( $path && strpos($path, "..") === false && file_exists($abs_path) && substr($path, -4) == ".svg" )
{
    $palette = [];
    foreach ( $_GET as $name => $val )
    {
        if ( $val && substr($name, 0, 6) == "color_" )
            $palette[substr($name, 6)] = $val;
    }

    $marker = new SvgFeatureMarker($palette);
    $document = new DOMDocument();
    $document->load($abs_path);

    echo "<h1>Original:</h1>";
    echo "<img src='./svg/charges/$path' class='preview' />";
    extract_colors($marker, $document, $path);

    if ( count($palette) )
        show_final($marker, $document, $path);
}

?>
</div>
</body>
</html>
