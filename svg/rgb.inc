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

  /*
   * Sources of colour palettes as follows:
   *
   * drawshield: Sorry, I can't recall!
   *
   * wikimedia: http://commons.wikimedia.org/wiki/Illustrated_atlas_of_French_and_English_heraldic_terms#Tinctures
   *
   * vibrant: Flags of the Word - http://www.crwflags.com/fotw/flags/vxt-h1.html
   * vibrant: http://www.hagsoc.org.au/index.php?option=com_content&view=article&id=77&Itemid=86
   *
   * copic: Isabel
   */

  static $tinctures = array (
    // basic tinctures
    'or' => array              ( 'drawshield' => '#EFD807', 'copic' => '#f9f16a', 'nocolour' => '#ffffff', 'pastel' => '#fbe418',  'vibrant' => '#ffff00', ),
    'azure' => array           ( 'drawshield' => '#1E7FCB', 'copic' => '#6197c5', 'nocolour' => '#ffffff', 'pastel' => '#017ac3',  'vibrant' => '#0000ff',),
    'vert' => array            ( 'drawshield' => '#149414', 'copic' => '#85dc5a', 'nocolour' => '#ffffff', 'pastel' => '#01a479',  'vibrant' => '#009600',),
    'gules' => array           ( 'drawshield' => '#E21313', 'copic' => '#ff726e', 'nocolour' => '#ffffff', 'pastel' => '#c6223a',  'vibrant' => '#ff0000',),
    'argent' => array          ( 'drawshield' => '#F0F0F0', 'copic' => '#F0F0F0', 'nocolour' => '#ffffff', 'pastel' => '#fcfbf9',  'vibrant' => '#f0f0f0',),
    'sable' => array           ( 'drawshield' => '#050505', 'copic' => '#050505', 'nocolour' => '#F0F0F0', 'pastel' => '#1a1d26',  'vibrant' => '#000000',),
    'purpure' => array         ( 'drawshield' => '#965578', 'copic' => '#965578', 'nocolour' => '#ffffff', 'pastel' => '#855ba7',  'vibrant' => '#aa00aa',),
    // additional tinctures                      
    'murrey' => array          ( 'drawshield' => '#8C004B', 'copic' => '#b380b9', 'nocolour' => '#ffffff', 'pastel' => '#af6367',  'vibrant' => '#8C004B',),
    'sanguine' => array        ( 'drawshield' => '#850606', 'copic' => '#cb4359', 'nocolour' => '#ffffff', 'pastel' => '#cb6a4a',  'vibrant' => '#BE0000',),
    'carnation' => array       ( 'drawshield' => '#FEC3AC', 'copic' => '#f9bec4', 'nocolour' => '#ffffff', 'pastel' => '#df7486',  'vibrant' => '#ffcccc',),
    'brunatre' => array        ( 'drawshield' => '#7E0001', 'copic' => '#8a5c4d', 'nocolour' => '#ffffff', 'pastel' => '#7d4b34',),
    'cendree' => array         ( 'drawshield' => '#848484', 'copic' => '#7c7d82', 'nocolour' => '#ffffff', 'pastel' => '#8f8c87',),
    'rose' => array            ( 'drawshield' => '#FF006E', 'copic' => '#ff51b6', 'nocolour' => '#ffffff', 'pastel' => '#d05b6b', ),
    'bis' => array             ( 'drawshield' => '#F1E2BE', 'copic' => '#ffe1c9', 'nocolour' => '#ffffff', 'pastel' => '#7d4b34', ),
    'celestial-azure' => array ( 'drawshield' => '#96C8F9' ,'copic' => '#97e4ee' ,'nocolour' => '#ffffff', 'pastel' => '#e2d189', 'vibrant' => '#96c8fa',),
    'senois' => array          ( 'drawshield' => '#8D4024' ,'copic' => '#734c45' ,'nocolour' => '#ffffff', 'pastel' => '#a1915d',),
    'tenne' => array           ( 'drawshield' => '#A75502' ,'copic' => '#ffa878' ,'nocolour' => '#ffffff', 'pastel' => '#df6a3e', 'vibrant' => '#fa9632',),
    'orange' => array          ( 'drawshield' => '#FAA401' ,'copic' => '#ff972e' ,'nocolour' => '#ffffff', 'pastel' => '#f35826', 'vibrant' => '#ff8000',),
    //Other decorative parts
    'charge-stroke' => array ( 'drawshield' => '#696969', 'nocolour' => '#848484', ),
    'ordinary-stroke' => array ( 'drawshield' => 'none', 'nocolour' => '#848484', ),
    'pattern-stroke' => array ( 'drawshield' => 'none', 'nocolour' => '#848484', ),
    'division-stroke' => array ( 'drawshield' => 'none', 'nocolour' => '#848484', ),
      // For sports fields
      'white-line' => array ( 'drawshield' => '#FFFFFF', 'nocolour' => '#848484', ),
    // Error colour
    'gray' => array ( 'drawshield' => '#888888', ),
    // 'pattern-stroke' => array ( 'drawshield' => '#FF0000')
  );

  /*
   * Source for codes: http://www.w3schools.com/html/html_colornames.asp
   */
