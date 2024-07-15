#include <BleGamepad.h>

// Definição dos pinos dos botões e LED
const int pinUp = 2;
const int pinDown = 0;
const int pinLeft = 3;
const int pinRight = 1;
const int pinSelect = 6;
const int pinStart = 4;
const int pinA = 9;
const int pinB = 10;
const int pinX = 8;
const int pinY = 7;
const int pinLED = 5;

BleGamepad bleGamepad("Super Pad Dynavision", "Dynacom", 100);

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);

  // Configura os pinos dos botões como entrada com pull-up
  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinDown, INPUT_PULLUP);
  pinMode(pinLeft, INPUT_PULLUP);
  pinMode(pinRight, INPUT_PULLUP);
  pinMode(pinSelect, INPUT_PULLUP);
  pinMode(pinStart, INPUT_PULLUP);
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinX, INPUT_PULLUP);
  pinMode(pinY, INPUT_PULLUP);

  // Configura o pino do LED como saída
  pinMode(pinLED, OUTPUT);

  // Inicia o BLE Gamepad
  bleGamepad.begin();
}

void loop() {
  // Verifica o estado de cada botão e envia para o BLE Gamepad
  bleGamepad.setButton(1, digitalRead(pinUp) == LOW); // Botão 1: UP
  bleGamepad.setButton(2, digitalRead(pinDown) == LOW); // Botão 2: DOWN
  bleGamepad.setButton(3, digitalRead(pinLeft) == LOW); // Botão 3: LEFT
  bleGamepad.setButton(4, digitalRead(pinRight) == LOW); // Botão 4: RIGHT
  bleGamepad.setButton(5, digitalRead(pinSelect) == LOW); // Botão 5: SELECT
  bleGamepad.setButton(6, digitalRead(pinStart) == LOW); // Botão 6: START
  bleGamepad.setButton(7, digitalRead(pinA) == LOW); // Botão 7: A
  bleGamepad.setButton(8, digitalRead(pinB) == LOW); // Botão 8: B
  bleGamepad.setButton(9, digitalRead(pinX) == LOW); // Botão 9: X
  bleGamepad.setButton(10, digitalRead(pinY) == LOW); // Botão 10: Y

  // Controle do LED baseado no botão A
  if (digitalRead(pinA) == LOW) {
    digitalWrite(pinLED, HIGH);
  } else {
    digitalWrite(pinLED, LOW);
  }

  // Envia as informações para o BLE Gamepad
  bleGamepad.sendReport();

  // Pequeno atraso para evitar sobrecarga no loop
  delay(100);
}
