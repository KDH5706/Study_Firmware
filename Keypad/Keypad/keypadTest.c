/*
* keypadTest.c
*
* Created: 2022-06-23 오후 4:02:36
* Author : PKNU
*/
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char FND_DATA_TBL[] =
{
	0x3f,   0x06,   0x5b,   0x4f,
	0x66,   0x6d,   0x7d,   0x27,
	0x7f,   0x6f,   0x77,   0x7c,
	0x39,   0x5e,   0x79,   0x71,
	0x00
};

void Init()
{
	DDRA = 0xF8;	//상위 5비트는 출력(Row:가로) 하위 3비트는 입력(Cow:세로)
	DDRB = 0xFF;
}

/*
unsigned int keyScan()
{
	//R0
	PORTA = 0xF7;	// 0b 1111 0111
	if(PINA == 0xFE) return 1;		//C0	0b 1111 1110
	else if(PINA == 0xFD) return 2;	//C1	0b 1111 1101
	else if(PINA == 0xFB) return 3;	//C2	0b 1111 1011
	_delay_ms(10);

	//R1
	PORTA = 0xEF;	// 0b 1110 1111
	if(PINA == 0xFE) return 4;		//C0	0b 1111 1110
	else if(PINA == 0xFD) return 5;	//C1	0b 1111 1101
	else if(PINA == 0xFB) return 6;	//C2	0b 1111 1011
	_delay_ms(10);

	//R2
	PORTA = 0xDF;	// 0b 1101 1111
	if(PINA == 0xFE) return 7;		//C0	0b 1111 1110
	else if(PINA == 0xFD) return 8;	//C1	0b 1111 1101
	else if(PINA == 0xFB) return 9;	//C2	0b 1111 1011
	_delay_ms(10);

	//R3
	PORTA = 0xBF;	// 0b 1011 1111
	if(PINA == 0xFE) return 10;			//C0	0b 1111 1110
	else if(PINA == 0xFD) return 11;	//C1	0b 1111 1101
	else if(PINA == 0xFB) return 12;	//C2	0b 1111 1011
	_delay_ms(10);
}
*/

unsigned int keyScan()
{
	int num = 16;
	
	//R0
	PORTA = 0x08;	// 0b 0000 1000
	if(((PINA >> 0) & 0x01) == 1) return 10;//C0 0b 1111 1110
	if(((PINA >> 1) & 0x01) == 1) return 2;	//C1 0b 1111 1101
	if(((PINA >> 2) & 0x01) == 1) return 3;	//C2 0b 1111 1011
	
	//R1
	PORTA = 0x10;	// 0b 0001 0000
	if(((PINA >> 0) & 0x01) == 1) return 1;	//C0 0b 1111 1110
	if(((PINA >> 1) & 0x01) == 1) return 5;	//C1 0b 1111 1101
	if(((PINA >> 2) & 0x01) == 1) return 6;	//C2 0b 1111 1011

	//R2
	PORTA = 0x20;	// 0b 0010 0000
	if(((PINA >> 0) & 0x01) == 1) return 4;	//C0 0b 1111 1110
	if(((PINA >> 1) & 0x01) == 1) return 8;	//C1 0b 1111 1101
	if(((PINA >> 2) & 0x01) == 1) return 9;	//C2 0b 1111 1011

	//R3
	PORTA = 0x40;	// 0b 0100 0000
	if(((PINA >> 0) & 0x01) == 1) return 7;	//C0 0b 1111 1110
	if(((PINA >> 1) & 0x01) == 1) return 11;//C1 0b 1111 1101
	if(((PINA >> 2) & 0x01) == 1) return 12;//C2 0b 1111 1011
	
	return num;
}


int main(void)
{
	Init();
	
	unsigned int key = 0;
	
	while (1)
	{
		key = keyScan();
		PORTB = FND_DATA_TBL[key];
	}
}

