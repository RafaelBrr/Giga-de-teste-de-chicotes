#include <Arduino.h>
#include <U8glib.h>
#include "TesteCAB_2558.h"

//int baudRate = 9600;
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
        //receivedString = Serial.readString();
        receivedString.trim(); // Remove any trailing newline or spaces
        return receivedString;
        
    }

    return "";

}

void handleCommand(const String &cmd) {

  u8g.firstPage();
  do {
    u8g.drawStr(10, 20, cmd.c_str());
  } while (u8g.nextPage());

  digitalWrite(A2, LOW); // Turn on an LED as an example

  if (cmd == "CAB-2558") {
    Serial.println("Cabo CAB-2558 command received.");
    // Additional handling code here
    // For example, update display
    digitalWrite(A2, HIGH); // Turn on an LED as an example
    //configurarPinosCabo10Vias();
    //realizarTesteContinuidade();

     
    delay(100); // Opcional
  }
}

void writeToSerial(const String &message) {
Serial.print(message);
}

// Overloaded function to handle C-style strings