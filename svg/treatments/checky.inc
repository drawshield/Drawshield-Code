<?php


  // Checky is rotated if on some ordinaries
  if ( $parent->nodeName == 'ordinary' ) {
        $ord_type = $parent->getAttribute('keyterm');
        if ( $ord_type == 'saltire' or substr($ord_type,0,6) == 'chevron' ) {
                $filename = 'checky45.svg';
        }
  }