/*  static $named = array(
    // Named web colors
    'alice blue' => '#f0f8ff',
    'antique white' => '#faebd7',
    'aqua' => '#00ffff',
    'aquamarine' => '#7fffd4',
//   '#f0ffff,'azure',
    'beige' => '#f5f5dc',
//   'bisque' => '#ffe4c4',
    'black' => '#000000',
    'blanched almond' => '#ffebcd',
    'blue' => '#0000ff',
    'blue violet' => '#8a2be2',
    'brown' => '#a52a2a',
    'burly wood' => '#deb887',
    'cadet blue' => '#5f9ea0',
    'chartreuse' => '#7fff00',
    'chocolate' => '#d2691e',
    'coral' => '#ff7f50',
    'cornflower blue' => '#6495ed',
    'cornsilk' => '#fff8dc',
    'crimson' => '#dc143c',
    'cyan' => '#00ffff',
    'dark blue' => '#00008b',
    'dark cyan' => '#008b8b',
    'dark goldenrod' => '#b8860b',
    'dark gray' => '#a9a9a9',
    'dark green' => '#006400',
    'dark khaki' => '#bdb76b',
    'dark magenta' => '#8b008b',
    'dark olive green' => '#556b2f',
    'dark orange' => '#ff8c00',
    'dark orchid' => '#9932cc',
    'dark red' => '#8b0000',
    'dark salmon' => '#e9967a',
    'dark sea green' => '#8fbc8f',
    'dark slate blue' => '#483d8b',
    'dark slate gray' => '#2f4f4f',
    'dark turquoise' => '#00ced1',
    'dark violet' => '#9400d3',
    'deep pink' => '#ff1493',
    'deep sky blue' => '#00bfff',
    'dim gray' => '#696969',
    'dodger blue' => '#1e90ff',
    'fire brick' => '#b22222',
    'floral white' => '#fffaf0',
    'forest green' => '#228b22',
    'fuchsia' => '#ff00ff',
    'gainsboro' => '#dcdcdc',
    'ghost white' => '#f8f8ff',
    'gold' => '#ffd700',
    'goldenrod' => '#daa520',
    'gray' => '#808080',
    'green' => '#008000',
    'green yellow' => '#adff2f',
    'honeydew' => '#f0fff0',
    'hot pink' => '#ff69b4',
    'indian red' => '#cd5c5c',
    'indigo' => '#4b0082',
    'ivory' => '#fffff0',
    'khaki' => '#f0e68c',
    'lavender' => '#e6e6fa',
    'lavender blush' => '#fff0f5',
    'lawn green' => '#7cfc00',
    'lemon chiffon' => '#fffacd',
    'light blue' => '#add8e6',
    'light coral' => '#f08080',
    'light cyan' => '#e0ffff',
    'light goldenrod yellow' => '#fafad2',
    'light gray' => '#d3d3d3',
    'light green' => '#90ee90',
    'light pink' => '#ffb6c1',
    'light salmon' => '#ffa07a',
    'light sea green' => '#20b2aa',
    'light sky blue' => '#87cefa',
    'light slate gray' => '#778899',
    'light steel blue' => '#b0c4de',
    'light yellow' => '#ffffe0',
    'lime' => '#00ff00',
    'lime green' => '#32cd32',
    'linen' => '#faf0e6',
    'magenta' => '#ff00ff',
    'maroon' => '#800000',
    'medium aquamarine' => '#66cdaa',
    'medium blue' => '#0000cd',
    'medium orchid' => '#ba55d3',
    'medium purple' => '#9370d8',
    'medium sea green' => '#3cb371',
    'medium slate blue' => '#7b68ee',
    'medium spring green' => '#00fa9a',
    'medium turquoise' => '#48d1cc',
    'medium violet red' => '#c71585',
    'midnight blue' => '#191970',
    'mint cream' => '#f5fffa',
    'misty rose' => '#ffe4e1',
    'moccasin' => '#ffe4b5',
    'navajo white' => '#ffdead',
    'navy' => '#000080',
    'old lace' => '#fdf5e6',
    'olive' => '#808000',
    'olive drab' => '#6b8e23',
//   'orange' => '#ffa500',
    'orange red' => '#ff4500',
    'orchid' => '#da70d6',
    'pale goldenrod' => '#eee8aa',
    'pale green' => '#98fb98',
    'pale turquoise' => '#afeeee',
    'pale violet red' => '#d87093',
    'papaya whip' => '#ffefd5',
    'peach puff' => '#ffdab9',
    'peru' => '#cd853f',
    'pink' => '#ffc0cb',
    'plum' => '#dda0dd',
    'powder blue' => '#b0e0e6',
    'purple' => '#800080',
    'red' => '#ff0000',
    'rosy brown' => '#bc8f8f',
    'royal blue' => '#4169e1',
    'saddle brown' => '#8b4513',
    'salmon' => '#fa8072',
    'sandy brown' => '#f4a460',
    'sea green' => '#2e8b57',
    'sea shell' => '#fff5ee',
    'sienna' => '#a0522d',
    'silver' => '#c0c0c0',
    'sky blue' => '#87ceeb',
    'slate blue' => '#6a5acd',
    'slate gray' => '#708090',
    'snow' => '#fffafa',
    'spring green' => '#00ff7f',
    'steel blue' => '#4682b4',
    'tan' => '#d2b48c',
    'teal' => '#008080',
    'thistle' => '#d8bfd8',
    'tomato' => '#ff6347',
    'turquoise' => '#40e0d0',
    'violet' => '#ee82ee',
    'wheat' => '#f5deb3',
    'white' => '#ffffff',
    'white smoke' => '#f5f5f5',
    'yellow' => '#ffff00',
    'yellow green' => '#9acd32',
  ); */

  $palette = $options['palette'];
  if ( array_key_exists($name,$tinctures) ) {
    $palettes = $tinctures[$name];
    if ( array_key_exists($palette,$palettes))
      return $palettes[$palette];
    else
      return $palettes['drawshield'];
  }
  // Not found heraldic colour, try named web colour (but not if nocolour)

/*    if ( array_key_exists($name,$named)) {
      if ( $palette == 'nocolour' )
        return '#ffffff';
      else
        return $named[$name];
    } */
    $messages->addMessage('internal', "unknown colour - $name" );
    return '#ffffff';
    }

