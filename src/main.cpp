#include <Arduino.h>
#include <Bounce2.h>
#include <BleGamepad.h>

#define BOUNCE_WITH_PROMPT_DETECTION

// Definição dos pinos dos botões e LED
const int pinUp = 10;
const int pinDown = 8;
const int pinLeft = 20;
const int pinRight = 9;
const int pinSelect = 4;
const int pinStart = 7;
const int pinA = 2;
const int pinB = 3;
const int pinX = 1;
const int pinY = 0;
const int pinLED = 21;
const int pinBateria = 5; // Pino analógico para leitura do valor da bateria

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

// Variáveis para debug de bateria
unsigned long lastDebugTime = 0;
const unsigned long debugInterval = 500; // Atualiza a cada 500ms

// Função para ler a tensão da bateria
float readBatteryVoltage() {
    int rawValue = analogRead(pinBateria);
    // Conversão: rawValue (0-4095) -> Tensão ADC (0-3.3V)
    // Tensão real da bateria = Tensão ADC * 2 (devido ao divisor de tensão 100k+100k)
    float adcVoltage = (rawValue / 4095.0) * 3.3;
    float batteryVoltage = adcVoltage * 2.0;
    return batteryVoltage;
}

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

    // Debug: Exibe estado dos botões e bateria a cada 500ms
    if (millis() - lastDebugTime >= debugInterval) {
        lastDebugTime = millis();
        
        float batteryVoltage = readBatteryVoltage();
        
        Serial.println("=== SUPER PAD DEBUG ===");
        Serial.print("Bateria: ");
        Serial.print(batteryVoltage, 2);
        Serial.println("V");
        
        Serial.print("Botões [U:"); Serial.print(debouncerUp.read() == LOW ? "1" : "0");
        Serial.print(" D:"); Serial.print(debouncerDown.read() == LOW ? "1" : "0");
        Serial.print(" L:"); Serial.print(debouncerLeft.read() == LOW ? "1" : "0");
        Serial.print(" R:"); Serial.print(debouncerRight.read() == LOW ? "1" : "0");
        Serial.print(" SEL:"); Serial.print(debouncerSelect.read() == LOW ? "1" : "0");
        Serial.print(" STR:"); Serial.print(debouncerStart.read() == LOW ? "1" : "0");
        Serial.print(" A:"); Serial.print(debouncerA.read() == LOW ? "1" : "0");
        Serial.print(" B:"); Serial.print(debouncerB.read() == LOW ? "1" : "0");
        Serial.print(" X:"); Serial.print(debouncerX.read() == LOW ? "1" : "0");
        Serial.print(" Y:"); Serial.print(debouncerY.read() == LOW ? "1" : "0");
        Serial.println("]");
        
        Serial.print("BLE: ");
        Serial.println(bleGamepad.isConnected() ? "Conectado" : "Desconectado");
        Serial.println("");
    }

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
