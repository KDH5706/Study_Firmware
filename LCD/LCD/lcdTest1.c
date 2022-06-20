/*
 * lcdTest1.c
 *
 * Created: 2022-06-20 오후 2:54:08
 * Author : PKNU
 */ 
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include "Lcd.h"

void Init()
{
	DDRA = 0xff;
	DDRG = 0xff;
	Lcd_Init();
	Lcd_Clear();
	_delay_ms(100);
}

int main(void)
{
	Byte str1[] = " LCD TEST";
	Byte str2[] = " ATmaga 128";
	
	Init();
	
	Lcd_Pos(0, 0);
	Lcd_STR(str1);
	Lcd_Pos(1, 0);
	Lcd_STR(str2);
	
    while (1) 
    {
    }
	
	return 0;
}

