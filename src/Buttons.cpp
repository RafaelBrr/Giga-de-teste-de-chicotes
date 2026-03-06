/* =====================================================
   BRANCH BRANCH TestMenuLabviewScrollDisplayBuzzer
   ===================================================== */

#include <Arduino.h>
#include "Buttons.h"
#include <U8glib.h>

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET



bool isButtonPressed(int pin) {

    digitalWrite(A2, LOW); // Turn on an LED as an example

    if(digitalRead(pin) == LOW) {

    u8g.firstPage();
  do {
    u8g.setFont(u8g_font_04b_03);//fonte de 5 pixels
    //u8g.drawStr(10, 10, "Button Pressed!");
    u8g.drawStr(10, 20, ("Button on pin: " + String(pin)).c_str());
    digitalWrite(A2, HIGH); // Turn on an LED as an example
    Serial.println("Button on pin " + String(pin) + " pressed.");
    delay(5);

    } while (u8g.nextPage());

    
    }
    return true;
}