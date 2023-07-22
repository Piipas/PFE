<?php
  require_once "db_connection.php";
  require_once "helpers/functions.php";
  if (isset($_GET["begin"])) {
    $begin = $_GET["begin"];
    if ($begin == "true") {
      $random_challenge = generate_challenge();
      save_challenge($random_challenge);
      echo $random_challenge;
    } else { header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect"); die; }
  } else if (isset($_GET["code"])) {
    $code = $_GET["code"];
    $db_code = get_code();
    $challenge = get_challenge();
    if (sha1($db_code.$challenge) == $code) {
      echo sha1("Access Granted!".$challenge);
    } else { header($_SERVER["SERVER_PROTOCOL"]." 403"); die; }
  } else { header($_SERVER["SERVER_PROTOCOL"]." 403"); die; }