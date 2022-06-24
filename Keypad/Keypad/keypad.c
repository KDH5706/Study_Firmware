/*
 * keyT.c
 *
 * Created: 2022-06-23 오후 5:23:44
 *  Author: Cking
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

void init_keypad()
{
	DDRA = 0xF8; //0b 1111 1000
	PORTA = 0x07; //0b 0000 0111
}

unsigned char keyscan()
{
	PORTA = 0x08;	//0b 0000 1000
	_delay_ms(1);
	if(((PINA >> 0) & 0x01) == 1) return 1;
	if(((PINA >> 1) & 0x01) == 1) return 2;
	if(((PINA >> 2) & 0x01) == 1) return 3;
	
	PORTA = 0x10;	//0b 0001 0000
	_delay_ms(1);
	if(((PINA >> 0) & 0x01) == 1) return 4;
	if(((PINA >> 1) & 0x01) == 1) return 5;
	if(((PINA >> 2) & 0x01) == 1) return 6;
	
	PORTA = 0x20;	//0b 0010 0000
	_delay_ms(1);
	if(((PINA >> 0) & 0x01) == 1) return 7;
	if(((PINA >> 1) & 0x01) == 1) return 8;
	if(((PINA >> 2) & 0x01) == 1) return 9;
	
	PORTA = 0x40;	//0b 0100 0000
	_delay_ms(1);
	if(((PINA >> 0) & 0x01) == 1) return 10;
	if(((PINA >> 1) & 0x01) == 1) return 11;
	if(((PINA >> 2) & 0x01) == 1) return 12;
	
	return 0;
}

int main()
{
	DDRC = 0xFF;
	init_keypad();
	
	while(1){
		unsigned char key = keyscan();
		PORTC = key;
	}
	return 0;
}