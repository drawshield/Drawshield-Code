<?php

// Fixups for English blazonry (reduce over-complex parse tree items to match blazonML)

class fixups {
    private $xml;

    public function __construct ($xml) {
        $this->xml = $xml;
    }

    public function ordinary($node) {
        // Expects to get ordinary->ord-type->ord-typeN
        // Moves ord-typeN up into ordinary, deletes unneeded children
        // echo "Ordinary called...\n";
        return $node;
    }

    public function charge($node) {
        // Move number from child node into attribute of this node
        $number = "1";
        foreach($node->childNodes as $child) {
            if ($child->nodeName == 'anynumber' || $child->nodeName == 't-number2') {
                $number = $child->getAttribute(BlazonML::A_KEYTERM);
                $node->removeChild($child);
                break;
            }
        }
        $node->setAttribute("number", $number);
        return $node;
    }

    public function objects($node) {
        // move children of ojbect up to children of objects directly
        foreach($node->childNodes as $child) {
            if ($child->nodeName == 'object') {
                $grandchild = $child->firstChild;
                $node->replaceChild($grandchild, $child);
            }
        }
        return $node;
    }

    public function tincture($node) {
        $node->setAttribute(blazonML::A_ORIGIN,"given");
        return $node;
    }

    public function bare_ordinary($node) {
        $tinc = $this->xml->createElement(blazonML::E_TINCTURE);
        $tinc->setAttribute(blazonML::A_ORIGIN,"pending");
        $node->appendChild($tinc);
        return $node;
    }
}
