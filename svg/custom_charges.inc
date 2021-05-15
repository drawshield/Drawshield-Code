<?php

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
        $this->charges[$charge->slug] = $charge;
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

    function get_charge(DOMElement $node, $charge, $chargeKey)
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
    function __construct($regexp, $slug)
    {
        $this->regexp = is_array($regexp) ? $regexp : [$regexp];
        $this->slug = $slug;
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
}

class RasterCharge extends SmartCharge
{
    private $filename;
    private $svg = null;

    function __construct($regexp, $basename)
    {
        parent::__construct($regexp, explode($basename, ".")[0]);
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
