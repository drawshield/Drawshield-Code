<?php /* Copyright 2010 Karl R. Wilcox

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */
   
   // modify a (1000x1000) placement to fit within the given bounding box
   // TODO Deal with any arrangment that extends below 1000 (is this only in pale...?)
   function adjustPlacement($placement,$boundingBox) {
     if ( $boundingBox == 'x' ) return $placement; // dummy entry to give multiple locations
     $newPlacement = '';
     $numberGroups = explode('/',$placement);
     list($bbx,$bby,$bbw,$bbh) = explode(',',$boundingBox);
     $xScale = $bbw / 1000;
     $yScale = $bbh / 1000;
     foreach ( $numberGroups as $numberGroup ) {
       if ( strlen($numberGroup) < 2 ) { // i.e. if empty or just a single character like 'x'
         $newPlacement .= $numberGroup . '/';
         continue;
       } // else
       $numbers = explode(':',$numberGroup);
       foreach ( $numbers as $number ) {
         list ( $x,$y,$w,$h,$rot,$inv,$rev ) = explode ( ',', $number . ',,,,,,,');
         $newPlacement .= sprintf("%.2f",$bbx + ($x * $xScale)); // new x
         $newPlacement .= ',';
         if ( $y != '' ) $newPlacement .= sprintf("%.2f", $bby + ( $y * $yScale));
         $newPlacement .= ',';
         if ( $w != '' ) $newPlacement .= sprintf("%.2f", $w * $xScale);
         $newPlacement .= ',';
         if ( $h != '' ) $newPlacement .= sprintf("%.2f", $h * $yScale);
         if ("$rot$inv$rev" != "") {
            $newPlacement .= ",$rot";
            $newPlacement .= ",$inv";
            $newPlacement .= ",$rev";
         }
         $newPlacement .= ':';
       }
       // Strip off last ':', add a slash
       $newPlacement = rtrim($newPlacement,':') . '/';
     }
     // strip off last '/'
     $newPlacement = rtrim($newPlacement,'/');
     return $newPlacement;
   }
   //TODO above / below modifiers

