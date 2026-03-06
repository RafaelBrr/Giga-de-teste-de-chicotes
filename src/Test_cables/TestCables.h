#ifndef __TESTECAB_2558__H__
#define __TESTECAB_2558__H__

#include <U8glib.h>

extern U8GLIB_ST7920_128X64_1X u8g; //Enable, RW, RS, RESET

bool TestCAB_2558();
bool TestCAB_2560();
bool TestCAB_2562();
bool TestCAB_2564();
bool TestCAB_2566();
bool TestCAB_2568();
bool TestCAB_2570();
bool TestCAB_2572();
void configurarPinosCabo10Vias();
bool realizarTesteContinuidade();
bool testeCurtoEntreVias();
bool testeCurtoDetalhado();
bool testeSequenciaCabo();
bool testeSequenciaDetalhado();
void Test();

#endif  //!__TESTECAB_2558__H__