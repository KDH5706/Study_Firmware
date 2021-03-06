/*
 * timer_fnd1.c
 *
 * Created: 2022-06-21 오후 4:24:33
 *  Author: PKNU
 */ 
#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char time_s = 0;
unsigned char timer0Cnt = 0;

unsigned char FND_DATA_TBL[] =
{
	0x3f,	0x06,	0x5b,	0x4f,
	0x66,	0x6d,	0x7d,	0x27,
	0x7f,	0x6f,	0x77,	0x7c,
	0x39,	0x5e,	0x79,	0x71
};

ISR(TIMER0_COMP_vect)
{
	cli();
	OCR0 += 72;

	timer0Cnt++;
	if(timer0Cnt == 100)
	{
		timer0Cnt = 0;
		time_s++;

		if(time_s >= 16)
		{
			time_s = 0;
		}
	}
	sei();
}

void Init()
{
	DDRA = 0xff;
	TCCR0 = 0x07;
	OCR0 = 72;
	TIMSK = 0x02;
	TIFR |= 1 << OCF0;
	sei();
}

int main(void)
{
	Init();
	
	while (1)
	{
		PORTA = FND_DATA_TBL[time_s];
	}
	return 0;
}