function get_placement(DOMElement $item) {
  global $dom, $xpath, $messages;
  global $aspect_ratio;

  static $bounding_boxes = array (
    //                 no-chief, no-entire,       no-chief, entire        chief, no-entire,         chief, entire
    '10:12' => array ( '100,100,800,900',         '0,0,1000,1200',        '150,350,700,700',        '0,300,1000,900' ),
    '10:10' => array ( '100,100,800,800',         '0,0,1000,1000',        '100,400,800,500',        '0,300,1000,900' ),
    'square' => array ( '0,0,1000,1000',          '0,0,1000,1000',        '0,0,1000,1000',          '0,0,1000,1000' ),
    '10:14' => array ( '170,100,660,900',         '70,0,860,1200',        '170,400,660,600',        '70,300,860,900' ),
    '10:24' => array ( '300,100,400,1000',        '250,0,500,1200',        '300,400,400,700',        '250,300,500,900' ),
    '10:20' => array ( '300,100,400,800',         '250,0,500,1000',        '300,400,400,500',        '250,300,500,700' ),
    '10:28' => array ( '300,100,400,900',         '285,0,430,1200',        '300,400,400,600',        '285,300,430,900' ),
    );
  
/*  $aspect_groups = array (
    '10:12' => 'wide',
    '10:10' => 'wide',
    '10:14' => 'wide',
    '10:24' => 'narrow',
    '10:20' => 'narrow',
    '10:28' => 'narrow',
  ); */
  // $aspect_group = $aspect_groups[$aspect_ratio];

    $placement = ''; // This is what we are after, keep going until we find one
    $adjustment = ''; // And we might also have an adjustment once we have found it
    
    // Set up parameters for this charge
    if ($aspect_ratio == 'square')
      $chief = false; // special layout arrangement, always ignore chief
    else
      $chief = $xpath->evaluate("boolean(../../../ordinary[@keyterm='chief'] or ../ordinary[@keyterm='chief'])", $item);
    $pos = getModifiersByTypeAsArray($item,'position'); // THIS IS AN array, go through each one, appending * bounding_box as many times as required.
    $arr = getModifierByType($item,'arrangement');
    $cadency = existModifierWithKeyterm($item,'cadency');
    $number = $item->getAttribute('number');
    $overall = ($item->parentNode->parentNode->nodeName == 'overall');
    $apply_location = true;
    $div_node = $xpath->query("parent::objects/parent::simple/field/tincture/division", $item);
    if ( $div_node->length > 0 ) {
      /**
       * @var DOMElement $divisionNode
       */
      $divisionNode = $div_node->item(0);
      $underlying_div = $divisionNode->getAttribute('keyterm');
      if (existModifierWithKeyterm($divisionNode,'sinister')) $underlying_div .= '-sinister';
      if (existModifierWithKeyterm($divisionNode,'inverted')) $underlying_div .= '-inverted';
      if (existModifierWithKeyterm($divisionNode,'reversed')) $underlying_div .= '-reversed';
      if ($underlying_div == 'gyronny') {
        $ofnum = getModifierValueByKeyterm($divisionNode,'ofnum');
        $underlying_div .= '_of_' . (($ofnum == null) ? '8' : $ofnum);
      }
    } else {
      $underlying_div = null;
    }
    
    // Figure out default bounding box size for this aspect ratio, chief presence and entire
    $bounding_index = 0;
    if ( $chief ) $bounding_index += 2;
    if ( existModifierWithKeyterm($item,'entire') ) $bounding_index += 1;
    $boundingBox = $bounding_boxes[$aspect_ratio][$bounding_index];

    // Try various placement regimes

    // strewn on a field (don't need any placement)
    if ( $item->parentNode->nodeName == 'semyde' )
      $placement = 'i';
    
    // Are we around another charge?
    if ( $placement == '' && $xpath->evaluate("boolean(parent::modifier[@keyterm='between']/parent::charge)",$item) ) {
      // If our parent charge is "on" an ordinary, we don't need to do anything
      if ( $xpath->evaluate("boolean(parent::modifier/parent::charge/parent::modifier[@keyterm='on'])",$item) ) {
        $placement = 'i';
        // similarly, if our parent charge has an explicit arrangement
      } elseif ( $xpath->evaluate("boolean(parent::modifier/parent::charge/modifier[@keyterm='arrangement'])",$item) ) {
        $placement = 'i';        
      } else { // Arrange on an open field
        if ( substr($item->parentNode->parentNode->getAttribute('keyterm'),0,8) == 'quadrate' ) // Special placement for items around a quadrate cross
          $placement = '///300,300,160,160:700,300,160,160:300,700:700,700';
        elseif ( $chief )
          $placement = 'x/125,700,200,600:875:700/250,450,160,160:750,450,160,160:500,950/250,450,160,160:500,700,700,300:750,450,160,160:250,950:750,950/250,450,160,160:500,700,700,300:750,450,160,160:500,200:250,950:750,950';
        else
          $placement = 'x/125,500,200,800:875,500/250,200,300,280:750,200,300,280:500,1000/250,200,280,280:750,200,280,280:250,900:750,900/250,200,240,240:750,200,240,240:500,200:250,900:750,900';
      }
      // If our parent charge has a position, then we need to apply it to ourselves also
      $pos = getModifiersByTypeAsArray($item->parentNode->parentNode,'position');
    }
    
    // explicit arrangements
    if ( $placement == '' && $arr ) {
      switch ( $arr ) { // TODO add incross + insaltire with chief,
        case 'facing-in': $placement = 'x/250,600,333,1000,0,0,1:750,600,,,0,0,0/x/266,333,333,400,0,0,1:734,333,,,0,0,0:266,900,,,0,0,1:734,900,,,0,0,0/x/260,220,320,240,,,1:740,220,320,240,,,0:260,550,320,240,,,1:740,550,320,240,,,0:260,880,320,240,,,1:740,880,320,240,,,0'; break;
        //case 'facing-in': $placement = ($chief) ? '' : 'x/250,600,333,1000,0,0,1:750,600,,,0,0,0/x/266,333,333,400,0,0,1:734,333,,,0,0,0:266,900,,,0,0,1:734,900,,,0,0,0/x/260,220,320,240,,,1:740,220,320,240,,,0:260,550,320,240,,,1:740,550,320,240,,,0:260,880,320,240,,,1:740,880,320,240,,,0'; break;
        case 'facing-out': $placement =  'x/250,600,333,1000,0,0,0:750,600,,,0,0,1/x/266,333,333,400,0,0,0:734,333,,,0,0,1:266,900,,,0,0,0:734,900,,,0,0,1/x/290,443,280,187:710,443,280,187,,,1:290,700,280,187,,,0:710,700,280,187,,,1:290,957,280,187,,,0:710,957,280,187,,,1'; break;
       // case 'facing-out': $placement = ($chief) ? '' : 'x/250,600,333,1000,0,0,0:750,600,,,0,0,1/x/266,333,333,400,0,0,0:734,333,,,0,0,1:266,900,,,0,0,0:734,900,,,0,0,1/x/290,443,280,187:710,443,280,187,,,1:290,700,280,187,,,0:710,700,280,187,,,1:290,957,280,187,,,0:710,957,280,187,,,1'; break;
      case 'inbend': $placement = ($chief) ? '500,800,350,700,45/333.33,633.33,180,180,45:666.67,966.67/200,500,180,180,45:400,700:600,900/166.67,466.67,180,180,45:333.33,633.33:500,800:666.67,966.67/142.86,442.86,180,180,45:285.71,585.71:428.57,728.57:571.43,871.43:714.29,1014.29/s' :
        '500,500,400,800,45/333.33,333.33,180,180,45:666.67,666.67/250,250,200,200,45:500,500:750,750/200,200,200,200,45:400,400:600,600:800,800/166.67,166.67,160,160,45:333.33,333.33:500,500:666.67,666.67:833.33,833.33/142.86,142.86,140,140,45:285.71,285.71:428.57,428.57:571.43,571.43:714.29,714.29:857.14,857.14';
        break;
      case 'inbendsin': $placement = ($chief) ? '500,800,350,700,-45/333.33,633.33,180,180,-45:666.67,966.67/200,500,180,180,-45:400,700:600,900/166.67,466.67,180,180,-45:333.33,633.33:500,800:666.67,966.67/142.86,442.86,180,180,-45:285.71,585.71:428.57,728.57:571.43,871.43:714.29,1014.29/s' :
        '500,500,400,800,-45/333.33,333.33,180,180,-45:666.67,666.67/250,250,200,200,-45:500,500:750,750/200,200,200,200,-45:400,400:600,600:800,800/166.67,166.67,160,160,-45:333.33,333.33:500,500:666.67,666.67:833.33,833.33/142.86,142.86,140,140,-45:285.71,285.71:428.57,428.57:571.43,571.43:714.29,714.29:857.14,857.14';
        break;
      case 'inbar': $placement = calcPlace($item,'v',$boundingBox);
                    $item->appendChild(createModifierNode('chargemod','fesswise'));
          break;
      case 'inpale': $placement = calcPlace($item,'v',$boundingBox);
        break;
      case 'inchiefthrough': $placement = calcPlace($item,'h','0,25,1000,250'); break;
      case 'inpall': $placement = ($chief) ? 'x/x/250,550,180,400,-45:750,550,,,45:500,950,,,0/s/s/s' : 'x/x/250,250,180,400,-45:750,250,,,45:500,750,,,0/s/s/s' ; break;
      case 'inpalethrough': $placement = calcPlace($item,'v',($chief) ? '350,300,300,900' : '350,0,300,1200'); break;
      case 'infess': $placement = calcPlace($item,'h','100,300,800,300'); break;
      case 'infessthrough': $placement = calcPlace($item,'h','0,300,1000,300'); break;
      case 'inchevron': $placement = ($chief) ? 'x/250,725,160,160,45:750,725,,,-45/250,725,150,150,45:500,500,,,0:750,725,,,-45/200,775,140,140,45:400,575:600,575,,,-45:800,775/200,775,140,140,45:350,625:500,500,,,0:650,625,,,-45:800,775/150,825,120,120,45:250,725:350,625:650,625,,,-45:750,725:850,825' :
        'x/250,625,160,160,45:750,625,,,-45/250,625,150,150,45:500,400,,,0:750,625,,,-45/200,675,140,140,45:400,475:600,475,,,-45:800,675/200,675,140,140,45:350,525:500,400,,,0:650,525,,,-45:800,675/150,725,120,120,45:250,625:350,525:650,525,,,-45:750,625:850,725';
        break;
      case 'incross': $placement = 'x/x/x/500,200,200,200:250,500:750,500:500,800/500,200,200,200:250,500:500,500:750,500:500,800/x'; break;
      case 'insaltire':
        // TODO If only one thing in saltire, is there a single charge adjacent?
        $placement = 'x/500,500,220,650,45:500,500,220,650,-45/x/200,200,200,200,-45:800,200,,,45:200,800,,,45:800,800,,,-45/200,200,200,200,-45:800,200,,,45:500,500,,,0:200,800,,,45:800,800,,,-45/x';
        break;
      case 'inpile': $placement = 'x/x/250,250,300,300:500,600:750,250/x/x/250,200,200,200:500:750:333.33,500:666.67:500,800'; break;
      case 'inorle': $placement = '///////////500,100,100,100:100,650:900,650:500,1050:100,100:900,100:100,400:900,400:200,900:800,900:300,100:700,100'; break;
      case 'counter-passant': $placement = 'x/500,333.33,800,350:500,666.67,,,,,1/500,250,800,200:500,500,,,,,1:500,750,,,,,0/500,200,600,160:500,400,,,,,1:500,600,,,,,0:500,800,,,,,1/500,166.67,160,160:500,333.33,,,,,1:500,500,,,,,0:500,666.67,,,,,1:500,833.33,,,,,0/500,142.86,500,140:500,285.71,,,,,1:500,428.57,,,,,0:500,571.43,,,,,1:500,714.29,,,,,0:500,857.14,,,,,1'; break;
      case 'pilewise': $placement = ($chief) ? 'x/300,200,200,600,-30,1:700,,,,30,1/250,750,180,550,-30,1:500,700,,,0,1:750,750,,,30,1/200,850,150,500,-38,1:400,750,,,-16,1:600,,,,16,1:800,800,,,38,1/x/x' :
        'x/300,550,200,600,-30,1:700,,,,30,1/250,600,180,550,-30,1:500,550,,,0,1:750,600,,,30,1/200,650,150,500,-38,1:400,600,,,-16,1:600,,,,16,1:800,650,,,38,1/x/x';
        break;
      case 'quadrangle': $placement = ($chief) ? 'x/x/x/250,500,400,300:750,500:250,900:750,900' : 'x/x/x/250,250,400,400:750,250:250,750:750,750/x' ; break;
      default: $placement = 'i'; break; // internal error, no placement
      }
    }


    // If we have other charges around us (i.e. charge between charges), shrink a bit to make room
  if ( $xpath->evaluate("boolean(modifier[@keyterm='between']/charge)",$item ) && $item->getAttribute('type') != 'quadrate' ) {
    if ( $placement == '' )
      $placement = adjustplacement(calcPlace($item,'w',$boundingBox), ($chief ? '150,210,700,700' : '150,150,700,700'));
    else
      $placement = adjustplacement($placement, ($chief ? '150,210,700,700' : '150,150,700,700'));
  }

  // On an underlying division (but ignore if in a specific location)
    if ( $placement == '' and count($pos)==0 and $underlying_div != null ) {
      if ($underlying_div=='per-bend' and $number==2 and $chief == false) $placement = 'x/300,700,350,350:700,300/x/x/x/x';
      elseif ($underlying_div=='per-bend-sinister' and $number==2 and $chief == false) $placement = 'x/300,300,350,350:700,700/x/x/x/x';
      elseif ($underlying_div=='per-bend-sinister' and $number==2  and $chief == true) $placement = 'x/300,600,350,350:700,1000/x/x/x/x';
      elseif ($underlying_div=='per-bend' and $number==2  and $chief == true) $placement = 'x/300,900,250,250:600,600/x/x/x/x';
      elseif (substr($underlying_div,0,9)=='quarterly' and $number==4  and $chief == false) $placement = 'x/x/x/250,250,300,300:750,250:250,750:750,750/x/x/';
      elseif (substr($underlying_div,0,9)=='quarterly' and $number==4  and $chief == true) $placement = 'x/x/x/250,450,260,260:750,450:250,850:750,850/x/x/';
      elseif ($underlying_div=='gyronny_of_8' and ($number==8 or $number==4)  and $chief == false) $placement = 'x/x/x/200,500,300,300:500,200:800,500:500,800/x/x/x/200,400,160,160:400,200:600,200:800,400:800,600:600,800:400,800:200,600';
      elseif ($underlying_div=='gyronny_of_8' and ($number==8 or $number==4)  and $chief == true) $placement = 'x/x/x/200,700,250,250:500,400:800,700:500,1000/x/x/x/200,600,150,150:400,400:600,400:800,600:800,800:600,1000:400,1000:200,800';
      elseif ($underlying_div=='gyronny_of_6' and ($number==6 or $number==3)  and $chief == false) $placement = 'x/200,500,300,600:800,500/x/x/x/350,200,200,200:650,200:150,500:850,500:350,800:650,800';
      elseif ($underlying_div=='gyronny_of_6' and ($number==6 or $number==3)  and $chief == true) $placement = 'x/200,800,300,400:800,800/x/x/x/350,450,180,180:650,450:150,700:850,700:350,950:650,950';
      elseif ($underlying_div=='per-chevron' and $number==3  and $chief == false) $placement = 'x/x/250,200,240,240:500,750,300,300:750,200,240,240';
      elseif ($underlying_div=='per-chevron' and $number==3  and $chief == true) $placement = 'x/x/250,500,220,220:500,950,300,300:750,500,220,220';
      elseif ($underlying_div=='per-pall-reversed' and $number==3  and $chief == false) $placement = 'x/x/250,300,300,300:500,900:750,300/x/x/x/';
      elseif ($underlying_div=='per-pall-reversed' and $number==3  and $chief == true) $placement = 'x/x/250,500,260,260:500,950:750,500/x/x/x/';
      elseif ($underlying_div=='per-pall' and $number==3  and $chief == false) $placement = 'x/x/250,700,300,300:500,200,300,300:750,700,300,300:/x/x/x/';
      elseif ($underlying_div=='per-pall' and $number==3  and $chief == true) $placement = 'x/x/250,850,260,260:500,500:750,850:/x/x/x/';
      elseif ($underlying_div=='per-saltire' and $number==3  and $chief == false) $placement = 'x/200,500,240,240:800,500/200,500,240,240:500,850,300,300:800,500,240,240/200,500,240,240:500,850,300,300:800,500,240,240:500,200,240,240/x/x';
      elseif ($underlying_div=='per-saltire' and $number>=2 and $number<=4  and $chief == true) $placement = 'x/200,700,220,220:800,700/200,700,220,220:500,950,260,260:800,700,220,220/200,700,220,220:500,950,260,260:800,700,220,220:500,450,220,220/x/x';
    }
   
    // now, do we need to modify placement to put the arrangement in a specific location?
    if ( count($pos) > 0 ) {
      if ( !$apply_location ) {
        $messages->addMessage('warning','"in" location conflicts with "on" or "between"');
      } else {
        $bigOrdinary = $xpath->evaluate("boolean(../ordinary[@keyterm='fess'] or ../ordinary[@keyterm='pale'] or ../ordinary[@keyterm='plain-cross'])", $item);
        $orig_placement = $placement;
        $placements = array();
        foreach ( array_keys($pos) as $position ) {
          switch ( $position ) {
          case 'ineach': // TODO more divisions!
            if ( !$underlying_div ) {
              $messages->addMessage('warning','Need a division for "in each"');
              break;
            } // else
            switch ( $underlying_div ) {
            case 'quarterly':
            case 'quarterly-per-fess':
            case 'quarterly-per-pale':
              $adjustment = '0,0,500,500*500,0,500,500*0,500,500,500*500,500,500,500'; 
              break;
            case 'tierced-in-pale':
              $adjustment = '50,100,233,1000*383,100,233,1000*716,100,233,1000';
              break;
            case 'tierced-in-fess':
              $adjustment = '100,100,800,200*100,500,800,200*100,900,800,200';
              break;
            case 'tierced-in-bend':
              $adjustment = '650,50,300,300*350,350,300,300*50,700,300,300';
              break;
            case 'tierced-in-bend-sinister':
              $adjustment = '50,50,300,300*350,350,300,300*650,700,300,300';
              break;
            case 'tierced-in-chevron':
              $adjustment = '400,0,200,200*350,250,300,300*350,850,300,300';
              break;
            case 'per-pall':
              $adjustment = '50,550,400,400*350,50,300,300*550,550,400,400';
              break;
            case 'per-pile':
              $adjustment = '50,700,250,250*300,100,400,400*700,700,250,250';
              break;
            case 'per-pall-inverted':
              $adjustment = '50,50,400,400*550,50,400,400*350,750,300,300';
              break;
            case 'gyronny_of_8':
              $adjustment = '50,250,200,200*250,50,200,200*550,50,200,200*750,250,200,200*750,550,200,200*550,750,200,200*250,750,200,200*50,550,200,200';
              break;
            default:
              $messages->addMessage('warning','"in each" for ' . $underlying_div . 'not available' );
              $adjustment = 'x';
              break;
            }
            break;
          case 'ineach1st':
            if ( !$underlying_div ) {
              $messages->addMessage('warning','Need a division for "in each"');
              break;
            } // else
            switch ( $underlying_div ) {
            case 'quarterly':
            case 'quarterly-per-fess':
            case 'quarterly-per-pale':
              $adjustment = '0,0,500,500*500,500,500,500'; 
              break;
            case 'gyronny_of_8':
              $adjustment = '250,50,200,200*750,250,200,200*550,750,200,200*50,550,200,200';
              break;
            }
            break;
          case 'ineach2nd':
            if ( !$underlying_div ) {
              $messages->addMessage('warning','Need a division for "in each"');
              break;
            } // else
            switch ( $underlying_div ) {
            case 'quarterly':
            case 'quarterly-per-fess':
            case 'quarterly-per-pale':
              $adjustment = '500,0,500,500*0,500,500,500'; 
              break;
            case 'gyronny_of_8':
              $adjustment = '50,250,200,200*550,50,200,200*750,550,200,200*250,750,200,200';
              break;
            }
            break;
          case 'ineachQ': $adjustment = $bigOrdinary ? '0,0,350,300*650,0,350,300*0,650,350,400*650,650,350,400' : '0,0,500,500*500,0,500,500*0,500,500,500*500,500,500,500'; break;
          case 'ineachQ12': $adjustment = $bigOrdinary ? '0,0,350,300*650,0,350,300' : '0,0,500,500*500,0,500,500'; break;
          case 'ineachQ13': $adjustment = $bigOrdinary ? '0,0,350,300*0,650,350,400' : '0,0,500,500*0,500,500,500'; break;
          case 'ineachQ24': $adjustment = $bigOrdinary ? '650,0,350,300*650,650,350,400' : '650,0,350,300*500,500,500,500'; break;
          case 'ineachQ34': $adjustment = $bigOrdinary ? '0,650,350,400*650,650,350,400' : '0,500,500,500*500,500,500,500'; break;
          case 'inchief':
            if ( $orig_placement == '' ) { // no placement yet,
              $placement = calcPlace($item,'h','100,25,800,250');
              $adjustment = 'x';
            } else 
              $adjustment = '50,50,850,300';
            break;
          case 'inbase':
            if ( $orig_placement == '' ) {
              $placement = '500,950,500,240/333.33,950,300,200:666.67,950/333.33,900,160,160:666.67,900:500,1050/x/x/x';
              $adjustment = 'x';
            } else
              $adjustment = '400,850,200,200';
            break;
          case 'inflank': $adjustment = '0,375,300,300*700,375,300,300'; break;
          case 'indexchief': $adjustment = ($cadency) ? '0,0,200,200' : '0,0,350,300'; break;
          case 'insinchief': $adjustment = ($cadency) ? '800,0,200,200' : '650,0,350,300'; break;
          case 'inmidchief': $adjustment = ($cadency) ? '400,0,200,200' : '300,0,400,400'; break;
          case 'indexbase': $adjustment = ($cadency) ? '200,800,200,200' : '150,750,350,400'; break;
          case 'insinbase': $adjustment = ($cadency) ? '600,800,200,200' : '500,750,350,400'; break;
          case 'inmidbase': $adjustment = ($cadency) ? '400,800,200,200' : '300,750,400,400'; break;
          case 'inhonpoint': $adjustment = ($cadency) ? '400,250,200,200' : '200,50,500,500'; break;
          case 'infesspoint': $adjustment = ($cadency) ? '400,400,200,200' : '200,200,600,600'; break;
          case 'innombril': $adjustment = ($cadency) ? '400,800,200,200' : '300,600,400,400'; break;
          case 'insinflank': $adjustment = ($cadency) ? '800,500,200,200' : '700,375,300,300'; break;
          case 'indexflank': $adjustment = ($cadency) ? '0,500,200,200' : '0,375,300,300'; break;
          case 'infirst': 
            switch ( $underlying_div ) { 
            case 'tierced-in-pale': $adjustment = '50,100,233,1000'; break;
            case 'tierced-in-fess': $adjustment = '100,100,800,200'; break;
            case 'tierced-in-bend': $adjustment = '650,50,300,300'; break;
            case 'tierced-in-bend-sinister': $adjustment = '50,50,300,300'; break;
            case 'tierced-in-chevron': $adjustment = '400,0,200,200'; break;
            case 'per-pile': $adjustment = '50,700,250,250'; break;
            case 'per-pall': $adjustment = '50,550,400,400'; break;
            case 'per-pall-inverted': $adjustment = '50,50,400,400'; break;
            case 'quarterly':
            case 'quarterly-per-pale':
            case 'quarterly-per-fess': $adjustment = '50,50,400,400'; break;
              default: $messages->addMessage('warning','no division for "in first"'); break;
            }
            break;
          case 'insecond': 
            switch ( $underlying_div ) { 
            case 'tierced-in-pale': $adjustment = '383,100,233,1000'; break;
            case 'tierced-in-fess': $adjustment = '100,500,800,200'; break;
            case 'tierced-in-bend':  // same as sinister
            case 'tierced-in-bend-sinister': $adjustment = '350,350,300,300'; break;
            case 'tierced-in-chevron':  $adjustment = '350,250,300,300'; break;
            case 'per-pall': $adjustment = '350,50,300,300'; break;
            case 'per-pile': $adjustment = '300,100,400,400'; break;
            case 'per-pall-inverted': $adjustment = '550,50,400,400'; break;
            case 'quarterly': 
            case 'quarterly-per-pale':
            case 'quarterly-per-fess': $adjustment = '550,50,400,400'; break;
              default: $messages->addMessage('warning','no division for "in second"'); break;
            }
            break;
          case 'inthird':
            switch ( $underlying_div ) {
            case 'tierced-in-pale': $adjustment = '716,100,233,1000'; break;
            case 'tierced-in-fess': $adjustment = '100,900,800,200'; break;
            case 'tierced-in-bend': $adjustment = '50,700,300,300'; break;
            case 'tierced-in-bend-sinister': $adjustment = '650,700,300,300'; break;
            case 'tierced-in-chevron':  $adjustment = '350,850,300,300'; break;
            case 'per-pall': $adjustment = '550,550,400,400'; break;
            case 'per-pile': $adjustment = '700,700,250,250'; break;
            case 'per-pall-inverted': $adjustment = '350,750,300,300'; break;
            case 'quarterly':  
            case 'quarterly-per-pale':
            case 'quarterly-per-fess': $adjustment = '50,550,400,400'; break;
              default: $messages->addMessage('warning','no division for "in third"'); break;
            }
            break;
          case 'inQ1': $adjustment = '0,0,500,500'; break;
          case 'inQ2': $adjustment = '500,0,500,500'; break;
          case 'inQ3': $adjustment = '0,500,500,500'; break;
          case 'inQ4': $adjustment = '500,500,500,500'; break;
            default: break;
          }
          // If we don't have a placement yet the default is just to arrange in wide rows
          if ( $placement == '' ) 
            $placement = calcPlace($item,'w',$boundingBox);
          foreach ( explode('*',$adjustment) as $adjust )
            $placements[] = adjustPlacement($placement,$adjust);
          $placement = $orig_placement;
        }
        $placement = implode('*',$placements);
      }
    } elseif ( $cadency ) { // unless there is a specific location, put in the center, but small
      $placement = calcPlace($item,'w',$boundingBox);
      $placement = adjustplacement($placement,'400,400,200,200');
    } elseif ( $overall ) { // like cadency, but a bit bigger
      $placement = calcPlace($item,'w',$boundingBox);
      $placement = adjustplacement($placement,'300,300,400,400');
    }
    
    // If we don't have a placement yet the default is just to arrange in wide rows
    if ( $placement == '' ) {
      $placement = calcPlace($item,'w',$boundingBox);
    }
    
    return $placement;
}
