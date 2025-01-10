#include "SketchPins.h"
#include <Arduino.h>

#define WIFI_LED 8
#define LOCATION_LED 9
#define SMS_LED 10

void SketchPins::setup() {
  pinMode(WIFI_LED, OUTPUT);
  pinMode(LOCATION_LED, OUTPUT);
  pinMode(SMS_LED, OUTPUT);
}

void SketchPins::turnOnWiFiLED() {
  digitalWrite(WIFI_LED, HIGH);
}

void SketchPins::turnOnLocationLED() {
  digitalWrite(LOCATION_LED, HIGH);
}

void SketchPins::turnOnSMSLED() {
  digitalWrite(SMS_LED, HIGH);
}
