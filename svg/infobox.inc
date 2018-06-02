<?php
/**
 * Drawshield PHP Source File
 *
 * Description...
 *
 * @author Karl R. Wilcox <karl@karlwilcox.com>
 * @copyright Karl R. Wilcox 2014
 * @license http://www.apache.org/licenses/LICENSE-2.0
 * Date: 12/08/14
 * Time: 18:50
 */

class infobox {
  private $tabs;
  private $texts;
  private $ypos;
  const LINEHEIGHT = 36;
  const PAGEEND = 1080;
  const NUMTABS = 4;
  const INSET = 35;
  const LINELENGTH = 62;
  const STARTY = 120;
  const LONGBREAK = 15; // Break words longer than this across lines if needed (e.g. URLs)

  private function format($string) {
    $lines = array();
    $line_count = 0;
//    $length = 0;
    $lines[0] = '';

    $words = explode(' ',$string);

    $lineLeft = self::LINELENGTH;

    foreach($words as $word) {
      $linkStart = '';
      $linkEnd = '';
      if ( $word == '') continue;
      if (strpos ( $word, 'http:') === 0) { // looks like a URL
        $linkStart = "<tspan fill=\"blue\" text-decoration=\"underline\" onclick=\"window.open('$word','_blank')\">";
        $linkEnd = '</tspan>';
      }
      $wordLength = strlen($word);
      if ( $lineLeft - $wordLength < 0 ) {
        if ( $wordLength > self::LONGBREAK and $lineLeft > 4 ) { // longword, some space left
          $wordBreak = $lineLeft - 2;
          $lines[$line_count] .= ' ' . $linkStart . substr($word,0,$wordBreak) . '-' .$linkEnd;
          $line_count += 1;
          $lines[$line_count] = $linkStart . substr($word,$wordBreak) . $linkEnd;
          $lineLeft = self::LINELENGTH - $wordLength + $wordBreak;
        } else { // short word that doesn't fit
          $line_count += 1;
          $lines[$line_count] = $linkStart . $word . $linkEnd;
          $lineLeft = self::LINELENGTH - $wordLength - 1;
        }
      } else { // word will fit
        $lines[$line_count] .= ' ' . $linkStart . $word . $linkEnd;
        $lineLeft -= ($wordLength + 1);
      }
    }

    return $lines;
  }

  public function __construct() {
    $this->tabs = array(null, 'Tab 1', 'Tab 2', 'Tab 3', 'Tab 4');
    $this->texts = array(null,'', '', '', '');
    $this->ypos = array(null, self::STARTY, self::STARTY, self::STARTY, self::STARTY);
  }

  public function setTab($num,$tab) {
    if ( $num > 0 and $num <= self::NUMTABS ) $this->tabs[$num] = $tab;
  }

  private function setLinks($text) {
    $newText = '';
    $words = explode(' ', $text);
    foreach ($words as $word) {
      if (strpos ( $word, 'http') === 0) { // looks like a URL
        $newText .= '<a href="' . $word . '">' . $word . '</a>';
      } else {
        $newText .= htmlspecialchars($word) . ' ';
      }
    }
    return $newText;
  }

  public function addText($num,$text,$class='') {
    $start = "";
    $end = "";
    if ($class != '') {
      $start = '<g><desc class="' . $class . '">' . $this->setLinks($text) . '</desc>';
      $end = '</g>';
    }
    $lines = self::format($text);
    foreach ($lines as $line) {
      if ( $this->ypos[$num] >= self::PAGEEND ) break; // reached end of page...
      $this->texts[$num] .= $start . '<text x="'. self::INSET . '" y="' . ($this->ypos[$num] += self::LINEHEIGHT) . '" text-decoration="none" font-size="30" fill="black" >' .
      htmlspecialchars($line) . '</text>' . $end;
      $start = ''; // only do this for the first one
      $end = '';
    }
  }

  public function addHeader($num,$header,$colour='black') {
    $this->ypos[$num] += self::LINEHEIGHT / 2;
    if ($this->ypos[$num] > self::PAGEEND) return;
    $this->texts[$num] .= '<text x="'. self::INSET . '" y="' . ($this->ypos[$num] += self::LINEHEIGHT) . '" text-decoration="none" font-weight="bold" font-size="30" fill="' . $colour . '" >' . $header . '</text>';
    $this->ypos[$num] += self::LINEHEIGHT / 2;
  }

