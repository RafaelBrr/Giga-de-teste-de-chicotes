#include <Arduino.h>
#include <U8glib.h>

//U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 ,7); //Enable, RW, RS, RESET
U8GLIB_ST7920_128X64_1X u8g(6, 4, 2 ,8); //Enable, RW, RS, RESET

//U8GLIB_SSD1309_128X64 u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9

//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

unsigned long lastUpdate = 0;
bool showMessage = true;
const unsigned long interval = 1000;


void drawHeader();
void drawMessage(const char* message);
void drawBoxedText(uint8_t x, uint8_t y, uint8_t width, const char* text, bool selected);
void drawMessageDouble(const char* message, const char* message2);
void updateMessage();

void setup() {
  Serial.begin(9600);

  u8g.firstPage();
  do{
    u8g.setFont(u8g_font_6x10);
    u8g.setRot180();  // Rotaciona a tela em 180 graus se necessário
    u8g.drawStr(0, 10, "Hello World!");
  } while (u8g.nextPage());
  
}

void loop() {
  u8g.firstPage();
  do {
    drawHeader();
    //drawMessage("Welcome to U8glib!");
    drawBoxedText(0, 50, 128, "Selectable Item", true);
    updateMessage();
    drawMessageDouble("Message One", "Message Two");
  } while (u8g.nextPage());
}

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

