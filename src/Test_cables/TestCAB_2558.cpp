#include <Arduino.h>
#include "Test_cables/TestCables.h"

bool TestCAB_2558() {
  Serial.println("Iniciando teste do cabo CAB-2558...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 CAB-2558: Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 CAB-2558: Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2560() {
  Serial.println("Iniciando teste do cabo CAB-2560...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 CAB-2560: Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 CAB-2560: Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2562() {
  Serial.println("Iniciando teste do cabo CAB-2562...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 CAB-2562: Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 CAB-2562: Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2564() {
  Serial.println("Iniciando teste do cabo CAB-2564...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 CAB-2564: Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 CAB-2564: Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2566() {
  Serial.println("Iniciando teste do cabo CAB-2566...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 CAB-2566: Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 CAB-2566: Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2568() {
  Serial.println("Iniciando teste do cabo CAB-2568...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 CAB-2568: Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 CAB-2568: Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2570() {
  Serial.println("Iniciando teste do cabo CAB-2570...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 CAB-2570: Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 CAB-2570: Teste concluído com FALHAS.");
  }

  return true;
}

bool TestCAB_2572() {
  Serial.println("Iniciando teste do cabo CAB-2572...");

  configurarPinosCabo10Vias();

  bool continuidade = realizarTesteContinuidade();
  bool curto = testeCurtoEntreVias();

  if (continuidade && !curto) {
    Serial.println("🟢 CAB-2572: Teste concluído com SUCESSO!");
  } else {
    Serial.println("🔴 CAB-2572: Teste concluído com FALHAS.");
  }

  return true;
}

//*************************************************************************************************************** */