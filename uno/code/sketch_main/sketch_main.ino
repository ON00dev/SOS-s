#include <SoftwareSerial.h>
#include <TinyGPS.h>

// Configurações do GPS
SoftwareSerial SerialGPS(8, 9);
TinyGPS GPS;
float lat, lon, vel;
unsigned long data, hora;
unsigned short sat;

// Configurações do ESP01
SoftwareSerial esp8266(2, 3); // RX e TX do ESP8266
#define LED_SUCCESS 13
#define LED_ERROR 12

void sendData(String command, const int timeout, bool debug) {
  Serial.println("Enviando comando: " + command); // Adicionado
  esp8266.print(command);
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (esp8266.available()) {
      String response = esp8266.readString();
      if (debug) {
        Serial.println("Resposta: " + response); // Adicionado
      }
    }
  }
}


void setup() {
  // Inicializa comunicação serial
  Serial.begin(9600);
  SerialGPS.begin(9600);
  esp8266.begin(115200);

  // Configura os LEDs como saída
  pinMode(LED_SUCCESS, OUTPUT);
  pinMode(LED_ERROR, OUTPUT);
  digitalWrite(LED_SUCCESS, LOW);
  digitalWrite(LED_ERROR, LOW);

  // Informa o início da configuração
  Serial.println("Iniciando...");

  // Reinicia o ESP8266
  sendData("AT+RST\r\n", 2000, true);

  // Conecta ao Wi-Fi
  sendData("AT+CWJAP=\"DFRANCY DRYWALL CENTRO\",\"dfrancy2023drywall\"\r\n", 5000, true);

  // Configura o modo Wi-Fi
  sendData("AT+CWMODE=1\r\n", 1000, true);

  // Mostra o endereço IP
  sendData("AT+CIFSR\r\n", 1000, true);
  if (esp8266.available()) {
    String response = esp8266.readString();
    Serial.println("Endereço IP: " + response);
    if (response.indexOf("ERROR") != -1) {
      digitalWrite(LED_ERROR, HIGH);
      digitalWrite(LED_SUCCESS, LOW);
      return;
    }
  }

  // Configura múltiplas conexões
  sendData("AT+CIPMUX=1\r\n", 1000, true);

  // Inicia o servidor na porta 80
  sendData("AT+CIPSERVER=1,80\r\n", 1000, true);

  // Verifica se o servidor foi iniciado com sucesso
  while (esp8266.available()) {
    String response = esp8266.readString();
    if (response.indexOf("OK") != -1) {
      digitalWrite(LED_SUCCESS, HIGH);
      Serial.println("Servidor iniciado com sucesso!");
    } else {
      digitalWrite(LED_ERROR, HIGH);
      digitalWrite(LED_SUCCESS, LOW);
      Serial.println("Erro ao iniciar o servidor.");
    }
  }
}

void loop() {
  // Verifica se há dados do GPS disponíveis
  while (SerialGPS.available()) {
    if (GPS.encode(SerialGPS.read())) {
      // Obtém hora e data
      GPS.get_datetime(&data, &hora);

      // Obtém latitude e longitude
      GPS.f_get_position(&lat, &lon);

      // Obtém velocidade
      vel = GPS.f_speed_kmph();

      // Obtém número de satélites
      sat = GPS.satellites();

      // Formata os dados em JSON
      String json = "{\"latitude\": " + String(lat, 6) + ", ";
      json += "\"longitude\": " + String(lon, 6) + ", ";
      json += "\"velocidade\": " + String(vel) + ", ";
      json += "\"satelites\": " + String(sat) + ", ";
      json += "\"hora\": \"" + formatTime(hora) + "\", ";
      json += "\"data\": \"" + formatDate(data) + "\"}";

      // Envia os dados ao cliente
      sendData("AT+CIPSEND=0," + String(json.length()) + "\r\n", 1000, true);
      sendData(json + "\r\n", 1000, true);

      // Debug no monitor serial
      Serial.println("Dados enviados: " + json);
    }
  }
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
