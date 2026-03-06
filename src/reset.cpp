#include <Arduino.h>
#include <avr/wdt.h> // Biblioteca para controle do Watchdog Timer
#include <avr/sleep.h>
#include "reset.h"


void resetArduino() {
  // Watchdog Timer Reset
  wdt_enable(WDTO_15MS); // Configura o watchdog para resetar após 15ms
  while (1) {
    // Aguarda o watchdog resetar o Arduino
  }
}

void sleepArduino() {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_cpu();
}