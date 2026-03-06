/* =====================================================
   BRANCH TestMenuLabviewScrollDisplayBuzzer
   ===================================================== */

#include <Arduino.h>
#include <U8glib.h>
#include "Firmware.h"
#include "menu.h"
#include "Test_cables/TestCables.h"
#include "Buzzer.h"
#include "LED.h"

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

#define COUNT_OF(x) (sizeof(x) / sizeof(x[0]))
#define VISIBLE_ITEMS 5

//static uint8_t scrollOffset = 0;

#define FEEDBACK_PIN   46   // pode ser buzzer
#define FEEDBACK_TIME  80          // ms (ajuste como quiser)

static bool feedbackActive = false;
static unsigned long feedbackStart = 0;

/* =====================================================
   BRANCH TestMenuLabviewScrollDisplay
   ===================================================== */
/* =====================================================
   DEFINICOES
   ===================================================== */
#define COUNT_OF(x) (sizeof(x) / sizeof(x[0]))
#define MAX_LEVELS 4
#define VISIBLE_ITEMS 5

/* =====================================================
   BOTOES
   ===================================================== */
#define BTN_UP     48
#define BTN_DOWN   50
#define BTN_OK     52
#define BTN_BACK   47
//#define pinBuzzer  LED_BUILTIN



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
static uint8_t   scrollOffset = 0;

/* =====================================================
   FLAGS DE EVENTO (BOTOES + SERIAL)
   ===================================================== */
static bool evUp   = false;
static bool evDown = false;
static bool evOk   = false;
static bool evBack = false;

/* =====================================================
   ACOES
   ===================================================== */
static void actionLedGreenOn()  { Serial.println(F("LED GREEN ON"));  digitalWrite(A0, HIGH); }
static void actionLedGreenOff() { Serial.println(F("LED GREEN OFF")); digitalWrite(A0, LOW);  }
static void actionLedRedOn()    { Serial.println(F("LED RED ON"));    digitalWrite(A2, HIGH); }
static void actionLedRedOff()   { Serial.println(F("LED RED OFF"));   digitalWrite(A2, LOW);  }

static void actionBlinkTest() {
  Serial.println(F("BLINK"));
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(A0, HIGH);
    digitalWrite(A2, LOW);
    delay(300);
    digitalWrite(A0, LOW);
    digitalWrite(A2, HIGH);
    delay(300);
    digitalWrite(A2, LOW);
  }
}

static void actionBuzzerTest() { testBuzzer(); }
static void actionInfo()       { showFirmwareInfo(); }
static void actionInfoVersion(){ showFirmwareVersion(); }
static void actionTestCAB_2558(){ TestCAB_2558(); }
static void actionTestCAB_2560(){ TestCAB_2560(); }
static void actionTestCAB_2562(){ TestCAB_2562(); }
static void actionTestCAB_2564(){ TestCAB_2564(); }
static void actionTestCAB_2566(){ TestCAB_2566(); }
static void actionTestCAB_2568(){ TestCAB_2568(); }
static void actionTestCAB_2570(){ TestCAB_2570(); }
static void actionTestCAB_2572(){ TestCAB_2572(); }
static void actionAutoTest(){ autoTest(); }


/* =====================================================
   MENUS
   ===================================================== */
static MenuItem menuLEDs[] = {
  { "LED GREEN ON",  NULL, 0, actionLedGreenOn  },
  { "LED GREEN OFF", NULL, 0, actionLedGreenOff },
  { "LED RED ON",    NULL, 0, actionLedRedOn    },
  { "LED RED OFF",   NULL, 0, actionLedRedOff   },
  { "BLINK",         NULL, 0, actionBlinkTest  },
  //{ "BUZZER TEST",   NULL, 0, actionBuzzerTest },
  //{ "AUTO TEST",     NULL, 0, actionAutoTest   },
};

static MenuItem menuSettings[] = {
  { "LEDs",  menuLEDs, COUNT_OF(menuLEDs), NULL },
  { "Buzzer", NULL, 0, actionBuzzerTest },
  { "AUTO TEST",     NULL, 0, actionAutoTest   },
};

static MenuItem menuCables[] = {
  { "CAB-2558",  NULL, 0, actionTestCAB_2558 },
  { "CAB-2560",  NULL, 0, actionTestCAB_2560 },
  { "CAB-2562",  NULL, 0, actionTestCAB_2562 },
  { "CAB-2564",  NULL, 0, actionTestCAB_2564 },
  { "CAB-2566",  NULL, 0, actionTestCAB_2566 },
  { "CAB-2568",  NULL, 0, actionTestCAB_2568 },
  { "CAB-2570",  NULL, 0, actionTestCAB_2570 },
  { "CAB-2572",  NULL, 0, actionTestCAB_2572 },
 

};

