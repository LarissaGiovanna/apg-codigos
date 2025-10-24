#define led1 3
#define led2 5
#define botao1 2
#define botao2 4

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);
}

bool estadoBotao1;
bool estadoBotao2;
bool estadoAnteriorBotao1;
bool estadoAnteriorBotao2;

void loop() {
  estadoBotao1 = !digitalRead(botao1);
  estadoBotao2 = !digitalRead(botao2);

  if(estadoBotao1 != estadoAnteriorBotao1 && estadoBotao1 == 1){
    digitalWrite(led1, !digitalRead(led1));
  }

  estadoAnteriorBotao1 = estadoBotao1;

  if(estadoBotao2 != estadoAnteriorBotao2 && estadoBotao2 == 1){
    digitalWrite(led2, !digitalRead(led2));
  }

  estadoAnteriorBotao2 = estadoBotao2;
}
