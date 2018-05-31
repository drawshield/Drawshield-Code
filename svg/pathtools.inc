<?php 

function subFuncPercent( $match ) {
	global $options;

	$height = 1000;
  $percent = substr($match[0],2,strlen($match[0])-3);
  $len = strlen($percent);
  // Flags
  if ($percent[$len-1] == '*') { // On a shield, use full height
    $height = 1200;
    $percent = substr($percent, 0, $len - 1); // strip flag
  }
  // Types
	if ($options['drawType'] == 'flag') {
		$height = $options['flagHeight'];
	}
	
	return (int)($height * $percent / 100);
}

function subPercent($str) {
  return preg_replace_callback('/{%[0-9]+\*?}/', 'subFuncPercent', $str);
}

function rotatePoint( $x, $y, $theta ) {
  switch ( $theta ) {
    case 360:
    case 0: // should not happen, but just in case...
      $x2 = $x;
      $y2 = $y;
      break;
    case 90:
      $x2 = $y * -1;
      $y2 = $x;
      break;
    case 180:
      $x2 = $x * -1;
      $y2 = $y * -1;
      break;
    case 270:
      $x2 = $y;
      $y2 = $x * -1;
      break;
    default:
      $cos = cos(deg2rad($theta));
      $sin = sin(deg2rad($theta));
      $x2 = ($cos * $x) - ($sin * $y);
      $y2 = ($cos * $y) + ($sin * $x);
      break;
  }
  $retval = sprintf('%.4g,%.4g',$x2,$y2);
  return $retval;
}

function getNumbers ( $argString ) {
  global $messages;
  $argString = preg_replace('/-/',' -', $argString);
  $argArray = array();
  $length = strlen($argString);
  $numStr = '';
  $expStr = '';
  $component = 'man';
  
  for ( $count = 0; $count < $length; $count++ ) {
    $char = $argString{$count};
    if ( $char == 'e' ) {
      $component = 'exp';
      $messages->addMessage('warning',"Found exponent");
    } elseif ( ctype_digit($char) or $char == '.' or $char == '-' ) {
      if ( $component == 'man' ) 
        $numStr .= $char;
      else
        $expStr .= $char;
    } else { // Got a complete number
      if ( strlen($numStr) ) {
        $number = (float) $numStr;
        if ( $component == 'exp' ) {
          $number = 0.0;
        }
        $argArray[] = $number;
        $numStr = '';
        $expStr = '';
        $component = 'man';
      }
    }
  } // Process last number
  if ( strlen($numStr) ) {
    $number = (float) $numStr;
    if ( $component == 'exp' ) {
      $number = 0.0;
    }
    $argArray[] = $number;
  }
  return $argArray;
}

function rotatePath( $path, $theta ) {
  if ( $theta == 0 ) return $path;
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) )
      $numbers = getNumbers( $chunks[$i+1] );
    switch ( strtolower($command) ) {
      case 'h':
        $x = $numbers[0];
        if ($theta == 270 )
          $newPath .= sprintf('v%.4g', ($x * -1));
        elseif ( $theta == 180 )
          $newPath .= sprintf('h%.4g', ($x * -1));
        elseif ( $theta == 90 )
          $newPath .= 'v' . $x;
        else
          $newPath .= 'l' . rotatePoint($x,0, $theta);
        break;
      case 'v':
        $y = $numbers[0];
        if ($theta == 270 )
          $newPath .= 'h' . $y;
        elseif ( $theta == 180 )
          $newPath .= sprintf('v%.4g', ($y * -1));
        elseif ( $theta == 90 )
          $newPath .= sprintf('h%.4g', ($y * -1));
        else
          $newPath .= 'l' . rotatePoint(0,$y, $theta);
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$sweep,$x,$y) = $numbers;
        $newPath .= sprintf('a%.4g,%.4g %.4g,%d,%d %s', $rx,$ry, (($rot + $theta)%360),$arc,$sweep, rotatePoint($x,$y, $theta));
        break;
      case 'l':
      case 'q':
      case 'c':
      case 'm':
        $newPath .= $command;
        if ($command == 'M') {
          $newPath .= $numbers[0] . ',' . $numbers[1];
        } else {
          for ( $j = 0; $j < count($numbers); $j += 2 )
            $newPath .= rotatePoint($numbers[$j], $numbers[$j+1], $theta ) . ' ';
          }
        break;
      case 'z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}

