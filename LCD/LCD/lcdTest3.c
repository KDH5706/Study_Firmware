/*
* lcdTest3.c
*
* Created: 2022-06-20 오후 2:54:08
* Author : PKNU
*/
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include "Lcd.h"

bool flag = true;

void Init()
{
	DDRA = 0xff;
	DDRG = 0xff;
	
	DDRD = 0x00;
	EIMSK = (1 << INT0);
	EICRA = (1 << ISC01) | (1 << ISC00);
	sei();
	
	Lcd_Init();
	Lcd_Clear();
	_delay_ms(1000);
}


ISR(INT0_vect)
{
	flag = !flag;
}

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
				
				if(flag)
				{
					Byte str[16] = "";
					sprintf((char*)str, " %d X %d = %d ", i, j, i*j);
					Lcd_STR((unsigned char*)str);
					_delay_ms(500);
					Lcd_Clear();
				}
				else
				{
					Byte str[] = " INTERRUPT!! ";
					Lcd_STR(str);
					
					while (1)
					{
						if(flag)
						{
							break;
						}
					}
				}
			}
		}
	}
	
	return 0;
}
