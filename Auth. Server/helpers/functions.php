<?php

  function check_code(string $code) {
    global $pfe;
    $check_code = $pfe->prepare("SELECT COUNT(code) AS is_correct FROM codes WHERE code = ?");
    $check_code->execute(array($code));
    return (bool) $check_code->fetch()["is_correct"];
  }