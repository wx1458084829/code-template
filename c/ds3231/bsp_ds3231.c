#include "ds3231.h"

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}

static void sda_Read_Mode(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = DS3231_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


static void sda_Write_Mode(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = DS3231_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	DS3231_I2C_SDA_1();
	DS3231_I2C_SCL_1();
	i2c_Delay();
	DS3231_I2C_SDA_0();
	i2c_Delay();
	DS3231_I2C_SCL_0();
	i2c_Delay();
}


/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	DS3231_I2C_SDA_0();
	DS3231_I2C_SCL_1();
	i2c_Delay();
	DS3231_I2C_SDA_1();
}


/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			DS3231_I2C_SDA_1();
		}
		else
		{
			DS3231_I2C_SDA_0();
		}
		i2c_Delay();
		DS3231_I2C_SCL_1();
		i2c_Delay();	
		DS3231_I2C_SCL_0();
		if (i == 7)
		{
			 DS3231_I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}


/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	DS3231_I2C_SDA_1();	/* CPU释放SDA总线 */
	sda_Read_Mode();
	i2c_Delay();
	DS3231_I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (DS3231_I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}

	
	DS3231_I2C_SCL_0();
	i2c_Delay();
  sda_Write_Mode();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	DS3231_I2C_SDA_0();	/* CPU驱动SDA = 0 */
	i2c_Delay();
	DS3231_I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	DS3231_I2C_SCL_0();
	i2c_Delay();
	DS3231_I2C_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	DS3231_I2C_SDA_1();	/* CPU驱动SDA = 1 */
	i2c_Delay();
	DS3231_I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	DS3231_I2C_SCL_0();
	i2c_Delay();	
}


/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;
	sda_Read_Mode();
	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		DS3231_I2C_SCL_1();
		i2c_Delay();
		if (DS3231_I2C_SDA_READ())
		{
			value++;
		}
		DS3231_I2C_SCL_0();
		i2c_Delay();
	}
	sda_Write_Mode();
	return value;
}


//DS3231的从机地址为 D0h，
//R/W = 0 时 ( 为D0h)，表示 主机 将向 DS3231 写入数据.。
//R/W = 1 时 ( 为D1h) ，表示 主机 将从 DS3231 读取数据。
uint8_t ds3231_Read_Data(uint8_t address)
{
	uint8_t res,temp;
	//开始信号
	i2c_Start();
	//发送设备地址以及写标志
	i2c_SendByte(0xD0);
	//等待设备相应
	i2c_WaitAck();
	//发送寄存器地址
	i2c_SendByte(address);
	i2c_WaitAck();

	i2c_Start();
	i2c_SendByte(0xD1);
	
	res =  i2c_ReadByte();
	i2c_NAck();
	i2c_Stop();
	return res;
}

void ds3231_Write_Data(uint8_t address,uint8_t data)
{
	uint8_t res,temp;
	//开始信号
	i2c_Start();
	//发送设备地址
	i2c_SendByte(0xD0);
	//等待设备相应
	i2c_WaitAck();
	//发送地址
	i2c_SendByte(address);
	//等待设备相应
	i2c_WaitAck();
	i2c_SendByte(data);
	i2c_WaitAck();
	i2c_Stop();
}

//void set_Seconds(uint8_t seconds)
//{
//		uint8_t sec = seconds;
//		if(seconds>10){
//				 (seconds/5);
//		}
//		ds3231_Write_Data(SECONDS_ADDRESS,sec);
//}

static uint8_t decToBcd(const uint8_t dec)
{
  const uint8_t tens = dec / 10;
  const uint8_t ones = dec % 10;
  return (tens << 4) | ones;
}

static uint8_t bcdToDec(const uint8_t bcd)
{
  return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
}



DS3231_TypeDef ds3231_Get_Date(void)
{
	DS3231_TypeDef res;

	//开始信号
	i2c_Start();
	//发送设备地址以及写标志
	i2c_SendByte(0xD0);
	//等待设备相应
	i2c_WaitAck();
	//发送寄存器地址
	i2c_SendByte(0x00);
	//等待设备相应
	i2c_WaitAck();
	//发送开始信号
	i2c_Start();
	//发送设备地址和读信号
	i2c_SendByte(0xD1);
	i2c_WaitAck();
	res.second =  bcdToDec(i2c_ReadByte());
	i2c_Ack();
	res.minute =  bcdToDec(i2c_ReadByte());
	i2c_Ack();
	res.hour =  bcdToDec(i2c_ReadByte());
	i2c_Ack();
	res.day =  bcdToDec(i2c_ReadByte());
	i2c_Ack();
	res.date =  bcdToDec(i2c_ReadByte());
	i2c_Ack();
	res.month =  bcdToDec(i2c_ReadByte());
	i2c_Ack();
	res.year =  bcdToDec(i2c_ReadByte());
	i2c_NAck();
	i2c_Stop();
	return res;
}


void ds3231_Set_Date(DS3231_TypeDef DS3231_Struct)
{

	//开始信号
	i2c_Start();
	//发送设备地址以及写标志
	i2c_SendByte(0xD0);
	//等待设备相应
	i2c_WaitAck();
	//发送寄存器地址
	i2c_SendByte(0x00);
	//等待设备相应
	i2c_WaitAck();
	
	i2c_SendByte(decToBcd(DS3231_Struct.second));
	i2c_WaitAck();
	
	i2c_SendByte(decToBcd(DS3231_Struct.minute));
	i2c_WaitAck();
	
	i2c_SendByte(decToBcd(DS3231_Struct.hour));
	i2c_WaitAck();
	
	i2c_SendByte(decToBcd(DS3231_Struct.day));
	i2c_WaitAck();

	i2c_SendByte(decToBcd(DS3231_Struct.date));
	i2c_WaitAck();
	
	i2c_SendByte(decToBcd(DS3231_Struct.month));
	i2c_WaitAck();
	
	i2c_SendByte(decToBcd(DS3231_Struct.year));
	i2c_WaitAck();
	
	i2c_Stop();
}




