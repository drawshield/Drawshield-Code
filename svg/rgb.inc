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

function rgb($name) {
    global $options, $messages;

    static $loaded = false;
    static $tinctures = array ( // These are the default tinctures, that can be overwritten by palette files
        // basic tinctures
        'or' =>                '#EFD807', 
        'azure' =>             '#1E7FCB', 
        'vert' =>              '#149414', 
        'gules' =>             '#E21313', 
        'argent' =>            '#F0F0F0', 
        'sable' =>             '#050505',
        'purpure' =>           '#965578',
        // additional tinctures    
        'murrey' =>           '#8C004B',
        'sanguine' =>         '#850606',
        'carnation' =>        '#FEC3AC',
        'brunatre' =>         '#7E0001',
        'cendree' =>          '#848484',
        'rose' =>             '#FF006E',
        'bis' =>              '#F1E2BE',
        'celestial-azure' =>  '#96C8F9',
        'senois' =>           '#8D4024',
        'tenne' =>            '#A75502',
        'orange' =>           '#FAA401',
        //Other decorative parts
        'charge-stroke' =>    '#696969',
        'ordinary-stroke' =>  'none',
        'pattern-stroke' =>   'none',
        'division-stroke' =>  'none',
          // For sports fields
        'white-line' =>       '#FFFFFF',
        // Error colour
        'gray' =>             '#888888',
        // Additions by https://www.reddit.com/user/Mysterious_Italy
        'iron' =>             '#bcbcbc',
        'bronze' =>             '#c28039',
        'copper' =>             '#ba702f',
        'lead' =>             '#6b949e',
        'steel' =>             '#bdbdbd',
        'white' =>             '#ffffff',
        // other heraldic authorities
        'buff' =>           '#f0dc82',
        'red-ochre' =>      '#890520',
        'yellow-ochre' =>      '#ccaa2b',
        'crimson' =>        '#850606', //same as sanguine
    );

    if (!$loaded) {
        $palette = $options['palette'];
        if ($palette != 'drawshield') {
            if (file_exists('svg/schemes/' . $palette . '.txt')) {
                foreach (file('svg/schemes/' . $palette . '.txt') as $line) {
                    if ($line[0] != '#') {
                        list($key,$value) = explode('=', $line);
                        if (!is_null($key) && !is_null($value)) {
                            if ($value[0] != '#') {
                                $value = '#' . $value;
                            }
                            $tinctures[$key] = $value;
                        }
                    }
                }
            } else {
                $messages->addMessage('internal', "unknown colour scheme - $palette" );
            }
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
