#include <Arduino.h>
#include <U8glib.h>
#include <string.h>
#include "TesteCAB_2558.h"

#include "ProgressBar.h"
#include "menu.h"
#include "Firmware.h"
#include "Serial.h"
#include "Buttons.h"

// --- Display LCD ST7920 (modo SPI via software) ---
U8GLIB_ST7920_128X64_1X u8g(6, 4, 2 ,8); //Enable, RW, RS, RESET - Config for IMBEL projects
//U8GLIB_ST7920_128X64_1X u8g(7, 4, 2 ,8); //Enable, RW, RS, RESET - Configuração alternativa para casa
//U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 ,7); //Enable, RW, RS, RESET
//u8g_SSD1306_128X64_NONAME_F_HW_I2C u8g(u8g_R0);

// Inicialização do display ST7567S em I2C com driver correto
// u8g_ST7567_ENH_DG128064I_F_SW_I2C u8g(u8g_R0, SCL, SDA, U8X8_PIN_NONE);// Use standard I2C pins for Arduino Uno(A4-SDA, A5-SCL)

String cableModel = "";

int tipo = -1; // Tipo de cabo a ser testado
bool res = false; // Resultado do teste de continuidade
int percent = 0; // Percentual da barra de progresso

bool progressBar = false;
bool screenFirmware = false;
bool screenSerial = false;
String lastCommand = "";

void setup() {
  // Inicializa comunicação
  Serial.begin(9600);
  u8g.begin();

  //Configure pin as output
  pinMode(11, OUTPUT); // j
  pinMode(13, OUTPUT); // 
  pinMode(15, OUTPUT); // 
  pinMode(17, OUTPUT); // 
  pinMode(19, OUTPUT); // 
  pinMode(21, OUTPUT); // 
  pinMode(23, OUTPUT); // 
  pinMode(25, OUTPUT); // 
  pinMode(27, OUTPUT); // 

  //Configure pin buttons
  pinMode(47, INPUT_PULLUP); // 
  pinMode(48, INPUT_PULLUP); // 
  pinMode(50, INPUT_PULLUP); // 
  pinMode(52, INPUT_PULLUP); // 

  //Configure pin as input
  pinMode(A4, INPUT); // 
  pinMode(A6, INPUT); // 
  // pinMode(A8, INPUT); // 
  // pinMode(A10, INPUT); // 
  // pinMode(A12, INPUT); // 
  // pinMode(A14, INPUT); // 
  pinMode(32, INPUT); // 
  pinMode(34, INPUT); // 
  pinMode(36, INPUT); // 

  pinMode(A0, OUTPUT); // LED gree indicador de status
  pinMode(A2, OUTPUT); // LED red indicador de status

  //ProgressBar.init();

  u8g.firstPage();
  do {
    u8g.setRot180();
    u8g.setFont(u8g_font_04b_03);//fonte de 5 pixels
    u8g.drawStr(10, 20, "Inicializando Giga de teste...");
  } while (u8g.nextPage());

  Serial.println("Sistema pronto!");
  menuInit();
  delay(200);
  //drawProgressBar(percent);

  }

void loop() {

 
if(progressBar == false){
  drawProgressBar(percent);
  progressBar = true;
  percent = 0;
}

//digitalWrite(A0, HIGH); // Turn the LED on (HIGH is the voltage level)


   
  if(screenFirmware == false){
    showFirmwareInfo();
    screenFirmware = true;
   // initializeSerial();
      
  delay(2000);
 
}

  // isButtonPressed(47); // Check if button on pin 47 is pressed
  // isButtonPressed(48); // Check if button on pin 48 is pressed
  // isButtonPressed(50); // Check if button on pin 50 is pressed
  // isButtonPressed(52); // Check if button on pin 52 is pressed

  menuLoop();
    
    // String comand = readFromSerial();

    //   if(comand.length() > 0 && comand != lastCommand) {

    //     lastCommand = comand;
    //     handleCommand(comand);
    //   }

    
  //   String stringReceived = readFromSerial();
  //   if(stringReceived.equals("CAB-2558")){
  //      do {
  //   u8g.drawStr(10, 20, "Comando recebido: CAB-2558");

  // } while (u8g.nextPage());
      

  //   }
      
  
 
}

//showMenu();




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


//*********************************************************************************************************************************************************************** */