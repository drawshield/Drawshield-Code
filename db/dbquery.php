<?php header('Content-Type: text/xml; charset=utf-8'); ?>
<table class="dbquery">
<tr>
<th>Key</th><th>Description</th><th>Blazon</th><th>Source</th>
</tr>

<?php
$maxRecords = 20;

if (isset($_GET['term'])) {
  $pass = trim(file_get_contents('../../../etc/blazondb.txt'));
  
  $term = html_entity_decode(strip_tags ($_GET['term']));
  
  $db = @mysql_connect('localhost', 'karlwilc_blazons', $pass);
  if ( !$db ) {
    echo  "<tr><td colspan=\"4\">No database connection</td></tr>\n";
  } else {
    mysql_select_db('karlwilc_blazons');
    $sql = "SELECT * from blazon WHERE lower(description) LIKE '%$term%';";
    $results = mysql_query($sql);
    if ( !$results ) {
      echo  "<tr><td colspan=\"4\">Query failed</td></tr>\n";
    } else {
      $count = mysql_num_rows($results);
      if ( $count > $maxRecords ) {
        echo "<tr><td colspan=\"4\">Only first $maxRecords entries shown, be more specific.</td></tr>\n";
        $count = $maxRecords;
      }
      for ( $i = 0; $i < $count; $i++ ) {
        $record = mysql_fetch_assoc($results);
        echo '<tr><td>'. $record['keyname'] . "</td>\n";
        echo '<td>'. $record['description'] . "</td>\n";
        echo '<td>'. $record['blazon'] . "</td>\n";
        echo '<td>'. $record['source'] . "</td></tr>\n";
      }
    }
    mysql_close($db);
  }
} else {
    echo  "<tr><td colspan=\"4\">No search term</td></tr>\n";
}
?>

</table>

