/*
* lcdTest2.c
*
* Created: 2022-06-20 오후 2:54:08
* Author : PKNU
*/
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "Lcd.h"

void Init()
{
	DDRA = 0xff;
	DDRG = 0xff;
	Lcd_Init();
	Lcd_Clear();
	_delay_ms(1000);
}

void Make_string(unsigned char* str, int i, int j);

int main(void)
{
	Init();
	
	while (1)
	{
		for (int i = 2; i < 10 ; i++)
		{
			for (int j = 1; j < 10 ; j++)
			{
				Lcd_Pos(0, 0);
				Byte str[16] = "";
				//Make_string(str, i, j);
				sprintf((char*)str, " %d X %d = %d ", i, j, i*j);
				Lcd_STR((unsigned char*)str);
				_delay_ms(500);
				Lcd_Clear();
			}
		}
	}
	
	return 0;
}

void Make_string(unsigned char* str, int i, int j)
{
	for(int num = 0; num < 16 ; num++)
	{
		str[num] = ' ';
	}
	
	str[1] = ('0' - 0 + i);
	str[3] = ('X');
	str[5] = ('0' - 0 + j);
	str[7] = '=';
	
	if(i*j >= 10)
	{
		str[9] = ('0' - 0 + (i*j)/10);
		str[10] = ('0' - 0 + (i*j)%10);
	}
	else
	{
		str[10] = ('0' - 0 + i*j);
	}
}
