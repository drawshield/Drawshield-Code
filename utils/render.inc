<?php

function callSharp($sourceSVG, $format = 'png', $width = 500, $height = 600) {
    global $messages, $options;

    $ch = curl_init();
    $post = array(
        'format' => $format,
        'width' => $width,
        'height' => $height,
        'copyright' => $messages->getCredits('copyright'),
        'svg' => $sourceSVG
    );
    if ($options['requestSource'] == 'shell') {
        curl_setopt($ch, CURLOPT_URL, "http://192.168.1.10:3000/convert");
    } else {
        curl_setopt($ch, CURLOPT_URL, "http://localhost:3000/convert");
    }
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: multipart/form-data'));
    curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
    // curl_setopt($ch, CURLINFO_HEADER_OUT, true);
    $output = curl_exec($ch);
    curl_close($ch);
    return $output;
}

function convertImageFormat($sourceSVG, $targetFormat) {
    global $options;

    if ($options['units'] == 'in') {
        $options['printSize'] *= 90;
    } elseif ($options['units'] == 'cm') {
        $options['printSize'] *= 35;
    }
    $proportion = ($options['shape'] == 'flag' || $options['shape'] == 'roll') ? $options['aspectRatio'] : 1.2;
    $pageWidth = $pageHeight = false;
    switch ($targetFormat) {
        case 'pdfLtr':
            $pageWidth = 765;
            $pageHeight = 990;
        // flowthrough
        case 'pdfA4':
            if (!$pageWidth) $pageWidth = 744;
            if (!$pageHeight) $pageHeight = 1051;
            $im = new Imagick();
            $im->readimageblob($sourceSVG);
            $im->setimageformat('pdf');
            $margin = 40; // bit less than 1/2"
            $maxWidth = $pageWidth - $margin - $margin;
            $imageWidth = $options['printSize'];
            if ($imageWidth > $maxWidth) $imageWidth = $maxWidth;
            $imageHeight = $imageWidth * $proportion;
            $im->scaleImage($imageWidth, $imageHeight);
            $fromBottom = $pageHeight - $margin - $margin - $imageHeight;
            $fromSide = $margin + (($pageWidth - $margin - $margin - $imageWidth) / 2);
            $im->setImagePage($pageWidth, $pageHeight, $fromSide * 0.9, $fromBottom * 0.9);
            return $im->getimageblob();
        case 'png':
        case 'jpg':
        case 'jpeg':
        case 'webp':
            // $im = new Imagick();
            // $im->readimageblob($sourceSVG);
            // $im->setimageformat($targetFormat);
            // $imageWidth = $options['printSize'];
            // $imageHeight = round($imageWidth * $proportion);
            // $im->resizeImage($imageWidth, $imageHeight, \Imagick::FILTER_LANCZOS, 1 );
            // return $im->getimageblob();
            $imageWidth = $options['printSize'];
            $imageHeight = round($imageWidth * $proportion);
            return callSharp($sourceSVG, $targetFormat, $imageWidth, $imageHeight);
        default:
        case 'svg': // already there
            return $sourceSVG;
    }
}
