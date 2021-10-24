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
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/.
 */

$response = "No file selected";
if (isset($_FILES['blazonFile']) && ($_FILES['blazonFile']['name'] != "")) {
	$fileName = $_FILES['blazonFile']['name'];
	$fileSize = $_FILES['blazonFile']['size'];
	$fileTmpName  = $_FILES['blazonFile']['tmp_name'];
	$fileType = $_FILES['blazonFile']['type'];
	if (preg_match('/.txt$/i', $fileName) && $fileSize < 1000000) {
	  $response = file_get_contents($fileTmpName);
	} else {
		$response = "File must be .txt and less than 1Mb";
	}
}
header('Content-Type: text/plain; charset=utf-8');
echo $response;
