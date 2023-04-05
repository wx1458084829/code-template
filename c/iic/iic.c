#include "iic.h"
static uint8_t fac_us = 72;    //������ʱ��Ϊ72M, ������1us��ticks���72��

/*
  * @brief	��ʱ����
  * @note   ��ʱ΢�뺯��
  * @param  ΢��
  * @retval void
  */
void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;	//װ��ֵ
	ticks=nus*fac_us; //��Ҫ�Ľ�����
	told=SysTick->VAL; //�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)tcnt+=told-tnow;	//�������ݼ�
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;	//ʱ�䳬��������ӳٵ�ʱ��ʱ�˳�.
		}
	};
}

void IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  //����GPIOʱ��
  IIC_SDA_GPIO_CLK_ENABLE();
  IIC_SCL_GPIO_CLK_ENABLE();
  // ����SDA����
  GPIO_InitStruct.Pin = SDA_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  // ��ʼ��SDA
  HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);

  // ����SCL����
  GPIO_InitStruct.Pin = SCL_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  // ��ʼ��SCL
  HAL_GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStruct);

  //����ֹͣ�ź�
  IIC_Stop();
}

/**
 * ����ֹͣ�ź�
 */
void IIC_Start(void)
{
  // ׼��������ʼ�ź�
  IIC_SDA_1();
  IIC_SCL_1();
  delay_us(1);
  //������ʼ�ź�
  IIC_SDA_0();
  delay_us(1);
}

/**
 * ֹͣ�ź�
 */
void IIC_Stop(void)
{
  //׼������ֹͣ�ź�
  IIC_SDA_0();
  IIC_SCL_1();
  delay_us(1);
  // ����ֹͣ�ź�
  IIC_SDA_1();
}

/**
 * ��������
 */
void IIC_SendByte(uint8_t _ucByte)
{
	uint8_t i;
	IIC_SCL_0();
 /* д7bit���� */
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
		_ucByte <<= 1; /* ����һ��bit */
		delay_us(1);
	}
	IIC_SDA_1();
	delay_us(1);
}

/**
 * ��ȡ����
 */
uint8_t IIC_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ��ȡ���� */
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
 * �ȴ����豸��Ӧ
 */
uint8_t IIC_WaitAck(void)
{
	uint8_t re;
	IIC_SDA_1();  //�ͷ�SDA
	delay_us(1);
	IIC_SCL_1(); //����SCL����
	delay_us(1);
	if (IIC_SDA_READ())  //��ȡSDA�ߣ��ж�slave��û�з���ACK
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	IIC_SCL_0(); //����SCL���ߣ���ȡȷ���ź����
	delay_us(1);
	return re;
}

/**
 * ����ACK
 */
void IIC_Ack(void)
{
	IIC_SDA_0();	//����SDA�ߣ���ACK�ź�
	delay_us(1);
	IIC_SCL_1();	//����SCL
	delay_us(1);
	IIC_SCL_0();	//����SCL������ACK���
	delay_us(1);
	IIC_SDA_1();	//�ͷ�SDA
}
/**
 * ����NCK
 */
void IIC_NAck(void)
{
	IIC_SDA_1();	//����SDA�ߣ���NACK�ź�
	delay_us(1);
	IIC_SCL_1();	//����SCL��
	delay_us(1);
	IIC_SCL_0();	//����SCL�ߣ�����NACK���
	delay_us(1);
}
