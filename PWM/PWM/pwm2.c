/*
 * pwm2.c
 *
 * Created: 2022-06-22 오후 4:14:51
 * Author : PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile int Light = 0;
volatile unsigned char Light_flag = 1;

void Init()
{
	DDRB |= (1 << DDB7);
	DDRE = 0x00;
	
	TCCR2 |= (1 << WGM20);	// PC PWM 모드
	TCCR2 |= (1 << COM21);	// Clear OC2 on compare match
	TCCR2 |= (1 << CS21);	// 분주비 : 8
	
	TCNT2 = 0;
	
	EICRB = 0xff;
	EIMSK = 0xf0;
	EIFR = 0xf0;
	
	sei();
}

int main(void)
{
	Init();
	
    while (1) 
    {
		if(Light_flag)
		{
			OCR2 = Light;
			Light_flag = 0;
		}
    }
}

ISR(INT4_vect)
{
	cli();
	Light = 0;
	Light_flag = 1;
	sei();
}

ISR(INT5_vect)
{
	cli();
	Light -= 51;
	if(Light < 0)
	{
		Light = 0;
	}
	Light_flag = 1;
	sei();
}

ISR(INT6_vect)
{
	cli();
	Light += 51;
	if(Light > 255)
	{
		Light = 255;
	}
	Light_flag = 1;
	sei();
}

ISR(INT7_vect)
{
	cli();
	Light = 255;
	Light_flag = 1;
	sei();
}
