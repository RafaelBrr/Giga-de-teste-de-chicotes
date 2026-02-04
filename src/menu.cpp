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
   FLAGS SERIAL (SIMULAM BOTOES)
   ===================================================== */
static bool serialUp   = false;
static bool serialDown = false;
static bool serialOk   = false;
static bool serialBack = false;

/* =====================================================
   ACOES (EXEMPLOS)
   ===================================================== */
static void actionLedGreenOn()  { Serial.println("LED GREEN ON"); digitalWrite(A0, HIGH); }
static void actionLedGreenOff() { Serial.println("LED GREEN OFF"); digitalWrite(A0, LOW); }
static void actionLedRedOn ()  { Serial.println("LED RED ON"); digitalWrite(A2, HIGH); }
static void actionLedRedOff () { Serial.println("LED RED OFF"); digitalWrite(A2, LOW); }
static void actionBlinkTest()  { Serial.println("BLINK");  for(int i = 0; i< 20; i++){digitalWrite(A0,HIGH);digitalWrite(A2,LOW);delay(500);digitalWrite(A0,LOW);digitalWrite(A2,HIGH);delay(500);} }
static void actionInfo()       { showFirmwareInfo();   }
static void actionInfoVersion(){ showFirmwareVersion(); }
static void actionSettings()   { Serial.println("SETTINGS"); }
static void actionBuzzerTest() { Serial.println("BUZZER TEST");  for(int i = 0; i< 3; i++){digitalWrite(A2,HIGH);delay(200);digitalWrite(A2,LOW);delay(200);} }
static void actionTestCAB2558() { Serial.println("TESTE CAB-2558");  }
static void actionTestCAB2560() { Serial.println("TESTE CAB-2560");  }
static void actionTestCAB2562() { Serial.println("TESTE CAB-2562");  }
static void actionTestCAB2564() { Serial.println("TESTE CAB-2564");  }
static void actionTestCAB2566() { Serial.println("TESTE CAB-2566   ");  }

/* =====================================================
   MENUS
   ===================================================== */


static MenuItem menuLEDs[] = {
  { "LED GREEN ON",  NULL, 0, actionLedGreenOn  },
  { "LED GREEN OFF", NULL, 0, actionLedGreenOff },
  { "LED RED ON",  NULL, 0, actionLedRedOn  },
  { "TESTE BUZZER", NULL, 0, actionBuzzerTest },
  { "BLINK",   NULL, 0, actionBlinkTest  },
 
};

static MenuItem menuCables[] = {
  { "CAB-2558",  NULL, 0, actionTestCAB2558  },
  { "CAB-2560", NULL, 0, actionTestCAB2560 },
  { "CAB-2562",  NULL, 0, actionTestCAB2562  },
  { "CAB-2564",  NULL, 0, actionTestCAB2564  },
  { "CAB-2566",   NULL, 0, actionTestCAB2566  }

};

static MenuItem menuTests[] = {
  { "LEDs",  menuLEDs, COUNT_OF(menuLEDs), NULL },
  { "CABLES", menuCables, COUNT_OF(menuCables), NULL },
};



static MenuItem menuSettings[] = {  
  { "Teste LED", menuTests, COUNT_OF(menuTests), NULL },
  { "TESTE BUZZER", NULL, 0, actionBuzzerTest },
  { "Adjust",  NULL, 0, actionLedGreenOn  },
  
};

static MenuItem menuMain[] = {
  //{ "TESTES", menuTests, 3, NULL },
  { "Testes", menuTests, COUNT_OF(menuTests), NULL },
  { "Settings", menuSettings, COUNT_OF(menuSettings), actionSettings },
  { "About",  NULL,      0, actionInfo },
  { "Version",  NULL,      0, actionInfoVersion }
};





/* =====================================================
   LEITURA DE BOTAO (DEBOUNCE SIMPLES)
   ===================================================== */
static bool pressed(uint8_t pin) {
  if (digitalRead(pin) == LOW) {
    delay(150);
    return true;
  }
  return false;
}

/* =====================================================
   FUNCOES AUXILIARES (COMANDOS DIRETOS)
   ===================================================== */
static MenuItem* findItemByName(MenuItem* menu, uint8_t count, const char* name) {
  for (uint8_t i = 0; i < count; i++) {
    if (strcasecmp(menu[i].label, name) == 0) {
      return &menu[i];
    }
  }
  return NULL;
}

static void gotoMenu(MenuItem* menu, uint8_t count) {
  currentMenu  = menu;
  currentCount = count;
  currentIndex = 0;
  currentLevel = 0;
}

