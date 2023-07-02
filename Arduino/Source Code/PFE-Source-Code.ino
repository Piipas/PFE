// include LCD Display Library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// include WIFI/HTTP Libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
// include Keypad Display Library
#include <Keypad.h>
// include hashes library
#include <Hash.h>
// include Servo library
#include <Servo.h>

#define Servo_PWM D8
Servo MG995_Servo;

const byte n_rows = 4;
const byte n_cols = 4;

char keys[n_rows][n_cols] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};
byte colPins[n_rows] = {D6, D5, D2, D0};
byte rowPins[n_cols] = {0, D9, 1, D7};

// Objects Creation
LiquidCrystal_I2C lcd(0x3F, 16, 2);
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols);
ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;

int digitsLength = 0;
const int MAX_DIGITS = 4;
String ifiagCode = "";
String ifiagChallenged = "";
String requestURI;
String challenge = "";
bool allowTypingCode = false;

String Wifi_SSID = "TP-LINK_2AFB42";
String Wifi_PSWD = "7q8jhv6a";

String first_challenge_uri = "";

IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 1, 1);
IPAddress local_IP(192, 168, 1, 184);

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  home();

  WiFi.mode(WIFI_STA);
  WiFi.begin(Wifi_SSID, Wifi_PSWD);
  WiFi.config(local_IP, gateway, subnet);

  close_door();
}

void loop() {
  if (WiFiMulti.run() == WL_CONNECTED) {
    char myKey = myKeypad.getKey();
    if (myKey) {
      if (allowTypingCode == false && myKey == 'A') {
        String beginConnectionUri = "http://192.168.1.50:5678/?begin=true";
        if (http.begin(client, beginConnectionUri)) {
          int beginResponseCode = http.GET();
          if (beginResponseCode == 200) {
            challenge = http.getString();
            Serial.println();
            Serial.println(challenge);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Code d'acces");
            lcd.setCursor(0, 1);
            lcd.print("-> ");
            if (challenge) {
              allowTypingCode = true;
            }
          } else {
            reset(); home();
          }
        }
      } else if (allowTypingCode == true && digitsLength < MAX_DIGITS && digitsLength >= 0 && myKey != 'C' && myKey != 'D' && myKey != 'A') {
        digitsLength = digitsLength + 1;
        ifiagCode += myKey;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Code d'acces");
        lcd.setCursor(0, 1);
        lcd.print("-> ");
        lcd.setCursor(3, 1);
        lcd.print(ifiagCode);
      } else if (allowTypingCode == true && digitsLength <= MAX_DIGITS && digitsLength >= 1 && myKey == 'D' && myKey != 'C' && myKey != 'A') {
        digitsLength = digitsLength - 1;
        ifiagCode.remove(ifiagCode.length() - 1);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Code d'acces");
        lcd.setCursor(0, 1);
        lcd.print("-> ");
        lcd.setCursor(3, 1);
        lcd.print(ifiagCode);
      } else if (allowTypingCode == true && digitsLength == MAX_DIGITS && digitsLength >= 1 && myKey == 'C' && myKey != 'D' && myKey != 'A') {
        ifiagChallenged = sha1(ifiagCode+challenge);
        Serial.print(ifiagCode);
        Serial.print(" + ");
        Serial.println(challenge);
        requestURI = "http://192.168.1.50:5678/?code="+ifiagChallenged;
        if (http.begin(client, requestURI)) {
          int httpResponseCode = http.GET();
          if (httpResponseCode == 200) {
            open_door();
            reset();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Acces autorise!");
            Serial.println("Acces autorisé!");
            delay(5000);
            close_door();
            home();
          } else {
            close_door();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Acces refuse!");
            reset();
            delay(3000);
            lcd.clear();
            home();
          }
        }
      }
      delay(1000);
    }
  } else {
    reset();
    lcd.setCursor(0, 1);
    lcd.print("Pas de connexion");
    Serial.println("Pas de connexion");
  }
}

void reset() {
  ifiagCode = "";
  digitsLength = 0;
  allowTypingCode = false;
  challenge = "";
}

void home() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bienvenu IFIAG!");
}

void open_door() {
  MG995_Servo.attach(Servo_PWM, 500, 2400);
  MG995_Servo.write(10);
  delay(3000);
  MG995_Servo.detach();
}

void close_door() {
  MG995_Servo.attach(Servo_PWM, 500, 2400);
  MG995_Servo.write(90);
  delay(3000);
  MG995_Servo.detach();
}