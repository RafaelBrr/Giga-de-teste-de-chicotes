#include <Arduino.h>
#include "LED.h"

void setupLEDs() {
  // pinMode(A0, OUTPUT); // LED Verde
  // pinMode(A2, OUTPUT); // LED Vermelho
  // pinMode(46, OUTPUT); // LED Verde (feedback)
}

void turnOnGreenLED() {
  digitalWrite(A0, HIGH);
}

void turnOffGreenLED() {
  digitalWrite(A0, LOW);
}

void turnOnRedLED() {
  digitalWrite(A2, HIGH);
}

void turnOffRedLED() {
  digitalWrite(A2, LOW);
}

void blinkLEDs(int times, int delayTime) {
  for (int i = 0; i < times; i++) {
    turnOnGreenLED();
    turnOffRedLED();
    delay(delayTime);
    turnOffGreenLED();
    turnOnRedLED();
    delay(delayTime);
  }
  turnOffRedLED(); // Certifique-se de desligar o LED vermelho no final
}

void testBuzzer() {
  Serial.println(F("Testing buzzer..."));
  //tone(8, 1000); // Emite um tom de 1000 Hz no pino 8
  digitalWrite(46, HIGH); // Acende o LED verde para indicar teste do buzzer
  delay(1000);   // Aguarda por 1 segundo
  //noTone(8);     // Para o tom
  digitalWrite(46, LOW); // Apaga o LED verde
  Serial.println(F("Buzzer test completed."));
  delay(500);
}   

void autoTest() {
  Serial.println(F("Testing LEDs..."));
  turnOnGreenLED();
  delay(1000);
  turnOffGreenLED();
  delay(500);
  turnOnRedLED();
  delay(1000);
  turnOffRedLED();
  delay(500);
  blinkLEDs(5, 300);
  Serial.println(F("LED test completed."));
  testBuzzer();
}

//************************************************************************************************** */
// Fim do arquivo LED.cpp