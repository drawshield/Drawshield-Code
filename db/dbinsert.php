<?php header('Content-Type: text/xml; charset=utf-8'); ?>
<div class="dbinsert">


<?php
if (!isset($_GET['keyname']) or !isset($_GET['blazon'])) {
  echo '<p>Must set keyname and blazon</p>';
} else {
  $pass = trim(file_get_contents('../../../etc/blazondb.txt'));
  
  $db = @mysql_connect('localhost', 'karlwilc_blazons', $pass);

  $keyname = strtolower(mysql_real_escape_string( html_entity_decode($_GET['keyname'])));
  $blazon = mysql_real_escape_string( html_entity_decode($_GET['blazon']));
  $description = empty($_GET['description']) ? $keyname : mysql_real_escape_string( html_entity_decode($_GET['description']));
  $notes = empty($_GET['notes']) ? '' : mysql_real_escape_string( html_entity_decode($_GET['notes']));
  $source = empty($_GET['source']) ? 'Karl Wilcox' : mysql_real_escape_string( html_entity_decode($_GET['source']));
  
  if ( !$db ) {
    echo  "<p>No database connection</p>\n";
  } else {
    mysql_select_db('karlwilc_blazons');
    // Check for existing keyname in database
    $keynames = array();
    $sql = "SELECT * from blazon WHERE lower(keyname) LIKE '$term%';";
    $results = mysql_query($sql);
    if ( !$results ) {
      echo  "<p>keyname check query failed or no results</p>\n";
    } else {
      $count = mysql_num_rows($results);
      for ( $i = 0; $i < $count; $i++ ) {
        $record = mysql_fetch_assoc($results);
        $keynames[] = strtolower($record['keyname']);
      }
    }
    // If existing, keep adding postfix until no match found
    if ( in_array ( $keyname, $keynames ) ) {
      $postfix = 1;
      while ( in_array ( $keyname . $postfix, $keynames ) )
        $postfix += 1;
      $keyname = $keyname . $postfix;
    }
    // Can now do the insert
    $sql = 'INSERT INTO blazon (keyname, description, blazon, notes, status, source ) VALUES ' .
        "('$keyname', '$description', '$blazon', '$notes', 'OK', '$source');";
    $results = mysql_query($sql);
    if ( $results )
      echo "<p>Inserted as $keyname</p>\n";
    else
      echo "<p>" . mysql_error() . "</p>\n";
        
    mysql_close($db);
  }
}
?>

</div>

