#ifndef __IIC_H
#define __IIC_H
#include "main.h"

// IIC 端口定义
#define SDA_GPIO_PORT GPIOB
#define SDA_GPIO_PIN GPIO_PIN_7
#define SCL_GPIO_PORT GPIOB
#define SCL_GPIO_PIN GPIO_PIN_6

// 开启GPIO时钟
#define IIC_SDA_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define IIC_SCL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()


// 端口电平读写宏定义
#define IIC_SDA_1() HAL_GPIO_WritePin(SDA_GPIO_PORT, SDA_GPIO_PIN, GPIO_PIN_SET)
#define IIC_SDA_0() HAL_GPIO_WritePin(SDA_GPIO_PORT, SDA_GPIO_PIN, GPIO_PIN_RESET)
#define IIC_SCL_1() HAL_GPIO_WritePin(SCL_GPIO_PORT, SCL_GPIO_PIN, GPIO_PIN_SET)
#define IIC_SCL_0() HAL_GPIO_WritePin(SCL_GPIO_PORT, SCL_GPIO_PIN, GPIO_PIN_RESET)

// 读数据宏定义
#define IIC_SDA_READ() HAL_GPIO_ReadPin(SDA_GPIO_PORT, SDA_GPIO_PIN)
#define IIC_SCL_READ() HAL_GPIO_ReadPin(SCL_GPIO_PORT, SCL_GPIO_PIN)

//IIC函数
void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(uint8_t _ucByte);
uint8_t IIC_ReadByte(void);
uint8_t IIC_WaitAck(void);
void IIC_Ack(void);
void IIC_NAck(void);

#endif  /** __IIC_H */

