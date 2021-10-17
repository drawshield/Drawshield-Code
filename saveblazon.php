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
    along with Foobar.  If not, see https://www.gnu.org/licenses/.
 */

$response = "// No blazon entered";
if ((isset($_POST['blazon'])) && $_POST['blazon'] != '') {
	$response = $_POST['blazon'];
}
header("Content-type: application/force-download");
header('Content-Disposition: inline; filename="blazon.txt"');
header("Content-Transfer-Encoding: text");
header('Content-Disposition: attachment; filename="blazon.txt"');
header('Content-Type: text/plain; charset=utf-8');
echo $response;
