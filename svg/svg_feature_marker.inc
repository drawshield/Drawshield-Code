<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see https://www.gnu.org/licenses/. */


class SvgFeatureMarker
{
    private $color_mapping;
    private $id_maps;
    private $feature_colors;
    static private $avail_colors = [
        '#ffff00',
        '#ff0000',
        '#0000ff',
        '#00ff00',
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

        if ( $feature == "shading" )
            return "rgba(0, 0, 0, 0.3)";

        if ( $feature == "highlight" )
            return "rgba(255, 255, 255, 0.3)";

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
            $this->id_maps[$feature] = 1;
            return 1;
        }

        return ++$this->id_maps[$feature];
    }

    private function get_fill(DOMElement $node)
    {
        $fill = trim($node->getAttribute("fill"));
        if ( !$fill )
        {
            $style = $node->getAttribute("style");

            $matches = [];
            if ( !preg_match("/fill\s*:\s*([^;]+)/", $style, $matches) )
                return null;

            $fill = trim($matches[1]);
        }

        if ( $fill == "none" || $fill == "transparent" || !$fill )
            return null;

        return $fill;
    }

    private function apply_style(DOMElement $node, $clear_style)
    {
        $fill = $this->get_fill($node);

        if ( isset($this->color_mapping[$fill]) )
        {
            $feature = $this->color_mapping[$fill];

            $node->setAttribute("id", "$feature-" . $this->feature_id($feature));
            $node->setAttribute("fill", $this->feature_color($feature));

            if ( $clear_style && $node->hasAttribute("style") )
            {
                $style = $node->getAttribute("style");
                $node->setAttribute("style", preg_replace('/fill\s*:\s*[^;]+;?/', "", $style));
            }
        }
    }

    private function mark_node_features(DOMNode $node, $clear_style)
    {
        if ( $node->nodeType == XML_ELEMENT_NODE )
            $this->apply_style($node, $clear_style);

        foreach ( $node->childNodes as $child )
            $this->mark_node_features($child, $clear_style);
    }

    function convert_document(DOMDocument $document, $clear_style = false)
    {
        $this->feature_colors = [];
        $this->next_feature_color = 0;
        $this->id_maps = [];

        $this->mark_node_features($document, $clear_style);

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

    private function extract_colors_node(DOMNode $node, &$colors)
    {

        if ( $node->nodeType == XML_ELEMENT_NODE )
        {
            $fill = $this->get_fill($node);
            if ( $fill && !in_array($fill, $colors) )
                $colors[] = $fill;
        }

        foreach ( $node->childNodes as $child )
            $this->extract_colors_node($child, $colors);
    }

    function extract_colors_file($svg_file_path)
    {
        $document = new DOMDocument();
        $document->load($svg_file_path);
        return $this->extract_colors_document($document);
    }

    function extract_colors_document(DOMDocument $document)
    {
        $colors = [];
        $this->extract_colors_node($document, $colors);
        return $colors;
    }

    function palette()
    {
        return $this->color_mapping;
    }

    function set_palette($palette)
    {
        $this->color_mapping = $palette;
    }

    function feature_colors()
    {
        return $this->feature_colors;
    }

    function set_credits(DOMDocument $document, $credits)
    {
        if ( !$credits )
            return;
        $dc = "http://purl.org/dc/elements/1.1/";
        $cc = "http://creativecommons.org/ns#";
        $rdf = "http://www.w3.org/1999/02/22-rdf-syntax-ns#";
        $document->documentElement->setAttribute("xmlns:dc", $dc);
        $document->documentElement->setAttribute("xmlns:cc", $cc);
        $document->documentElement->setAttribute("xmlns:rdf", $rdf);
        $meta = $document->documentElement->appendChild($document->createElement("metadata"));
        $rdf = $meta->appendChild($document->createElementNS($rdf, "RDF"));
        $work = $rdf->appendChild($document->createElementNS($cc, "Work"));
        $creator = $work->appendChild($document->createElementNS($dc, "creator"));
        $agent = $creator->appendChild($document->createElementNS($cc, "Agent"));
        $title = $agent->appendChild($document->createElementNS($dc, "title"));
        $title->appendChild($document->createTextNode($credits));
    }
}
