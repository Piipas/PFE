<?php
  if (isset($_GET["code"])) {
    $first_challenges = array("", "", "");
    $second_challenges = array("", "", "dddddd");
    $code =  $_GET["code"];
    if (in_array($code, $first_challenges)) {
      echo sha1($second_challenges[rand(0, count($second_challenges) - 1)]);
    }
  }