function translatePath( $path, $xTrans, $yTrans ) {
  if ( $xTrans == 0 and $yTrans == 0 ) return $path;
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  $firstMove = true;
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) )
      $numbers = getNumbers( $chunks[$i+1] );
    switch ( $command ) {
      case 'H':
        $newPath .= sprintf('H%.4g', $numbers[0] + $xTrans);
        break;
      case 'V':
        $newPath .= sprintf('V%.4g', $numbers[0] + $yTrans);
        break;
      case 'M':
      case 'L':
      case 'C':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $newPath .= sprintf('%.4g,', ($numbers[$j] + $xTrans));
          $newPath .= sprintf('%.4g ', ($numbers[$j+1] + $yTrans));
        }
        if ( $command == 'M' ) $firstMove = false;
        break;
      case 'm':
        $newPath .= 'm';
        if ( $firstMove ) {
          $newPath .= sprintf('%.4g,', ($numbers[0] + $xTrans));
          $newPath .= sprintf('%.4g ', ($numbers[1] + $yTrans));
          for ( $j = 2; $j < count($numbers); $j += 2 ) {
            $newPath .= sprintf('%.4g,', ($numbers[$j]));
            $newPath .= sprintf('%.4g ', ($numbers[$j+1]));
          }
          $firstMove = false;
        } else {  
          for ( $j = 0; $j < count($numbers); $j += 2 ) {
            $newPath .= sprintf('%.4g,', ($numbers[$j]));
            $newPath .= sprintf('%.4g ', ($numbers[$j+1]));
          }
        }
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$sweep,$x,$y) = $numbers;
        $newPath .= sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry, $rot,$arc,$sweep, $x,$y);
        break;
      case 'h':
      case 'v': // Pass all unchanged
        $newPath .= $command . sprintf('%.4g', $numbers[0]);
        break;
      case 'l':
      case 'q':
      case 'c':
        $newPath .= $command;
        $odd = true;
        foreach ( $numbers as $number ) {
          $newPath .= (sprintf('%.4g', $number)) . ($odd ? ',':' ');
          $odd = !$odd;
        }
        break;
      case 'Z':
      case 'z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}

function matMul ( $a, $b, $c, $d, $e, $f, $x, $y ) {
  return array ( (($a * $x) + ($c * $y) + $e), (($b * $x) + ($d * $y) + $f) );
}

function matrixPath( $path, $a, $b, $c, $d, $e, $f ) {
  global $messages;
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  $firstMove = true;
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) )
      $numbers = getNumbers( $chunks[$i+1] );
    switch ( $command ) {
      case 'H':
        $coords =  matMul($a, $b, $c, $d, $e, $f, $numbers[0], 0);
        $newPath .= sprintf('l%.4g,%.4g ', $coords[0], $coords[1]);
        break;
      case 'V':
        $coords =  matMul($a, $b, $c, $d, $e, $f, 0, $numbers[0]);
        $newPath .= sprintf('l%.4g,%.4g ', $coords[0], $coords[1]);
        break;
      case 'M':
      case 'L':
      case 'C':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $coords =  matMul($a, $b, $c, $d, $e, $f, $numbers[$j], $numbers[$j+1]);
          $newPath .= sprintf('%.4g,', $coords[0]);
          $newPath .= sprintf('%.4g ', $coords[1]);
        }
        if ( $command == 'M' ) $firstMove = false;
        break;
      case 'm':
        $newPath .= 'm';
        if ( $firstMove ) {
          $coords = matMul($a, $b, $c, $d, $e, $f, $numbers[0], $numbers[1]);
          $newPath .= sprintf('%.4g, %.4g ', $coords[0], $coords[1]);
          for ( $j = 2; $j < count($numbers); $j += 2 ) {
            $coords = matMul($a, $b, $c, $d, 0, 0, $numbers[$j], $numbers[$j+1]);
            $newPath .= sprintf('%.4g,', ($coords[0]));
            $newPath .= sprintf('%.4g ', ($coords[1]));
          }
          $firstMove = false;
        } else {  
          for ( $j = 0; $j < count($numbers); $j += 2 ) {
            $coords = matMul($a, $b, $c, $d, 0, 0, $numbers[$j], $numbers[$j+1]);
            $newPath .= sprintf('%.4g,', ($coords[0]));
            $newPath .= sprintf('%.4g ', ($coords[1]));
          }
        }
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$sweep,$x,$y) = $numbers; /* TODO extract rotation & apply */
        $newPath .= sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry, $rot,$arc,$sweep, $x,$y);
        $messages->addMessage('internal','no matrix apply for arc');
        break;
      case 'h':
        $coords =  matMul($a, $b, $c, $d, 0, 0, $numbers[0], 0);
        $newPath .= sprintf('l%.4g,%.4g ', $coords[0], $coords[1]);
        break;
      case 'v':
        $coords =  matMul($a, $b, $c, $d, 0, 0, 0, $numbers[0]);
        $newPath .= sprintf('l%.4g,%.4g ', $coords[0], $coords[1]);
        break;
      case 'l':
      case 'q':
      case 'c':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $coords = matMul($a, $b, $c, $d, 0, 0, $numbers[$j], $numbers[$j+1]);
          $newPath .= sprintf('%.4g,', ($coords[0]));
          $newPath .= sprintf('%.4g ', ($coords[1]));
        }
        break;
      case 'Z':
      case 'z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}

