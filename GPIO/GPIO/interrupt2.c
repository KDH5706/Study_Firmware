/*
* interrupt2.c
*
* Created: 2022-06-20 오전 9:54:50
*  Author: PKNU
*/
#define  F_CPU 7432800UL
#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>

bool flag = true;

ISR(INT4_vect)
{
	flag = 1 - flag;
}

void Init()
{
	DDRE = 0x00;	// E포트를 입력(input)포트로 설정
	DDRA = 0xff;	// A포트를 출력(output)포트로 설정
	EIMSK = (1 << INT4);
	EICRB = (1 << ISC41);
	sei();
}

int main(void)
{
	Init();
	
	while (1)
	{
		if(flag)
			PORTA = 0x00;
		else
			PORTA = 0xff;
	}
	
	return 0;
}

