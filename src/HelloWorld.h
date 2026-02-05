#ifndef __HELLOWORLD__H__
#define __HELLOWORLD__H__

void helloWorld();
void drawHeader();
void drawMessage(const char* message); 
void updateMessage();
void drawMessageDouble(const char* message, const char* message2);
void drawBoxedText(uint8_t x, uint8_t y, uint8_t width, const char* text, bool selected);



#endif  //!__HELLOWORLD__H__