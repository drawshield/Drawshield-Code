<?php
function tidyOptions() {
    global $options;

    // Tidy up options
    if (!array_key_exists('shape',$options) || $options['shape'] == 'default') {
        $options['shape'] = 'heater';
    } elseif (in_array($options['shape'],array('circular','round'))) {
        // Synonyms for circle shape
        $options['shape'] = 'circle';
    } elseif ($options['shape'] == 'flag') {
        $options['flagHeight'] = (int)(round($options['aspectRatio'] * 1000));
    }
    if ($options['palette'] == 'default') $options['palette'] = 'drawshield';
}

function testCpuUsage($limit) {
    $counts = [
        [0,0,0,0],
        [0,0,0,0],
        [0,0,0,0],
        [0,0,0,0],
    ];
    if (is_readable("/proc/stat")) {
        for ($i = 0; $i < 6; $i++) {
            $stats = file("/proc/stat", FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
            foreach ($stats as $stat) {
                $terms = preg_split('/\s+/', $stat);
                if ($terms[0] == 'cpu') {
                    $counts[$i][0] = $terms[1];
                    $counts[$i][1] = $terms[2];
                    $counts[$i][2] = $terms[3];
                    $counts[$i][3] = $terms[4];
                    break;
                }
            }
            sleep(1);
            if ($i > 1) {
                $cputime = ($counts[$i][0] - $counts[$i-1][0]) +
                    ($counts[$i][1] - $counts[$i-1][1]) +
                    ($counts[$i][2] - $counts[$i-1][2]) +
                    ($counts[$i][3] - $counts[$i-1][3]);
                $load = 100 - (($counts[$i][3] - $counts[$i-1][3]) * 100 / $cputime);
                // ds_error_log("Testing limit $limit against $load%\n");
                if ($load < $limit) return true;
            }
        }
        // Waited 5 seconds, still busy
        ds_error_log("PNG generation limited at load $load%");
        return false;
    }
    return true; // if we can't read /proc then just say it is all ok
}

function calculateAR($ar) {
    if (strpos($ar, ':') > 0) {
        $arParts = explode(':', $ar);
        $numerator = intval($arParts[0]);
        $denominator = 0;
        if (count($arParts) > 1) {
            $denominator = intval($arParts[1]);
        }
        if ($denominator == 0) $denominator = 1;
        $ar = $numerator / $denominator;
    } else {
        $ar = floatval($ar);
    }
    if ($ar > 4) {
        $ar = 4;
    } elseif ($ar < 0.25) {
        $ar = 0.25;
    }
    return $ar;
}

function arguments($args) {
    array_shift($args);
    $endofoptions = false;
    $ret = array(
        // 'commands' => array(),
        'options' => array(),
        'flags' => array(),
        'arguments' => array(),
    );
    while ($arg = array_shift($args)) {
        // if we have reached end of options,
        //we cast all remaining argvs as arguments
        if ($endofoptions) {
            $ret['arguments'][] = $arg;
            continue;
        }
        // Is it a command? (prefixed with --)
        if (substr($arg, 0, 2) === '--') {
            // is it the end of options flag?
            if (!isset($arg[2])) {
                $endofoptions = true;; // end of options;
                continue;
            }
            $value = "";
            $com = substr($arg, 2);
            // is it the syntax '--option=argument'?
            if (strpos($com, '='))
                list($com, $value) = explode("=", $com, 2);
            // is the option not followed by another option but by arguments
            elseif(strpos($args[0], '-') !== 0) {
                while (strpos($args[0], '-') !== 0)
                    $value .= array_shift($args) . ' ';
                $value = rtrim($value, ' ');
            }
            $ret['options'][$com] = !empty($value) ? $value : true;
            continue;
        }
        // Is it a flag or a serial of flags? (prefixed with -)
        if (substr($arg, 0, 1) === '-') {
            for ($i = 1; isset($arg[$i]); $i++)
                $ret['flags'][] = $arg[$i];
            continue;
        }
        // finally, it is not option, nor flag, nor argument
        //$ret['commands'][] = $arg;
        $ret['arguments'][] = $arg;
        continue;
    }
    //if (!count($ret['options']) && !count($ret['flags'])) {
    //  $ret['arguments'] = array_merge($ret['commands'], $ret['arguments']);
    //  $ret['commands'] = array();
    //}
    return $ret;
}

function display_bug(Throwable $exception) {
    echo file_get_contents("bug.svg");
    flush();
    ds_error_log($exception->getMessage() . ' ' . $exception->getFile() . ' ' . $exception->getLine());
    exit();
}

function report_errors_svg($errno, $errstr, $errfile, $errline)
{
    global $messages;
    if ($messages) {
        $dirname = __dir__;
        if (substr($errfile, -1, strlen($dirname)) == $dirname)
            $errfile = substr($errfile, strlen($dirname));
        $reportError = true;

        switch ($errno) {
            case E_ERROR:
            case E_PARSE:
            case E_CORE_ERROR:
            case E_COMPILE_ERROR:
            case E_USER_ERROR:
            case E_RECOVERABLE_ERROR:
                $err_type = "error";
                break;
            case E_COMPILE_WARNING:
            case E_WARNING:
            case E_USER_WARNING:
                $err_type = "warning";
                $reportError = false;
                break;
            default:
                $err_type = "notice";
                break;
        }

        if ($reportError)
            $messages->addMessage("$err_type", "$errfile:$errline : $errstr\n");
    }
    return false;
}

function showTimings() {
    global $timings;

    $output = 'Execution time- ';
    asort($timings);
    $keys = array_keys($timings);
    for($i = 1; $i < count($keys); $i++) {
        $output .= $keys[$i] . ': ';
        $diff = $timings[$keys[$i]] - $timings[$keys[$i -1]];
        $output .= sprintf("%.4fs ", $diff);
    }
    $total = microtime(true) - $timings[$keys[0]];
    $output .= sprintf("total: %.4fs ", $total);
    return $output;
}

function showMemory() {
    global $memory;

    $output = 'Memory use-';
    foreach($memory as $key => $size) {
        $output .= " $key: ";
        $output .= sprintf("%.2fmb", $size / 1000000);
    }
    $size = memory_get_peak_usage(true);
    $output .= " Peak: " . sprintf("%.2fmb", $size / 1000000);
    return $output;
}

function makeJsonArray($prettyPrint = false) {
    global $dom, $messages, $targetImage, $target, $jsonData, $version;

    if (!is_null($dom)) {
        $allMessages = $dom->getElementsByTagName('message');
        foreach ($allMessages as $node) {
            $messages->addMessage($node->getAttribute('category'),$node->nodeValue);
        }
        if ($prettyPrint) {
            $dom->preserveWhiteSpace = true;
            $dom->formatOutput = true;
        }
        $domText = $dom->saveXML();
        $jsonData['tree'] = $domText;
        $baggage = $dom->getElementsByTagName('input')->item(0);
        if (!is_null($baggage)) $baggage->parentNode->removeChild($baggage);
        $baggage = $dom->getElementsByTagName('messages')->item(0);
        if (!is_null($baggage)) $baggage->parentNode->removeChild($baggage);
        $minTree = $domText;
        $minTree = preg_replace('/blazonML:/', '', $minTree);
        $minTree = preg_replace('/<\?xml.*\?>\n/', '', $minTree);
        $minTree = preg_replace('/<\/?blazon.*>\n/', '', $minTree);
        $minTree = preg_replace('/[<>"]/', '', $minTree);
        $jsonData['mintree'] = $minTree;
    }
    $jsonData['messages'] = $messages->getMessages(null, 'array');
    $jsonData['release'] = $version['release'];
    $jsonData['reldate'] = $version['date'];
    if ($target == 'svg') {
        $jsonData['image'] = $targetImage;
    } else {
        $jsonData['image'] = base64_encode($targetImage);
    }
    $flags = JSON_PARTIAL_OUTPUT_ON_ERROR;
    if ($prettyPrint) $flags |= JSON_PRETTY_PRINT;
    $jsonData['memory'] = showMemory();
    $jsonData['timing'] = showTimings();
    // This is about the last point that we can take timings, so make sure to complete as
    // quickly as possible after this point.
    return json_encode($jsonData, $flags);
}

class unique {
    protected $counter = 1;
    protected $myInstance;
    protected static $instance = 1;

    public function __construct(){
        $this->myInstance = self::$instance;
        self::$instance += 1;
    }

    public function getRef($prefix = '') {
        return $prefix . $this->myInstance . '-' . $this->counter++;
    }
}

