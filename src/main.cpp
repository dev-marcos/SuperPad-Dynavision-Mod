#include <Arduino.h>
#include <Bounce2.h>
#include <BleGamepad.h>

#define BOUNCE_WITH_PROMPT_DETECTION

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

// Debouncers
Bounce debouncerUp = Bounce();
Bounce debouncerDown = Bounce();
Bounce debouncerLeft = Bounce();
Bounce debouncerRight = Bounce();
Bounce debouncerSelect = Bounce();
Bounce debouncerStart = Bounce();
Bounce debouncerA = Bounce();
Bounce debouncerB = Bounce();
Bounce debouncerX = Bounce();
Bounce debouncerY = Bounce();

BleGamepad bleGamepad("Super Pad Dynavision", "Dynacom", 100);
unsigned long startSelectPressTime = 0;
const unsigned long pairHoldTime = 5000; // 5 seconds

void setup() {
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

    // Configura os debouncers
    debouncerUp.attach(pinUp);
    debouncerDown.attach(pinDown);
    debouncerLeft.attach(pinLeft);
    debouncerRight.attach(pinRight);
    debouncerSelect.attach(pinSelect);
    debouncerStart.attach(pinStart);
    debouncerA.attach(pinA);
    debouncerB.attach(pinB);
    debouncerX.attach(pinX);
    debouncerY.attach(pinY);

    debouncerUp.interval(5);
    debouncerDown.interval(5);
    debouncerLeft.interval(5);
    debouncerRight.interval(5);
    debouncerSelect.interval(5);
    debouncerStart.interval(5);
    debouncerA.interval(5);
    debouncerB.interval(5);
    debouncerX.interval(5);
    debouncerY.interval(5);
}

void loop() {
    // Atualiza os debouncers
    debouncerUp.update();
    debouncerDown.update();
    debouncerLeft.update();
    debouncerRight.update();
    debouncerSelect.update();
    debouncerStart.update();
    debouncerA.update();
    debouncerB.update();
    debouncerX.update();
    debouncerY.update();

    if (bleGamepad.isConnected()) {
        // Liga o LED se estiver conectado
        digitalWrite(pinLED, HIGH);

        // Verifica o estado de cada botão e envia para o BLE Gamepad
        bleGamepad.setButton(1, debouncerUp.read() == LOW);
        bleGamepad.setButton(2, debouncerDown.read() == LOW);
        bleGamepad.setButton(3, debouncerLeft.read() == LOW);
        bleGamepad.setButton(4, debouncerRight.read() == LOW);
        bleGamepad.setButton(5, debouncerSelect.read() == LOW);
        bleGamepad.setButton(6, debouncerStart.read() == LOW);
        bleGamepad.setButton(7, debouncerA.read() == LOW);
        bleGamepad.setButton(8, debouncerB.read() == LOW);
        bleGamepad.setButton(9, debouncerX.read() == LOW);
        bleGamepad.setButton(10, debouncerY.read() == LOW);

        // Controle do modo de emparelhamento
        if (debouncerStart.read() == LOW && debouncerSelect.read() == LOW) {
            if (startSelectPressTime == 0) {
                startSelectPressTime = millis();
            } else if (millis() - startSelectPressTime >= pairHoldTime) {
                // Entra no modo de emparelhamento
                Serial.println("Entering pairing mode...");
                bleGamepad.disconnect();
                bleGamepad.begin();
                startSelectPressTime = 0; // Reseta o temporizador
            }
        } else {
            startSelectPressTime = 0; // Reseta o temporizador se os botões não estiverem pressionados
        }

        bleGamepad.sendReport();

    } else {
        // Pisca o LED se não estiver conectado
        digitalWrite(pinLED, millis() % 1000 < 500);
    }

    // Pequeno atraso para evitar sobrecarga no loop
    delay(10);
}