  public function getInfobox() {
    $box = '<g id="infobox" visibility="hidden">
    <rect
       width="1000"
       height="1200"
       x="0"
       y="0"
       style="fill:#ffffff;fill-opacity:1;stroke:#000000;stroke-width:3" />
    <g id="return" onclick="document.getElementById(\'infobox\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text1\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text2\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text3\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text4\').setAttribute(\'visibility\',\'hidden\');">
      <rect
         width="950"
         height="75"
         rx="10"
         ry="10"
         x="25"
         y="1100"
         style="fill:#cccccc;fill-opacity:1;stroke:#000000;stroke-width:3" />
      <text
         x="500"
         y="1150"
         xml:space="preserve"
         style="font-size:40px;font-style:normal;font-weight:normal;text-align:center;line-height:125%;letter-spacing:0px;word-spacing:0px;text-anchor:middle;fill:#000000;fill-opacity:1;stroke:none;font-family:Sans"
         ><tspan
           x="500"
           y="1150"
           id="tspan2991">Return  to  Shield  Image</tspan></text>
    </g>
    <g onclick="document.getElementById(\'text1\').setAttribute(\'visibility\',\'visible\');document.getElementById(\'text2\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text3\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text4\').setAttribute(\'visibility\',\'hidden\');"
       id="tab1">
      <path
         d="m 35,100 c 0,0 -5,0 5,0 10,0 10,0 10,-10 0,-15 0,-41.25 0,-55 0,-10 10,-10 10,-10 0,0 170,0 185,0 4.71405,0 10,0 10,10 0,15 0,40 0,55 0,0 0,10 10,10"
         id="rect2993"
         style="fill:#f9f9f9;fill-opacity:1;stroke:#000000;stroke-width:3"
          />
      <text
         x="71.269531"
         y="70.494957"
         id="text2998"
         xml:space="preserve"
         style="font-size:40px;font-style:normal;font-weight:normal;text-align:start;line-height:125%;letter-spacing:0px;word-spacing:0px;text-anchor:start;fill:#000000;fill-opacity:1;stroke:none;font-family:Sans"
         ><tspan
           x="71.269531"
           y="70.494957"
           id="tspan3000">' . $this->tabs[1] . '</tspan></text>
    </g>
    <g onclick="document.getElementById(\'text1\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text2\').setAttribute(\'visibility\',\'visible\');document.getElementById(\'text3\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text4\').setAttribute(\'visibility\',\'hidden\');"
       id="tab2">
      <path
         d="m 270,100 c 10,0 10,0 10,-10 0,-15 0,-41.25 0,-55 0,-10 10,-10 10,-10 0,0 170,0 185,0 4.71405,0 10,0 10,10 0,15 0,40 0,55 0,0 0,10 10,10"
         id="path3016"
         style="fill:#f9f9f9;fill-opacity:1;stroke:#000000;stroke-width:3"
          />
      <text
         x="301.26953"
         y="70.494957"
         id="text3018"
         xml:space="preserve"
         style="font-size:40px;font-style:normal;font-weight:normal;text-align:start;line-height:125%;letter-spacing:0px;word-spacing:0px;text-anchor:start;fill:#000000;fill-opacity:1;stroke:none;font-family:Sans"
         ><tspan
           x="301.26953"
           y="70.494957"
           id="tspan3020">' . $this->tabs[2] . '</tspan></text>
    </g>
    <g onclick="document.getElementById(\'text1\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text2\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text3\').setAttribute(\'visibility\',\'visible\');document.getElementById(\'text4\').setAttribute(\'visibility\',\'hidden\');"
       id="tab3">
      <path
         d="m 500,100 c 10,0 10,0 10,-10 0,-15 0,-41.25 0,-55 0,-10 10,-10 10,-10 0,0 170,0 185,0 4.71405,0 10,0 10,10 0,15 0,40 0,55 0,0 0,10 10,10"
         style="fill:#f9f9f9;fill-opacity:1;stroke:#000000;stroke-width:3"/>
      <text
         x="526.26953"
         y="70.494957"
         id="text3028"
         xml:space="preserve"
         style="font-size:40px;font-style:normal;font-weight:normal;text-align:start;line-height:125%;letter-spacing:0px;word-spacing:0px;text-anchor:start;fill:#000000;fill-opacity:1;stroke:none;font-family:Sans">
         <tspan
           x="526.26953"
           y="70.494957"
           id="tspan3030">' . $this->tabs[3] . '</tspan></text>
    </g>
    <g onclick="document.getElementById(\'text1\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text2\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text3\').setAttribute(\'visibility\',\'hidden\');document.getElementById(\'text4\').setAttribute(\'visibility\',\'visible\');" id="tab4">
      <path
         style="fill:#f9f9f9;fill-opacity:1;stroke:#000000;stroke-width:3"
         d="m 730,100 c 10,0 10,0 10,-10 0,-15 0,-41.25 0,-55 0,-10 10,-10 10,-10 0,0 170,0 185,0 4.71405,0 10,0 10,10 0,15 0,40 0,55 0,0 0,10 10,10"
         />
      <text
         x="761.26953"
         y="70.494957"
         id="text3036"
         xml:space="preserve"
         style="font-size:40px;font-style:normal;font-weight:normal;text-align:start;line-height:125%;letter-spacing:0px;word-spacing:0px;text-anchor:start;fill:#000000;fill-opacity:1;stroke:none;font-family:Sans"
         ><tspan
           x="761.26953"
           y="70.494957"
           id="tspan3038">' . $this->tabs[4] . '</tspan><tspan
           x="761.26953"
           y="20.494957"
           id="tspan3040" /></text>
    </g>
    <g visibility="hidden"
       id="text4">
      <path
         id="path6789"
         d="m 945,90 c 0,0 0,10 10,10 15,0 10,0 10,0 5.54,0 10,4.46 10,10 l 0,955 c 0,5.54 -4.46,10 -10,10 l -930,0 c -5.54,0 -10,-4.46 -10,-10 l 0,-955 c 0,-5.54 0,-10 10,-10 0,0 685,0 695,0 10,0 10,0 10,-10"
         style="fill:#f9f9f9;fill-opacity:1;stroke:#000000;stroke-width:3"
      />' . $this->texts[4] . '</g>
    <g visibility="hidden"
       id="text3">
      <path
         style="fill:#f9f9f9;fill-opacity:1;stroke:#000000;stroke-width:3"
         id="path3031"
         d="m 715,90 c 0,0 0,10 10,10 15,0 240,0 240,0 5.54,0 10,4.46 10,10 l 0,955 c 0,5.54 -4.46,10 -10,10 l -930,0 c -5.54,0 -10,-4.46 -10,-10 l 0,-955 c 0,-5.54 0,-10 10,-10 0,0 455,0 465,0 10,0 10,0 10,-10"
          />' . $this->texts[3] . '</g>
    <g visibility="hidden"
       id="text2">
      <path
         style="fill:#f9f9f9;fill-opacity:1;stroke:#000000;stroke-width:3"
         id="path3047"
         d="m 485,90 c 0,0 0,10 10,10 15,0 470,0 470,0 5.54,0 10,4.46 10,10 l 0,955 c 0,5.54 -4.46,10 -10,10 l -930,0 c -5.54,0 -10,-4.46 -10,-10 l 0,-955 c 0,-5.54 0,-10 10,-10 0,0 225,0 235,0 10,0 10,0 10,-10"
          />' . $this->texts[2] . '</g>
    <g
       id="text1">
      <path
         style="fill:#f9f9f9;fill-opacity:1;stroke:#000000;stroke-width:3"
         id="path3055"
         d="m 255,90 c 0,0 0,10 10,10 15,0 700,0 700,0 5.54,0 10,4.46 10,10 l 0,955 c 0,5.54 -4.46,10 -10,10 l -930,0 c -5.54,0 -10,-4.46 -10,-10 l 0,-955 c 0,-5.54 0,-10 10,-10 0,0 -5,0 5,0 10,0 10,0 10,-10"
          />' . $this->texts[1] . '</g>
  </g>';
    return $box;
  }

}

