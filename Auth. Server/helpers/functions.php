<?php
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
    if ($get_challenges->rowCount()) {
      return $get_challenges->fetch()["challenge"];
    } else { header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect"); die; }
  }

  function unactive_challenges(string $except_challenge) {
    global $pfe;
    $unactive_challenge = $pfe->prepare("UPDATE challenges SET is_active = ? WHERE challenge != ?");
    $unactive_challenge->execute(array(0, $except_challenge));
  }

  // -------------------------------------------------------- //

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