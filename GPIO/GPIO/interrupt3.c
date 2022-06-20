/*
* interrupt3.c
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
	flag = !flag;
}

void Init()
{
	DDRE = 0x00;	// E포트를 입력(input)포트로 설정
	DDRA = 0xff;	// A포트를 출력(output)포트로 설정
	EIMSK = (1 << INT4);
	EICRB = (1 << ISC41) | (1 << ISC40);
	sei();
}

int main(void)
{
	Init();
	
	//volatile : 컴파일러는 해당 변수를 최적화에서 제외하여 항상 메모리에 접근하도록 만든다.
	volatile unsigned int led_state = 0x01;
	
	while (1)
	{
		PORTA = led_state;
		if(flag)
		{
			_delay_ms(100);
			
			if(led_state >= 0x80)
				led_state = 0x01;
			else
				led_state <<= 1;
		}
	}
	
	return 0;
}

