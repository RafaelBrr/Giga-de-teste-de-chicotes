#include <Arduino.h>
#include <U8glib.h>

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

unsigned long lastUpdate = 0;
bool showMessage = true;
const unsigned long interval = 1000;

void drawHeader() {
  u8g.setFont(u8g_font_6x10);
  u8g.drawStr(0, 20, "HEADER");
}

void drawMessage(const char* message) {
  u8g.setFont(u8g_font_6x10);
  u8g.drawStr(0, 30, message);
}

void updateMessage() {
  unsigned long now = millis();

  if(now - lastUpdate >= 1000) { // Update every second
    lastUpdate = now;
    showMessage = !showMessage;

  }
}
void drawMessageDouble(const char* message, const char* message2) {
  u8g.setFont(u8g_font_6x10);

  if(showMessage) {
    u8g.drawStr(0, 30, message);
  } else {
    u8g.drawStr(0, 30, message2);
  }
  
}

void drawBoxedText(uint8_t x, uint8_t y,uint8_t width, const char* text, bool selected)
{
  const uint8_t padding = 2;
  const uint8_t fontHeight = 10;   // u8g_font_6x10
  const uint8_t boxHeight = fontHeight + padding * 2;

  if (selected) {
    // Desenha fundo
    u8g.drawBox(x, y - fontHeight - padding,
                width, boxHeight);

    // Inverte cor do texto
    u8g.setColorIndex(0);
    u8g.drawStr(x + padding, y, text);
    u8g.setColorIndex(1);
  } else {
    // Apenas texto
    u8g.drawStr(x + padding, y, text);
  }
}