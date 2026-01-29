#include <Arduino.h>
#include "ProgressBar.h"
#include "U8g2lib.h"

extern U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /*clk=*/52, /*data=*/51, /*cs=*/53, /*reset=*/U8X8_PIN_NONE);

void drawProgressBar(int x, int y, int width, int height, uint8_t progressPercent) {
  // Desenha o contorno da barra de progresso
  u8g2.drawFrame(x, y, width, height);

  // Calcula a largura preenchida com base na porcentagem de progresso
  int filledWidth = (width - 2) * progressPercent / 100;

  // Desenha a parte preenchida da barra de progresso
  if (filledWidth > 0) {
    u8g2.drawBox(x + 1, y + 1, filledWidth, height - 2);
  }
}