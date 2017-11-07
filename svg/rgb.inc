<?php
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 13/08/14
 * Time: 18:08
 */

function readTinctureFile($filename) {
    $retval = array();
    foreach (file($filename) as $line) {
        // remove comments
        if (($comment = strpos($line, '//')) !== false) {
            $line = substr($line, 0, $comment);
        }
        // Now remove spaces
        $line = str_replace(' ', '', $line);
        $line = str_replace("\t", '', $line);
        if ($line != '') {
            list($key,$value) = explode('=', $line);
            if (!is_null($key) && !is_null($value)) {
                if ($value[0] != '#') {
                    $value = '#' . $value;
                }
                $retval[$key] = $value;
            }
        }
    }
    return $retval;
}

function rgb($name) {
    global $options, $messages;

    static $loaded = false;
    static $tinctures = array ();

    if (!$loaded) {
        // always read in the drawshield colours as these are the defaults for any not given
        $tinctures = readTinctureFile('svg/schemes/drawshield.txt');
        $palette = $options['palette'];
        if (!file_exists('svg/schemes/' . $palette . '.txt')) {
            $palette = 'drawshield';
            $messages->addMessage('internal', "unknown colour scheme - $palette" );
        } else {
            $tinctures = array_merge($tinctures, readTinctureFile('svg/schemes/' . $palette . '.txt'));
        }
        $loaded = true;
    }

    if (array_key_exists($name, $tinctures)) {
        return $tinctures[$name];
    } else {
        $messages->addMessage('internal', "unknown colour - $name" );
    }
    return '#ffffff';

}
