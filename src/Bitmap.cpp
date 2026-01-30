#include <Arduino.h>
#include "Bitmap.h"
#include <U8glib.h>

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

void drawBitmap() {

u8g.firstPage();
do {
  u8g.drawXBMP(10, 10, logo_width, logo_height, logo_bits);
} while (u8g.nextPage());

}

//************************************************************************************************** */