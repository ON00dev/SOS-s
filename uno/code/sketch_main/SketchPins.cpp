#include "SketchPins.h"
#include <Arduino.h>

void SketchPins::setup() {
    // Configuração dos pinos
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
}

void SketchPins::turnOnWiFiLED() {
    digitalWrite(8, HIGH);
}

void SketchPins::turnOnLocationLED() {
    digitalWrite(9, HIGH);
}

void SketchPins::turnOnSMSLED() {
    digitalWrite(10, HIGH);
}
