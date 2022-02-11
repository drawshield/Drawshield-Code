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

function mkGules($maxHeight) {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i <= 1000; $i += 20) {
		$retval .= "<path d=\"M $i, 0 V $maxHeight\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}


function mkCelestialAzure($maxHeight) {
    $retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\" stroke-dasharray='5,7,21,7'>\n";
    for ($i = 0; $i <= $maxHeight; $i += 40) {
        $retval .= "<path d=\"M 0,$i H 1000\"/>\n";
        $i2 = $i + 20;
        $retval .= "<path d=\"M -20,$i2 H 1040\"/>\n";
    }
    $retval .= "</g>\n";
    return $retval;
}

function mkOrange($maxHeight) {
    $maxHeightPlus = $maxHeight + 30;
    $retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\" stroke-dasharray='5,7,21,7'>\n";
    for ($i = 0; $i <= 1000; $i += 40) {
        $retval .= "<path d=\"M $i,0 V $maxHeight\"/>\n";
        $i2 = $i + 20;
        $retval .= "<path d=\"M $i2,-20 V $maxHeightPlus\"/>\n";
    }
    $retval .= "</g>\n";
    return $retval;
}

function mkCendree($maxHeight) {
    $retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\" stroke-dasharray='40,40'>\n";
    for ($i = -20; $i <= 1020; $i += 80) {
        $retval .= "<path d=\"M $i,-20 V $maxHeight\"/>\n";
    }
    for ($i = 0; $i <= $maxHeight; $i += 80) {
        $retval .= "<path d=\"M 0,$i H 1000\"/>\n";
    }
    for ($i = 20; $i <= 1020; $i += 80) {
        $retval .= "<path d=\"M $i,20 V $maxHeight\"/>\n";
    }
    for ($i = 40; $i <= $maxHeight + 40; $i += 80) {
        $retval .= "<path d=\"M -40,$i H 1000\"/>\n";
    }
    $retval .= "</g>\n";
    return $retval;
}

function mkBisque($maxHeight) {
    $maxHeightPlus = $maxHeight + 30;
    $retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\" stroke-dasharray='40,40'>\n";
    for ($i = 0; $i <= 1000; $i += 40) {
            $retval .= "<path d=\"M $i,0 V $maxHeight\"/>\n";
            $i2 = $i + 20;
            $retval .= "<path d=\"M $i2,40 V $maxHeightPlus\"/>\n";
    }
    $retval .= "</g>\n";
    return $retval;
}

function mkbrunatre($maxHeight) {
    $retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
    for ($i = 0; $i <= $maxHeight; $i += 20) {
        $retval .= "<path d=\"M $i, 0 V $maxHeight\"/>\n";
        $retval .= "<path d=\"M 0, $i l 1420, 1420\"/>\n";
        $retval .= "<path d=\"M $i, 0 l 1420, 1420\"/>\n";
        $retval .= "<path d=\"M 0, $i H $maxHeight\"/>\n";
    }
    $retval .= "</g>\n";
    return $retval;
}

function mkTenne($maxHeight) {
    $retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
    for ($i = 0; $i <= $maxHeight; $i += 20) {
        $retval .= "<path d=\"M $i, 0 V $maxHeight\"/>\n";
        $retval .= "<path d=\"M 0, $i l 1420, 1420\"/>\n";
        $retval .= "<path d=\"M $i, 0 l 1420, 1420\"/>\n";
    }
    $retval .= "</g>\n";
    return $retval;
}

function mkAzure($maxHeight) {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i <= $maxHeight; $i += 20) {
		$retval .= "<path d=\"M 0, $i H 1200\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}

function mkSable($maxHeight) {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i <= $maxHeight; $i += 20) {
		$retval .= "<path d=\"M 0, $i H 1200\"/>\n";
		$retval .= "<path d=\"M $i, 0 V $maxHeight\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}

function mkVert($maxHeight) {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i <= $maxHeight; $i += 20) {
		$retval .= "<path d=\"M 0, $i l 1420, 1420\"/>\n";
		$retval .= "<path d=\"M $i, 0 l 1420, 1420\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}

function mkRose($maxHeight) {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i < $maxHeight; $i += 40) {
		$retval .= "<path stroke-dasharray=\"15 10 40 10\" d=\"M 1200, $i l -1420, 1420\"/>\n";
        $retval .= "<path stroke-dasharray=\"15 10 40 10\" d=\"M $i, 0 l -1420, 1420\"/>\n";
        $j = $i + 20;
		$retval .= "<path stroke-dasharray=\"40 10 15 10\" d=\"M 1200, $j l -1420, 1420\"/>\n";
		$retval .= "<path stroke-dasharray=\"40 10 15 10\" d=\"M $j, 0 l -1420, 1420\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}


function mkPurpure($maxHeight) {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i < $maxHeight; $i += 20) {
		$retval .= "<path d=\"M 1200, $i l -1420, 1420\"/>\n";
		$retval .= "<path d=\"M $i, 0 l -1420, 1420\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}

function mkMurrey($maxHeight) {
	$retval = "<g fill=\"none\" stroke=\"#000000\" stroke-width=\"2\">\n";
	for ($i = 0; $i <= $maxHeight; $i += 20) {
		$retval .= "<path d=\"M 1200, $i l -1420, 1420\"/>\n";
		$retval .= "<path d=\"M $i, 0 l -1420, 1420\"/>\n";
	}
	for ($i = 0; $i <= $maxHeight; $i += 20) {
		$retval .= "<path d=\"M 0, $i l 1420, 1420\"/>\n";
		$retval .= "<path d=\"M $i, 0 l 1420, 1420\"/>\n";
	}
	$retval .= "</g>\n";
	return $retval;
}


function mkOr($maxHeight) {
	$retval = "<g stroke=\"none\" fill=\"#000000\">\n";
	for ($i = 0; $i <= $maxHeight; $i += 40) {
		for ($j = 0; $j <= $maxHeight; $j += 40) {
			$retval .= "<circle cx=\"$i\" cy=\"$j\" r=\"2\" />\n";
			$k = $i + 20;
			$l = $j + 20;
			$retval .= "<circle cx=\"$k\" cy=\"$l\" r=\"2\" />\n";
		}
	}
	$retval .= "</g>\n";
	return $retval;
}


function mkHatches($maxHeight) {
    $defs = "";
    $defs .= "<g id=\"gules\" >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkGules($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"azure\" >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkAzure($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"cendree\" >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkCendree($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"orange\" >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkOrange($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"celestialAzure\" >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkcelestialAzure($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"or\" >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkOr($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"sable\"  >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkSable($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"rose\" >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkRose($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"vert\" >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkVert($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"purpure\"  >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkPurpure($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"bisque\"  >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkBisque($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"tenne\"  >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkTenne($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"brunatre\"  >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkbrunatre($maxHeight);
    $defs .= "</g>\n";

    $defs .= "<g id=\"murrey\"  >\n";
    $defs .= '<rect x="0" y="0" width="1000" height="' . $maxHeight . '" fill="#ffffff" stroke="none" />' . "\n";
    $defs .= mkMurrey($maxHeight);
    $defs .= "</g>\n";
    return $defs;
}
