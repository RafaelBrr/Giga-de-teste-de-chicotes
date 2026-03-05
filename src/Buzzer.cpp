#include <Arduino.h>
#include "Buzzer.h"

#define FEEDBACK_PIN   46   // pode ser buzzer
#define FEEDBACK_TIME  80          // ms (ajuste como quiser)

static bool feedbackActive = false;
static unsigned long feedbackStart = 0;

 void triggerFeedback() {
  digitalWrite(FEEDBACK_PIN, HIGH);
  feedbackActive = true;
  feedbackStart = millis();
}

 void updateFeedback() {
  if (feedbackActive && (millis() - feedbackStart >= FEEDBACK_TIME)) {
    digitalWrite(FEEDBACK_PIN, LOW);
    feedbackActive = false;
  }
}


//************************************************************************************************** */