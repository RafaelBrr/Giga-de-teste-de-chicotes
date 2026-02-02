#include <Arduino.h>
#include <U8glib.h>
#include "Firmware.h"

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET


/* =====================================================
   BOTOES
   ===================================================== */
#define BTN_UP     48
#define BTN_DOWN   50
#define BTN_OK     52
#define BTN_BACK   47

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
   ACOES (stubs)
   ===================================================== */
static void actionBrightness() {}
static void actionContrast() {}
static void actionInfo() { showFirmwareInfo();} /* Example action */ 
static void actionReset() {}
static void actionLedTest() {digitalWrite(A2, HIGH); }
static void actionBuzzerTest() {}

/* =====================================================
   DEFINICAO DOS MENUS (4 NIVEIS)
   ===================================================== */

// ---- NIVEL 4 ----
static MenuItem menuDisplay[] = {
  { "Brilho",    NULL, 0, actionBrightness },
  { "Contraste", NULL, 0, actionContrast }
};

static MenuItem menuSystem[] = {
  { "Info",  NULL, 0, actionInfo },
  { "Reset", NULL, 0, actionReset }
};

// ---- NIVEL 3 ----
static MenuItem menuConfig[] = {
  { "Display", menuDisplay, 2, NULL },
  { "Sistema", menuSystem,  2, NULL }
};

static MenuItem menuTests[] = {
  { "LED",    NULL, 0, actionLedTest },
  { "Buzzer", NULL, 0, actionBuzzerTest }
};

// ---- NIVEL 2 (RAIZ) ----
static MenuItem menuMain[] = {
  { "Config", menuConfig, 2, NULL },
  { "Testes", menuTests,  2, NULL },
  { "Sobre",  NULL,       0, actionInfo },
  { "Test",  NULL,       0, actionLedTest },
};

/* =====================================================
   FUNCOES INTERNAS
   ===================================================== */
static bool pressed(uint8_t pin) {
  if (digitalRead(pin) == LOW) {
    delay(150);   // debounce simples
    return true;
  }
  return false;
}

static void drawMenu() {
  u8g.setFont(u8g_font_6x10);

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

  currentMenu  = menuMain;
  currentCount = 4;   // Config, Testes, Sobre
  currentIndex = 0;
  currentLevel = 0;
}

void menuLoop() {

  /* -------- NAVEGACAO -------- */
  if (pressed(BTN_UP) && currentIndex > 0)
    currentIndex--;

  if (pressed(BTN_DOWN) && currentIndex < currentCount - 1)
    currentIndex++;

  /* -------- ENTER -------- */
  if (pressed(BTN_OK)) {
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

  /* -------- BACK -------- */
  if (pressed(BTN_BACK) && currentLevel > 0) {

    currentLevel--;

    currentMenu  = menuStack[currentLevel];
    currentIndex = indexStack[currentLevel];
    currentCount = countStack[currentLevel];
  }

  /* -------- DESENHO -------- */
  u8g.firstPage();
  do {
    drawMenu();
  } while (u8g.nextPage());
}