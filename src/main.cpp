#include <Arduino.h>

#include <U8g2lib.h>

#include "Functions.h"
#include <Menu.h>

// --- Display LCD ST7920 (modo SPI via software) ---
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /*clk=*/52, /*data=*/51, /*cs=*/53, /*reset=*/U8X8_PIN_NONE);

// --- Pinos dos botões ---
const int PIN_BTN_MENU  = 46;
const int PIN_BTN_ENTER = 47;
const int PIN_BTN_UP    = 48;
const int PIN_BTN_DOWN  = 49;

// --- LEDs indicadores ---
const int LED_VERDE   = 44;
const int LED_VERMELHO = 45;

// --- Conectores de teste de cabos ---
const int txPins[8] = {22, 23, 24, 25, 26, 27, 28, 29}; // Saídas
const int rxPins[8] = {30, 31, 32, 33, 34, 35, 36, 37}; // Entradas

// --- Variáveis de controle de menu ---
int menuIndex = 0;
const char* menuItems[] = {"Iniciar Teste", "Ver Resultados", "Reset", "Sobre"};
const int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);

void realizarTeste();
void mostrarResultados();
void resetarSistema();
void mostrarSobre();
void drawMenu();


// --- Estado dos botões (debounce simples) ---
bool btnPressed(int pin) {
  return digitalRead(pin) == LOW;
}

void setup() {
  // Inicializa comunicação
  Serial.begin(9600);

  // Inicializa display
  u8g2.begin();

  // Configura botões como entrada com pull-up
  pinMode(PIN_BTN_MENU, INPUT_PULLUP);
  pinMode(PIN_BTN_ENTER, INPUT_PULLUP);
  pinMode(PIN_BTN_UP, INPUT_PULLUP);
  pinMode(PIN_BTN_DOWN, INPUT_PULLUP);

  // Configura LEDs
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);

  // Configura pinos de teste
  for (int i = 0; i < 8; i++) {
    pinMode(txPins[i], OUTPUT);
    digitalWrite(txPins[i], LOW);
    pinMode(rxPins[i], INPUT_PULLUP);
  }
}


void loop() {
  drawMenu();

  if (btnPressed(PIN_BTN_DOWN)) {
    menuIndex = (menuIndex + 1) % menuSize;
    delay(250);
  }

  if (btnPressed(PIN_BTN_UP)) {
    menuIndex = (menuIndex - 1 + menuSize) % menuSize;
    delay(250);
  }

  if (btnPressed(PIN_BTN_ENTER)) {
    delay(250);
    switch (menuIndex) {
      case 0:
        realizarTeste();
        break;
      case 1:
        mostrarResultados();
        break;
      case 2:
        resetarSistema();
        break;
      case 3:
        mostrarSobre();
        break;
    }
  }
}

void realizarTeste() {
  bool sucesso = true;

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(0, 12, "Executando Teste...");
  u8g2.sendBuffer();

  delay(500);

  for (int i = 0; i < 8; i++) {
    digitalWrite(txPins[i], HIGH);
    delay(10);

    for (int j = 0; j < 8; j++) {
      int val = digitalRead(rxPins[j]);

      if (val == HIGH) {
        if (i == j) {
          Serial.print("Linha ");
          Serial.print(i);
          Serial.println(": OK");
        } else {
          Serial.print("Erro! Curto entre ");
          Serial.print(i);
          Serial.print(" e ");
          Serial.println(j);
          sucesso = false;
        }
      }
    }

    digitalWrite(txPins[i], LOW);
  }

  if (sucesso) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    u8g2.drawStr(0, 30, "RESULTADO: OK");
  } else {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
    u8g2.drawStr(0, 30, "ERROS DETECTADOS");
  }

  u8g2.sendBuffer();
  delay(2000);
}

void mostrarResultados() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(0, 12, "Resultados via Serial");
  u8g2.drawStr(0, 24, "Consulte LabVIEW.");
  u8g2.sendBuffer();
  delay(2000);
}

void resetarSistema() {
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(0, 24, "Sistema Resetado");
  u8g2.sendBuffer();
  delay(2000);
}

void mostrarSobre() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(0, 12, "Jiga de Teste IMBEL");
  u8g2.drawStr(0, 24, "Versao: 1.0");
  u8g2.drawStr(0, 36, "Arduino Mega Pro Mini");
  u8g2.sendBuffer();
  delay(2500);
}

//*********************************************************************************************************************************************************************** */