/* =====================================================
   SERIAL / LABVIEW
   ===================================================== */
static void handleSerial() {
  if (!Serial.available())
    return;

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toUpperCase();

  /* ----- NAVEGACAO ----- */
  if (cmd == "UP")   { serialUp = true;   Serial.println("ACK UP");   return; }
  if (cmd == "DOWN") { serialDown = true; Serial.println("ACK DOWN"); return; }
  if (cmd == "OK")   { serialOk = true;   Serial.println("ACK OK");   return; }
  if (cmd == "BACK") { serialBack = true; Serial.println("ACK BACK"); return; }

  /* ----- STATUS ----- */
  if (cmd == "STATUS") {
    Serial.print("LEVEL=");
    Serial.print(currentLevel);
    Serial.print(";INDEX=");
    Serial.print(currentIndex);
    Serial.print(";COUNT=");
    Serial.println(currentCount);
    return;
  }

  /* ----- GOTO ----- */
  if (cmd == "GOTO MAIN") {
    gotoMenu(menuMain, COUNT_OF(menuMain));
    Serial.println("ACK GOTO MAIN");
    return;
  }

  if (cmd == "GOTO TESTES") {
    gotoMenu(menuTests, COUNT_OF(menuTests));
    Serial.println("ACK GOTO TESTES");
    return;
  }

   if (cmd == "GOTO SETTINGS") {
    gotoMenu(menuSettings, COUNT_OF(menuSettings));
    Serial.println("ACK GOTO SETTINGS");
    return;
  }

  /* ----- RUN (EXECUCAO DIRETA) ----- */
  if (cmd.startsWith("RUN ")) {
    String itemName = cmd.substring(4);
    itemName.trim();

    MenuItem* item = findItemByName(menuSettings, COUNT_OF(menuSettings), itemName.c_str());
    if (item == NULL) {
      item = findItemByName(menuMain, COUNT_OF(menuMain), itemName.c_str());
    } if(item == NULL) {
      item = findItemByName(menuTests, COUNT_OF(menuTests), itemName.c_str());
    } if(item == NULL) {
      item = findItemByName(menuLEDs, COUNT_OF(menuLEDs), itemName.c_str());
    } if(item == NULL) {
      item = findItemByName(menuCables, COUNT_OF(menuCables), itemName.c_str());
    }
    //MenuItem* item = findItemByName(menuSettings, COUNT_OF(menuSettings), itemName.c_str());
    Serial.println(itemName.c_str());

    if (item && item->action) {
      item->action();
      Serial.print("ACK RUN ");
      //Serial.println(itemName);
    } else {
      Serial.println("ERR ITEM NOT FOUND");
      //Serial.println(itemName.c_str());
    }
    return;
  }

  Serial.println("ERR UNKNOWN");
}

/* =====================================================
   DESENHO DO MENU
   ===================================================== */
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

  Serial.begin(9600);
  Serial.println("MENU READY");
  Serial.println("CMD: UP DOWN OK BACK STATUS");
  Serial.println("CMD: GOTO MAIN | GOTO TESTES");
  Serial.println("CMD: RUN LED ON | RUN LED OFF | RUN BLINK");

  currentMenu  = menuMain;
  currentCount = COUNT_OF(menuMain);
  currentIndex = 0;
  currentLevel = 0;
}

void menuLoop() {

  /* ----- SERIAL ----- */
  handleSerial();

  /* ----- EVENTOS (BOTOES + SERIAL) ----- */
  bool up   = pressed(BTN_UP)   || serialUp;
  bool down = pressed(BTN_DOWN) || serialDown;
  bool ok   = pressed(BTN_OK)   || serialOk;
  bool back = pressed(BTN_BACK) || serialBack;

  serialUp = serialDown = serialOk = serialBack = false;

  /* ----- NAVEGACAO ----- */
  if (up && currentIndex > 0)
    currentIndex--;

  if (down && currentIndex < currentCount - 1)
    currentIndex++;

  /* ----- ENTER ----- */
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

  /* ----- BACK ----- */
  if (back && currentLevel > 0) {
    currentLevel--;
    currentMenu  = menuStack[currentLevel];
    currentIndex = indexStack[currentLevel];
    currentCount = countStack[currentLevel];
  }

  /* ----- DISPLAY ----- */
  u8g.firstPage();
  do {
    drawMenu();
  } while (u8g.nextPage());
}


//********************************************************************************************************************************** */
//********************************************************************************************************************************** */