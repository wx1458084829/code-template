#ifndef __LCD1602_H
#define __LCD1602_H
#include "main.h"


void initLCD1602(void);
void showLine(uint8_t row,uint8_t col,char *string);
void showChar(uint8_t row,uint8_t col,char ch);

void lcdClear(void);

void printf_double(uint8_t row,uint8_t col,uint32_t number);
//todo

#endif

