
// --- Bibliotecas ---
#include <SoftwareSerial.h>
 
//RX pino 2, TX pino 3
SoftwareSerial esp8266(2, 3);

//constantes 
#define DEBUG true

// =============================================================================================================
// --- Protótipo das Funções Auxiliares ---
String sendData(String command, const int timeout, boolean debug);
 
// =============================================================================================================
// --- Configurações Iniciais ---
void setup()
{
    Serial.begin(9600);
    esp8266.begin(115200);
   
    sendData("AT+RST\r\n", 2000, DEBUG); // rst
    // Conecta a rede wireless
    sendData("AT+CWJAP=\"DFRANCY DRYWALL CENTRO\",\"dfrancy2023drywall\"\r\n", 2000, DEBUG);
    delay(3000);
    sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
    // Mostra o endereco IP
    sendData("AT+CIFSR\r\n", 1000, DEBUG);
    // Configura para multiplas conexoes
    sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
    // Inicia o web server na porta 80
    sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
  
} //end setup


// =============================================================================================================
// --- Loop Infinito ---
void loop()
{
  // Verifica se o ESP8266 esta enviando dados
  if (esp8266.available())
  {
    if (esp8266.find("+IPD,"))
    {
      delay(300);
      int connectionId = esp8266.read() - 48;
 
      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
     
      webpage += "</head><h1><u>Mamute - Web Server - Vander LAB</u></h1><h2>";


  //==========>
      
      webpage += " Ah melhor loja do Brasil</u></h1><h2>";
      webpage += " #vempramamute ";
   
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";
 
      sendData(cipSend, 1000, DEBUG);
      sendData(webpage, 1000, DEBUG);
 
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";
 
      sendData(closeCommand, 3000, DEBUG);
    }
  }
  
} //end loop


 // --- Funções Auxiliares ---
String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  
  return response;
}

 
