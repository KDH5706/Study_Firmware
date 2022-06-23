/*
 * piezo1.c
 *
 * Created: 2022-06-22 오후 4:14:51
 * Author : PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

unsigned int DoReMi[8] = {523,587, 659, 698, 783,880, 987, 1046};

int main(void)
{
	unsigned char piano = 0;
	
	DDRB |= (1 << DDB7);
	
	TCCR1A |= (1 << WGM11);		// Fast PWM 모드 설정 TOP : ICR1
	TCCR1B |= (1 << WGM12);		// Fast PWM 모드 설정 TOP : ICR1
	TCCR1B |= (1 << WGM13);		// Fast PWM	모드 설정 TOP : ICR1
	
	TCCR1A |= (1 << COM1C1);	// Clear OCnA/OCnB/OCnC on compare match (set output to low level).

	TCCR1B |= (1 << CS10);		// clkI/O/1 (No prescaling)
	
	//TCCR1C = 0x00;
	TCNT1 = 0;
	
    while (1) 
    {
		ICR1 = 7372800 / DoReMi[piano];
		OCR1C = ICR1/25;
		piano++;
		if(piano > 7)
		{
			piano = 0;
		}
		_delay_ms(500);
    }
}

