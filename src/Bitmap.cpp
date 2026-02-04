#include <Arduino.h>
#include "Bitmap.h"
#include <U8glib.h>
#include "images.h"

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

void drawBitmap() {

    //u8g.drawBitmapP(85, 1, 5, 44, bitmap_linux_logo_icon_37x44_Inverter);

    u8g.drawBitmapP(70, 1, 8, 44, bitmap_Arduino_logo_icon_58x44_Inverter);


}

//************************************************************************************************** */