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
    } else { header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect"); die; }
  } else if (isset($_GET["code"])) {
    $plain_text_active_challenge = "";
    $ifiag_code =  $_GET["code"];
    $active_challenge = get_active_challenge();
    $plain_text_challenges = array("miolkpfiklormtitrla", "mpolkifikyhdbyzla", "myogkufpniekslss", "mroikrfirpbuel3");
    $plain_text_code = "2580";
    if ($active_challenge) {
      $code = get_code();
      foreach($plain_text_challenges as $plain_text_challenge) {
        if (sha1($plain_text_challenge) == $active_challenge) { $plain_text_active_challenge = $plain_text_challenge; }
      }
      if ($plain_text_active_challenge) {
        $found = False;
        if (sha1($plain_text_code.$plain_text_active_challenge) == $ifiag_code) { $found = True; }
        if ($found) {
          change_challenge_status($active_challenge, False);
        } else { header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect1"); die; }
      } else { header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect2"); die; }
    } else { header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect3"); die; }
  } else { header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect4"); die; }