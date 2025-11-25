#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "uaifai-tiradentes";
const char* password = "bemvindoaocesar";
                                                                                                                                                          
const char* phoneNumber = "+558193142442";
const char* apiKey = "7722631";

#define RX2_PIN 16   // ajuste se necessário
#define TX2_PIN 17

bool alertaEnviado = false;

// --- Pinos da LED RGB (cátodo comum) ---
const int LED_R = 25;
const int LED_G = 26;
// const int LED_B = 27; // não usado neste caso

// limites
const float LIMITE_ALERTA = 0.500; // envia alerta se I1 > 0.500 A
const float LIMITE_RESET  = 0.300; // reseta alerta se I1 < 0.300 A

void ledVerde() {
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH);
}
void ledVermelha() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);

  // configurar pinos do LED
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  // pinMode(LED_B, OUTPUT);

  // inicia verde
  ledVerde();

  // esvaziar qualquer lixo que venha do Arduino enquanto o ESP inicializa
  delay(200);
  while (Serial2.available()) Serial2.readStringUntil('\n');

  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (Serial2.available()) {
    String linha = Serial2.readStringUntil('\n'); // lê até \n
    linha.trim();                                 // remove espaços e \r
    if (linha.length() == 0) {
      // linha vazia — ignora
      return;
    }

    Serial.print("Recebido (bruto): '");
    Serial.print(linha);
    Serial.println("'");

    // Tentativa robusta de extrair 4 floats. Se não der 4, ignoramos a linha.
    float V1, I1, V2, I2;
    int matched = sscanf(linha.c_str(), "%f,%f,%f,%f", &V1, &I1, &V2, &I2);

    if (matched != 4) {
      Serial.println("Linha ignorada (formato inválido).");
      return; // volta pro loop sem erro
    }

    Serial.print("Corrente 1 = ");
    Serial.println(I1, 3);

    // ---- DETECÇÃO DO GATO ----
    if (I1 > LIMITE_ALERTA && !alertaEnviado) {
      sendMessage("Na área da Cesar, está tendo um possível gato de energia");
      alertaEnviado = true;
      ledVermelha(); // muda LED para vermelho quando enviar alerta
    }

    // Reseta alerta se corrente cair bastante (evita spam)
    if (I1 < LIMITE_RESET && alertaEnviado) {
      alertaEnviado = false;
      ledVerde(); // volta para verde
    }
  }

  delay(10);
}

void sendMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    message.replace(" ", "%20");
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + String(phoneNumber) + "&text=" + message + "&apikey=" + String(apiKey);

    Serial.println("Enviando mensagem...");
    Serial.println(url);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("Código HTTP: %d\n", httpCode);
      Serial.println(http.getString());
    } else {
      Serial.printf("Erro na requisição: %d\n", httpCode);
    }
    http.end();
  } else {
    Serial.println("Wi-Fi não conectado!");
  }
}