function mirrorPath ( $path, $axis = 'x' ) {
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) ) {
      $numbers = getNumbers($chunks[$i+1]);
    }
    switch ( strtolower($command) ) {
      case 'h':
        $x = $numbers[0];
        if ($axis == 'x')
          $newPath .= sprintf('h%.4g',$x);
        else
          $newPath .= sprintf('h%.4g',($x * -1));
        break;
      case 'v':
        $y = $numbers[0];
        if ($axis == 'x')
          $newPath .= sprintf('v%.4g',($y * -1));
        else
          $newPath .= sprintf('v%.4g',$y);
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        if ( $axis == 'x' ) {
          $swap = $swap == 1? 0 : 1;
          $newPath .= sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry, (($rot + 180)%360),$arc,$swap, $x,$y);
        } else {
          $x *= -1;
          $newPath .= sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry,(($rot + 180)%360),$arc,$swap, $x,$y);
        }
        break;
      case 'l':
      case 'q':
      case 'c':
      case 's':
      case 'm':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          if ( $command == 'M' ) {
            $newX = $numbers[$j];
            $newY = $numbers[$j+1];
          } elseif ( $axis == 'x' ) {
            $newX = $numbers[$j];
            $newY = $numbers[$j+1] * -1;
          } else {
            $newX = $numbers[$j] * -1;
            $newY = $numbers[$j+1];
          }
          $newPath .= sprintf ('%.4g,%.4g ', $newX, $newY);
        }
        break;
      case 'z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}

function scalePath ( $path, $xScale = 1, $yScale = 1 ) {
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) ) {
      $numbers = getNumbers($chunks[$i+1]);
    }
    switch ( strtolower($command) ) {
      case 'h':
        $x = $numbers[0];
        if ($xScale == 1)
          $newPath .= sprintf('h%.4g',$x);
        else
          $newPath .= sprintf('h%.4g',($x * $xScale));
        break;
      case 'v':
        $y = $numbers[0];
        if ($yScale == 1)
          $newPath .= sprintf('v%.4g',$y);
        else
          $newPath .= sprintf('v%.4g',($y * $yScale));
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        if ( $xScale != 1 ) {
          $rx *= $xScale;
          $x *= $xScale;
        }
        if ( $yScale != 1 ) {
          $ry *= $yScale;
          $y *= $yScale;
        }
        $newPath .=  sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry, $rot,$arc,$swap, $x,$y);
        break;
      case 'c':
      case 'l':
      case 'q':
      case 's':
      case 'm':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $newY = $numbers[$j+1];
          $newX = $numbers[$j];
          if ( $xScale != 1 ) {
            $newX *= $xScale;
          }
          if ( $yScale != 1 ) {
            $newY *= $yScale;
          }
          $newPath .= sprintf ('%.4g,%.4g ', $newX, $newY);
        }
        break;
      case 'z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}

