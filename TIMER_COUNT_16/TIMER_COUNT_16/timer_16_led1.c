/*
 * timer_16_led1.c
 *
 * Created: 2022-06-22 오전 10:00:36
 * Author : PKNU
 */ 
#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data = 0x00;

void Init()
{
	DDRC = 0xff;
	
	//TCCR1A = 0x00;
	TCCR1B |= (1 << CS12); //TCCR1B = 0x04;	분주비 : 256
	
	TCNT1 = 36736;	//65536 - 78800
	TIMSK |= (1 << TOIE1);//TIMSK = 0x04;
	TIFR |= (1 << TOV1);
	
	sei();
}

ISR(TIMER1_OVF_vect)
{
	cli();
	TCNT1 = 36736;
	
	LED_Data++;
	if(LED_Data > 0xff)
	{
		LED_Data = 0;
	}
	
	sei();
}

int main(void)
{
	Init();
    while (1) 
    {
		PORTC = LED_Data;
    }
	return 0;
}

