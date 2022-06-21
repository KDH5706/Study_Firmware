/*
* UART1.c
*
* Created: 2022-06-21 오전 10:36:58
*  Author: PKNU
*/

#include <avr/io.h> // AVR 입출력에 대한 헤더 파일

volatile unsigned char TX_flag = 0;
volatile char TX_data = 0;

void putch(unsigned char data)
{
	while((UCSR0A & 0x20) == 0); // 전송준비가 될때까지 대기
	UDR0 = data; // 데이터를 UDR0에 쓰면 전송
	UCSR0A |= 0x20;
}

void putch_Str(char *str)
{
	unsigned char i = 0;
	while (str[i] != '\0')
	{
		putch(str[i++]);
	}
}

void Init()
{
	DDRE = 0x02;	// Rx(입력 0), Tx(출력, 1), SW0~3 입력
	UCSR0A = 0x00;
	UCSR0B = 0x18; // Rx, Tx enable
	UCSR0C = 0x06; // 비동기 방식, No Parity bit, 1 Stop bit
	UBRR0H = 0x00;
	UBRR0L = 3; // 115200 bps
	EICRB = 0xff;
	EIMSK = 0xf0;
	sei();
}

ISR(INT4_vect)
{
	cli();
	TX_data = '0';
	TX_flag = 1;
	sei();
}

ISR(INT5_vect)
{
	cli();
	TX_data = '1';
	TX_flag = 1;
	sei();
}

ISR(INT6_vect)
{
	cli();
	TX_data = '2';
	TX_flag = 1;
	sei();
}

ISR(INT7_vect)
{
	cli();
	TX_data = '3';
	TX_flag = 1;
	sei();
}

int main(void)
{
	Init();

	while(1)
	{
		if(TX_flag == 1)
		{
			putch_Str("\n\r Input Switch : ");
			putch(TX_data);
			TX_flag = 0;
		}
	}
}
