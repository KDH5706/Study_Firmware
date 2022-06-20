/*
 * Lcd_4bit.c
 *
 * Created: 2017-07-09 오후 10:44:06
 *  Author: CastleKing
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "Lcd.h"

void LcdCmd_4bit(Byte ch)
{
	LCD_CTRL &= ~(1 << LCD_RS); // RS == 0
	LCD_CTRL &= ~(1 << LCD_RW);	// RW == 0
	LCD_INSTWR = (ch & 0xf0) | (1 << LCD_EN);
	_delay_us(50);
	LCD_CTRL &= ~(1 << LCD_EN);

	LCD_CTRL &= ~(1 << LCD_RS); // RS == 0
	LCD_CTRL &= ~(1 << LCD_RW);	// RW == 0
	LCD_INSTWR = ((ch << 4) & 0xf0)	| (1 << LCD_EN);
	_delay_us(50);
	LCD_CTRL &= ~(1 << LCD_EN);
}

void LcdData_4bit(Byte ch)
{
//	LCD_CTRL |= (1 << LCD_RS);	// RS == 1
	LCD_CTRL &= ~(1 << LCD_RW); // RW == 0
	LCD_DATAWR = (ch & 0xf0) | (1 << LCD_RS) | (1 << LCD_EN);
	_delay_us(50);				// BF = 0 이 된 뒤에 다음명령을 쓸 수 있다. 따라서 delay를 준다.
	LCD_CTRL &= ~(1 << LCD_EN);	// Lcd Disable
	
//	LCD_CTRL |= (1 << LCD_RS);	// RS == 1
	LCD_CTRL &= ~(1 << LCD_RW); // RW == 0
	LCD_DATAWR = ((ch << 4) & 0xf0) | (1 << LCD_RS) | (1 << LCD_EN);
	_delay_us(50);				// BF = 0 이 된 뒤에 다음명령을 쓸 수 있다. 따라서 delay를 준다.
	LCD_CTRL &= ~(1 << LCD_EN);	// Lcd Disable
	
}

void Lcd_CHAR(Byte c)
{
	LcdData_4bit(c);
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
	LcdCmd_4bit(0x80 | (row + col * 0x40));
}

void Lcd_Clear(void)
{
	LcdCmd_4bit(0x01);
	_delay_ms(2);
}

void Lcd_Shift(Byte p)
{
	if(p == RIGHT){
		LcdCmd_4bit(0x1c);
		_delay_ms(1);
		}else if(p == LEFT){
		LcdCmd_4bit(0x18);
		_delay_ms(1);
	}
}

void LcdInit_4bit(void)
{
	LcdCmd_4bit(0x02);
	_delay_ms(2);	//명령어를 IR에 쓰고나면 40us 또는 1.64ms을 기다림.
	LcdCmd_4bit(0x28);
	_delay_ms(2);
	LcdCmd_4bit(0x28);
	_delay_us(50);
	LcdCmd_4bit(0x0c);
	_delay_us(50);
	LcdCmd_4bit(0x06);
	_delay_us(50);
	Lcd_Clear();
}

//void LcdInit_4bit(void)
//{
//LcdCmd_4bit(0x02);
//LcdCmd_4bit(0x28);
//LcdCmd_4bit(0x0c);
//LcdCmd_4bit(0x06);
//LcdCmd_4bit(0x01);
//LcdCmd_4bit(0x80);
//}