static MenuItem menuMain[] = {
  { "CABLES",      menuCables, COUNT_OF(menuCables), NULL },
  { "SETTINGS",    menuSettings, COUNT_OF(menuSettings), NULL },  
  { "ABOUT",   NULL,     0, actionInfo        },
  { "VERSION", NULL,     0, actionInfoVersion },
};

/* =====================================================
   BOTAO – DETECCAO DE BORDA
   ===================================================== */
static bool buttonPressed(uint8_t pin) {
  static uint8_t lastState[70];
  uint8_t state = digitalRead(pin);
  //digitalWrite(pinBuzzer, state); // DEBUG: mostra estado do botao no LED

  if (lastState[pin] == HIGH && state == LOW) {
    lastState[pin] = state;
    return true;
  }
  lastState[pin] = state;
  return false;
}

/* =====================================================
   SERIAL – PARSER ROBUSTO
   ===================================================== */
static void handleSerial() {
  static char buf[40];
  static uint8_t pos = 0;

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\r') continue;

    if (c == '\n') {
      buf[pos] = '\0';
      pos = 0;

      if (!strcmp(buf, "UP"))        evUp   = true;
      else if (!strcmp(buf, "DOWN")) evDown = true;
      else if (!strcmp(buf, "OK"))   evOk   = true;
      else if (!strcmp(buf, "BACK")) evBack = true;

      else if (!strcmp(buf, "STATUS")) {
        Serial.print(F("LEVEL=")); Serial.print(currentLevel);
        Serial.print(F(";INDEX=")); Serial.print(currentIndex);
        Serial.print(F(";COUNT=")); Serial.println(currentCount);
      }
      return;
    }

    if (pos < sizeof(buf) - 1) {
      if (c >= 'a' && c <= 'z') c -= 32;
      buf[pos++] = c;
    }
  }
}

/* =====================================================
   DESENHO DO MENU COM SCROLL
   ===================================================== */
static void drawMenu() {
  u8g.setFont(u8g_font_6x10);

  uint8_t end = scrollOffset + VISIBLE_ITEMS;
  if (end > currentCount) end = currentCount;

  uint8_t line = 0;
  for (uint8_t i = scrollOffset; i < end; i++) {
    int y = 14 + line * 12;

    if (i == currentIndex) {
      u8g.drawBox(0, y - 9, 128, 11);
      u8g.setColorIndex(0);
      u8g.drawStr(2, y, currentMenu[i].label);
      u8g.setColorIndex(1);
    } else {
      u8g.drawStr(2, y, currentMenu[i].label);
    }
    line++;
  }
}

/* =====================================================
   INIT
   ===================================================== */
void menuInit() {
  pinMode(BTN_UP,   INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_OK,   INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  //Serial.begin(9600);
  Serial.println(F("MENU READY"));

  currentMenu  = menuMain;
  currentCount = COUNT_OF(menuMain);
  currentIndex = 0;
  currentLevel = 0;
  scrollOffset = 0;
}

static void triggerFeedback() {
  digitalWrite(FEEDBACK_PIN, HIGH);
  feedbackActive = true;
  feedbackStart = millis();
}

static void updateFeedback() {
  if (feedbackActive && (millis() - feedbackStart >= FEEDBACK_TIME)) {
    digitalWrite(FEEDBACK_PIN, LOW);
    feedbackActive = false;
  }
}
/* =====================================================
   LOOP
   ===================================================== */
void menuLoop() {

  handleSerial();

  bool up   = buttonPressed(BTN_UP)   || evUp;
  bool down = buttonPressed(BTN_DOWN) || evDown;
  bool ok   = buttonPressed(BTN_OK)   || evOk;
  bool back = buttonPressed(BTN_BACK) || evBack;


  if (up || down || ok || back) {
  triggerFeedback();
}
  evUp = evDown = evOk = evBack = false;

  if (up && currentIndex > 0) currentIndex--;
  if (down && currentIndex < currentCount - 1) currentIndex++;

  /* ----- SCROLL ----- */
  if (currentIndex < scrollOffset)
    scrollOffset = currentIndex;
  else if (currentIndex >= scrollOffset + VISIBLE_ITEMS)
    scrollOffset = currentIndex - VISIBLE_ITEMS + 1;

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
      scrollOffset = 0;
    } else if (item.action) {
      item.action();
    }
  }

  if (back && currentLevel > 0) {
    currentLevel--;
    currentMenu  = menuStack[currentLevel];
    currentIndex = indexStack[currentLevel];
    currentCount = countStack[currentLevel];
    scrollOffset = 0;
  }

  u8g.firstPage();
  do {
    drawMenu();
  } while (u8g.nextPage());

  updateFeedback();

}


//********************************************************************************************************************************** */
//********************************************************************************************************************************** */