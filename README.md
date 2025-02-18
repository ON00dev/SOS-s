# SOS-s :sos:

### **1. Tema e Escopo do Projeto**  
Este projeto aborda a **localização em tempo real** e a **transmissão de dados geográficos** via rede Wi-Fi, com o objetivo de oferecer uma solução acessível e eficiente para rastreamento de objetos ou pessoas. A aplicação pode ser utilizada em áreas como logística, segurança, ou monitoramento de veículos.  

---

### **2. Funcionalidade**  
Este sistema combina diferentes módulos e sensores para atingir sua funcionalidade principal:  
- **Interface Web**: [SOS-s](https://sos-s-sos-s.up.railway.app/)
- **GPS**: Captura a localização em tempo real (latitude, longitude) e outros dados relevantes, como velocidade e número de satélites disponíveis.  
- **ESP8266 (ESP01)**: Conecta o sistema à internet, transmitindo os dados obtidos pelo GPS para um servidor remoto.  
- **Arduino**: Atua como controlador central, integrando os módulos e gerenciando a lógica de operação, incluindo:  
  - Configuração do ESP8266 via comandos AT.  
  - Recebimento e processamento de dados do módulo GPS.  
  - Envio de informações para o servidor em formato JSON.  
  - Indicação de status com LEDs (por exemplo, sucesso ou falha de conexão).  

---

### **3. Requisitos Técnicos e Componentes do Sistema** 🔧
Para montar o projeto, você precisará de:  
- **Módulo GPS**  
- **Módulo ESP8266 (ESP01)**  
- **Arduino Uno** ou outro microcontrolador compatível  
- **Protoboard e jumpers**  
- Resistores e capacitores para estabilização elétrica  
- LEDs para indicação de status  

O projeto utiliza bibliotecas de código aberto, como:  
- `TinyGPS.h` para manipulação dos dados do módulo GPS.  
- `SoftwareSerial.h` para comunicação serial com dispositivos adicionais.  

---

### **4. Etapas de Desenvolvimento e Montagem**  🛠️

#### **4.1. Planejamento e Configuração**  
1. Identifique o problema que o sistema resolve (por exemplo, rastreamento de frotas).  
2. Configure o ESP8266 para se conectar a uma rede Wi-Fi utilizando comandos AT.  
3. Teste o módulo GPS para garantir que ele capture os dados corretamente.  

#### **4.2. Montagem do Circuito**  
1. Conecte o GPS ao Arduino utilizando portas digitais para comunicação serial.  
2. Conecte o ESP8266 ao Arduino, com resistores para ajuste de níveis de tensão, garantindo a compatibilidade com o ESP01.  
3. Use LEDs para indicar o status de conexão e operação.  

#### **4.3. Programação**  
1. Escreva o código para integrar os módulos.  
2. Adicione a lógica para capturar dados do GPS, processá-los e enviá-los ao servidor.  
3. Implemente mecanismos para lidar com erros (por exemplo, falha na conexão Wi-Fi).  

---

### **5. Como Funciona**  
1. O sistema inicia e configura automaticamente o ESP8266 para conectar à rede Wi-Fi.
2. O módulo GPS captura dados em tempo real, incluindo latitude, longitude, velocidade e hora.  
3. O Arduino processa os dados do GPS e os envia ao servidor via ESP8266.  
4. LEDs indicam o status da operação:  
   - 🟢LED Verde: Sistema funcionando corretamente.  
   - 🔴LED Vermelho: Erro na conexão Wi-Fi ou na aquisição de dados.

---

### **6. Problemas Resolvidos**  
O projeto resolve o problema de rastreamento em tempo real para aplicações que exigem mobilidade e conectividade, reduzindo custos e aumentando a acessibilidade a soluções baseadas em IoT.  

---

### **7. Referências**  
- [Manual do módulo GPS utilizado.](https://github.com/neosarchizo/TinyGPS)  
- [Documentação oficial do ESP8266.](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)  
- Inclusão e configuração inicial dos módulos (`TinyGPS` e `SoftwareSerial`):
```cpp
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
```
execução de comando *AT* para controlar módulo ESP01
```cpp
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
```
**[código completo aqui](https://github.com/ON00dev/SOS-s/blob/main/uno/code/sketch_main/sketch_main.ino)**

---

### **8. Observações Importantes**  
Certifique-se de ajustar os parâmetros do código para a rede Wi-Fi utilizada, como SSID e senha. Além disso, revise as conexões elétricas para evitar danos aos componentes.  
```cpp
// Conecta ao Wi-Fi
  sendData("AT+CWJAP=\"SSID_DO_WIFI\",\"SENHA_DO_WIFI\"\r\n", 5000, true);
```

🛃 **Nota:** Este projeto é adaptável a outras funcionalidades e pode ser estendido para integrar sensores adicionais, como acelerômetros ou câmeras, dependendo do escopo desejado. 
---
See also [SOS-s documentation](https://docs.google.com/document/d/1nDGwWKtZFCkAieFbK5GynQkjosFsjziR2pj2ZRsQTPE/edit?usp=sharing) [ :us: ]

---  
