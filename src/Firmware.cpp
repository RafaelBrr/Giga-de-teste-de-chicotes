
/* =====================================================
   BRANCH BuzzerFeedback
   ===================================================== */
   
#include <Arduino.h>
#include <U8glib.h>

#include "images.h"

extern U8GLIB_ST7920_128X64_1X u8g;

#define FIRMWARE_VERSION "1.0.0"
#define FIRMWARE_NAME "Giga de Teste Chicotes"
//#define FIRMWARE_AUTHOR "Rafael Souza"
#define FIRMWARE_DESCRIPTION "Firmware "
#define FIRMWARE_DATE "29-01-2026"
//#define FIRMWARE_COPYRIGHT "Copyright (c) 2024 Seu Nome Aqui. Todos os direitos reservados."
//#define FIRMWARE_LICENSE "MIT License"
//#define FIRMWARE_WEBSITE "https://seusiteaqui.com"
#define BRANCH_NAME "BuzzerFeedback"

void showFirmwareInfo() {

     u8g.firstPage();
  do {
    u8g.setFont(u8g_font_04b_03);//fonte de 5 pixels
    u8g.setPrintPos(20, 5);
    u8g.println("Firmware Information:");

    u8g.setPrintPos(0, 15);
    u8g.println(FIRMWARE_NAME);

    u8g.setPrintPos(0, 25);
    u8g.print("Version: ");
    u8g.println(FIRMWARE_VERSION);
    

   // u8g.setPrintPos(0, 35);
   // u8g.print("Author: ");
   // u8g.println(FIRMWARE_AUTHOR);

    u8g.setPrintPos(0, 45);
    u8g.print("Description: ");
    u8g.println(FIRMWARE_DESCRIPTION);

    u8g.setPrintPos(0, 55);
    u8g.print("Date: ");
    u8g.println(FIRMWARE_DATE);

    u8g.setPrintPos(0, 64);
    u8g.print("Branch: ");
    //u8g.println(GIT_BRANCH);//This variable is defined in platformio.ini and populated by the git_version.py script

    Serial.println(GIT_BRANCH);
    u8g.println(BRANCH_NAME);
    u8g.drawBitmapP(90, 15, 5, 40, bitmap_Linux);

    } while (u8g.nextPage());
    delay(2000);
}

void showFirmwareVersion() {

     u8g.firstPage();
  do {
    u8g.setFont(u8g_font_04b_03);//fonte de 5 pixels
    u8g.setPrintPos(30, 25);
    u8g.print("Version: ");
    u8g.println(FIRMWARE_VERSION);

    } while (u8g.nextPage());
    delay(2000);
}