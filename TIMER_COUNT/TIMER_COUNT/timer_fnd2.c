/*
 * timer_fnd2.c
 *
 * Created: 2022-06-21 오후 4:24:33
 *  Author: PKNU
 */ 
#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL[] =
{
	0x3f,	0x06,	0x5b,	0x4f,
	0x66,	0x6d,	0x7d,	0x27,
	0x7f,	0x6f,	0x77,	0x7c,
	0x39,	0x5e,	0x79,	0x71
};

volatile unsigned char time_s = 0;
volatile unsigned char Time_STOP = 0;

unsigned char timer0Cnt = 0;

ISR(TIMER2_COMP_vect)
{
	cli();
	OCR2 += 72;

	timer0Cnt++;
	if(timer0Cnt == 50)
	{
		if(Time_STOP == 0)
		{
			time_s++;

			if(time_s >= 16)
			{
				time_s = 0;
			}
		}
		timer0Cnt = 0;
	}
	sei();
}

ISR(INT4_vect)
{
	cli();
	Time_STOP = 1 - Time_STOP;
	sei();
}

void Init()
{
	DDRA = 0xff;
	DDRE = 0x00;
	
	TCCR2 = 0x05;
	OCR2 = 72;
	TIMSK = 0x80;
	TIFR |= 1 << OCF2;
	
	EIMSK = 0x10;
	EICRB = 0x03;
	EIFR = 0x10;
	
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

