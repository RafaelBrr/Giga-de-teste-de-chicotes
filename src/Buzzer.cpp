#include <Arduino.h>
#include "Buzzer.h"

#define FEEDBACK_TIME  60          // ms (ajuste como quiser)
#define BUZZER 46
#define TIMEBUZZER 1000 // Tempo que o buzzer ficará ligado durante o teste (em ms)

static bool feedbackActive = false;
static unsigned long feedbackStart = 0;

 void triggerFeedback() {
  digitalWrite(BUZZER, HIGH);
  feedbackActive = true;
  feedbackStart = millis();
}

 void updateFeedback() {
  if (feedbackActive && (millis() - feedbackStart >= FEEDBACK_TIME)) {
    digitalWrite(BUZZER, LOW);
    feedbackActive = false;
  }
}

void testBuzzer() {
  Serial.println(F("Testing buzzer..."));
  //tone(46, 1000); // Emite um tom de 1000 Hz no pino 46
  digitalWrite(BUZZER, HIGH); // Acende o LED verde para indicar teste do buzzer
  delay(TIMEBUZZER);   // Aguarda por 1 segundo
  //noTone(46);     // Para o tom
  digitalWrite(BUZZER, LOW); // Apaga o LED verde
  Serial.println(F("Buzzer test completed."));
  delay(TIMEBUZZER);   // Aguarda por 1 segundo para garantir que o buzzer desligou
}   

void enableBuzzer() {
  digitalWrite(BUZZER, HIGH);
  feedbackActive = true;
}

void disableBuzzer() {
  digitalWrite(BUZZER, LOW);
  feedbackActive = false;
}


//************************************************************************************************** */