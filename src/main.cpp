#include <Arduino.h>
#include <U8glib.h>
#include <string.h>
#include "TesteCAB_2558.h"

//#include "ProgressBar.h"

// --- Display LCD ST7920 (modo SPI via software) ---
U8GLIB_ST7920_128X64_1X u8g(6, 4, 2 ,8); //Enable, RW, RS, RESET
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

// Inicialização do display ST7567S em I2C com driver correto
// U8G2_ST7567_ENH_DG128064I_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);// Use standard I2C pins for Arduino Uno(A4-SDA, A5-SCL)

String cableModel = "";

int tipo = -1; // Tipo de cabo a ser testado
bool res = false; // Resultado do teste de continuidade


void setup() {
  // Inicializa comunicação
  Serial.begin(9600);
  u8g.begin();

  //Configure pin as output
  pinMode(11, OUTPUT); // 
  pinMode(13, OUTPUT); // 
  pinMode(15, OUTPUT); // 
  pinMode(17, OUTPUT); // 
  pinMode(19, OUTPUT); // 
  pinMode(21, OUTPUT); // 
  pinMode(23, OUTPUT); // 
  pinMode(25, OUTPUT); // 
  pinMode(27, OUTPUT); // 

  //Configure pin buttons
  pinMode(47, INPUT); // 
  pinMode(48, INPUT); // 
  pinMode(50, INPUT); // 
  pinMode(52, INPUT); // 


  //Configure pin as input
  pinMode(A4, INPUT); // 
  pinMode(A6, INPUT); // 
  pinMode(A8, INPUT); // 
  pinMode(A10, INPUT); // 
  pinMode(A12, INPUT); // 
  pinMode(A14, INPUT); // 
  pinMode(32, INPUT); // 
  pinMode(34, INPUT); // 
  pinMode(36, INPUT); // 

  //ProgressBar.init();

  u8g2.firstPage();
  do {
    u8g.setFont(u8g2_font_6x12_tr);
    u8g.drawStr(10, 20, "Inicializando Giga de teste...");
  } while (u8g.nextPage());

  Serial.println("Sistem pronto!");
  

  }

void loop() {

  if(Serial.available()){
    cableModel = Serial.readStringUntil('\n');
    Serial.print("Modelo do cabo recebido: ");
    Serial.println(cableModel);

    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x12_tr);
      u8g2.drawStr(10, 20, cableModel.c_str());
    } while (u8g2.nextPage());

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

//*********************************************************************************************************************************************************************** */