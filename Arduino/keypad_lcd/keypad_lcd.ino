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

// Objects Creation
LiquidCrystal_I2C lcd(0x3F, 16, 2);
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols);
ESP8266WiFiMulti WiFiMulti;

int digitsLength = 0;
const int MAX_DIGITS = 4;
String ifiagCode;
String requestURI;

char* Wifi_SSID = "TP-LINK_2AFB42";
char* Wifi_PSWD = "7q8jhv6a";

void setup()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  home();

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(Wifi_SSID, Wifi_PSWD);
}

void loop()
{
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
        WiFiClient client;
        HTTPClient http;
        requestURI = "http://192.168.1.50:5678/?code="+ifiagCode;
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