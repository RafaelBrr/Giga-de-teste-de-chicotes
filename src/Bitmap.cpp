/* =====================================================
   BRANCH BuzzerFeedback
   ===================================================== */

#include <Arduino.h>
#include "Bitmap.h"
#include <U8glib.h>
#include "images.h"

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

void drawBitmap() {

    //u8g.drawBitmapP(85, 1, 5, 44, bitmap_linux_logo_icon_37x44_Inverter);

    u8g.drawBitmapP(10, 0, 14, 40, bitmap_Linux_logo_icon_112x40_Inverter);


}

//************************************************************************************************** */