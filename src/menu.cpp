#include <Arduino.h>
#include <U8glib.h>
#include "Firmware.h"

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

#define COUNT_OF(x) (sizeof(x) / sizeof(x[0]))


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
static void actionInfo() {}
static void actionReset() {}
//static void actionLedTest() {digitalWrite(A2, HIGH);}
static void actionLedRedOn() {digitalWrite(A2, HIGH);}
static void actionLedRedOff() {digitalWrite(A2, LOW);}
static void actionLedGreenOn() {digitalWrite(A0, HIGH);}
static void actionLedGreenOff() {digitalWrite(A0, LOW);}
static void actionBlinkTest() {for(int i = 0; i <= 20; i++){digitalWrite(A2, HIGH);digitalWrite(A0, LOW);delay(500);digitalWrite(A2, LOW);delay(500);digitalWrite(A0, HIGH);delay(500);}}

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
  { "LED VERMELHO ON",    NULL, 0, actionLedRedOn },
  { "LED VERMELHO OFF",    NULL, 0, actionLedRedOff },
  { "LED VERDE ON",    NULL, 0, actionLedGreenOn },
  { "LED VERDE OFF",    NULL, 0, actionLedGreenOff },
  { "Blink", NULL, 0, actionBlinkTest }
};

// ---- NIVEL 2 (RAIZ) ----
static MenuItem menuMain[] = {
  { "Config", menuConfig, COUNT_OF(menuConfig), NULL },
  { "Testes", menuTests,  COUNT_OF(menuTests),  NULL },
  { "Sobre",  NULL,       0, actionInfo }
};


/* =====================================================
   LEITURA BOTAO (DEBOUNCE SIMPLES)
   ===================================================== */
static bool pressed(uint8_t pin) {
  if (digitalRead(pin) == LOW) {
    delay(150);
    return true;
  }
  return false;
}

/* =====================================================
   SERIAL -> SIMULACAO DE "CLIQUE" (NOVO)
   ===================================================== */
static bool serialUp = false;
static bool serialDown = false;
static bool serialOk = false;
static bool serialBack = false;

static char serialBuf[32];
static uint8_t serialPos = 0;

static void serialTrigger(const char* cmd) {
  // Comandos aceitos (case-insensitive):
  // UP/U, DOWN/D, OK/O, BACK/B, STATUS
  if (!cmd || !cmd[0]) return;

  // normaliza para maiúsculo sem usar String (mais leve)
  char c0 = cmd[0];
  char c1 = cmd[1];

  // UP
  if ((c0=='U' || c0=='u') && (c1=='P' || c1=='p' || c1=='\0')) { serialUp = true; return; }
  // DOWN
  if ((c0=='D' || c0=='d') && ( (c1=='O'||c1=='o') || (c1=='\0') )) { // aceita "D" e "DO..." (tratamos melhor abaixo)
    // Se for "DOWN"
    if (cmd[1] == '\0') { serialDown = true; return; }
    // Checagem simples para "DOWN"
    if ((cmd[0]=='D'||cmd[0]=='d') && (cmd[1]=='O'||cmd[1]=='o')) { serialDown = true; return; }
  }
  // OK/O
  if ((c0=='O' || c0=='o') && ( (c1=='K'||c1=='k') || c1=='\0' )) { serialOk = true; return; }
  // BACK/B
  if ((c0=='B' || c0=='b') && ( (c1=='A'||c1=='a') || c1=='\0' )) { serialBack = true; return; }

  // STATUS (opcional)
  if ((cmd[0]=='S'||cmd[0]=='s')) {
    Serial.print("LEVEL=");
    Serial.print(currentLevel);
    Serial.print(" INDEX=");
    Serial.print(currentIndex);
    Serial.print(" COUNT=");
    Serial.println(currentCount);
  }
}

static void handleSerial() {
  while (Serial.available() > 0) {
    char ch = (char)Serial.read();

    // ignora CR
    if (ch == '\r') continue;

    // fim de linha -> processa comando
    if (ch == '\n') {
      serialBuf[serialPos] = '\0';
      serialTrigger(serialBuf);
      serialPos = 0;
      continue;
    }

    // armazena até encher
    if (serialPos < sizeof(serialBuf) - 1) {
      // converte para maiúsculo para facilitar (opcional)
      if (ch >= 'a' && ch <= 'z') ch = ch - 32;
      serialBuf[serialPos++] = ch;
    } else {
      // overflow: zera buffer para evitar lixo
      serialPos = 0;
    }
  }
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
  Serial.println("Menu ready. Commands: UP/DOWN/OK/BACK (or U/D/O/B).");

  currentMenu  = menuMain;
  currentCount = 3;   // Config, Testes, Sobre
  currentIndex = 0;
  currentLevel = 0;
}

void menuLoop() {

  // NOVO: lê comandos seriais (não bloqueante)
  handleSerial();

  // Combina botões físicos + "botões virtuais" pela Serial
  bool up   = pressed(BTN_UP)   || serialUp;
  bool down = pressed(BTN_DOWN) || serialDown;
  bool ok   = pressed(BTN_OK)   || serialOk;
  bool back = pressed(BTN_BACK) || serialBack;

  // Consome os eventos seriais (1 comando = 1 clique)
  serialUp = serialDown = serialOk = serialBack = false;

  /* -------- NAVEGACAO -------- */
  if (up && currentIndex > 0)
    currentIndex--;

  if (down && currentIndex < currentCount - 1)
    currentIndex++;

  /* -------- ENTER -------- */
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

  /* -------- BACK -------- */
  if (back && currentLevel > 0) {

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
//********************************************************************************************************************************** */
//********************************************************************************************************************************** */