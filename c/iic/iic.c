#include "iic.h"


// IIC���ߵ���ʱ����������ʵ���������ʱ��
void IIC_Delay()
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 10; i++);
}


// IIC���ߵĳ�ʼ������
void IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

	//����ʱ��
  IIC_SDA_GPIO_CLK_ENABLE();
	IIC_SCL_GPIO_CLK_ENABLE();

  // ����SDA��SCL����Ϊ��©���
  GPIO_InitStruct.Pin = SDA_GPIO_PIN | SCL_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	//ע�⣺�������ͬ��port����Ҫ��ʼ������
  HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);

  // ��ʼ��ʱ��SDA��SCL�ø�
  IIC_SDA_1();
  IIC_SCL_1();
	
}

// IIC���ߵ���ʼ�źź���
void IIC_Start(void)
{
  // �Ƚ�SDA��SCL�ø�
  IIC_SDA_1();
  IIC_SCL_1();
  IIC_Delay();

  // ����SDA����ʼ��ʼ�ź�
  IIC_SDA_0();
  IIC_Delay();

  // ����SCL����ʼ��������
  IIC_SCL_0();
  IIC_Delay();
}

// IIC���ߵ�ֹͣ�źź���
void IIC_Stop(void)
{
  // �Ƚ�SDA��SCL�õ�
  IIC_SDA_0();
  IIC_SCL_1();
  IIC_Delay();
  // ����SCL���������
  IIC_SDA_1();
}

//��������
void IIC_SendByte(uint8_t _ucByte)
{
	uint8_t i;

 /* �ȷ����ֽڵĸ�λ bit7 */
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
			IIC_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1; /* ����һ�� bit */
		IIC_Delay();
	}
}
//��������
uint8_t IIC_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������ 1 �� bit Ϊ���ݵ� bit7 */
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

//�ȴ����豸��Ӧ
uint8_t IIC_WaitAck(void)
{
	uint8_t re;

	IIC_SDA_1();	/* CPU�ͷ�SDA���� */
	IIC_Delay();
	IIC_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	IIC_Delay();
	if (IIC_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IIC_Ack(void)
{
	IIC_SDA_0();	/* CPU����SDA = 0 */
	IIC_Delay();
	IIC_SCL_1();	/* CPU����1��ʱ�� */
	IIC_Delay();
	IIC_SCL_0();
	IIC_Delay();
	IIC_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IIC_NAck(void)
{
	IIC_SDA_1();	/* CPU����SDA = 1 */
	IIC_Delay();
	IIC_SCL_1();	/* CPU����1��ʱ�� */
	IIC_Delay();
	IIC_SCL_0();
	IIC_Delay();	
}


