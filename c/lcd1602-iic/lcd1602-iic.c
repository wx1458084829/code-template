#include "iic.h"
//发送数据
void sendData(uint8_t data)
{
	uint8_t x = 0;
	IIC_Init();
	IIC_Start();
	IIC_SendByte(0x4e); //写地址
  IIC_WaitAck();
	IIC_SendByte(data); //数据
	IIC_WaitAck();
	IIC_Stop();
}

//设置高为|=

#define LCD1602_E_1 0x04
#define LCD1602_RW_1 0x02
#define LCD1602_RS_1 0x01
#define LCD1602_P3_1 0x08



//设置低为&=
#define LCD1602_P3_0 0xf7
#define LCD1602_E_0 0xfb
#define LCD1602_RW_0 0xfd
#define LCD1602_RS_0 0xfe


// 通过IIC发送两个8bit数据
void IIC_SendTwoData(uint8_t data1,uint8_t data2)
{
	IIC_Start(); //发送起始信号
	IIC_SendByte(0x4e); //写设备地址
  IIC_WaitAck(); //等待响应
	IIC_SendByte(data1); //发送数据1
	IIC_WaitAck();//等待响应
	IIC_SendByte(data2); //发送数据2
	IIC_WaitAck();//等待响应
	IIC_Stop();//发送结束信号
}


//发送4字节命令，高位为数据，低四位为控制
void WriteHalfCommand(uint8_t command)
{
	uint8_t tempCommand = command; 
	tempCommand &= LCD1602_RS_0; //模式为指令模式
	tempCommand &= LCD1602_RW_0; //模式为写模式
	tempCommand |= LCD1602_E_1; //设置时钟为高
	tempCommand |= LCD1602_P3_1; //设置背景灯亮
	IIC_SendTwoData(tempCommand,tempCommand&LCD1602_E_0);
	HAL_Delay(1);
}

//发送8字节命令
void WriteFullCommand(uint8_t command)
{
	WriteHalfCommand((command&0xf0));
	WriteHalfCommand((command&0x0f)<<4);
}

//发送4字节数据
void WriteHalfData(uint8_t data)
{
	uint8_t tempData = data; 
	tempData |= LCD1602_RS_1; //模式为数据模式
	tempData &= LCD1602_RW_0; //模式为写模式
	tempData |= LCD1602_E_1; //设置时钟为高
	tempData |= LCD1602_P3_1; //设置背景灯亮
	IIC_SendTwoData(tempData,tempData&LCD1602_E_0);
}

//发送8字节数据
void WriteFullData(uint8_t data)
{
	WriteHalfData((data&0xf0));
	WriteHalfData((data&0x0f)<<4);
}

//发送8字节数据
void ClearScreen(uint8_t data)
{
		//清屏
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
	//设置4bit模式
	WriteHalfCommand(0x20);
	WriteFullCommand(0x28);
	WriteFullCommand(0x08);
	WriteFullCommand(0x01);
	WriteFullCommand(0x06);
	//开启显示、不显示光标、不闪烁光标
  WriteFullCommand(0x0c);
	
	
}