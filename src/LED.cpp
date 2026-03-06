/* =====================================================
   BRANCH BuzzerFeedback
   ===================================================== */

#include <Arduino.h>
#include "LED.h"
#include "Buzzer.h"

#define REDLED A2
#define GREENLED A0

void setupLEDs() {
  // pinMode(A0, OUTPUT); // LED Verde
  // pinMode(A2, OUTPUT); // LED Vermelho
  // pinMode(46, OUTPUT); // LED Verde (feedback)
}

void turnOnGreenLED() {
  digitalWrite(GREENLED, HIGH);
}

void turnOffGreenLED() {
  digitalWrite(GREENLED, LOW);
}

void turnOnRedLED() {
  digitalWrite(REDLED, HIGH);
}

void turnOffRedLED() {
  digitalWrite(REDLED, LOW);
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

void autoTest() {
  Serial.println(F("Auto-test Started..."));
  turnOnGreenLED();
  delay(1000);
  turnOffGreenLED();
  delay(500);
  turnOnRedLED();
  delay(1000);
  turnOffRedLED();
  delay(500);
  blinkLEDs(5, 300);
  testBuzzer();
  Serial.println(F("Auto-test completed."));
}



//************************************************************************************************** */
// Fim do arquivo LED.cpp