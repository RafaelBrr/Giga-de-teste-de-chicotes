#include <Arduino.h>
#include <U8glib.h>

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET


void showMenu() {

    u8g.firstPage();
  do {
    // Implementação da função de exibição do menu
    Serial.println("Exibindo o menu...");

     } while (u8g.nextPage());
}