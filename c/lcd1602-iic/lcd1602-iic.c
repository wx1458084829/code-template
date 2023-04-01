#include "iic.h"
//��������
void sendData(uint8_t data)
{
	uint8_t x = 0;
	IIC_Init();
	IIC_Start();
	IIC_SendByte(0x4e); //д��ַ
  IIC_WaitAck();
	IIC_SendByte(data); //����
	IIC_WaitAck();
	IIC_Stop();
}

//���ø�Ϊ|=

#define LCD1602_E_1 0x04
#define LCD1602_RW_1 0x02
#define LCD1602_RS_1 0x01
#define LCD1602_P3_1 0x08



//���õ�Ϊ&=
#define LCD1602_P3_0 0xf7
#define LCD1602_E_0 0xfb
#define LCD1602_RW_0 0xfd
#define LCD1602_RS_0 0xfe


// ͨ��IIC��������8bit����
void IIC_SendTwoData(uint8_t data1,uint8_t data2)
{
	IIC_Start(); //������ʼ�ź�
	IIC_SendByte(0x4e); //д�豸��ַ
  IIC_WaitAck(); //�ȴ���Ӧ
	IIC_SendByte(data1); //��������1
	IIC_WaitAck();//�ȴ���Ӧ
	IIC_SendByte(data2); //��������2
	IIC_WaitAck();//�ȴ���Ӧ
	IIC_Stop();//���ͽ����ź�
}


//����4�ֽ������λΪ���ݣ�����λΪ����
void WriteHalfCommand(uint8_t command)
{
	uint8_t tempCommand = command; 
	tempCommand &= LCD1602_RS_0; //ģʽΪָ��ģʽ
	tempCommand &= LCD1602_RW_0; //ģʽΪдģʽ
	tempCommand |= LCD1602_E_1; //����ʱ��Ϊ��
	tempCommand |= LCD1602_P3_1; //���ñ�������
	IIC_SendTwoData(tempCommand,tempCommand&LCD1602_E_0);
	HAL_Delay(1);
}

//����8�ֽ�����
void WriteFullCommand(uint8_t command)
{
	WriteHalfCommand((command&0xf0));
	WriteHalfCommand((command&0x0f)<<4);
}

//����4�ֽ�����
void WriteHalfData(uint8_t data)
{
	uint8_t tempData = data; 
	tempData |= LCD1602_RS_1; //ģʽΪ����ģʽ
	tempData &= LCD1602_RW_0; //ģʽΪдģʽ
	tempData |= LCD1602_E_1; //����ʱ��Ϊ��
	tempData |= LCD1602_P3_1; //���ñ�������
	IIC_SendTwoData(tempData,tempData&LCD1602_E_0);
}

//����8�ֽ�����
void WriteFullData(uint8_t data)
{
	WriteHalfData((data&0xf0));
	WriteHalfData((data&0x0f)<<4);
}

//����8�ֽ�����
void ClearScreen(uint8_t data)
{
		//����
	WriteFullCommand(0x01);
}


void showChar(uint8_t row,uint8_t col,char ch)
{
		WriteFullCommand(0x80+(0x40*row)+col);
		WriteFullData(ch);
}



void showLine(uint8_t row,uint8_t col,char *string)
{
		WriteFullCommand(0x80+(0x40*row)+col);
		while(*string){
		WriteFullData(*string);
			string++;
		}
}
void initLCD1602(void)
{

	IIC_Init();
	//����4bitģʽ
	WriteHalfCommand(0x20);
	WriteFullCommand(0x28);
	WriteFullCommand(0x08);
	WriteFullCommand(0x01);
	WriteFullCommand(0x06);
	//������ʾ������ʾ��ꡢ����˸���
  WriteFullCommand(0x0c);
	
	
}