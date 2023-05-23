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

int digitsLength = 0;
const int MAX_DIGITS = 4;
String ifiagCode = "";
String ifiagChallenged = "";
String requestURI;
String proposedSecondChallenge = "";
bool is_second_challenge_valid = false;

char* Wifi_SSID = "TP-LINK_2AFB42";
char* Wifi_PSWD = "7q8jhv6a";

const String FIRST_CHALLENGES[3] = {"5555", "2222", "1111"};
const String SECOND_CHALLENGES[4] = {"miolkpfiklormtitrla", "mpolkifikyhdbyzla", "myogkufpniekslss", "mroikrfirpbuel3"};
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

  while(true) {
    pinStatePrevious = pinStateCurrent;
    pinStateCurrent = digitalRead(PIN_TO_SENSOR);

    if (digitalRead(PIN_TO_SENSOR) == HIGH) {
      Serial.println("Motion detected!");
      first_challenge_uri = "http://192.168.1.50:8080/?code="+sha1(FIRST_CHALLENGES[random(3)]);
      WiFiClient client;
      HTTPClient http;
      Serial.print(first_challenge_uri);
      if (http.begin(client, first_challenge_uri)) {
        Serial.println("First Challenge Sent!");
        delay(1000);
        if (http.GET() == 200) {
          proposedSecondChallenge = http.getString();
          for (int i = 0; i < sizeof(SECOND_CHALLENGES); i++) {
            if (proposedSecondChallenge == SECOND_CHALLENGES[i]) {
              is_second_challenge_valid = true;
            }
          }
        } else {
          home();
        }
      }
      delay(30000);
    }
  }
}

void loop() {
  if (WiFiMulti.run() == WL_CONNECTED) {
    char myKey = myKeypad.getKey();
    if (myKey) {
      if (digitsLength < MAX_DIGITS && digitsLength >= 0 && myKey != 'C' && myKey != 'D') {
        digitsLength = digitsLength + 1;
        ifiagCode += myKey;
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print(ifiagCode);
      } else if (digitsLength <= MAX_DIGITS && digitsLength >= 1 && myKey == 'D' && myKey != 'C') {
        digitsLength = digitsLength - 1;
        ifiagCode.remove(ifiagCode.length() - 1);
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print(ifiagCode);
      } else if (digitsLength == MAX_DIGITS && digitsLength >= 1 && myKey == 'C' && myKey != 'D') {
        if (is_second_challenge_valid) {
        WiFiClient client;
        HTTPClient http;
        ifiagCode = sha1(ifiagCode);
        ifiagChallenged = sha1(ifiagCode+proposedSecondChallenge);
        requestURI = "http://192.168.100.50:8080/?code="+ifiagChallenged;
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
            Serial.println(ifiagCode + ": Code incorrect!");
            reset();
            delay(3000);
            lcd.clear();
            home();
          }
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
}

void home() {
  lcd.setCursor(0, 0);
  lcd.print("IFIAG Code:");
}