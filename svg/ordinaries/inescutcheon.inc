<?php 
// TODO make the version with a chief smaller also

if ($node->parentNode->parentNode->nodeName == 'overall') {
    $ordinary['body'] = array ( 
        'NC' => '<path d="M300,300h400v400a300,166,0,0,1 -200,166a300,166,0,0,1 -200,-166-400z" />',
        'CH' => '<path d="M200,500h600v300a400,200,0,0,1 -300,200a400,200,0,0,1 -300,-200v-300" />');

    $ordinary['chargesOn'] = array('w', chiefPresent() ? '300,600,400,250' : '330,330,340,440');
} else {
    $ordinary['body'] = array ( 
        'NC' => '<path d="M200,200h600v600a400,200,0,0,1 -300,200a400,200,0,0,1 -300,-200v-600z" />',
        'CH' => '<path d="M200,500h600v300a400,200,0,0,1 -300,200a400,200,0,0,1 -300,-200v-300" />');

    $ordinary['chargesOn'] = array('w', chiefPresent() ? '300,600,400,250' : '300,300,400,500');
}
