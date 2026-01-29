#include <Arduino.h>
#include "ProgressBar.h"
#include "U8glib.h"

#define progressBarPosX 13
#define progressBarPosY 50

#define progressBarWidth 101
#define progressBarHeight 10


extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

void drawProgressBar(int value) {

  
  int barWidth = map(value, 0, 100, 0, 100);

  u8g.firstPage();
  do {
    //u8g.setFont(u8g_font_6x10);
    u8g.setFont(u8g_font_04b_03);//fonte de 5 pixels
    u8g.drawStr(15, 15, "SimpleProgressBar");
    
    // Outer box
    u8g.drawFrame(progressBarPosX, progressBarPosY, progressBarWidth, progressBarHeight);
    //u8g.drawFrame(12, 24, 102, 17);
    u8g.drawFrame(progressBarPosX-1, progressBarPosY-2, progressBarWidth+4, progressBarHeight+4);
    // Filled bar
    u8g.drawBox(progressBarPosX, progressBarPosY, barWidth, progressBarHeight);

    // Percentage text
    char percentStr[5];
    sprintf(percentStr, "%d%%", value);
    u8g.drawStr(15, 25, percentStr);
    
   
  } while (u8g.nextPage());
}