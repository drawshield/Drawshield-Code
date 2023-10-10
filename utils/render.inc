<?php

function callSharp($sourceSVG, $format = 'png', $width = 500, $height = 600) {
    global $messages;

    $ch = curl_init();
    $post = array(
        'format' => $format,
        'width' => $width,
        'height' => $height,
        'copyright' => $messages->getCredits('copyright'),
        'svg' => $sourceSVG
    );
    curl_setopt($ch, CURLOPT_URL, "http://localhost:3000/convert");
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
    // $proportion = ($options['shape'] == 'flag') ? $options['aspectRatio'] : 1.2;
    $proportion = 1.2;
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
            $imageWidth = $options['printSize'];
            $imageHeight = $imageWidth * $proportion;
            return callSharp($sourceSVG, $targetFormat, $imageWidth, $imageHeight);
        case 'png-old':   // Deprecated, will be withdrawn soon I hope
            if (file_exists("cpulimit.txt")) {
                $limit = intval(file_get_contents("cpulimit.txt"));
                if ($limit > 0) {
                    if (!testCpuUsage($limit)) {
                        header('Content-type: text/text');
                        echo "Sorry, the server does not sufficient resources to complete your request to create a PNG download.\n";
                        echo "Please try later, or download the SVG version and convert it yourself using Inkscape.\n";
                        echo "Your blazon was: " . $options['blazon'];
                        exit(0);
                    }
                }
            }
            $dir = sys_get_temp_dir();
            $base = tempnam($dir, 'shield');
            rename($base, $base . '.svg');
            file_put_contents($base . '.svg', $sourceSVG);
            $width = $options['printSize'];
            $height = $width * $proportion;
            $result = shell_exec("java -jar /opt/bitnami/apache/etc/batik/batik-rasterizer-1.16.jar $base.svg -d $base.png -w $width -h $height");
            unlink($base . '.svg');
            $content = file_get_contents($base . '.png');
            unlink($base . '.png');
            return $content;
        default:
        case 'svg': // already there
            return $sourceSVG;
    }
}