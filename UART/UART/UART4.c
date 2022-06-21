/*
* UART4.c
*
* Created: 2022-06-21 오전 11:17:55
*  Author: PKNU
*/
#define F_CPU 7372800UL
#include <avr/io.h> // AVR 입출력에 대한 헤더 파일
#include <stdio.h>
#include <util/delay.h>

void putch_USART0(unsigned char data)
{
	while((UCSR0A & 0x20) == 0); // 전송준비가 될때까지 대기
	UDR0 = data; // 데이터를 UDR0에 쓰면 전송
	UCSR0A |= 0x20;
}

void puts_USART0(unsigned char *str)
{
	while (*str != 0)
	{
		putch_USART0(*str);
		str++;
	}
}

unsigned char getch()
{
	unsigned char data;
	while ((UCSR0A & 0x80) == 0);
	data = UDR0;
	UCSR0A |= 0x80;
	return data;
}

void Show_GuGuDan(unsigned char Dan)
{
	int num = Dan - 0x30;
	putch_USART0('\n');
	for(int i = 1; i < 10 ; i++)
	{
		unsigned char str[15] = "";
		sprintf((char*)str, "%d X %d = %d\n\r", num, i, num*i);
		puts_USART0((unsigned char*)str);
		_delay_ms(250);
	}
}

void Show_Input_Value(unsigned char Dan)
{
	unsigned char str[15] = "";
	sprintf((char*)str, "INPUT : %c\n\r", Dan);
	puts_USART0((unsigned char*)str);
}

void Init()
{
	DDRE = 0x0e;
	UCSR0A = 0x00;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	UBRR0H = 0x00;
	UBRR0L = 3;
}

int main()
{
	Init();
	unsigned char err_msg[] =
	"\r\nPlease enter a number button.\r\n";

	unsigned char info_msg[] =
	"\r\nEnter the multiplication table to print out.\r\n";

	puts_USART0(info_msg);

	while (1)
	{
		//출력할 구구단을 입력 받음
		unsigned char Dan = getch();
		Show_Input_Value(Dan);
		if((Dan > 0x30) && (Dan <= 0x39))
		{
			Show_GuGuDan(Dan);
			puts_USART0(info_msg);
		}
		else
		{
			puts_USART0(err_msg);
		}
	}
}