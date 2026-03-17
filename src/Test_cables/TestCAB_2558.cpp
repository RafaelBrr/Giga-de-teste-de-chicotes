/* =====================================================
   BRANCH BuzzerFeedback
   ===================================================== */

#include <Arduino.h>
#include "Test_cables/TestCables.h"
#include "LED.h"
#include "Buzzer.h"

#define CAB_2558 "CAB-2558"
#define CAB_2560 "CAB-2560"
#define CAB_2562 "CAB-2562"
#define CAB_2564 "CAB-2564"
#define CAB_2566 "CAB-2566"
#define CAB_2568 "CAB-2568"
#define CAB_2570 "CAB-2570"
#define CAB_2572 "CAB-2572"

bool TestCAB_2558() {

  String cableModel = CAB_2558;
  Serial.println("Iniciando teste do cabo " + cableModel + "...");
  TestMessageCable("Iniciando teste do cabo " + cableModel + "...");
  configurarPinosCabo10Vias();

  // bool continuidade = realizarTesteContinuidade();
  // bool curto = testeCurtoEntreVias();

  bool continuidade = true;
  bool curto = true;
  //bool testVias = testeVias();

  if (continuidade && !curto) {
    Serial.println("🟢 " + cableModel + ": Teste concluído com SUCESSO!");
    turnOnGreenLED();
    turnOffRedLED();
    TestMessageCable("Cabo: " + cableModel + " Aprovado!");
    delay(1000); // Aguarda um momento antes de tocar o buzzer
    DrawApprovedStatus();
    delay(1000); // Aguarda um momento para exibir o status aprovado
    approvedBuzzer();
    
  } else {
    Serial.println("🔴 " + cableModel + ": Teste concluído com FALHAS.");
    turnOffGreenLED();
    turnOnRedLED();
    TestMessageCable("Cabo: " + cableModel + " Reprovado!");
    DrawDisapprovedStatus();
    disapprovedBuzzer();
    
  }

  return true;
}

bool TestCAB_2560() {
  String cableModel = "CAB-2560";
  Serial.println("Iniciando teste do cabo " + cableModel + "...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 " + cableModel + ": Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 " + cableModel + ": Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2562() {
  String cableModel = "CAB-2562";
  String message = "Iniciando teste do cabo " + cableModel + "...";
  Serial.println("Iniciando teste do cabo " + cableModel + "...");
  TestMessageCable(message);

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 " + cableModel + ": Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 " + cableModel + ": Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2564() {
  String cableModel = "CAB-2564";
  Serial.println("Iniciando teste do cabo " + cableModel + "...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 " + cableModel + ": Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 " + cableModel + ": Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2566() {
  String cableModel = "CAB-2566";
  Serial.println("Iniciando teste do cabo " + cableModel + "...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 " + cableModel + ": Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 " + cableModel + ": Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2568() {
  String cableModel = "CAB-2568";
  Serial.println("Iniciando teste do cabo " + cableModel + "...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 " + cableModel + ": Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 " + cableModel + ": Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2570() {
  String cableModel = "CAB-2570";
  Serial.println("Iniciando teste do cabo " + cableModel + "...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 " + cableModel + ": Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 " + cableModel + ": Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2572() {
  String cableModel = "CAB-2572";
  Serial.println("Iniciando teste do cabo " + cableModel + "...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 " + cableModel + ": Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 " + cableModel + ": Teste concluído com FALHAS.");
  }

  return true;
}

//*************************************************************************************************************** */