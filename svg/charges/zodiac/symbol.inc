<?php

$item = getModifierValueByKeyterm($node,'value');
if ( file_exists( $folder . $item . '.svg'))
  $charge['file'] = "$item.svg";
else {
  $charge['file'] = "aries.svg";
  $messages->addMessage('warning',"No symbol for $item (using aries)");
}

$charge['licence'] = "By Tavmjong - Own work, CC BY 3.0, https://commons.wikimedia.org/w/index.php?curid=3544773";
