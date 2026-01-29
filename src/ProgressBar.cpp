#include <Arduino.h>
#include "ProgressBar.h"
#include "U8glib.h"

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

void drawProgressBar(int x, int y, int width, int height, uint8_t progressPercent) {

  u8g.firstPage();
  do {
  // Desenha o contorno da barra de progresso
  u8g.drawFrame(x, y, width, height);

  // Calcula a largura preenchida com base na porcentagem de progresso
  int filledWidth = (width - 2) * progressPercent / 100;

  // Desenha a parte preenchida da barra de progresso
  if (filledWidth > 0) {
    u8g.drawBox(x + 1, y + 1, filledWidth, height - 2);
  }

  } while (u8g.nextPage());

  delay(100);
}