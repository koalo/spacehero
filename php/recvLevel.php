<?php
ini_set("html_errors","false");
$dbh = new PDO('mysql:host=localhost;dbname=spacehero', "spacehero", "2vYtaZWSvFj9aTAn");
$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING);

$sql = "INSERT INTO levels (title, creator, level) VALUES (?, ?, ?);";
$sth = $dbh->prepare($sql);

$fromlevel = $_POST['level'];
$tolevel = "";

for($pos = 0; $pos < strlen($fromlevel); $pos++)
{
  $ch = $fromlevel[$pos];
  if($ch == ' ')
  {
    $tolevel .= "%20";
  }
  else if(urlencode($ch) == "%0A")
  {
    $tolevel .= "%0A";
  } 
  else if(($ch >= 'A' && $ch <= 'Z') || ($ch >= '0' && $ch <= '9'))
  {
    $tolevel .= $ch;
  }
}

$fromtitle = $_POST['title'];
$totitle = $fromtitle;

$fromcreator = $_POST['creator'];
$tocreator = $fromcreator;

if($sth->execute(array($totitle,$tocreator,$tolevel)))
{
  echo "OK";
}
?>
