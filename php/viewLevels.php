<?php
$dbh = new PDO('mysql:host=localhost;dbname=spacehero', "spacehero", "2vYtaZWSvFj9aTAn");
$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING);

$SQLstart = 0;
$SQLcount = 100;
$sql = "SELECT * FROM levels LIMIT $SQLstart, $SQLcount";
$sth = $dbh->prepare($sql);
$sth->execute();
$levels = $sth->fetchAll();

foreach($levels as $level)
{
  echo $level['title']."<br />";
  echo $level['creator']."<br />";
  echo "<hr />";
}
?>
