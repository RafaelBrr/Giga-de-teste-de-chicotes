#include <Arduino.h>
#include "TestCables.h"

bool TestCable_2558() {
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