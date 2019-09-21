<?php
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
