/* =====================================================
   BRANCH BuzzerFeedback
   ===================================================== */

#include <Arduino.h>
#include "Buzzer.h"
#include <EEPROM.h>

#define FEEDBACK_TIME  60          // ms (ajuste como quiser)
#define BUZZER 46
#define TIMEBUZZER 1000 // Tempo que o buzzer ficará ligado durante o teste (em ms)
#define EEPROM_ADDR_BUZZER 0
#define BUZZER_ON HIGH
#define BUZZER_OFF LOW
#define TIMEBUZZERAPPROVED 1000 // Tempo que o buzzer ficará ligado durante o teste (em ms)
#define TIMEBUZZERDISAPPROVED 200 // Tempo de cada tom para indicação de reprovação (em ms)

static bool feedbackActive = false;
static bool buzzerActive = true;
static unsigned long feedbackStart = 0;

 void triggerFeedback() {
  if(buzzerActive == true){
  digitalWrite(BUZZER, HIGH);
  feedbackActive = true;
  feedbackStart = millis();

  }
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

void saveBuzzerState() {
  EEPROM.update(EEPROM_ADDR_BUZZER, buzzerActive);
}

void loadBuzzerState() {
  buzzerActive = EEPROM.read(EEPROM_ADDR_BUZZER);
}

void enableBuzzer() {
  //digitalWrite(BUZZER, HIGH);
  buzzerActive = true;
   saveBuzzerState();   // salva na EEPROM
}

void disableBuzzer() {
  //digitalWrite(BUZZER, LOW);
  buzzerActive = false;
  saveBuzzerState();   // salva na EEPROM
}

void approvedBuzzer() {
  if(buzzerActive == true){
  // Emite um tom curto para indicar aprovação
  digitalWrite(BUZZER, BUZZER_ON);
  delay(TIMEBUZZERAPPROVED); // Duração do tom (200 ms)
  digitalWrite(BUZZER, BUZZER_OFF);
  }
}

void disapprovedBuzzer() {
  if(buzzerActive == true){

  for(int i=0; i < 4; i++){
    // Emite um tom curto para indicar reprovação
    digitalWrite(BUZZER, BUZZER_ON);
    delay(TIMEBUZZERDISAPPROVED); // Duração do tom (200 ms)
    digitalWrite(BUZZER, BUZZER_OFF);
    delay(TIMEBUZZERDISAPPROVED); // Pausa entre os tons (200 ms)
  }
  
  }
}


//************************************************************************************************** */