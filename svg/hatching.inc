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

function mkGules() {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i <= 1200; $i += 20) {
		$retval .= "<path d=\"M $i, 0 V 1200\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}


function mkCelestialAzure() {
    $retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\" stroke-dasharray='5,7,21,7'>\n";
    for ($i = 0; $i <= 1200; $i += 40) {
        $retval .= "<path d=\"M 0,$i H 1000\"/>\n";
        $i2 = $i + 20;
        $retval .= "<path d=\"M -20,$i2 H 1040\"/>\n";
    }
    $retval .= "</g>\n";
    return $retval;
}

function mkOrange() {
    $retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\" stroke-dasharray='5,7,21,7'>\n";
    for ($i = 0; $i <= 1000; $i += 40) {
        $retval .= "<path d=\"M $i,0 V 1200\"/>\n";
        $i2 = $i + 20;
        $retval .= "<path d=\"M $i2,-20 V 1230\"/>\n";
    }
    $retval .= "</g>\n";
    return $retval;
}

function mkCendree() {
    $retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\" stroke-dasharray='40,40'>\n";
    for ($i = -20; $i <= 1020; $i += 80) {
        $retval .= "<path d=\"M $i,-20 V 1200\"/>\n";
    }
    for ($i = 0; $i <= 1200; $i += 80) {
        $retval .= "<path d=\"M 0,$i H 1000\"/>\n";
    }
    for ($i = 20; $i <= 1020; $i += 80) {
        $retval .= "<path d=\"M $i,20 V 1200\"/>\n";
    }
    for ($i = 40; $i <= 1240; $i += 80) {
        $retval .= "<path d=\"M -40,$i H 1000\"/>\n";
    }
    $retval .= "</g>\n";
    return $retval;
}

function mkBisque() {
    $retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\" stroke-dasharray='40,40'>\n";
    for ($i = 0; $i <= 1000; $i += 40) {
            $retval .= "<path d=\"M $i,0 V 1200\"/>\n";
            $i2 = $i + 20;
            $retval .= "<path d=\"M $i2,40 V 1230\"/>\n";
    }
    $retval .= "</g>\n";
    return $retval;
}

function mkbrunatre() {
    $retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
    for ($i = 0; $i <= 1200; $i += 20) {
        $retval .= "<path d=\"M $i, 0 V 1200\"/>\n";
        $retval .= "<path d=\"M 0, $i l 1420, 1420\"/>\n";
        $retval .= "<path d=\"M $i, 0 l 1420, 1420\"/>\n";
        $retval .= "<path d=\"M 0, $i H 1200\"/>\n";
    }
    $retval .= "</g>\n";
    return $retval;
}

function mkTenne() {
    $retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
    for ($i = 0; $i <= 1200; $i += 20) {
        $retval .= "<path d=\"M $i, 0 V 1200\"/>\n";
        $retval .= "<path d=\"M 0, $i l 1420, 1420\"/>\n";
        $retval .= "<path d=\"M $i, 0 l 1420, 1420\"/>\n";
    }
    $retval .= "</g>\n";
    return $retval;
}

function mkAzure() {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i <= 1200; $i += 20) {
		$retval .= "<path d=\"M 0, $i H 1200\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}

function mkSable() {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i <= 1200; $i += 20) {
		$retval .= "<path d=\"M 0, $i H 1200\"/>\n";
		$retval .= "<path d=\"M $i, 0 V 1200\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}

function mkVert() {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i <= 1200; $i += 20) {
		$retval .= "<path d=\"M 0, $i l 1420, 1420\"/>\n";
		$retval .= "<path d=\"M $i, 0 l 1420, 1420\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}

function mkRose() {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i < 1200; $i += 40) {
		$retval .= "<path stroke-dasharray=\"15 10 40 10\" d=\"M 1200, $i l -1420, 1420\"/>\n";
        $retval .= "<path stroke-dasharray=\"15 10 40 10\" d=\"M $i, 0 l -1420, 1420\"/>\n";
        $j = $i + 20;
		$retval .= "<path stroke-dasharray=\"40 10 15 10\" d=\"M 1200, $j l -1420, 1420\"/>\n";
		$retval .= "<path stroke-dasharray=\"40 10 15 10\" d=\"M $j, 0 l -1420, 1420\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}


function mkPurpure() {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i < 1200; $i += 20) {
		$retval .= "<path d=\"M 1200, $i l -1420, 1420\"/>\n";
		$retval .= "<path d=\"M $i, 0 l -1420, 1420\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}

function mkMurrey() {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i <= 1200; $i += 20) {
		$retval .= "<path d=\"M 1200, $i l -1420, 1420\"/>\n";
		$retval .= "<path d=\"M $i, 0 l -1420, 1420\"/>\n";
	}
	for ($i = 0; $i <= 1200; $i += 20) {
		$retval .= "<path d=\"M 0, $i l 1420, 1420\"/>\n";
		$retval .= "<path d=\"M $i, 0 l 1420, 1420\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}


function mkOr() {
	$retval = "<g stroke=\"none\" fill=\"#000000\">\n";
	for ($i = 0; $i <= 1200; $i += 40) {
		for ($j = 0; $j <= 1200; $j += 40) {
			$retval .= "<circle cx=\"$i\" cy=\"$j\" r=\"2\" />\n";
			$k = $i + 20;
			$l = $j + 20;
			$retval .= "<circle cx=\"$k\" cy=\"$l\" r=\"2\" />\n";
		}
	}
	$retval .= "</g>\n";
	return $retval;
}


function mkHatches() {
    $defs = "";
    $defs .= "<g id=\"gules\" >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkGules();
    $defs .= "</g>\n";

    $defs .= "<g id=\"azure\" >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkAzure();
    $defs .= "</g>\n";

    $defs .= "<g id=\"cendree\" >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkCendree();
    $defs .= "</g>\n";

    $defs .= "<g id=\"orange\" >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkOrange();
    $defs .= "</g>\n";

    $defs .= "<g id=\"celestialAzure\" >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkcelestialAzure();
    $defs .= "</g>\n";

    $defs .= "<g id=\"or\" >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkOr();
    $defs .= "</g>\n";

    $defs .= "<g id=\"sable\"  >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkSable();
    $defs .= "</g>\n";

    $defs .= "<g id=\"rose\" >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkRose();
    $defs .= "</g>\n";

    $defs .= "<g id=\"vert\" >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkVert();
    $defs .= "</g>\n";

    $defs .= "<g id=\"purpure\"  >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkPurpure();
    $defs .= "</g>\n";

    $defs .= "<g id=\"bisque\"  >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkBisque();
    $defs .= "</g>\n";

    $defs .= "<g id=\"tenne\"  >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkTenne();
    $defs .= "</g>\n";

    $defs .= "<g id=\"brunatre\"  >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkbrunatre();
    $defs .= "</g>\n";

    $defs .= "<g id=\"murrey\"  >\n";
    $defs .= '<rect x="0" y="0" width="1200" height="1200" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkMurrey();
    $defs .= "</g>\n";
    return $defs;
}
