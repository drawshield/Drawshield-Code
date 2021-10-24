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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */

require_once("svg_feature_marker.inc");


class SmartChargeGroup
{
    private $loaded = false;
    private $charges = [];
    private $group;
    private $search_path;
    private static $instance = null;

    private function __construct($group)
    {
        $this->group = $group;
        $this->search_path = __dir__ . DIRECTORY_SEPARATOR . "charges" . DIRECTORY_SEPARATOR . $group;
    }

    function add_patterns(languageDB $lexicon)
    {
        $this->initialize();
        foreach ( $this->charges as $charge )
            $charge->add_pattern($lexicon);

    }

    function register(SmartCharge $charge)
    {
        $this->charges[$charge->slug()] = $charge;
    }

    private function initialize()
    {
        if ( $this->loaded )
            return;

        $this->loaded = true;
        $group = $this;

        foreach ( scandir($this->search_path) as $basename )
        {
            if ( substr($basename, -4) == ".php" )
                include($this->file_path($basename));
        }
    }

    function charge($slug) : SmartCharge
    {
        return $this->charges[$slug];
    }

    function charge_data(DOMElement $node, $charge, $chargeKey)
    {
        return $this->charges[$chargeKey]->charge_data($this, $node, $charge);
    }

    static function instance() : SmartChargeGroup
    {
        if ( static::$instance === null )
            static::$instance = new SmartChargeGroup("custom");

        return static::$instance;
    }

    function file_path($basename)
    {
        return $this->search_path. DIRECTORY_SEPARATOR . $basename;
    }
}

class SmartCharge
{
    protected $regexp;
    protected $slug;

    function __construct($regexp, $slug)
    {
        $this->regexp = is_array($regexp) ? $regexp : [$regexp];
        $this->slug = $slug;
    }

    function slug()
    {
        return $this->slug;
    }

    function add_pattern(languageDB $lexicon)
    {
        foreach ( $this->regexp as $regexp )
            $lexicon->appendPattern(languageDB::CHARGE, [$regexp, "custom/" . $this->slug]);
    }

    function charge_data(SmartChargeGroup $group, DOMElement $node, $charge)
    {
        $charge['file'] = $this->slug . ".svg";
        return $charge;
    }

    function proper($feature)
    {
        return null;
    }
}

class RasterCharge extends SmartCharge
{
    private $filename;
    private $svg = null;

    function __construct($regexp, $basename)
    {
        parent::__construct($regexp, basename(explode(".", $basename)[0]));
        $this->filename = $basename;
    }


    function charge_data(SmartChargeGroup $group, DOMElement $node, $charge)
    {
        if ( $this->svg == null )
        {
            $full_path = $group->file_path($this->filename);
            $size = getimagesize($full_path);
            $width = $size[0];
            $height = $size[0];
            $mime = $size["mime"];
            $data = base64_encode(file_get_contents($full_path));
            $this->svg = <<<HERE
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<svg
   xmlns:svg="http://www.w3.org/2000/svg"
   xmlns="http://www.w3.org/2000/svg"
   xmlns:xlink="http://www.w3.org/1999/xlink"
   width="$width"
   height="$height"
   viewBox="0 0 $width $height">
    <image
       width="$width"
       height="$height"
       xlink:href="data:$mime;base64,$data"
    />
</svg>
HERE;
        }
        $charge['svgCode'] = $this->svg;
        return $charge;
    }
}

class PaletteFeatureCharge extends SmartCharge
{
    /// @var SvgFeatureMarker
    private $marker;
    private $svg = null;

    public function __construct(SvgFeatureMarker $marker, $regexp, $slug)
    {
        parent::__construct($regexp, $slug);
        $this->marker = $marker;
    }

    protected function full_path(SmartChargeGroup $group)
    {
        return $group->file_path($this->slug . ".svg");
    }

    function charge_data(SmartChargeGroup $group, DOMElement $node, $charge)
    {
        if ( $this->svg === null )
            $this->svg = $this->marker->convert_file($this->full_path($group));

        $charge['svgCode'] = $this->svg;
        return $charge;
    }


    function proper($feature)
    {
        return $this->marker->proper($feature);
    }
}
