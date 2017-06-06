<?php

class xmlformatter {
  protected $dom;
  protected $format = 'html';
  protected $tokens = false;
  
    function __construct($xml) {
      if ( is_string($xml) ) {
        $this->dom = new DOMDocument;
        // We don't want to bother with white spaces
        $this->dom->Loadxml($xml);  
      } elseif ( is_a($xml,'DOMDocument')) 
        $this->dom = $xml;
      else
        echo "internal -> cannot figure out xml data type for format";
    }
    
    public function format_xml() {
      $this->dom->formatOutput = true;
      $this->dom->preserveWhiteSpace = true;
      return $this->dom->saveXML();
    }
    
    protected function keyterm($node) {
      if ( $ref = $node->getAttribute('reference'))
        $retval = '<a href="' . $ref . '">' . ucfirst($node->getAttribute('keyterm')) . '</a>';
      else
        $retval = ucfirst($node->getAttribute('keyterm'));
      return $retval;
    }
      
    protected function printNode($node, $indent, $numDash, $numLines ) {
      $output = substr($indent,0,-1*($numDash+1)) . '+' . str_repeat('-', $numDash)  . ucfirst($node->nodeName) . ': ';
      $output .= '(' . $node->getAttribute('ID') . ') ';
      switch ( $node->nodeName ) {
      case 'tincture': 
        if ( ($origin = $node->getAttribute('origin')) != 'given' ) $output .= ucfirst($origin) . ' ';
        break;
      case 'colour':
      case 'fur':
      case 'treatment':
        $output .= self::keyterm($node);
        break;
      case 'shield':
        if ( ($index = $node->getAttribute('index') ) )
          $output .= "index $index; ";
        break;
      case 'ordinary':
      case 'charge':
        $output .= $node->getAttribute('number') . ' ' . self::keyterm($node);
        break;
      case 'modifier':
        $output .= ucfirst($node->getAttribute('type'));
        $output .= ' (' . self::keyterm($node) . ') ';
        if ( ($param = $node->getAttribute('value') ) )
          $output .= "- $param ";
        break;
      case 'division':
        $output .= self::keyterm($node);
        break;
      }
      if ( ($idref = $node->getAttribute('IDREF') ) )
        $output .= "(same as $idref)";
      if ( $this->tokens and ($tokens = $node->getAttribute('tokens')))
        $output .= "~~~~~[$tokens]";
      $output .= "<br />\n";
        
      for ( $i = 0; $i < $numLines; $i++ ) {
        if ( $node->hasChildNodes() )
          $output .= $indent . "|<br />\n";
        else
          $output .= $indent . "<br />\n";
      }
      foreach($node->childNodes as $child) {
        if ( $child->nextSibling ) 
          $output .= self::printNode($child,$indent . '|' . str_repeat('~',$numDash), $numDash, $numLines);
        else
          $output .= self::printNode($child,$indent . '~' . str_repeat('~',$numDash), $numDash, $numLines);
      }
      return $output;
    }
      
    
    public function text_tree( $numDash = 3, $numLines = 1 ) {
      $output = '<p style="font-family:Courier \'Courier New\' monospace; line-height: 1.1;">' . "\n";
     // $output .= self::printNode($this->dom->firstChild,'~' . str_repeat('~',$numDash),$numDash,$numLines);
      $output .= preg_replace('/~/','&nbsp;',self::printNode($this->dom->firstChild,'~' . str_repeat('~',$numDash),$numDash,$numLines));
      $output .= "</p>\n";
      return $output;
    }
    
    public function setOptions($format = 'html',$tokens = false) {
      $this->format = $format;
      $this->tokens = $tokens;
    }

}

?>
