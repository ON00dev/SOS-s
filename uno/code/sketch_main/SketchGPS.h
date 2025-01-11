#ifndef SKETCH_GPS_H
#define SKETCH_GPS_H

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

class SketchGPS {
  private:
    TinyGPSPlus gps;
    SoftwareSerial gpsSerial;

  public:
    SketchGPS() : gpsSerial(4, 3) {} // RX, TX
    void setup();
    bool handle();
    float getLatitude();
    float getLongitude();
};

#endif
