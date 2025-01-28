#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// Configurações do WiFi
const char* ssid = "DFRANCY DRYWALL CENTRO";
const char* password = "dfrancy2023drywall";

// Servidor Web na porta 80
ESP8266WebServer server(80);

// Buffer para dados do GPS
String lastGPSData = "{}";

void setup() {
  // Configura o LED embutido
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // LED começa desligado (lógica invertida)
  
  // Inicializa Serial para comunicação com Arduino e debug
  Serial.begin(9600);
  Serial.println("\n\nIniciando ESP01...");
  
  // Conecta ao WiFi
  Serial.print("Conectando ao WiFi ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  // Aguarda conexão, piscando LED
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);   // Liga LED
    Serial.print(".");
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);  // Desliga LED
    delay(250);
  }
  
  // LED fica aceso quando conectado
  digitalWrite(LED_BUILTIN, LOW);
  
  // Mostra informações da conexão
  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Força do sinal: ");
  Serial.println(WiFi.RSSI());
  
  // Configura rotas do servidor
  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);
  
  // Inicia o servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  // Gerencia servidor web
  server.handleClient();
  
  // Lê dados do Arduino se disponível
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    if (data.startsWith("{")) {  // Verifica se é JSON
      Serial.println("Dados recebidos do Arduino: " + data);
      lastGPSData = data;
      // Pisca LED ao receber dados
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  
  // Verifica status do Arduino periodicamente
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 5000) {  // A cada 5 segundos
    Serial.println("Verificando status do Arduino...");
    Serial.println("STATUS");
    lastCheck = millis();
  }
  
  // Se perder conexão WiFi, pisca LED rapidamente e tenta reconectar
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Conexão WiFi perdida! Tentando reconectar...");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      Serial.print(".");
    }
    Serial.println("\nWiFi reconectado!");
  }
}

void handleRoot() {
  Serial.println("Página principal acessada");
  String html = "<html><head>";
  html += "<title>SOS-s GPS Tracker</title>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<style>body{font-family:Arial;margin:20px;}</style>";
  html += "</head><body>";
  html += "<h1>SOS-s GPS Tracker</h1>";
  html += "<div id='data'>" + lastGPSData + "</div>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleData() {
  Serial.println("Endpoint /data acessado");
  server.send(200, "application/json", lastGPSData);
} 