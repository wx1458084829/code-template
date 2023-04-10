#ifndef __BSP_TM1638_H
#define __BSP_TM1638_H
#include "stm32f10x.h"

#define BSP_TM1638_DIO GPIO_Pin_1
#define BSP_TM1638_CLK GPIO_Pin_3
#define BSP_TM1638_STB GPIO_Pin_5

#define BSP_TM1638_PORT GPIOD
#define BSP_TM1638_RCC_CLK RCC_APB2Periph_GPIOD

void Bsp_Key_Init(void);
void Write_DATA(uint8_t add, uint8_t data);
void Write_COM(uint8_t cmd);
uint8_t Read_Key(void);

#endif /*__BSP_TM1638_H*/
