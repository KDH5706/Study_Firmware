/*
 * timer_16_fnd2.c
 *
 * Created: 2022-06-22 오전 10:00:36
 * Author : PKNU
 */ 
#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL[] =
{
	0x3f,   0x06,   0x5b,   0x4f,
	0x66,   0x6d,   0x7d,   0x27,
	0x7f,   0x6f,   0x77,   0x7c,
	0x39,   0x5e,   0x79,   0x71
};

volatile unsigned char time_s = 0;

ISR(TIMER1_COMPA_vect)
{
	cli();
	OCR1A += 7200; 	//7372800/1024 = 7200
	
	time_s++;

	if(time_s >= 16)
	{
		time_s = 0;
	}
	
	sei();
}

void Init()
{
	DDRA = 0xff;
	
	//TCCR1A = 0x00;
	TCCR1B |= (1 << CS12) | (1 << CS10); //TCCR1B = 0x05;	분주비 : 1024
	
	OCR1A = 7200;	//7372800/1024 = 7200
	TIMSK |= (1 << OCIE1A);//TIMSK = 0x10;
	TIFR |= (1 << OCF1A);
	
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
