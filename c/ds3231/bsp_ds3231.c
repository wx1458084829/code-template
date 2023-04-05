#include "ds3231.h"

/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

static void i2c_Delay(void)
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
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
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
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	DS3231_I2C_SDA_0();
	DS3231_I2C_SCL_1();
	i2c_Delay();
	DS3231_I2C_SDA_1();
}


/*
*********************************************************************************************************
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
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
			 DS3231_I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}


/*
*********************************************************************************************************
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	DS3231_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	sda_Read_Mode();
	i2c_Delay();
	DS3231_I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (DS3231_I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	DS3231_I2C_SDA_0();	/* CPU����SDA = 0 */
	i2c_Delay();
	DS3231_I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	DS3231_I2C_SCL_0();
	i2c_Delay();
	DS3231_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	DS3231_I2C_SDA_1();	/* CPU����SDA = 1 */
	i2c_Delay();
	DS3231_I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	DS3231_I2C_SCL_0();
	i2c_Delay();	
}


/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;
	sda_Read_Mode();
	/* ������1��bitΪ���ݵ�bit7 */
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


//DS3231�Ĵӻ���ַΪ D0h��
//R/W = 0 ʱ ( ΪD0h)����ʾ ���� ���� DS3231 д������.��
//R/W = 1 ʱ ( ΪD1h) ����ʾ ���� ���� DS3231 ��ȡ���ݡ�
uint8_t ds3231_Read_Data(uint8_t address)
{
	uint8_t res,temp;
	//��ʼ�ź�
	i2c_Start();
	//�����豸��ַ�Լ�д��־
	i2c_SendByte(0xD0);
	//�ȴ��豸��Ӧ
	i2c_WaitAck();
	//���ͼĴ�����ַ
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
	//��ʼ�ź�
	i2c_Start();
	//�����豸��ַ
	i2c_SendByte(0xD0);
	//�ȴ��豸��Ӧ
	i2c_WaitAck();
	//���͵�ַ
	i2c_SendByte(address);
	//�ȴ��豸��Ӧ
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

	//��ʼ�ź�
	i2c_Start();
	//�����豸��ַ�Լ�д��־
	i2c_SendByte(0xD0);
	//�ȴ��豸��Ӧ
	i2c_WaitAck();
	//���ͼĴ�����ַ
	i2c_SendByte(0x00);
	//�ȴ��豸��Ӧ
	i2c_WaitAck();
	//���Ϳ�ʼ�ź�
	i2c_Start();
	//�����豸��ַ�Ͷ��ź�
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

	//��ʼ�ź�
	i2c_Start();
	//�����豸��ַ�Լ�д��־
	i2c_SendByte(0xD0);
	//�ȴ��豸��Ӧ
	i2c_WaitAck();
	//���ͼĴ�����ַ
	i2c_SendByte(0x00);
	//�ȴ��豸��Ӧ
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




