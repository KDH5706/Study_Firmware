/*
 * interrupt1.c
 *
 * Created: 2022-06-20 오전 9:54:50
 *  Author: PKNU
 */ 
#define  F_CPU 7432800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char count = 0;



ISR(INT0_vect)
{
	count++;
	PORTA = count;
}

void Init()
{
	DDRD = 0x00;	// D포트를 입력(input)포트로 설정
	DDRA = 0xff;	// A포트를 출력(output)포트로 설정
	//EIMSK = 0x01;	// INT0 활성화
	EIMSK = (1 << INT0);
	//EICRA = 0x03;	// 0b 0000 0011	(상승엣지로 설정)
	EICRA = (1 << ISC01) | (1 << ISC00);
	sei();
}

int main(void)
{
	Init();
	
	while (1)
	{
		
	}
	
	return 0;
}

