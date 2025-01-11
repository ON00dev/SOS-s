#include "SketchGPS.h"

void SketchGPS::setup() {
  gpsSerial.begin(9600);
  Serial.println("GPS inicializado.");
}

bool SketchGPS::handle() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isUpdated()) {
        return true; // Localização atualizada
      }
    }
  }
  return false; // Nenhuma atualização
}

float SketchGPS::getLatitude() {
  return gps.location.lat();
}

float SketchGPS::getLongitude() {
  return gps.location.lng();
}
