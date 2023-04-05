#ifndef __DS3231_H
#define __DS3231_H

#include "main.h"
#include <stdio.h>

#define  DS3231_I2C_SDA_1() HAL_GPIO_WritePin(GPIOB, DS3231_SDA_Pin, GPIO_PIN_SET)
#define  DS3231_I2C_SCL_1() HAL_GPIO_WritePin(GPIOB, DS3231_SCL_Pin, GPIO_PIN_SET)

#define  DS3231_I2C_SDA_0() HAL_GPIO_WritePin(GPIOB, DS3231_SDA_Pin, GPIO_PIN_RESET)
#define  DS3231_I2C_SCL_0() HAL_GPIO_WritePin(GPIOB, DS3231_SCL_Pin, GPIO_PIN_RESET)

#define DS3231_I2C_SDA_READ() HAL_GPIO_ReadPin(GPIOB, DS3231_SDA_Pin)


#define SECONDS_ADDRESS 0X00
#define MINUTES_ADDRESS 0X01
#define HOURS_ADDRESS 0X02
#define DAY_ADDRESS 0X03
#define DATE_ADDRESS 0X04
#define MONTH_ADDRESS 0X05
#define YEAR_ADDRESS 0X06


typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}DS3231_TypeDef;



uint8_t get_Seconds(void);
uint8_t ds3231_Read_Data(uint8_t address);
uint8_t get_Minutes(void);
DS3231_TypeDef ds3231_Get_Date(void);
void ds3231_Set_Date(DS3231_TypeDef DS3231_Struct);

#endif  /** __DS3231_H */
