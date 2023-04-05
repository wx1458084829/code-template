#include "iic.h"
static uint8_t fac_us = 72;    //这里主时钟为72M, 所以在1us内ticks会减72次

/*
  * @brief	延时函数
  * @note   延时微秒函数
  * @param  微秒
  * @retval void
  */
void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;	//装载值
	ticks=nus*fac_us; //需要的节拍数
	told=SysTick->VAL; //刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)tcnt+=told-tnow;	//计数器递减
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;	//时间超过或等于延迟的时间时退出.
		}
	};
}

void IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  //开启GPIO时钟
  IIC_SDA_GPIO_CLK_ENABLE();
  IIC_SCL_GPIO_CLK_ENABLE();
  // 设置SDA参数
  GPIO_InitStruct.Pin = SDA_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  // 初始化SDA
  HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);

  // 设置SCL参数
  GPIO_InitStruct.Pin = SCL_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  // 初始化SCL
  HAL_GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStruct);

  //发送停止信号
  IIC_Stop();
}

/**
 * 发送停止信号
 */
void IIC_Start(void)
{
  // 准备发送起始信号
  IIC_SDA_1();
  IIC_SCL_1();
  delay_us(1);
  //发送起始信号
  IIC_SDA_0();
  delay_us(1);
}

/**
 * 停止信号
 */
void IIC_Stop(void)
{
  //准备发送停止信号
  IIC_SDA_0();
  IIC_SCL_1();
  delay_us(1);
  // 发送停止信号
  IIC_SDA_1();
}

/**
 * 发送数据
 */
void IIC_SendByte(uint8_t _ucByte)
{
	uint8_t i;
	IIC_SCL_0();
 /* 写7bit数据 */
	for (i = 0; i < 8; i++) {
		if (_ucByte & 0x80) {
			IIC_SDA_1();
		}else{
			IIC_SDA_0();
		}
		  delay_us(1);
		IIC_SCL_1();
		  delay_us(1);
		IIC_SCL_0();
		_ucByte <<= 1; /* 左移一个bit */
		delay_us(1);
	}
	IIC_SDA_1();
	delay_us(1);
}

/**
 * 读取数据
 */
uint8_t IIC_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读取数据 */
	value = 0;
	for (i = 0; i < 8; i++) {
		value <<= 1;
		IIC_SCL_1();
		delay_us(1);
		if (IIC_SDA_READ()) {
			value++;
		}
		IIC_SCL_0();
		delay_us(1);
	}
	return value;
}

/**
 * 等待从设备响应
 */
uint8_t IIC_WaitAck(void)
{
	uint8_t re;
	IIC_SDA_1();  //释放SDA
	delay_us(1);
	IIC_SCL_1(); //拉升SCL总线
	delay_us(1);
	if (IIC_SDA_READ())  //读取SDA线，判断slave有没有发送ACK
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	IIC_SCL_0(); //拉低SCL总线，读取确认信号完成
	delay_us(1);
	return re;
}

/**
 * 发送ACK
 */
void IIC_Ack(void)
{
	IIC_SDA_0();	//拉低SDA线，是ACK信号
	delay_us(1);
	IIC_SCL_1();	//拉升SCL
	delay_us(1);
	IIC_SCL_0();	//拉低SCL，发送ACK完成
	delay_us(1);
	IIC_SDA_1();	//释放SDA
}
/**
 * 发送NCK
 */
void IIC_NAck(void)
{
	IIC_SDA_1();	//拉高SDA线，是NACK信号
	delay_us(1);
	IIC_SCL_1();	//拉升SCL线
	delay_us(1);
	IIC_SCL_0();	//拉低SCL线，发送NACK完成
	delay_us(1);
}
