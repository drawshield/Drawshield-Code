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

$mulletMods = array('pierced', 'interlaced', 'voided',);


$num_of = getModifierValueByKeyterm($node, 'of-num', true);
if ( $num_of < 4 )
  $num_of = 5;
elseif ( $num_of > 10 )
  $num_of = 10;

$suffix = '-of' . $num_of . '-';

foreach($mulletMods as $aMod) {
  if (existModifierWithKeyterm($node, $aMod)) {
    $suffix .= $aMod . '-';
  }   
}

$file = getBestMatchFilename("svg/charges/mullet/", 'mullet', rtrim($suffix,'-'));
if ($file != '')
  $charge['file'] = $file;
//$charge['file'] = 'mullet-of' . $num_of . $suffix . '.svg';

