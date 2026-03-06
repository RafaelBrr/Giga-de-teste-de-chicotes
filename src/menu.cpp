/* =====================================================
   BRANCH drawBitmap
   ===================================================== */

#include <Arduino.h>
#include <U8glib.h>

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET


void showMenu() {

    u8g.firstPage();
  do {
    // Implementação da função de exibição do menu
    //Serial.println("Exibindo o menu...");
    u8g.setFont(u8g_font_04b_03);//fonte de 5 pixels
    u8g.drawStr(5, 15, "Menu Principal");
    u8g.drawStr(10, 25, "1. Iniciar Teste");
    u8g.drawStr(10, 35, "2. Configuracoes");  


     } while (u8g.nextPage());
}