#include "SketchPins.h"

void SketchPins::begin() {
    // Configura os pinos 8, 9 e 10 como saída
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
}

void SketchPins::turnOn(int pin) {
    digitalWrite(pin, HIGH); // Liga o LED conectado ao pino
}

void SketchPins::turnOff(int pin) {
    digitalWrite(pin, LOW); // Desliga o LED conectado ao pino
}
