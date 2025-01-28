#include <SoftwareSerial.h>
#include <TinyGPS.h>

// Configurações do GPS
SoftwareSerial SerialGPS(8, 9);  // RX, TX do GPS
TinyGPS GPS;

// Configuração do ESP01
SoftwareSerial ESP01(2, 3);  // RX(2), TX(3) para ESP01

// Variáveis do GPS
float lat, lon, vel;
unsigned long data, hora;
unsigned short sat;

// LEDs de status
#define LED_SUCCESS 13
#define LED_ERROR 12

void setup() {
  // Inicializa comunicação serial
  Serial.begin(9600);        // Serial monitor
  SerialGPS.begin(9600);     // GPS
  ESP01.begin(9600);         // ESP01
  
  // Configura os LEDs
  pinMode(LED_SUCCESS, OUTPUT);
  pinMode(LED_ERROR, OUTPUT);
  digitalWrite(LED_SUCCESS, LOW);
  digitalWrite(LED_ERROR, LOW);

  Serial.println("\n\nArduino iniciado!");
  Serial.println("GPS: Aguardando dados...");
  Serial.println("ESP01: Aguardando comunicação...");
}

void loop() {
  // Verifica se há dados do GPS disponíveis
  while (SerialGPS.available()) {
    if (GPS.encode(SerialGPS.read())) {
      // Obtém dados do GPS
      GPS.get_datetime(&data, &hora);
      GPS.f_get_position(&lat, &lon);
      vel = GPS.f_speed_kmph();
      sat = GPS.satellites();

      // Formata os dados em JSON
      String json = "{\"latitude\":" + String(lat, 6) + ",";
      json += "\"longitude\":" + String(lon, 6) + ",";
      json += "\"velocidade\":" + String(vel) + ",";
      json += "\"satelites\":" + String(sat) + ",";
      json += "\"hora\":\"" + formatTime(hora) + "\",";
      json += "\"data\":\"" + formatDate(data) + "\"}";

      // Debug no monitor serial
      Serial.println("\nDados do GPS:");
      Serial.println("Latitude: " + String(lat, 6));
      Serial.println("Longitude: " + String(lon, 6));
      Serial.println("Velocidade: " + String(vel) + " km/h");
      Serial.println("Satélites: " + String(sat));
      Serial.println("Data/Hora: " + formatDate(data) + " " + formatTime(hora));

      // Envia para o ESP8266 via SoftwareSerial
      Serial.println("Enviando dados para ESP01...");
      ESP01.println(json);
      
      // Indica sucesso
      digitalWrite(LED_SUCCESS, HIGH);
      delay(100);
      digitalWrite(LED_SUCCESS, LOW);
    }
  }
  
  // Verifica se há comandos do ESP8266
  if (ESP01.available()) {
    String cmd = ESP01.readStringUntil('\n');
    Serial.println("Recebido do ESP01: " + cmd);
    
    if (cmd == "STATUS") {
      Serial.println("Enviando status OK para ESP01");
      ESP01.println("{\"status\":\"OK\"}");
    }
  }
  
  // Se não há dados do GPS, pisca LED de erro
  static unsigned long lastGPSCheck = 0;
  if (millis() - lastGPSCheck > 5000) {  // Verifica a cada 5 segundos
    if (sat == 0) {
      Serial.println("AVISO: Sem sinal GPS!");
      digitalWrite(LED_ERROR, HIGH);
      delay(100);
      digitalWrite(LED_ERROR, LOW);
    }
    lastGPSCheck = millis();
  }
  
  delay(1000); // Atualiza a cada segundo
}

String formatTime(unsigned long hora) {
  char buffer[9];
  sprintf(buffer, "%02lu:%02lu:%02lu", (hora / 1000000) - 3, (hora % 1000000) / 10000, (hora % 10000) / 100);
  return String(buffer);
}

String formatDate(unsigned long data) {
  char buffer[11];
  sprintf(buffer, "%02lu/%02lu/%02lu", data / 10000, (data % 10000) / 100, data % 100);
  return String(buffer);
}
