<?php

require_once("svg/svg_feature_marker.inc");
require_once("svg/custom_charges.inc");


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
    echo "<p>Use 'main' for the main tincutre, 'outline' for outlines, and feature names for the rest.</p>";
    echo "<p>You can also use 'shading' or 'outline' to make a color transparent black/white.</p>";
    echo "<div class='palette'>";
    $idn = 0;
    foreach ( $palette as $color => $feature )
    {
        $id = "color_$idn";
        $idn++;
        echo "<p><label title='$color' for='$id' style='background:$color'></label><input name='color_$color' value='$feature' id='$id'/></p>";
    }

    echo '</div><button type="submit">Apply</button>';
}

function show_final(SvgFeatureMarker $marker, DOMDocument $document, $path)
{
    $marker->convert_document($document, true);
    echo "<h1>Processed:</h1>";
    echo "<div id='output'>" . $document->saveXml() . "</div>";

    $features = [];
    foreach ( $marker->feature_colors() as $feat => $col )
        if ( $feat != "main" )
            $features[] = "<span style='color:$col'>$feat</span>";
    if ( !empty($features) )
        echo "<p>(" . implode(", ", $features) . ")</p>";

    $base = basename($path);
    echo "<button onclick='svg_element_save(document.getElementById(\"output\").querySelector(\"svg\"), \"$base\"); return false;'>Download</button>";

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

function show_preview(SvgFeatureMarker $marker, $path, $abs_path)
{
    echo "<h1>Preview:</h1>";
    echo "<p>The charge will be available as <tt>test charge</tt>.</p>";
    $blazon = trim($_GET["blazon"] ?? "argent a test charge proper");
    echo "<textarea name='blazon'>$blazon</textarea>";
    echo '<button type="submit">Draw</button>';
    echo "</form>";

    if ( $blazon )
    {
        SmartChargeGroup::instance()->register(new PreviewCharge($marker, "test charges?", "test-charge", $abs_path));

        $root = __dir__;

        global $chg_data_cache;
        global $dom;
        global $messages;
        global $options;
        global $placementData;

        global $subArg;
        global $toReverse;

        global $svg_chief;
        global $svg_region;
        global $targetColours;
        global $trace;
        global $version;
        global $xpath;

        require("$root/version.inc");
        require_once("$root/parser/utilities.inc");
        $options["blazon"] = strip_tags($blazon);
        $options["size"] = 420;
        $options["shape"] = "heater";
        $options["asFile"] = "1";
        require_once("$root/parser/parser.inc");
        $p = new parser('english');
        $dom = $p->parse($options["blazon"], 'dom');
        unset($p);
        require_once("$root/analyser/utilities.inc");
        require_once("$root/analyser/references.inc");
        $references = new references($dom);
        $dom = $references->setReferences();
        unset($references);
        $xpath = new DOMXPath($dom);
        require_once("$root/svg/draw.inc");

        $output = draw();
        unset($options);
        unset($version);
        unset($xpath);
        unset($dom);
        unset($targetColours);

        echo $output;
    }
}

class PreviewCharge extends PaletteFeatureCharge
{
    private $full_path;

    function __construct(SvgFeatureMarker $marker, $regexp, $slug, $full_path)
    {
        parent::__construct($marker, $regexp, $slug);
        $this->full_path = $full_path;
    }

    protected function full_path(SmartChargeGroup $group)
    {
        return $this->full_path;
    }
}

$path = $_GET["path"] ?? "";

if ( substr($path, 0, 7) == "http://" || substr($path, 0, 8) == "https://" )
{
    $remote = true;
    $abs_path = $path;
    $ok = true;
    $url = $path;
}
else
{
    $remote = false;
    $abs_path = __dir__ . "/svg/charges/" . $path;
    $ok = $path && strpos($path, "..") === false && file_exists($abs_path) && substr($path, -4) == ".svg";
    $url = "./svg/charges/$path";
}

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
        .palette p {
            margin: 0;
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
        textarea {
            width: 100%;
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
    <label for='path'>Charge file (eg: <tt>dragon/dragon-segreant.svg</tt>) or URL</label>
    <input id='path' type="text" value="<?php echo $path; ?>" style="width: 100%;" name="path" />
    <button type="submit">Load</button>
</form>
<?php

if ( $ok )
{
    echo "<form method='get' autocomplete='off'><input type='hidden' value='$path' name='path' />";
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
    echo "<img src='$url' class='preview' />";
    extract_colors($marker, $document, $path);

    show_preview($marker, $path, $abs_path);

    if ( count($palette) )
        show_final($marker, $document, $path);

    echo "</form>";
}

?>
</div>
</body>
</html>
