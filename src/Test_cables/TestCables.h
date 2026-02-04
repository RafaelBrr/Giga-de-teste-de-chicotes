#ifndef __TESTECAB_2558__H__
#define __TESTECAB_2558__H__

#include <U8g2lib.h>

extern U8G2_ST7920_128X64_F_SW_SPI u8g2;

// void TestCable_2558();
// void TestCable_2560();
// void TestCable_2562();
// void TestCable_2564();
// void TestCable_2566();
void configurarPinosCabo10Vias();
bool realizarTesteContinuidade();
bool testeCurtoEntreVias();
bool testeCurtoDetalhado();
bool testeSequenciaCabo();
bool testeSequenciaDetalhado();

#endif  //!__TESTECAB_2558__H__