/*
  ESP32 - Medição de Corrente DC com Sensor ACS712
  Baseado no código de Krishnarajsinh Jadav (adaptado para ESP32 por ChatGPT)
*/

const int ACS_PIN = 27;  // Pino analógico conectado ao sensor (GPIO27)
double voltage = 0;
double current = 0;

// Configuração do sensor
const double VCC = 3.3;          // Tensão de referência do ADC do ESP32
const int ADC_RES = 4095;        // Resolução ADC 12 bits
const double SENSITIVITY = 0.185; // Sensibilidade do ACS712-5A (V/A)
const double OFFSET = 1.31;   // Valor típico de saída do sensor sem corrente (meio da tensão de referência)

void setup() {
  Serial.begin(9600);
  analogReadResolution(12);      // Garante resolução de 12 bits
  delay(1000);
  Serial.println("Iniciando leitura do ACS712 no ESP32...");
}

void loop() {
  voltage = 0;

  // Média de 1000 leituras para maior precisão
  for (int i = 0; i < 1000; i++) {
    voltage += (analogRead(ACS_PIN) * (VCC / ADC_RES));
    delayMicroseconds(500);  // pequena pausa entre amostras
  }
  voltage /= 1000.0;

  // Converter tensão medida em corrente (A)
  current = (voltage - OFFSET) / SENSITIVITY;

  Serial.print("Tensão (V): ");
  Serial.print(voltage, 3);
  Serial.print("\t Corrente (A): ");
  Serial.println(current, 3);

  delay(500);
}
