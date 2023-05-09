<?php
  $hostname = "localhost";
  $db_name = "iot_door_auth";
  $username = "root";
  $password = "";

  try {
    $pfe = new PDO("mysql:host=$hostname;dbname=$db_name;charset=utf8;", $username, $password);
  } catch (Exception $e) { }