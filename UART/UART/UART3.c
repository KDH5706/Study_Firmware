/*
* UART3.c
*
* Created: 2022-06-21 오전 11:17:55
*  Author: PKNU
*/

#include <avr/io.h> // AVR 입출력에 대한 헤더 파일
#include <avr/interrupt.h>

volatile unsigned char TX_flag = 0;
volatile char TX_data = 0;

unsigned char FND_DATA_TBL[] =
{
	0x3f,	0x06,	0x5b,	0x4f,
	0x66,	0x6d,	0x7d,	0x27,
	0x7f,	0x6f,	0x77,	0x7c,
	0x39,	0x5e,	0x79,	0x71
};

void putch(unsigned char data)
{
	while ((UCSR0A & 0x20) == 0);
	UDR0 = data;
	UCSR0A |= 0x20;
}

unsigned char getch(void)
{
	unsigned char data;
	while ((UCSR0A & 0x80) == 0);
	data = UDR0;
	UCSR0A |= 0x80;
	return data;
}

void Init()
{
	DDRE = 0x0e;
	DDRA = 0xff;
	UCSR0A = 0x00;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	UBRR0H = 0x00;
	UBRR0L = 3;
}
int main()
{
	Init();
	
	unsigned char RX_data = 0;

	while (1)
	{
		RX_data = getch();
		if((RX_data >= 0x30) && (RX_data <= 0x39))
		{
			PORTA = FND_DATA_TBL[RX_data - 0x30];
		}

	}
}