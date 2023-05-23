<?php
  require_once "db_connection.php";
  require_once "helpers/functions.php";
  if (isset($_GET["code"])) {
    $first_challenges = array(sha1("5555"), sha1("2222"), sha1("1111"));
    $second_challenges = get_challenges();
    $code =  $_GET["code"];
    if (in_array($code, $first_challenges)) {
      $random_challenge = $second_challenges[rand(0, count($second_challenges) - 1)];
      change_challenge_status($random_challenge, true);
      unactive_challenges($random_challenge);
      echo $random_challenge;
    } else {
      $active_challenge = get_active_challenge();
      if ($active_challenge) {
        $code_challenge_hash = $_GET["code"];
        $codes = get_codes();
        $found = False;
        foreach($codes as $code) {
          if (sha1($code.$active_challenge) == $_GET["code"]) {
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
    }
  } else {
    header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect");
    die;
  }