#ifndef SKETCH_ESP01_H
#define SKETCH_ESP01_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> // Biblioteca para requisições HTTP
#include <vector>
#include <String>
#include "SketchGPS.h"

class SketchESP01 {
  private:
    const char* ssid = "DFRANCY DRYWALL CENTRO";
    const char* password = "dfrancy2023drywall";
    WiFiServer server;
    std::vector<String> coordinates;  // Lista para armazenar coordenadas
    unsigned long lastCaptureTime;
    bool firstCoordinateSent = false; // Controle para enviar a primeira coordenada

    void captureCoordinates(SketchGPS& gps);
    void sendToServer(const String& coord); // Função para envio ao servidor

  public:
    SketchESP01() : server(80), lastCaptureTime(0) {}
    void setup();
    void handle(SketchGPS& gps);
};

#endif
