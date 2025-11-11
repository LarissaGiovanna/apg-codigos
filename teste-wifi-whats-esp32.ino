#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "uaifai-tiradentes";        // nome da rede Wi-Fi
const char* password = "bemvindoaocesar";   // senha da rede Wi-Fi

// Substitua pelos seus dados do CallMeBot:
const char* phoneNumber = "+558192417746";  // Exemplo: +5511999999999
const char* apiKey = "3953632";   // Exemplo: 123456

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Conectando ao Wi-Fi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Envia a mensagem
  sendMessage("Olá! Mensagem enviada do meu ESP32 via CallMeBot 🚀");
}
void loop() {
  // Nada por enquanto
  sendMessage("vai tomar no cu");
  
}

void sendMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    message.replace(" ", "%20"); // Codifica espaços na URL
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + String(phoneNumber) + "&text=" + message + "&apikey=" + String(apiKey);
   
    Serial.println("Enviando mensagem...");
    Serial.println(url);
   
    http.begin(url);
    int httpCode = http.GET();
   
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.printf("Código HTTP: %d\n", httpCode);
      Serial.println("Resposta:");
      Serial.println(payload);
    } else {
      Serial.printf("Erro na requisição: %d\n", httpCode);
    }
   
    http.end();
  } else {
    Serial.println("Wi-Fi não conectado!");
  }
}

