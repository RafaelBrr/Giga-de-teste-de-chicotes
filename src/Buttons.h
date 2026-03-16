/* =====================================================
   BRANCH BuzzerFeedback
   ===================================================== */

#ifndef __BUTTONS__H__
#define __BUTTONS__H__
#include <Arduino.h>

void setupButtons();
void triggerFeedback();
void updateFeedback();
void testBuzzer();
void saveBuzzerState();
void loadBuzzerState();
void enableBuzzer();
void disableBuzzer();
void approvedBuzzer();
void disapprovedBuzzer();

// bool isButtonPressed(int pin);



#endif  //!__BUTTONS__H__