<?php

  function get_code() {
    global $pfe;
    $get_code = $pfe->prepare("SELECT * FROM codes");
    $get_code->execute();
    $code = $get_code->fetch();
    return $code["code"];
  }

  function generate_challenge() {
    $chars = "123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    $challenge = "";
    for ($i=0; $i<=rand(7, 12); $i++) {
      $challenge.=$chars[rand(0, strlen($chars)-1)];
    }
    return $challenge;
  }

  function save_challenge($challenge) {
    global $pfe;
    // Truncate challenges table
    $truncate_challenges = $pfe->prepare("TRUNCATE TABLE challenges");
    $truncate_challenges->execute();
    // Insert a new challenge
    $set_challenge = $pfe->prepare("INSERT INTO challenges (challenge) VALUES (:chal)");
    $set_challenge->execute(array(":chal" => $challenge));
  }

  function get_challenge() {
    global $pfe;
    $get_challenge = $pfe->prepare("SELECT * FROM challenges");
    $get_challenge->execute();
    return $get_challenge->fetch()["challenge"];
  }

  function generate_response_code() {
    $excluded_response_codes = array(403, 200);
    $response_code = rand(0, 600);
    while (in_array($response_code, $excluded_response_codes)) {
      $response_code = rand(0, 600);
    }
    return $response_code;
  }