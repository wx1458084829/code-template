#include "lcd1602.h"

#define LCD1602_E_1 0x04
#define LCD1602_RW_1 0x02
#define LCD1602_RS_1 0x01
#define LCD1602_P3_1 0x08

#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup

#define LCD1602_P3_0 0xf7
#define LCD1602_E_0 0xfb
#define LCD1602_RW_0 0xfd
#define LCD1602_RS_0 0xfe



void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

//发�?8字节数据
void ClearScreen(uint8_t data)
{
		//清屏
	lcd_send_cmd(0x01);
}


void showChar(uint8_t row,uint8_t col,char ch)
{
		lcd_send_cmd(0x80+(0x40*row)+col);
		lcd_send_data(ch);
}



void showLine(uint8_t row,uint8_t col,char *string)
{
		lcd_send_cmd(0x80+(0x40*row)+col);
		while(*string){
		lcd_send_data(*string);
			string++;
		}
}



void initLCD1602(void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void printf_double(uint8_t row,uint8_t col,uint32_t number)
{
	uint16_t out_array[3] = {'0','0','0'}; 
	uint8_t i =0;
	if(number == 0)
	{
		showChar(row,col,'0');
		showChar(row,col+1,'0');
		showChar(row,col+2,'0');
		return;
	}
	while(number!=0)
	{
			out_array[2-i] = '0' + (number%10);
			number = number/10;
			i++;
	}
	for(i=0;i<3;i++){
			showChar(row,row+i,out_array[i]);
	}
}


