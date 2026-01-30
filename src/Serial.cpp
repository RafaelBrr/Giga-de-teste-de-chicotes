#include <Arduino.h>
#include <U8glib.h>

int baudRate = 9600;
String receivedString = "";

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

void initializeSerial() {
     u8g.firstPage();
  do {
    u8g.drawStr(10, 10, "Initializing Serial...");

  } while (u8g.nextPage());
}

String readFromSerial() {
    while (Serial.available()) {
        receivedString = Serial.readStringUntil('\n');
        receivedString.trim(); // Remove any trailing newline or spaces
        
    }

        u8g.firstPage();
  do {
    u8g.drawStr(10, 20, receivedString.c_str());

  } while (u8g.nextPage());

    if(receivedString == "CAB-2558"){
            Serial.println("Cabo CAB-2558 command received.");  

    u8g.firstPage();
  do {
    u8g.drawStr(10, 20, "Cabo CAB-2558");

  } while (u8g.nextPage());
}
delay(10);
return receivedString;
}

void writeToSerial(const String &message) {
    Serial.print(message);
}

// Overloaded function to handle C-style strings