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

const byte n_rows = 4;
const byte n_cols = 4;

char keys[n_rows][n_cols] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};
byte colPins[n_rows] = {D6, D5, D2, D0};
byte rowPins[n_cols] = {0, D9, D8, D7};

const int PIN_TO_SENSOR = D2;   // the pin that OUTPUT pin of sensor is connected to
int pinStateCurrent   = LOW; // current state of pin
int pinStatePrevious  = LOW; // previous state of pin
int detectionCount = 0;

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
String proposedSecondChallenge = "";
String plainTextChallenge = "";
bool is_second_challenge_valid = false;
bool allowTypingCode = false;

String Wifi_SSID = "TP-LINK_2AFB42";
String Wifi_PSWD = "7q8jhv6a";

String CHALLENGES[4] = {"miolkpfiklormtitrla", "mpolkifikyhdbyzla", "myogkufpniekslss", "mroikrfirpbuel3"};
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
            String challenge = http.getString();
            Serial.println();
            Serial.println(challenge);
            if (check_challenge_validation(challenge) != "invalid") {
              plainTextChallenge = check_challenge_validation(challenge);
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
        lcd.setCursor(0, 1);
        lcd.print(ifiagCode);
      } else if (allowTypingCode == true && digitsLength <= MAX_DIGITS && digitsLength >= 1 && myKey == 'D' && myKey != 'C' && myKey != 'A') {
        digitsLength = digitsLength - 1;
        ifiagCode.remove(ifiagCode.length() - 1);
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print(ifiagCode);
      } else if (allowTypingCode == true && digitsLength == MAX_DIGITS && digitsLength >= 1 && myKey == 'C' && myKey != 'D' && myKey != 'A') {
        ifiagCode = ifiagCode;
        ifiagChallenged = sha1(ifiagCode+plainTextChallenge);
        Serial.print(ifiagCode);
        Serial.print(" + ");
        Serial.println(plainTextChallenge);
        requestURI = "http://192.168.1.50:5678/?code="+ifiagChallenged;
        if (http.begin(client, requestURI)) {
          int httpResponseCode = http.GET();
          if (httpResponseCode == 200) {
            reset();
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("Bienvenue!!!");
            Serial.println("Bienvenue!!!");
          } else {
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("Code incorrect!");
            // Serial.println(ifiagCode + ": Code incorrect!");
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
  plainTextChallenge = "";
}

void home() {
  lcd.setCursor(0, 0);
  lcd.print("IFIAG Code:");
}

String check_challenge_validation(String challenge) {
  String Return = "invalid";
  for (int i = 0; i < sizeof(CHALLENGES); i++) {
    if (challenge == sha1(CHALLENGES[i])) {
      Return = CHALLENGES[i];
    }
  }
  return Return;
}