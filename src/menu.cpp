#include <Arduino.h>
#include <U8glib.h>
#include "Firmware.h"

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

#define COUNT_OF(x) (sizeof(x) / sizeof(x[0]))

/* =====================================================
   BRANCH TESTMENULABVIEW
   ===================================================== */


/* =====================================================
   BOTOES
   ===================================================== */
#define BTN_UP     8
#define BTN_DOWN   9
#define BTN_OK     10
#define BTN_BACK   11

/* =====================================================
   LIMITES
   ===================================================== */
#define MAX_LEVELS 4

/* =====================================================
   ESTRUTURA DO MENU
   ===================================================== */
struct MenuItem {
  const char* label;
  MenuItem* children;
  uint8_t childCount;
  void (*action)();
};

/* =====================================================
   STACK DE NAVEGACAO
   ===================================================== */
static MenuItem* menuStack[MAX_LEVELS];
static uint8_t   indexStack[MAX_LEVELS];
static uint8_t   countStack[MAX_LEVELS];

/* =====================================================
   ESTADO ATUAL
   ===================================================== */
static MenuItem* currentMenu;
static uint8_t   currentIndex = 0;
static uint8_t   currentCount = 0;
static uint8_t   currentLevel = 0;

/* =====================================================
   FLAGS SERIAL (SIMULAM BOTOES)
   ===================================================== */
static bool serialUp   = false;
static bool serialDown = false;
static bool serialOk   = false;
static bool serialBack = false;

/* =====================================================
   ACOES
   ===================================================== */
static void actionLedOn()  { Serial.println(F("LED ON"));  }
static void actionLedOff() { Serial.println(F("LED OFF")); }
static void actionBlink() { Serial.println(F("BLINK"));   }
static void actionInfo()  { Serial.println(F("INFO"));    }

/* =====================================================
   MENUS
   ===================================================== */
static MenuItem menuTests[] = {
  { "LED ON",  NULL, 0, actionLedOn  },
  { "LED OFF", NULL, 0, actionLedOff },
  { "BLINK",   NULL, 0, actionBlink  }
};

static MenuItem menuMain[] = {
  { "TESTES", menuTests, 3, NULL },
  { "SOBRE",  NULL,      0, actionInfo }
};

/* =====================================================
   BOTAO (DEBOUNCE SIMPLES)
   ===================================================== */
static bool pressed(uint8_t pin) {
 static uint8_t lastState[20]; // indexado por pino
  uint8_t idx = pin;

  uint8_t state = digitalRead(pin);

  if (lastState[idx] == HIGH && state == LOW) {
    lastState[idx] = state;
    return true;   // borda de descida
  }

  lastState[idx] = state;
  return false;
}

/* =====================================================
   SERIAL – PARSER FIXO (UNO SAFE)
   ===================================================== */
static char serialBuf[32];
static uint8_t serialPos = 0;

static void handleSerial() {
  while (Serial.available()) {
    char c = (char)Serial.read();

    if (c == '\r')
      continue;

    if (c == '\n') {
      serialBuf[serialPos] = '\0';
      serialPos = 0;

      /* ---- COMANDOS ---- */
      if (!strcmp(serialBuf, "UP"))       serialUp   = true;
      else if (!strcmp(serialBuf, "DOWN")) serialDown = true;
      else if (!strcmp(serialBuf, "OK"))   serialOk   = true;
      else if (!strcmp(serialBuf, "BACK")) serialBack = true;

      else if (!strcmp(serialBuf, "STATUS")) {
        Serial.print(F("LEVEL="));
        Serial.print(currentLevel);
        Serial.print(F(";INDEX="));
        Serial.print(currentIndex);
        Serial.print(F(";COUNT="));
        Serial.println(currentCount);
      }
      else {
        Serial.println(F("ERR"));
      }
      return;
    }

    if (serialPos < sizeof(serialBuf) - 1) {
      if (c >= 'a' && c <= 'z') c -= 32; // upper
      serialBuf[serialPos++] = c;
    } else {
      serialPos = 0; // overflow → reset
    }
  }
}

/* =====================================================
   DESENHO DO MENU
   ===================================================== */
static void drawMenu() {
  //u8g.setFont(u8g_font_6x10);
  u8g.setFont(u8g_font_04b_03);//fonte de 5 pixels

  for (uint8_t i = 0; i < currentCount; i++) {
    int y = 14 + i * 12;

    if (i == currentIndex) {
      u8g.drawBox(0, y - 9, 128, 11);
      u8g.setColorIndex(0);
      u8g.drawStr(2, y, currentMenu[i].label);
      u8g.setColorIndex(1);
    } else {
      u8g.drawStr(2, y, currentMenu[i].label);
    }
  }
}

/* =====================================================
   API PUBLICA
   ===================================================== */
void menuInit() {
  pinMode(BTN_UP,   INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_OK,   INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  //Serial.begin(9600);
  Serial.println(F("MENU READY (UNO SAFE)"));

  currentMenu  = menuMain;
  currentCount = 2;
  currentIndex = 0;
  currentLevel = 0;
}

void menuLoop() {

  //handleSerial();

  bool up   = pressed(BTN_UP)   || serialUp;
  bool down = pressed(BTN_DOWN) || serialDown;
  bool ok   = pressed(BTN_OK)   || serialOk;
  bool back = pressed(BTN_BACK) || serialBack;

  serialUp = serialDown = serialOk = serialBack = false;

  if (up && currentIndex > 0)
    currentIndex--;

  if (down && currentIndex < currentCount - 1)
    currentIndex++;

  if (ok) {
    MenuItem& item = currentMenu[currentIndex];

    if (item.children && currentLevel < MAX_LEVELS - 1) {
      menuStack[currentLevel]  = currentMenu;
      indexStack[currentLevel] = currentIndex;
      countStack[currentLevel] = currentCount;

      currentLevel++;
      currentMenu  = item.children;
      currentCount = item.childCount;
      currentIndex = 0;
    }
    else if (item.action) {
      item.action();
    }
  }

  if (back && currentLevel > 0) {
    currentLevel--;
    currentMenu  = menuStack[currentLevel];
    currentIndex = indexStack[currentLevel];
    currentCount = countStack[currentLevel];
  }

  u8g.firstPage();
  do {
    drawMenu();
  } while (u8g.nextPage());
}

//********************************************************************************************************************************** */
//********************************************************************************************************************************** */