function populateInfoBox(infobox $infoBox) {
  global $options;
  global $messages;
  global $dom, $xpath;

  $infoBox->setTab(2,'(Spare)');
  $infoBox->setTab(3,'(Spare)');
  $infoBox->setTab(4,'Legal');
  $infoBox->addHeader(4,'Disclaimer:');
  $infoBox->addText(4,'Provided for education and information only. The use of heraldic devices is restricted in many countries, independently of copyright. The granting of a Coat of Arms is solely the right of the appropriate Heraldic Authority for your nationality and country of residence.');
  $infoBox->AddHeader(4,'Program Code Credits:');
  $infoBox->addText(4,'(c) Karl R. Wilcox 2010-2017' );
  $infoBox->addText(4,'Apache 2.0 License karl@karlwilcox.com');

  $infoBox->setTab(1,'Blazon');
  $infoBox->addText(1,ucfirst($dom->documentElement->getAttribute('blazonText')));
  
  if (count($items = $messages->getMessages('builtin'))) {
    $infoBox->addHeader(1,"Referenced Blazons:");
    foreach($items as $item) $infoBox->addText(1,$item);
  }
  $items = array_merge( $messages->getMessages('blazon'), $messages->getMessages('internal'));
  if (count($items)) {
    $options['notify'] = true;
    $infoBox->addHeader(1,'Errors:','red');
    foreach($items as $item) $infoBox->addText(1,$item,'error-message');
  }
  if (count($items = $messages->getMessages('warning'))) {
    $options['notify'] = true;
    $infoBox->addHeader(1,'Warnings:','orange');
    foreach($items as $item) $infoBox->addText(1,$item,'warning-message');
  }
  if (count($items = $messages->getMessages('other'))) {
    $infoBox->addHeader(1,'Other Notices:','orange');
    foreach($items as $item) $infoBox->addText(1,$item);
  }
  if (count($items = $messages->getMessages('licence'))) {
    $infoBox->addHeader(4,'Image Component Credits:');
    foreach($items as $item) $infoBox->addText(4,$item,'credits');
  }
  $infoBox->setTab(2,'Links');
  /**
   * @var DOMElement $element
   */
  foreach ( $xpath->query('//*[@link]') as $element ) { // Get every node with a link
    $infoBox->addText(2, $element->getAttribute('tokens') . ' - ' . $element->getAttribute('link'),'links');
  }
}
