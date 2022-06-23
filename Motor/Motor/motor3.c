/*
 * motor3.c
 *
 * Created: 2022-06-23 오후 3:26:35
 *  Author: PKNU
 */ 
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h> // delay 함수사용을 위한 헤더파일

unsigned char Step[8] = {0x90,0x80,0xC0,0x40,0x60,0x20,0x30,0x10};

int main(void) {
	unsigned char i, t = 0;
	
	//	DDRB = (1 << DDB5); // MOTOR1_EN를 출력 포트로 설정
	DDRD = 0xF0; // STEP0 ~ STEP3을 출력 포트로 설정
	//	PORTB &= ~(1 << DDB5); // M1 Disable, DC 모터 정지

	while (1) {
		
		for (i = 0 ; i < 24	; i++)
		{
			PORTD = Step[t];
			t++;
			if(t > 7)
			{
				t = 0;
			}
			_delay_ms(5);
		}
		_delay_ms(1000);
	}
	
	return 0;
}
