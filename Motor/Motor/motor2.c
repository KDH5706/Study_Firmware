/*
 * motor2.c
 *
 * Created: 2022-06-23 오후 3:14:38
 *  Author: PKNU
 */ 
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h> // delay 함수사용을 위한 헤더파일

int main(void) {
	unsigned char i;
	
	//	DDRB = (1 << DDB5); // MOTOR1_EN를 출력 포트로 설정
	DDRD = 0xF0; // STEP0 ~ STEP3을 출력 포트로 설정
	//	PORTB &= ~(1 << DDB5); // M1 Disable, DC 모터 정지

	while (1) {
		
		for (i = 0 ; i < 12	; i++)
		{
			PORTD = (1 << PORTD4) | (1 << PORTD5); // 1 step
			_delay_ms(5);
			PORTD = (1 << PORTD4) | (1 << PORTD7); // 2 step
			_delay_ms(5);
			PORTD = (1 << PORTD5) | (1 << PORTD7); // 3 step
			_delay_ms(5);
			PORTD = (1 << PORTD5) | (1 << PORTD6); // 4 step
			_delay_ms(5);
		}
		_delay_ms(1000);
	}
	
	return 0;
}
