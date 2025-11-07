//site: https://medium.com/@keirish15/acs-712-20a-current-sensor-interfacing-with-esp-32-5a3d240acfe9
#define currentpin 4; //pino onde o sensor esta conectado no esp32

//valores de resistencia para o esp32 nao queimar
  // o sensor trabalha somente com 5v e o esp32 trabalha com 3.3v, entao, R1 e R2 são valores de resistencia para fazer uns calculos e nao queimar a placa
float R1 = 6800.0;
float R2 = 12000.0;

void setup() {
  Serial.begin(115200);
  pinMode(currentpin, INPUT);
}

void loop() {
  int adc = analogRead(currentpin);
  float adc_voltage = adc * (3.3 / 4096.0);
  float current_voltage = (adc_voltage * (R1+R2)/R2);
  float current = (current_voltage - 2.5) / 0.185; //calculo da corrente ja corrigido para a versao de 5A do sensor
  Serial.print("Current Value: ");
  Serial.println(current);
  delay(1000);
}
