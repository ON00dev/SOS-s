#ifndef SKETCH_PIN_H
#define SKETCH_PIN_H

#include <Arduino.h>

class SketchPins {
public:
    void setup();
    void turnOnWiFiLED();
    void turnOnLocationLED();
    void turnOnSMSLED();
};

#endif
