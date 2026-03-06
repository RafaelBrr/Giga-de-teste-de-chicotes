/* =====================================================
   BRANCH TestMenuLabviewScrollDisplay
   ===================================================== */

#ifndef __LED__H__
#define __LED__H__
#include <Arduino.h>

void setupLEDs();
void turnOnGreenLED();
void turnOffGreenLED();
void turnOnRedLED();
void turnOffRedLED();
void blinkLEDs(int times, int delayTime);
void testBuzzer();
void autoTest();


#endif  //!__LED__H__