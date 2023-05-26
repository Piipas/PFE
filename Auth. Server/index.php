<?php
  require_once "db_connection.php";
  require_once "helpers/functions.php";
  if (isset($_GET["begin"])) {
    $begin = $_GET["begin"];
    if ($begin == "true") {
      $challenges = get_challenges();
      $random_challenge = $challenges[rand(0, count($challenges) - 1)];
      change_challenge_status($random_challenge, true);
      unactive_challenges($random_challenge);
      echo $random_challenge;
    } else {
      header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect");
      die;
    }
  } else if (isset($_GET["code"])) {
    $code =  $_GET["code"];
    $active_challenge = get_active_challenge();
    if ($active_challenge) {
      $code_challenge_hash = $code;
      $codes = get_codes();
      $found = False;
      foreach($codes as $code) {
        if (sha1($code.$active_challenge) == $code) {
          $found = True;
        }
      }
      if (!$found) {
        header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect");
        die;
      } else {
        change_challenge_status($active_challenge, false);
      }
    } else {
      header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect");
      die;
    }
  } else {
    header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect");
    die;
  }