<?php


class SvgFeatureMarker
{
    private $color_mapping;
    private $id_maps;
    private $feature_colors;
    static private $avail_colors = [
        '#ffff00',
        '#ff0000',
        '#00ff00',
        '#0000ff',
        '#00ffff',
        '#ff00ff',
    ];
    private $next_feature_color;

    function __construct($color_mapping)
    {
        $this->color_mapping = $color_mapping;
    }

    private function feature_color($feature)
    {
        if ( $feature == "outline" )
            return "#000";

        if ( isset($this->feature_colors[$feature]) )
            return $this->feature_colors[$feature];

        $color = self::$avail_colors[$this->next_feature_color];
        $this->feature_colors[$feature] = $color;
        $this->next_feature_color = ($this->next_feature_color + 1) % count(self::$avail_colors);
        return $color;
    }

    private function feature_id($feature)
    {
        if ( !isset($this->id_maps[$feature]) )
        {
            $id_maps[$feature] = 1;
            return 1;
        }

        return ++$id_maps[$feature];
    }

    private function apply_style(DOMElement $node)
    {
        $fill = trim($node->getAttribute("fill"));
        if ( !$fill )
        {
            $style = $node->getAttribute("style");

            $matches = [];
            if ( !preg_match("/fill\s*:\s*([^;]+)/", $style, $matches) )
                return;

            $fill = trim($matches[1]);
        }

        if ( $fill == "none" )
            return;

        if ( isset($this->color_mapping[$fill]) )
        {
            $feature = $this->color_mapping[$fill];

            $node->setAttribute("id", "$feature-" . $this->feature_id($feature));
            $node->setAttribute("fill", $this->feature_color($feature));
        }
    }

    private function mark_node_features(DOMNode $node)
    {
        if ( $node->nodeType == XML_ELEMENT_NODE )
            $this->apply_style($node);

        foreach ( $node->childNodes as $child )
            $this->mark_node_features($child);
    }

    function convert_document(DOMDocument $document)
    {
        $this->feature_colors = [];
        $this->next_feature_color = 0;
        $this->id_maps = [];

        $this->mark_node_features($document);

        return $document;
    }

    function convert_file($svg_file_path)
    {
        $document = new DOMDocument();
        $document->load($svg_file_path);
        return $this->convert_document($document)->saveXml();
    }

    function proper($feature)
    {
        if ( $feature === null )
            $feature = "main";

        foreach ( $this->color_mapping as $color => $mapped_feature )
            if ( $feature == $mapped_feature )
                return $color;

        return null;
    }
}
