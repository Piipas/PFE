<?php

  function check_code(string $code) {
    global $pfe;
    
  }

  function get_challenges() {
    global $pfe;
    $get_challenges = $pfe->prepare("SELECT * FROM challenges");
    $get_challenges->execute();
    $challenges = $get_challenges->fetchAll();
    $challenges_array = [];
    foreach ($challenges as $challenge) {
      $challenges_array[] = $challenge["challenge"];
    }
    return $challenges_array;
  }

  function change_challenge_status(string $challenge, bool $action) {
    global $pfe;
    $status = $action?1:0;
    $update_challenge = $pfe->prepare("UPDATE challenges SET is_active = ? WHERE challenge = ?");
    $update_challenge->execute(array($status, $challenge));
  }

  function get_active_challenge() {
    global $pfe;
    $get_challenges = $pfe->prepare("SELECT * FROM challenges WHERE is_active = ?");
    $get_challenges->execute(array(1));
    return $get_challenges->fetch()["challenge"];
  }

  function get_codes() {
    global $pfe;
    $get_codes = $pfe->prepare("SELECT * FROM codes");
    $get_codes->execute();
    $codes = $get_codes->fetchAll();
    $codes_array = [];
    foreach ($codes as $code) {
      $codes_array[] = $code["code"];
    }
    return $codes_array;
  }