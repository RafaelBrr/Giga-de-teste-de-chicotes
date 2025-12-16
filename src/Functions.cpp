#include "Functions.h"
#include <U8g2lib.h>

// Use 'extern' para reutilizar o objeto do display declarado no main.cpp
extern U8G2_ST7920_128X64_F_SW_SPI u8g2;

// Variáveis externas do menu (se estiverem no main.cpp)
extern int menuIndex;
extern int menuSize;
//extern const int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
extern const char* menuItems[];
extern int LED_VERDE;
extern int LED_VERMELHO;

void drawMenu() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(0, 12, "== Menu Jiga de Teste ==");

  for (int i = 0; i < menuSize; i++) {
    if (i == menuIndex) {
      u8g2.drawBox(0, 16 + i * 12, 128, 12);
      u8g2.setDrawColor(0);
      u8g2.drawStr(2, 26 + i * 12, menuItems[i]);
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(2, 26 + i * 12, menuItems[i]);
    }
  }

  u8g2.sendBuffer();
}

//*************************************************************************** */

void mostrarResultados() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(0, 12, "Resultados via Serial");
  u8g2.drawStr(0, 24, "Consulte LabVIEW.");
  u8g2.sendBuffer();
  delay(2000);
}

//*************************************************************************** */

void resetarSistema() {
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(0, 24, "Sistema Resetado");
  u8g2.sendBuffer();
  delay(2000);
}

//*************************************************************************** */

void mostrarSobre() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(0, 12, "Jiga de Teste IMBEL");
  u8g2.drawStr(0, 24, "Versao: 1.0");
  u8g2.drawStr(0, 36, "Arduino Mega Pro Mini");
  u8g2.sendBuffer();
  delay(2500);
}
