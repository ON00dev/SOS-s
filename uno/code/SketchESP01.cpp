#include "SketchESP01.h"

void SketchESP01::setup() {
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Servidor iniciado.");
}

void SketchESP01::handle(SketchGPS& gps) {
  // Captura coordenadas a cada 30 segundos
  if (millis() - lastCaptureTime > 30000) {
    captureCoordinates(gps);
    lastCaptureTime = millis();
  }

  // Aguarda conexões de clientes
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado!");
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Gera a página HTML com as coordenadas
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<h1>Coordenadas Capturadas</h1>");
    client.println("<ul>");
    for (const String& coord : coordinates) {
      client.println("<li>" + coord + "</li>");
    }
    client.println("</ul>");
    client.println("</html>");
    delay(1);
    Serial.println("Resposta enviada ao cliente.");
  }
}

void SketchESP01::captureCoordinates(SketchGPS& gps) {
  if (gps.handle()) {
    float latitude = gps.getLatitude();
    float longitude = gps.getLongitude();
    String coord = "{\"latitude\": " + String(latitude, 6) + ", \"longitude\": " + String(longitude, 6) + "}";
    coordinates.push_back(coord);
    Serial.println("Coordenada capturada: " + coord);

    // Envia a primeira coordenada ao servidor
    if (!firstCoordinateSent) {
      sendToServer(coord);
      firstCoordinateSent = true; // Marca como enviada
    }
  } else {
    Serial.println("Falha ao capturar coordenadas.");
  }
}

void SketchESP01::sendToServer(const String& coord) {
  if (WiFi.status() == WL_CONNECTED) { // Verifica se está conectado ao Wi-Fi
    HTTPClient http;
    http.begin("https://sos-s-sos-s.up.railway.app/sos"); // URL do servidor
    http.addHeader("Content-Type", "application/json"); // Define o cabeçalho da requisição

    int httpResponseCode = http.POST(coord); // Envia a coordenada no corpo da requisição

    if (httpResponseCode > 0) {
      Serial.println("Requisição enviada com sucesso. Código HTTP: " + String(httpResponseCode));
    } else {
      Serial.println("Erro ao enviar requisição. Código HTTP: " + String(httpResponseCode));
    }

    http.end(); // Encerra a conexão HTTP
  } else {
    Serial.println("Falha na conexão com Wi-Fi. Requisição não enviada.");
  }
}