function makeRelative ( $path, $Xoff = 0, $Yoff = 0 ) {
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  $curX = $Xoff; $curY = $Yoff; 
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) ) {
      $numbers = getNumbers($chunks[$i+1]);
    }
    switch ( $command ) {
      case 'h':
        $x = $numbers[0];
        $curX += $x;
        $newPath .= sprintf('h%.4g',$x);
        break;
      case 'H':
        $x = $numbers[0];
        $newPath .= sprintf('h%.4g',($x - $curX));
        $curX = $x;
        break;
      case 'v':
        $y = $numbers[0];
        $curY += $y;
        $newPath .= sprintf('v%.4g',$y);
        break;
      case 'V':
        $y = $numbers[0];
        $newPath .= sprintf('v%.4g',($y - $curY));
        $curY = $y;
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        $newPath =  sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry, $rot,$arc,$swap, $x,$y);
        $curX += $x;
        $curY += $y;
        break;
      case 'A':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        $newPath =  sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g', $rx,$ry,$rot,$arc,$swap, ($x - $curX),($y - $curY));
        $curX = $x;
        $curY = $y;
        break;
      case 'c':
        $tempPath = 'c';
        for ( $j = 0; $j < count($numbers); $j += 6 ) {
          $cpStartX = $numbers[$j+0];
          $cpStartY = $numbers[$j+1];
          $cpEndX = $numbers[$j+2];
          $cpEndY = $numbers[$j+3];
          $endX = $numbers[$j+4];
          $endY = $numbers[$j+5];
          $tempPath .= sprintf('%.4g,%.4g %.4g,%.4g %.4g,%.4g ',$cpStartX,$cpStartY, $cpEndX,$cpEndY, $endX,$endY);
          $curX += $endX;
          $curY += $endY;
        }
        $newPath .= $tempPath;
        break;
      case 'C': // $newPath .= "($curX,$curY)";
        $tempPath = 'c';
        for ( $j = 0; $j < count($numbers); $j += 6 ) {
          $cpStartX = $numbers[$j+0];
          $cpStartY = $numbers[$j+1];
          $cpEndX = $numbers[$j+2];
          $cpEndY = $numbers[$j+3];
          $endX = $numbers[$j+4];
          $endY = $numbers[$j+5];
          $tempPath .= sprintf('%.4g,%.4g %.4g,%.4g %.4g,%.4g ', ($cpStartX - $curX), ($cpStartY - $curY),($cpEndX - $curX),($cpEndY - $curY),($endX - $curX),($endY - $curY));
          $curX = $endX;
          $curY = $endY;
        }
        $newPath .= $tempPath;
        break;
      case 'l':
      case 'm':
        $tempPath = $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $y = $numbers[$j+1];
          $x = $numbers[$j];
          $tempPath .= sprintf('%.4g,%.4g ',$x,$y);
          $curX += $x;
          $curY += $y;
        }
        $newPath .= $tempPath;
        break;
      case 'L':
      case 'M':
        $tempPath = strtolower($command);
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $y = $numbers[$j+1];
          $x = $numbers[$j];
          $tempPath .= sprintf('%.4g,%.4g ', ($x - $curX),($y - $curY));
          $curX = $x;
          $curY = $y;
        }
        $newPath .= $tempPath;
        break;
      case 'z':
      case 'Z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}

function reversePath ( $path ) {
  $newPath = '';
  $numbers = array();
  $chunks = preg_split('/([a-z])/i', $path, null, PREG_SPLIT_DELIM_CAPTURE );
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) ) {
      $numbers = getNumbers($chunks[$i+1]);
    }
    switch ( strtolower($command) ) {
      case 'h':
        $x = $numbers[0];
        $newPath = sprintf('h%.4g',($x * -1)) . $newPath;
        break;
      case 'v':
        $y = $numbers[0];
        $newPath = sprintf('v%.4g',($y * -1)) . $newPath;
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        $newPath =   sprintf('a%.4g,%.4g %.4g,%d,%d %.4g,%.4g',$rx,$ry, $rot,$arc,$swap,($x * -1),($y * -1)) . $newPath;
        break;
      case 'c':
        $tempPath = '';
        for ( $j = 0; $j < count($numbers); $j += 6 ) {
          $cpStartX = $numbers[$j+0];
          $cpStartY = $numbers[$j+1];
          $cpEndX = $numbers[$j+2];
          $cpEndY = $numbers[$j+3];
          $endX = $numbers[$j+4];
          $endY = $numbers[$j+5];
          $newCPStart = sprintf( '%.4g,%.4g ', ($cpEndX - $endX),($cpEndY - $endY));
          $newCPEnd = sprintf( '%.4g,%.4g ', ($cpStartX - $endX),($cpStartY - $endY));
          $newEnd = sprintf( '%.4g,%.4g ', ($endX * -1),($endY * -1));
          $tempPath = $newCPStart . $newCPEnd . $newEnd . $tempPath;
        }
        $tempPath = $command . $tempPath;
        $newPath = $tempPath . $newPath;
        break;
      case 'l':
      case 'q':
      case 's':
        $tempPath = '';
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $newY = $numbers[$j+1] * -1;
          $newX = $numbers[$j] * -1;
          $tempPath = sprintf('%.4g,%.4g ', $newX,$newY) . $tempPath;
        }
        $tempPath = $command . $tempPath;
        $newPath = $tempPath . $newPath;
        break;
      case 'z':
        $newPath .= $command;
        break;
    }
    $newPath .= ' ';
  }
  return $newPath;
}


function lineStraight( $angle, $dist ) {
  return rotatePath('h'.$dist, $angle);
}

?>