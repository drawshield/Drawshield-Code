<?php /* Copyright 2010-2021 Karl Wilcox, Mattias Basaglia

This file is part of the DrawShield.net heraldry image creation program

    DrawShield is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

     DrawShield is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with  DrawShield.  If not, see https://www.gnu.org/licenses/. */

function subHeightPercent( $match, $layout ) {

    $percent = substr($match[0],2,strlen($match[0])-3);
    $len = strlen($percent);
    // Flags
    if ($percent[$len-1] == '*' ) { // On a shield, use full height
        $height = $layout->size->height;
        $percent = substr($percent, 0, $len - 1); // strip flag
    }
    // Types
    else if ($layout->shape == 'flag' || $layout->shape == 'quarter') {
        $height = $layout->size->height;
    }
    else {
        $height = $layout->size->height / 1.2;
    }

    return (int)($height * $percent / 100);
}

function subPercent($str, $layout) {
    if ( !is_string($str) )
        return $str;
  // $str = preg_replace_callback('/{@[0-9]+}/', 'subWidthPercent', $str);
  $callback = function ( $match ) use ( $layout ) {
    return subHeightPercent($match, $layout);
  };
  return preg_replace_callback('/{%[0-9.]+\*?}/', $callback, $str);
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
  $retval = sprintf('%.3f,%.3f',$x2,$y2);
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
    $char = $argString[$count];
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
  $chunks = preg_split('/([a-z])/i', $path, -1, PREG_SPLIT_DELIM_CAPTURE );
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) )
      $numbers = getNumbers( $chunks[$i+1] );
    switch ( strtolower($command) ) {
      case 'h':
        $x = $numbers[0];
        if ($theta == 270 )
          $newPath .= sprintf('v%.3f', ($x * -1));
        elseif ( $theta == 180 )
          $newPath .= sprintf('h%.3f', ($x * -1));
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
          $newPath .= sprintf('v%.3f', ($y * -1));
        elseif ( $theta == 90 )
          $newPath .= sprintf('h%.3f', ($y * -1));
        else
          $newPath .= 'l' . rotatePoint(0,$y, $theta);
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$sweep,$x,$y) = $numbers;
        $newPath .= sprintf('a%.3f,%.3f %.3f,%d,%d %s', $rx,$ry, (($rot + $theta)%360),$arc,$sweep, rotatePoint($x,$y, $theta));
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
        $newPath .= sprintf('H%.3f', $numbers[0] + $xTrans);
        break;
      case 'V':
        $newPath .= sprintf('V%.3f', $numbers[0] + $yTrans);
        break;
      case 'M':
      case 'L':
      case 'C':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $newPath .= sprintf('%.3f,', ($numbers[$j] + $xTrans));
          $newPath .= sprintf('%.3f ', ($numbers[$j+1] + $yTrans));
        }
        if ( $command == 'M' ) $firstMove = false;
        break;
      case 'm':
        $newPath .= 'm';
        if ( $firstMove ) {
          $newPath .= sprintf('%.3f,', ($numbers[0] + $xTrans));
          $newPath .= sprintf('%.3f ', ($numbers[1] + $yTrans));
          for ( $j = 2; $j < count($numbers); $j += 2 ) {
            $newPath .= sprintf('%.3f,', ($numbers[$j]));
            $newPath .= sprintf('%.3f ', ($numbers[$j+1]));
          }
          $firstMove = false;
        } else {  
          for ( $j = 0; $j < count($numbers); $j += 2 ) {
            $newPath .= sprintf('%.3f,', ($numbers[$j]));
            $newPath .= sprintf('%.3f ', ($numbers[$j+1]));
          }
        }
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$sweep,$x,$y) = $numbers;
        $newPath .= sprintf('a%.3f,%.3f %.3f,%d,%d %.3f,%.3f', $rx,$ry, $rot,$arc,$sweep, $x,$y);
        break;
      case 'h':
      case 'v': // Pass all unchanged
        $newPath .= $command . sprintf('%.3f', $numbers[0]);
        break;
      case 'l':
      case 'q':
      case 'c':
        $newPath .= $command;
        $odd = true;
        foreach ( $numbers as $number ) {
          $newPath .= (sprintf('%.3f', $number)) . ($odd ? ',':' ');
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
        $newPath .= sprintf('l%.3f,%.3f ', $coords[0], $coords[1]);
        break;
      case 'V':
        $coords =  matMul($a, $b, $c, $d, $e, $f, 0, $numbers[0]);
        $newPath .= sprintf('l%.3f,%.3f ', $coords[0], $coords[1]);
        break;
      case 'M':
      case 'L':
      case 'C':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $coords =  matMul($a, $b, $c, $d, $e, $f, $numbers[$j], $numbers[$j+1]);
          $newPath .= sprintf('%.3f,', $coords[0]);
          $newPath .= sprintf('%.3f ', $coords[1]);
        }
        if ( $command == 'M' ) $firstMove = false;
        break;
      case 'm':
        $newPath .= 'm';
        if ( $firstMove ) {
          $coords = matMul($a, $b, $c, $d, $e, $f, $numbers[0], $numbers[1]);
          $newPath .= sprintf('%.3f, %.3f ', $coords[0], $coords[1]);
          for ( $j = 2; $j < count($numbers); $j += 2 ) {
            $coords = matMul($a, $b, $c, $d, 0, 0, $numbers[$j], $numbers[$j+1]);
            $newPath .= sprintf('%.3f,', ($coords[0]));
            $newPath .= sprintf('%.3f ', ($coords[1]));
          }
          $firstMove = false;
        } else {  
          for ( $j = 0; $j < count($numbers); $j += 2 ) {
            $coords = matMul($a, $b, $c, $d, 0, 0, $numbers[$j], $numbers[$j+1]);
            $newPath .= sprintf('%.3f,', ($coords[0]));
            $newPath .= sprintf('%.3f ', ($coords[1]));
          }
        }
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$sweep,$x,$y) = $numbers; /* TODO extract rotation & apply */
        $newPath .= sprintf('a%.3f,%.3f %.3f,%d,%d %.3f,%.3f', $rx,$ry, $rot,$arc,$sweep, $x,$y);
        $messages->addMessage('internal','no matrix apply for arc');
        break;
      case 'h':
        $coords =  matMul($a, $b, $c, $d, 0, 0, $numbers[0], 0);
        $newPath .= sprintf('l%.3f,%.3f ', $coords[0], $coords[1]);
        break;
      case 'v':
        $coords =  matMul($a, $b, $c, $d, 0, 0, 0, $numbers[0]);
        $newPath .= sprintf('l%.3f,%.3f ', $coords[0], $coords[1]);
        break;
      case 'l':
      case 'q':
      case 'c':
        $newPath .= $command;
        for ( $j = 0; $j < count($numbers); $j += 2 ) {
          $coords = matMul($a, $b, $c, $d, 0, 0, $numbers[$j], $numbers[$j+1]);
          $newPath .= sprintf('%.3f,', ($coords[0]));
          $newPath .= sprintf('%.3f ', ($coords[1]));
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
  $chunks = preg_split('/([a-z])/i', $path, 0, PREG_SPLIT_DELIM_CAPTURE );
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) ) {
      $numbers = getNumbers($chunks[$i+1]);
    }
    switch ( strtolower($command) ) {
      case 'h':
        $x = $numbers[0];
        if ($axis == 'x')
          $newPath .= sprintf('h%.3f',$x);
        else
          $newPath .= sprintf('h%.3f',($x * -1));
        break;
      case 'v':
        $y = $numbers[0];
        if ($axis == 'x')
          $newPath .= sprintf('v%.3f',($y * -1));
        else
          $newPath .= sprintf('v%.3f',$y);
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        if ( $axis == 'x' ) {
          $swap = $swap == 1? 0 : 1;
          $newPath .= sprintf('a%.3f,%.3f %.3f,%d,%d %.3f,%.3f', $rx,$ry, (($rot + 180)%360),$arc,$swap, $x,$y);
        } else {
          $x *= -1;
          $newPath .= sprintf('a%.3f,%.3f %.3f,%d,%d %.3f,%.3f', $rx,$ry,(($rot + 180)%360),$arc,$swap, $x,$y);
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
          $newPath .= sprintf ('%.3f,%.3f ', $newX, $newY);
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
  $chunks = preg_split('/([a-z])/i', $path, 0, PREG_SPLIT_DELIM_CAPTURE );
  for ( $i = 0; $i < count($chunks); $i++ ) {
    $command = $chunks[$i];
    if ( isset($chunks[$i+1]) ) {
      $numbers = getNumbers($chunks[$i+1]);
    }
    switch ( strtolower($command) ) {
      case 'h':
        $x = $numbers[0];
        if ($xScale == 1)
          $newPath .= sprintf('h%.3f',$x);
        else
          $newPath .= sprintf('h%.3f',($x * $xScale));
        break;
      case 'v':
        $y = $numbers[0];
        if ($yScale == 1)
          $newPath .= sprintf('v%.3f',$y);
        else
          $newPath .= sprintf('v%.3f',($y * $yScale));
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
        $newPath .=  sprintf('a%.3f,%.3f %.3f,%d,%d %.3f,%.3f', $rx,$ry, $rot,$arc,$swap, $x,$y);
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
          $newPath .= sprintf ('%.3f,%.3f ', $newX, $newY);
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
        $newPath .= sprintf('h%.3f',$x);
        break;
      case 'H':
        $x = $numbers[0];
        $newPath .= sprintf('h%.3f',($x - $curX));
        $curX = $x;
        break;
      case 'v':
        $y = $numbers[0];
        $curY += $y;
        $newPath .= sprintf('v%.3f',$y);
        break;
      case 'V':
        $y = $numbers[0];
        $newPath .= sprintf('v%.3f',($y - $curY));
        $curY = $y;
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        $newPath =  sprintf('a%.3f,%.3f %.3f,%d,%d %.3f,%.3f', $rx,$ry, $rot,$arc,$swap, $x,$y);
        $curX += $x;
        $curY += $y;
        break;
      case 'A':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        $newPath =  sprintf('a%.3f,%.3f %.3f,%d,%d %.3f,%.3f', $rx,$ry,$rot,$arc,$swap, ($x - $curX),($y - $curY));
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
          $tempPath .= sprintf('%.3f,%.3f %.3f,%.3f %.3f,%.3f ',$cpStartX,$cpStartY, $cpEndX,$cpEndY, $endX,$endY);
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
          $tempPath .= sprintf('%.3f,%.3f %.3f,%.3f %.3f,%.3f ', ($cpStartX - $curX), ($cpStartY - $curY),($cpEndX - $curX),($cpEndY - $curY),($endX - $curX),($endY - $curY));
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
          $tempPath .= sprintf('%.3f,%.3f ',$x,$y);
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
          $tempPath .= sprintf('%.3f,%.3f ', ($x - $curX),($y - $curY));
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
        $newPath = sprintf('h%.3f',($x * -1)) . $newPath;
        break;
      case 'v':
        $y = $numbers[0];
        $newPath = sprintf('v%.3f',($y * -1)) . $newPath;
        break;
      case 'a':
        list($rx,$ry,$rot,$arc,$swap,$x,$y) = $numbers;
        $newPath =   sprintf('a%.3f,%.3f %.3f,%d,%d %.3f,%.3f',$rx,$ry, $rot,$arc,$swap,($x * -1),($y * -1)) . $newPath;
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
          $newCPStart = sprintf( '%.3f,%.3f ', ($cpEndX - $endX),($cpEndY - $endY));
          $newCPEnd = sprintf( '%.3f,%.3f ', ($cpStartX - $endX),($cpStartY - $endY));
          $newEnd = sprintf( '%.3f,%.3f ', ($endX * -1),($endY * -1));
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
          $tempPath = sprintf('%.3f,%.3f ', $newX,$newY) . $tempPath;
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
