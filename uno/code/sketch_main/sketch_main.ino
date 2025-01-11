#include "SketchESP01.h"
#include "SketchGPS.h"
#include "SketchPins.h"

// Instâncias dos módulos
SketchESP01 esp01;
SketchGPS gps;
SketchPins pinModule;

void setup() {
  esp01.begin();          // Inicializa o ESP-01
  gps.begin();            // Inicializa o GPS
  pinModule.begin();      // Inicializa os pinos
  pinModule.turnOn(8);    // Liga o LED do pino 8 (Wi-Fi conectado)
}

void loop() {
  gps.update();           // Atualiza os dados do GPS
  esp01.handle(gps);      // Passa o objeto GPS para o ESP-01 gerenciar

  if (gps.hasNewData()) { // Verifica se há novos dados de localização
    double latitude = gps.getLatitude();
    double longitude = gps.getLongitude();
    pinModule.turnOn(9);  // Liga o LED do pino 9 (localização capturada)

    // Envia a localização ao servidor
    String coordJson = "{\"latitude\": " + String(latitude, 6) + ", \"longitude\": " + String(longitude, 6) + "}";
    esp01.sendToServer(coordJson);
  }

  // Verifica se o SMS foi enviado
  if (esp01.checkSMSSent()) {
    pinModule.turnOn(10); // Liga o LED do pino 10
  }
}
