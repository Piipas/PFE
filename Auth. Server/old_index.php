<?php
  require_once "db_connection.php";
  require_once "helpers/functions.php";

  if (isset($_GET["code"])) {
    $code = preg_match("/[AB0-9]{4}/", $_GET["code"])?$_GET["code"]:NULL;
    if ($code) {
      $hashed_code = $code;
      $is_correct = check_code($hashed_code);
      if (!$is_correct) {
        header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect");
        die;
      }
    } else {
      header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect");
      die;
    }
  } else {
    header($_SERVER["SERVER_PROTOCOL"]." 403 Incorrect");
    die;
  }