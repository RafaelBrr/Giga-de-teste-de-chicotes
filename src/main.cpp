#include <Arduino.h>
#include <U8glib.h>

//U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 ,7); //Enable, RW, RS, RESET
U8GLIB_ST7920_128X64_1X u8g(6, 4, 2 ,8); //Enable, RW, RS, RESET

//U8GLIB_SSD1309_128X64 u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9

//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

void drawHeader();
void drawMessage(const char* message);

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
    drawMessage("Welcome to U8glib!");
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

