/*
 * pwm1.c
 *
 * Created: 2022-06-22 오후 4:14:51
 * Author : PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	unsigned char Light = 0;
	
	DDRB |= (1 << DDB4);
	
	TCCR0 |= (1 << WGM00);	//PC PWM 모드
	TCCR0 |= (1 << COM01) | (1 << COM00);	// Set OC0 on Compare Match
	TCCR0 |= (1 << CS02) | (1 << CS01);		// 분주비 : 256
	
	//TCCR0 = 0x76;
	
	TCNT0 = 0;
	
    while (1) 
    {
		for (Light = 0; Light < 255 ; Light++)
		{
			OCR0 = Light;
			_delay_ms(10);
		}
		for (Light = 255; Light > 0 ; Light--)
		{
			OCR0 = Light;
			_delay_ms(10);
		}
    }
}

