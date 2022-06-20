/*
 * Lcdeg.c
 * LCD 구동을 위한 기본함수
 * Created: 2017-07-05 오후 8:04:29
 * Author : CastleKing
 */
 #define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include "Lcd.h"

void Lcd_Data(Byte ch)			// DR -> DDRAM or CGRAM write
{
	LCD_CTRL |= (1 << LCD_RS);	// RS == 1
	LCD_CTRL &= ~(1 << LCD_RW); // RW == 0
	LCD_CTRL |= (1 << LCD_EN);	// Lcd Enable
	_delay_us(50);				// BF = 0 이 된 뒤에 다음명령을 쓸 수 있다. 따라서 delay를 준다.
	LCD_DATAWR = ch;
	_delay_us(50);
	LCD_CTRL &= ~(1 << LCD_EN);	// Lcd Disable
}

void Lcd_Cmd(Byte ch)
{
	LCD_CTRL &= ~(1 << LCD_RS); // RS == 0 
	LCD_CTRL &= ~(1 << LCD_RW);	// RW == 0
	LCD_CTRL |= (1 << LCD_EN);
	_delay_us(50);
	LCD_INSTWR = ch;
	_delay_us(50);
	LCD_CTRL &= ~(1 << LCD_EN);
}

void Lcd_CHAR(Byte c)
{
	Lcd_Data(c);
	_delay_ms(1);
}

void Lcd_STR(Byte *str)
{
	while(*str != 0){
		Lcd_CHAR(*str);
		str++;
	}	
}

void Lcd_Pos(Byte col, Byte row)
{
	Lcd_Cmd(0x80 | (row + col * 0x40));
	_delay_ms(10);
}

void Lcd_Clear(void)
{
	Lcd_Cmd(0x01);
	_delay_ms(10);
}

void Lcd_Shift(Byte p)
{
	if(p == RIGHT){
		Lcd_Cmd(0x1c);
		_delay_ms(1);
	}else if(p == LEFT){
		Lcd_Cmd(0x18);
		_delay_ms(1);
	}
}

void Lcd_Init(void)
{
	Lcd_Cmd(0x38);
	_delay_ms(2);	//명령어를 IR에 쓰고나면 40us 또는 1.64ms을 기다림.
	Lcd_Cmd(0x38);
	_delay_ms(2);
	Lcd_Cmd(0x38);
	_delay_ms(2);
	Lcd_Cmd(0x0c);
	_delay_ms(2);
	Lcd_Cmd(0x06);
	_delay_ms(2);
	Lcd_Clear();
}
