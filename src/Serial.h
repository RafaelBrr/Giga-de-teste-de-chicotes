/* =====================================================
   BRANCH BuzzerFeedback
   ===================================================== */

#ifndef __SERIAL__H__
#define __SERIAL__H__

#include <Arduino.h>
#include <U8glib.h>

void initializeSerial();
String readFromSerial();
void handleCommand(const String &cmd);
void writeToSerial(const String &message);
void writeToSerial(const char *message);


#endif  //!__SERIAL__H__