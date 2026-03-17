/* =====================================================
   BRANCH BuzzerFeedback
   ===================================================== */

#include <Arduino.h>
#include "U8glib.h"
#include "TestCables.h"
#include "LED.h"
#include "Buzzer.h"
#include "images.h"

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

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

String cableModel = "";

int tipo = -1; // Tipo de cabo a ser testado
bool res = false; // Resultado do teste de continuidade

void configurarPinosCabo10Vias() {
  for (int i = 0; i < 10; i++) {
    pinMode(txPins[i], OUTPUT);
    pinMode(46, OUTPUT);
    digitalWrite(46, LOW); // Ativa o buzzer para indicar início do teste
    digitalWrite(txPins[i], HIGH); // Mantém HIGH por padrão
    Serial.println("Configurado pino de saída: " + String(txPins[i]));
    pinMode(rxPins[i], INPUT_PULLUP); // INPUT_PULLUP simula um resistor interno
    Serial.println("Configurado pino de entrada: " + String(rxPins[i]));
  }
}


bool realizarTesteContinuidade() {
  Serial.println("⏳ Iniciando teste de continuidade do cabo de 10 vias...");
  
bool continuidade = true;


u8g.setFont(u8g_font_04b_03);//fonte de 5 pixels
u8g.drawStr(10, 30, "Testando continuidade... do cabo de 10 vias");


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
      continuidade = true;
      
    } else {
      Serial.print("❌ Via ");
      Serial.print(i);
      Serial.println(": FALHA (sem continuidade)");
      
      continuidade = false;
    }

    digitalWrite(txPins[i], HIGH); // Desativa o pino atual
  }

  if (continuidade) {
    Serial.println("🟢 Teste de continuidade: SUCESSO (todas as vias conectadas)");
      
  } else {
    Serial.println("🔴 Teste de continuidade: FALHA EM UMA OU MAIS VIAS");
      
  }

  return continuidade;
}

//************************************************************************************************************************** */
//Teste de curto entre N vias (genérico)
bool testeCurtoEntreVias() {

bool testeCurto = true;

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

  return testeCurto; // ✅ Nenhum curto
}

//************************************************************************************************************************** */
//Versão mais avançada (ideal para exibir no display OLED ou enviar ao LabVIEW).
bool testeCurtoDetalhado() {

bool testeCurto = true;

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

  return testeCurto;
}

//************************************************************************************************************************** */

bool testeSequenciaCabo() {

    bool sequenciaCorreta = true;

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

  return sequenciaCorreta; // sequência correta
}

//*************************************************************************************************************************** */

bool testeSequenciaDetalhado() {

    bool testeSequeciaDetalhado = true;

  for (int i = 0; i < NUM_VIAS; i++) {

    for (int j = 0; j < NUM_VIAS; j++) {
      pinMode(pinosA[j], INPUT_PULLUP);
      pinMode(pinosB[j], INPUT_PULLUP);
    }

    pinMode(pinosA[i], OUTPUT);
    digitalWrite(pinosA[i], LOW);

    delay(5);

    int detectado = -1;

    for (int j = 0; j < NUM_VIAS; j++) {
      if (digitalRead(pinosB[j]) == LOW) {
        detectado = j;
        break;
      }
    }

    if (detectado == -1) {
      Serial.print("Via ");
      Serial.print(i);
      Serial.println(": fio aberto");
    } else if (detectado != i) {
      Serial.print("Via ");
      Serial.print(i);
      Serial.print(" ligada incorretamente à via ");
      Serial.println(detectado);
    } else {
      Serial.print("Via ");
      Serial.print(i);
      Serial.println(": OK");
    }
  }

  return testeSequeciaDetalhado;
}



void TestCable_2560() {
  // Implementação do teste para o cabo CAB-2560
}

void TestCable_2562() {
  // Implementação do teste para o cabo CAB-2562
}

void TestCable_2564() {
  // Implementação do teste para o cabo CAB-2564
}

void TestCable_2566() {
  // Implementação do teste para o cabo CAB-2566
}


void Test() {
  // Implementação da função de teste

    if(Serial.available()){
    cableModel = Serial.readStringUntil('\n');
    Serial.print("Modelo do cabo recebido: ");
    Serial.println(cableModel);

    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_04b_03);//fonte de 5 pixels
      u8g.drawStr(10, 20, cableModel.c_str());
    } while (u8g.nextPage());

    if(cableModel.equals("CAB-2558")){
      Serial.println("Iniciando teste do Cabo Tipo 1...");
      tipo = 0;
    }
    else if(cableModel.equals("Cabo Tipo 2")){
      Serial.println("Iniciando teste do Cabo Tipo 2...");
      tipo = 1;
    }
    else{
      Serial.println("Modelo de cabo desconhecido.");
    }
  
  switch (tipo)
  {
  case 0:
    //TesteCabo_2558();
    configurarPinosCabo10Vias();
    res = realizarTesteContinuidade();
    break;
  case 2:
    //TesteCabo_Tipo2();
    break;
  
  default:
    break;
  }

  }
}

void TestMessageCable(String message) {
  Serial.println(message);
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_04b_03);//fonte de 5 pixels
    u8g.drawStr(10, 20, message.c_str());
  } while (u8g.nextPage());
}

void DrawApprovedStatus() {
  
  u8g.firstPage();
  do {
    u8g.drawBitmapP(10, 15, 4, 32, bitmap_signalaproved32x32);
  } while (u8g.nextPage());
}

void DrawDisapprovedStatus() {
  
  u8g.firstPage();
  do {
    u8g.drawBitmapP(10, 15, 4, 32, bitmap_signalDisaproved32x32);
  } while (u8g.nextPage());
}

//******************************************************************************************************************************* */