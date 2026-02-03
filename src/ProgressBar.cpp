#include <Arduino.h>
#include "ProgressBar.h"
#include "U8glib.h"

#define progressBarPosX 5
#define progressBarPosY 50

#define progressBarWidth 101
#define progressBarHeight 10


extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

String progressBarMessage = " ";

void drawProgressBar(int value) {

  
  int barWidth = map(value, 0, 100, 0, 100);

  
   do {

u8g.firstPage();
  do {
    //u8g.setFont(u8g_font_6x10);
    u8g.setFont(u8g_font_04b_03);//fonte de 5 pixels
    u8g.drawStr(15, 15, "Giga IMBEL");
    
    // Outer decorative frame
    u8g.drawFrame(progressBarPosX-3, progressBarPosY-2, progressBarWidth+6, progressBarHeight+4);
    // Inner decorative frame
    u8g.drawFrame(progressBarPosX, progressBarPosY, progressBarWidth, progressBarHeight);
    //u8g.drawFrame(12, 24, 102, 17);
   
    // Filled bar
    u8g.drawBox(progressBarPosX, progressBarPosY, barWidth, progressBarHeight);

    // Percentage text
    char percentStr[5];
    sprintf(percentStr, "%d%%", value);
    u8g.drawStr(112, 58, percentStr);

    //u8g.drawStr(13, 40, progressBarMessage);
    u8g.setPrintPos(13, 45);
    //progressBarMessage = (value < 75)?" Loading system...":"System ready...";
    //progressBarMessage = (value < 90)?(value < 25)?" Loading system...":"Loading parameters...":"System OK!";
    //progressBarMessage = (value < 90)?(value < 65)?(value < 15)?"Initializing system...":"Loading OS...":"Loading parameters...":"System OK!";
    progressBarMessage = (value < 90)?(value < 65)?(value < 55)?(value < 45)?(value < 40)?(value < 35)?(value < 30)?(value < 25)?(value < 20)?(value < 15)?"Initializing system...":"Loading OS ":"Loading OS.":"Loading OS..":"Loading OS...":"Loading OS":"Loading OS.":"Loading OS..":"Loading OS...":"Loading parameters...":"System OK!";
    u8g.print(progressBarMessage);
    
   
  } while (u8g.nextPage());

delay(10);
barWidth++;
value++;

   }while(value <= 100);

  
}
