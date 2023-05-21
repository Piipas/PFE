/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-motion-sensor
 */

const int PIN_TO_SENSOR = D2;   // the pin that OUTPUT pin of sensor is connected to
int pinStateCurrent   = LOW; // current state of pin
int pinStatePrevious  = LOW; // previous state of pin
int detectionCount = 0;

void setup() {
  Serial.begin(9600);            // initialize serial
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_TO_SENSOR, INPUT); // set arduino pin to input mode to read value from OUTPUT pin of sensor

  while(true) {
    pinStatePrevious = pinStateCurrent; // store old state
    pinStateCurrent = digitalRead(PIN_TO_SENSOR);   // read new state

    if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
      detectionCount++;
      Serial.print(String(detectionCount) + " ");
      Serial.println("Motion detected!");
      digitalWrite(LED_BUILTIN, LOW);
      // TODO: turn on alarm, light or activate a device ... here
      delay(30000);
    }
    else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
      Serial.println("Motion stopped!");
      digitalWrite(LED_BUILTIN, HIGH);
      // TODO: turn off alarm, light or deactivate a device ... here
    }
  }
}

void loop() {
  // pinStatePrevious = pinStateCurrent; // store old state
  // pinStateCurrent = digitalRead(PIN_TO_SENSOR);   // read new state

  // if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
  //   detectionCount++;
  //   Serial.print(String(detectionCount) + " ");
  //   Serial.println("Motion detected!");
  //   digitalWrite(LED_BUILTIN, LOW);
  //   // TODO: turn on alarm, light or activate a device ... here
  // }
  // else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
  //   Serial.println("Motion stopped!");
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   // TODO: turn off alarm, light or deactivate a device ... here
  // }

  // if (pinStateCurrent == LOW) {
  //   Serial.println("No motion");
  // } else {
  //   Serial.println("Motion detected  ALARM");
  // }
}
