<?php 

$y = ($layout->center_point->y - 500);

$ordinary['body'] = "<path
    transform='translate(0, $y)'
    d='m -57.56,68.9 422.84,422.84 42.43,-42.42 -422.84,-422.85 z m 84.85,-84.85 507.7,507.69 42.42,-42.42 -507.69,-507.699 z m 394.56,564.26 589.15,589.09 42.4,-42.4 -589.12,-589.12 z m 169.71,0 504.24,504.29 42.4,-42.5 -504.22,-504.22 z m -729.12,460.39 587.7,-587.67 42.42,42.43 -587.69,587.64 z m 644.26,-644.24 422.85,-422.839 42.42,42.429 -422.84,422.84 z m -559.41,729.14 502.85,-502.86 42.42,42.42 -502.84,502.84 z m 559.41,-559.43 507.7,-507.7 42.4,42.43 -507.67,507.7 z'
/>";




$old_ordinary = $ordinary;
include "saltire.inc";
$old_ordinary["chargesOn"] = $ordinary["chargesOn"];
$old_ordinary["chargesWithin"] = $ordinary["chargesWithin"];
$ordinary = $old_ordinary;
