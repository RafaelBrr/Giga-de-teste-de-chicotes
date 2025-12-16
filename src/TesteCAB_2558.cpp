#include <Arduino.h>
#include "U8g2lib.h"

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /*clk=*/52, /*data=*/51, /*cs=*/53, /*reset=*/U8X8_PIN_NONE);

// Pinos de saída (lado A do cabo)
const int txPins[10] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

// Pinos de entrada (lado B do cabo)
const int rxPins[10] = {32, 33, 34, 35, 36, 37, 38, 39, 40, 41};

const int NUM_VIAS = 6;
const int vias[NUM_VIAS] = {22, 23, 24, 25, 26, 27}; // pinos do Arduino

//const int NUM_VIAS = 6;

// Lado A do cabo
const int pinosA[NUM_VIAS] = {22, 23, 24, 25, 26, 27};

// Lado B do cabo
const int pinosB[NUM_VIAS] = {30, 31, 32, 33, 34, 35};



void configurarPinosCabo10Vias() {
  for (int i = 0; i < 10; i++) {
    pinMode(txPins[i], OUTPUT);
    digitalWrite(txPins[i], HIGH); // Mantém HIGH por padrão
    Serial.println("Configurado pino de saída: " + String(txPins[i]));
    pinMode(rxPins[i], INPUT_PULLUP); // INPUT_PULLUP simula um resistor interno
    Serial.println("Configurado pino de entrada: " + String(rxPins[i]));
  }
}


bool realizarTesteContinuidade() {
  Serial.println("⏳ Iniciando teste de continuidade do cabo de 10 vias...");
  
  bool sucesso = true;

u8g2.clearBuffer();
u8g2.setFont(u8g2_font_6x12_tr);
u8g2.drawStr(10, 30, "Testando continuidade... do cabo de 10 vias");
u8g2.sendBuffer();

  for (int i = 0; i < 10; i++) {
    // Zera todos os pinos de saída antes do teste
    for (int j = 0; j < 10; j++) {
      digitalWrite(txPins[j], HIGH);  // Desativa todos os outros
    }

    // Ativa o pino de teste atual (coloca em LOW)
    digitalWrite(txPins[i], LOW);
    delay(5); // Pequeno tempo para estabilizar

    int leitura = digitalRead(rxPins[i]);

    if (leitura == LOW) {
      Serial.print("✅ Via ");
      Serial.print(i);
      Serial.println(": OK (continuidade detectada)");
    } else {
      Serial.print("❌ Via ");
      Serial.print(i);
      Serial.println(": FALHA (sem continuidade)");
      sucesso = false;
    }

    digitalWrite(txPins[i], HIGH); // Desativa o pino atual
  }

  if (sucesso) {
    Serial.println("🟢 Teste de continuidade: SUCESSO (todas as vias conectadas)");
  } else {
    Serial.println("🔴 Teste de continuidade: FALHA EM UMA OU MAIS VIAS");
  }

  return sucesso;
}

//************************************************************************************************************************** */
//Teste de curto entre N vias (genérico)
bool testeCurtoEntreVias() {

  for (int i = 0; i < NUM_VIAS; i++) {

    // 1️⃣ Configura todas como INPUT_PULLUP
    for (int j = 0; j < NUM_VIAS; j++) {
      pinMode(vias[j], INPUT_PULLUP);
    }

    // 2️⃣ Coloca uma via como saída LOW
    pinMode(vias[i], OUTPUT);
    digitalWrite(vias[i], LOW);

    delay(5); // tempo para estabilizar

    // 3️⃣ Verifica se alguma outra via foi puxada para LOW
    for (int j = 0; j < NUM_VIAS; j++) {
      if (j == i) continue;

      if (digitalRead(vias[j]) == LOW) {
        return true; // ❌ Curto detectado
      }
    }
  }

  return false; // ✅ Nenhum curto
}

//************************************************************************************************************************** */
//Versão mais avançada (ideal para exibir no display OLED ou enviar ao LabVIEW).
void testeCurtoDetalhado() {

  for (int i = 0; i < NUM_VIAS; i++) {

    for (int j = 0; j < NUM_VIAS; j++) {
      pinMode(vias[j], INPUT_PULLUP);
    }

    pinMode(vias[i], OUTPUT);
    digitalWrite(vias[i], LOW);

    delay(5);

    for (int j = 0; j < NUM_VIAS; j++) {
      if (j == i) continue;

      if (digitalRead(vias[j]) == LOW) {
        Serial.print("Curto entre via ");
        Serial.print(i);
        Serial.print(" e via ");
        Serial.println(j);
      }
    }
  }
}

//************************************************************************************************************************** */

bool testeSequenciaCabo() {

  for (int i = 0; i < NUM_VIAS; i++) {

    // 1️⃣ Coloca todos os pinos A como INPUT_PULLUP
    for (int j = 0; j < NUM_VIAS; j++) {
      pinMode(pinosA[j], INPUT_PULLUP);
      pinMode(pinosB[j], INPUT_PULLUP);
    }

    // 2️⃣ Ativa apenas a via atual do lado A
    pinMode(pinosA[i], OUTPUT);
    digitalWrite(pinosA[i], LOW);

    delay(5); // estabilização

    int viaDetectada = -1;

    // 3️⃣ Varre lado B
    for (int j = 0; j < NUM_VIAS; j++) {
      if (digitalRead(pinosB[j]) == LOW) {
        viaDetectada = j;
        break;
      }
    }

    // 4️⃣ Validação
    if (viaDetectada == -1) {
      // fio aberto
      return false;
    }

    if (viaDetectada != i) {
      // fio trocado (cross)
      return false;
    }
  }

  return true; // sequência correta
}

//*************************************************************************************************************************** */



//******************************************************************************************************************************* */