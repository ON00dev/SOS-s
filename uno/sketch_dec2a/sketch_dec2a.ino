#include <ESP8266WiFi.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Configurações Wi-Fi
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

// Configurações do servidor Flask
const char* server = "seu-projeto.up.railway.app";
const int port = 80;

// Instâncias do GPS
TinyGPSPlus gps;
SoftwareSerial gpsSerial(2, 3); // GPIO2 (RX), GPIO3 (TX) do ESP-01

// LEDs (usando GPIO0 e GPIO2 do ESP-01)
const int led1 = 0; // Localização capturada
const int led2 = 2; // SMS enviado
// Nota: O ESP-01 tem poucos pinos, mas é possível multiplexar LEDs ou usar um pino para dois estados.

// Variáveis de controle dos LEDs
bool led1On = false;
bool led2On = false;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);

  // Configura os LEDs como saída
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // Apaga todos os LEDs inicialmente
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Wi-Fi conectado!");
}

void loop() {
  // Lê os dados do GPS
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isUpdated()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();

        // Indica que a localização foi capturada (LED1)
        digitalWrite(led1, HIGH);
        led1On = true;

        // Envia os dados ao servidor Flask
        if (sendSOS(latitude, longitude)) {
          // Indica que o SMS foi enviado (LED2)
          digitalWrite(led2, HIGH);
          led2On = true;
        } else {
          Serial.println("Falha ao enviar SOS.");
        }

        delay(60000); // Aguarda 60 segundos antes de repetir
      }
    }
  }
}

bool sendSOS(float lat, float lon) {
  if (client.connect(server, port)) {
    Serial.println("Conectado ao servidor!");

    // Monta o JSON com os dados de localização
    String postData = "{\"gps_location\": \"" + String(lat, 6) + "," + String(lon, 6) + "\"}";

    // Envia a requisição HTTP
    client.println("POST /sos HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Content-Type: application/json");
    client.println("Content-Length: " + String(postData.length()));
    client.println();
    client.println(postData);

    // Lê a resposta do servidor
    while (client.available()) {
      String response = client.readString();
      Serial.println(response);
    }
    client.stop();
    return true; // SOS enviado com sucesso
  } else {
    Serial.println("Falha ao conectar ao servidor.");
    return false;
  }
}
