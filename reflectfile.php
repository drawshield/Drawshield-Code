<?php
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
