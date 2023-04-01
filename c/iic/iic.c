#include "iic.h"


// IIC总线的延时函数，根据实际情况调整时间
void IIC_Delay()
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


// IIC总线的初始化函数
void IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

	//开启时钟
  IIC_SDA_GPIO_CLK_ENABLE();
	IIC_SCL_GPIO_CLK_ENABLE();

  // 配置SDA、SCL引脚为开漏输出
  GPIO_InitStruct.Pin = SDA_GPIO_PIN | SCL_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	//注意：如果不在同个port，需要初始化两次
  HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);

  // 初始化时将SDA、SCL置高
  IIC_SDA_1();
  IIC_SCL_1();
	
}

// IIC总线的起始信号函数
void IIC_Start(void)
{
  // 先将SDA、SCL置高
  IIC_SDA_1();
  IIC_SCL_1();
  IIC_Delay();

  // 拉低SDA，开始起始信号
  IIC_SDA_0();
  IIC_Delay();

  // 拉低SCL，开始传输数据
  IIC_SCL_0();
  IIC_Delay();
}

// IIC总线的停止信号函数
void IIC_Stop(void)
{
  // 先将SDA、SCL置低
  IIC_SDA_0();
  IIC_SCL_1();
  IIC_Delay();
  // 拉高SCL，传输结束
  IIC_SDA_1();
}

//发送数据
void IIC_SendByte(uint8_t _ucByte)
{
	uint8_t i;

 /* 先发送字节的高位 bit7 */
	for (i = 0; i < 8; i++) {
		if (_ucByte & 0x80) {
			IIC_SDA_1();
		}else{
			IIC_SDA_0();
		}
		IIC_Delay();
		IIC_SCL_1();
		IIC_Delay();
		IIC_SCL_0();
		if (i == 7) {
			IIC_SDA_1(); // 释放总线
		}
		_ucByte <<= 1; /* 左移一个 bit */
		IIC_Delay();
	}
}
//接收数据
uint8_t IIC_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第 1 个 bit 为数据的 bit7 */
	value = 0;
	for (i = 0; i < 8; i++) {
		value <<= 1;
		IIC_SCL_1();
		IIC_Delay();
		if (IIC_SDA_READ()) {
			value++;
		}
		IIC_SCL_0();
		IIC_Delay();
	}
	return value;
}

//等待从设备响应
uint8_t IIC_WaitAck(void)
{
	uint8_t re;

	IIC_SDA_1();	/* CPU释放SDA总线 */
	IIC_Delay();
	IIC_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	IIC_Delay();
	if (IIC_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	IIC_SCL_0();
	IIC_Delay();
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
void IIC_Ack(void)
{
	IIC_SDA_0();	/* CPU驱动SDA = 0 */
	IIC_Delay();
	IIC_SCL_1();	/* CPU产生1个时钟 */
	IIC_Delay();
	IIC_SCL_0();
	IIC_Delay();
	IIC_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void IIC_NAck(void)
{
	IIC_SDA_1();	/* CPU驱动SDA = 1 */
	IIC_Delay();
	IIC_SCL_1();	/* CPU产生1个时钟 */
	IIC_Delay();
	IIC_SCL_0();
	IIC_Delay();	
}


