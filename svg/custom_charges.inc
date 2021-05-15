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
                include($this->search_path. DIRECTORY_SEPARATOR . $basename);
        }
    }

    function get_charge(DOMElement $node, $charge, $chargeKey)
    {
        return $this->charges[$chargeKey]->charge_data($node, $charge);
    }

    static function instance() : SmartChargeGroup
    {
        if ( static::$instance === null )
            static::$instance = new SmartChargeGroup("custom");

        return static::$instance;
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

    function charge_data(DOMElement $node, $charge)
    {
        $charge['file'] = $this->slug . ".svg";
        return $charge;
    }
}

class RasterCharge
{
}
