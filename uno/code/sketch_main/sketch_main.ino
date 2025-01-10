#include "SketchESP01.h"
#include "SketchGPS.h"
#include "SketchPins.h"

// Instâncias das classes
SketchESP01 esp01;
SketchGPS gps;
SketchPins pins;

void setup() {
  Serial.begin(115200);
  esp01.setup();   // Configura o ESP-01
  gps.setup();     // Configura o GPS
  pins.setup();    // Configura os LEDs
}

void loop() {
  esp01.handle();  // Gerencia conexões do ESP-01
  if (gps.handle()) { // Captura localização
    float latitude = gps.getLatitude();
    float longitude = gps.getLongitude();
    pins.turnOnLocationLED(); // Acende o LED da localização
    esp01.sendLocation(latitude, longitude); // Envia localização ao servidor
  }
  if (esp01.checkSMSSent()) {
    pins.turnOnSMSLED(); // Acende o LED do SMS enviado
  }
}
