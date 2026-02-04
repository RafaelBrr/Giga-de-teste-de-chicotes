#ifndef __TESTECAB_2558__H__
#define __TESTECAB_2558__H__

#include <U8glib.h>

extern U8GLIB_ST7920_128X64_1X u8g;

void TesteCabo_2558();
void configurarPinosCabo10Vias();
bool realizarTesteContinuidade();
bool testeCurtoEntreVias();
bool testeCurtoDetalhado();
bool testeSequenciaCabo();
bool testeSequenciaDetalhado();

#endif  //!__TESTECAB_2558__H__