/*
 * timer_led1.c
 *
 * Created: 2022-06-21 오후 3:31:02
 * Author : PKNU
 */ 
#define F_CPU 7372800UL	
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data = 0x00;
unsigned char timer0Cnt = 0;

ISR(TIMER0_OVF_vect)
{
	cli();
	TCNT0 = 184;
	timer0Cnt++;
	if(timer0Cnt == 100)
	{
		LED_Data++;
		if(LED_Data > 0x0f)
		{
			LED_Data = 0;
		}
		timer0Cnt = 0;
	}
	sei();
}

void Init()
{
	DDRC = 0x0f;
	TCCR0 = 0x07;
	TCNT0 = 184;
	TIMSK |= 1 << TOIE0;
	TIFR |= 1 << TOV0;
	sei();
}

int main(void)
{
	Init();
	
    while (1) 
    {
		PORTC = LED_Data;
    }
}

