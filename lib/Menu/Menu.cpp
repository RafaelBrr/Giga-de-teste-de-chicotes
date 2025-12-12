#include "Menu.h"

void drawMenu() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(0, 12, "== Menu Jiga de Teste ==");

  for (int i = 0; i < menuSize; i++) {
    if (i == menuIndex) {
      u8g2.drawBox(0, 16 + i * 12, 128, 12);
      u8g2.setDrawColor(0);
      u8g2.drawStr(2, 26 + i * 12, menuItems[i]);
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(2, 26 + i * 12, menuItems[i]);
    }
  }

  u8g2.sendBuffer();
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